/*
Copyright 2013-present Barefoot Networks, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <saiacl.h>
#include "saiapi.h"
#include "saiinternal.h"
#include <switchapi/switch_handle.h>
#include <switchapi/switch_acl.h>
#include <arpa/inet.h>

/*
Note: SAI ACL action processing implementation changes in the future
This is an interim solution to handling actions for the ACL in a more
static way. In a future implementation a dynamic action composiitng 
scheme will allow for having multiple actions be speicifed in any
combination in response to a match
*/

typedef int table_match_qualifiers[SAI_ACL_TABLE_ATTR_FIELD_END - SAI_ACL_TABLE_ATTR_FIELD_START + 1]; 

static table_match_qualifiers ip_acl = {
        -1, -1,   // v6
        -1, -1, // MAC
        SWITCH_ACL_IP_FIELD_IPV4_SRC, SWITCH_ACL_IP_FIELD_IPV4_DEST, // v4
        -2, -1, -2, -1, // ports
        -1, -1, -1, -1, -1, -1, // VLAN outer and inner
        SWITCH_ACL_IP_FIELD_L4_SOURCE_PORT, SWITCH_ACL_IP_FIELD_L4_DEST_PORT, // l4 ports
        SWITCH_ACL_IP_FIELD_ETH_TYPE,
        SWITCH_ACL_IP_FIELD_IP_PROTO,
        SWITCH_ACL_IP_FIELD_DSCP,
        -1, // ecn
        SWITCH_ACL_IP_FIELD_TTL, // ttl
        SWITCH_ACL_IP_FIELD_TOS,
        SWITCH_ACL_IP_FIELD_IP_FLAGS,
        SWITCH_ACL_IP_FIELD_TCP_FLAGS, // tcp flags
        -1, // ip type
        -1, // ip frag
        -1, // ipv6 flow
        -1  // tc
        };

static table_match_qualifiers ipv6_acl = {
        SWITCH_ACL_IPV6_FIELD_IPV6_SRC, SWITCH_ACL_IPV6_FIELD_IPV6_DEST,
        -1, -1, // MAC
        -1, -1, // v4
        -2, -1, -2, -1, // ports
        -1, -1, -1, -1, -1, -1, // VLAN outer and inner
        SWITCH_ACL_IPV6_FIELD_L4_SOURCE_PORT, SWITCH_ACL_IPV6_FIELD_L4_DEST_PORT, // l4 ports
        SWITCH_ACL_IPV6_FIELD_ETH_TYPE,
        SWITCH_ACL_IPV6_FIELD_IP_PROTO,
        -1, // dscp
        -1, // ecn
        SWITCH_ACL_IPV6_FIELD_TTL, // ttl
        SWITCH_ACL_IPV6_FIELD_TOS,
        -1, // ip flags
        SWITCH_ACL_IPV6_FIELD_TCP_FLAGS, // tcp flags
        -1, // ip type
        -1, // ip frag
        SWITCH_ACL_IPV6_FIELD_FLOW_LABEL,
        -1  // tc
        };

static table_match_qualifiers mac_acl = {
        -1, -1,   // v6
        SWITCH_ACL_MAC_FIELD_SOURCE_MAC, SWITCH_ACL_MAC_FIELD_DEST_MAC, // MAC
        -1, -1, // v4
        -2, -1, -2, -1, // ports
        -1, SWITCH_ACL_MAC_FIELD_VLAN_PRI, SWITCH_ACL_MAC_FIELD_VLAN_CFI, -1, -1, -1, // VLAN outer and inner
        -1, -1, // l4 ports
        SWITCH_ACL_MAC_FIELD_ETH_TYPE,
        -1,
        -1,
        -1, // ecn
        -1, // ttl
        -1,
        -1,
        -1, // tcp flags
        -1, // ip type
        -1, // ip frag
        -1, // ipv6 flow
        -1  // tc
        };


static int *get_p4_match_table(switch_acl_type_t table_type)
{
    switch(table_type) {
        case SWITCH_ACL_TYPE_IP:
            return ip_acl;
        case SWITCH_ACL_TYPE_IPV6:
            return ipv6_acl;
        case SWITCH_ACL_TYPE_MAC:
            return mac_acl;
        default:
            return NULL;
    }
}

/*
    Ensure that all the fields in the attribute list can be handled by the ACL
*/
static int match_table_type(
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list) 
{
    int i=0;
    for(i=0;i<SWITCH_ACL_TYPE_MAX;i++) {
        int *table = get_p4_match_table(i);
        if(table) {
            uint32_t j=0;
            for(j=0;j<attr_count;j++) {
                // skip ports and VLAN attributes on check
                switch(attr_list[j].id) {
                    case SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS:
                    case SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORTS:
                    case SAI_ACL_ENTRY_ATTR_FIELD_IN_PORT:
                    case SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORT:
                    case SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_ID:
                    case SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_ID:
                        break;
                    default:
                        if(table[attr_list[j].id - SAI_ACL_TABLE_ATTR_FIELD_START] == -1)
                            return -1;
                }
            }
            if(j == attr_count)
                return i;
        }
    }
    return -1;
}


static int match_table_field(int table_id, 
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list,
        _Out_ int *match_fields,
        _Out_ int *actions) 
{
    int *table = get_p4_match_table(table_id);
    if(table) {
        uint32_t j=0;
        for(j=0;j<attr_count;j++) {
            int id = attr_list[j].id;
            if(id >= SAI_ACL_TABLE_ATTR_FIELD_START && id <= SAI_ACL_TABLE_ATTR_FIELD_END) {
                id -= SAI_ACL_TABLE_ATTR_FIELD_START;
                if(table[id] != -1) {
                    match_fields[j] = table[id];
                }
                else
                    return -1;
            }
        }
        return 0;
    }
    return -1;
}

static int xform_field_value(switch_acl_type_t acl_type, int field, void *dest, const sai_acl_field_data_t *source)
{
    switch(acl_type) {
        case SWITCH_ACL_TYPE_IP:
            {
                switch_acl_ip_key_value_pair_t *kvp = (switch_acl_ip_key_value_pair_t *)dest;
                switch(field) {
                    case SWITCH_ACL_IP_FIELD_IPV4_SRC:
                        kvp->value.ipv4_source = ntohl(source->data.ip4);
                        kvp->mask.u.mask = ntohl(source->mask.ip4);
                        break;
                    case SWITCH_ACL_IP_FIELD_IPV4_DEST:
                        kvp->value.ipv4_dest = ntohl(source->data.ip4);
                        kvp->mask.u.mask = ntohl(source->mask.ip4);
                        break;
                    case SWITCH_ACL_IP_FIELD_IP_PROTO:
                        kvp->value.ip_proto = source->data.u16;
                        kvp->mask.u.mask = source->mask.u16;
                        break;
                    case SWITCH_ACL_IP_FIELD_L4_SOURCE_PORT:
                        kvp->value.l4_source_port = source->data.u16;
                        kvp->mask.u.mask = source->mask.u16;
                        break;
                    case SWITCH_ACL_IP_FIELD_L4_DEST_PORT:
                        kvp->value.l4_dest_port = source->data.u16;
                        kvp->mask.u.mask = source->mask.u16;
                        break;
                    case SWITCH_ACL_IP_FIELD_ICMP_TYPE:
                    case SWITCH_ACL_IP_FIELD_ICMP_CODE:
                    case SWITCH_ACL_IP_FIELD_TCP_FLAGS:
                        kvp->value.tcp_flags = source->data.u8;
                        kvp->mask.u.mask = source->mask.u8;
                    case SWITCH_ACL_IP_FIELD_TTL:
                        kvp->value.ttl = source->data.u8;
                        kvp->mask.u.mask = source->mask.u8;
                        break;
                    case SWITCH_ACL_IP_FIELD_ETH_TYPE:
                        kvp->value.eth_type = source->data.u16;
                        kvp->mask.u.mask = source->mask.u16;
                        break;
                    case SWITCH_ACL_IP_FIELD_DSCP:
                        kvp->value.dscp = source->data.u8;
                        kvp->mask.u.mask = source->mask.u8;
                        break;
                    case SWITCH_ACL_IP_FIELD_IP_FLAGS:
                        kvp->value.ip_flags = source->data.u8;
                        kvp->mask.u.mask = source->mask.u8;
                        break;
                    case SWITCH_ACL_IP_FIELD_TOS:
                        kvp->value.tos = source->data.u8;
                        kvp->mask.u.mask = source->mask.u8;
                        break;
                    case SWITCH_ACL_IP_FIELD_IP_FRAGMENT:
                        kvp->value.ip_frag = source->data.u8;
                        kvp->mask.u.mask = source->mask.u8;
                        break;
                    default:
                        break;
                }
            }
            break;
        case SWITCH_ACL_TYPE_IPV6:
            {
                //                switch_acl_ipv6_key_value_pair_t *kvp = (switch_acl_ipv6_key_value_pair_t *)dest;
                switch(field) {
                    case SWITCH_ACL_IPV6_FIELD_IPV6_SRC:
                    case SWITCH_ACL_IPV6_FIELD_IPV6_DEST:
                    case SWITCH_ACL_IPV6_FIELD_IP_PROTO:
                    case SWITCH_ACL_IPV6_FIELD_L4_SOURCE_PORT:
                    case SWITCH_ACL_IPV6_FIELD_L4_DEST_PORT:
                    case SWITCH_ACL_IPV6_FIELD_ICMP_TYPE:
                    case SWITCH_ACL_IPV6_FIELD_ICMP_CODE:
                    case SWITCH_ACL_IPV6_FIELD_TCP_FLAGS:
                    case SWITCH_ACL_IPV6_FIELD_TTL:
                    case SWITCH_ACL_IPV6_FIELD_ETH_TYPE:
                    case SWITCH_ACL_IPV6_FIELD_FLOW_LABEL:
                        break;
                    default:
                        break;
                }
            }
            break;
        case SWITCH_ACL_TYPE_MAC:
            {
                switch_acl_mac_key_value_pair_t *kvp = (switch_acl_mac_key_value_pair_t *)dest;
                switch(field) {
                    case SWITCH_ACL_MAC_FIELD_SOURCE_MAC:
                        memcpy(kvp->value.source_mac, source->data.mac, 6);
                        memcpy(kvp->mask.u.mac_mask, source->mask.mac, 6);
                        break;
                    case SWITCH_ACL_MAC_FIELD_DEST_MAC:
                        memcpy(kvp->value.dest_mac, source->data.mac, 6);
                        memcpy(kvp->mask.u.mac_mask, source->mask.mac, 6);
                        break;
                    case SWITCH_ACL_MAC_FIELD_VLAN_PRI:
                        kvp->value.vlan_pri = source->data.u8;
                        kvp->mask.u.mask16 = source->mask.u8;
                        break;
                    case SWITCH_ACL_MAC_FIELD_VLAN_CFI:
                        kvp->value.vlan_cfi = source->data.u8;
                        kvp->mask.u.mask16 = source->mask.u8;
                        break;
                    case SWITCH_ACL_MAC_FIELD_ETH_TYPE:
                        kvp->value.eth_type = source->data.u16;
                        kvp->mask.u.mask16 = source->mask.u16;
                        break;
                    default:
                        break;
                }
            }
            break;
        default:
            break;
    }
    return 0;
}

/*
* Routine Description:
*   Create an ACL table
*
* Arguments:
*  [out] acl_table_id - the the acl table id
*  [in] attr_count - number of attributes
*  [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_create_acl_table(
        _Out_ sai_object_id_t* acl_table_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    sai_status_t status = SAI_STATUS_SUCCESS;
    switch_acl_type_t acl_type = 0;
    acl_type = match_table_type( attr_count, attr_list);
    if(acl_type < 0) 
        return SAI_STATUS_INVALID_PARAMETER;
    *acl_table_id = (sai_object_id_t)switch_api_acl_list_create(device, acl_type);
    return (sai_status_t) status;
}

/*
* Routine Description:
*   Delete an ACL table
*
* Arguments:
*   [in] acl_table_id - the acl table id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_delete_acl_table(
        _In_ sai_object_id_t acl_table_id) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    status = switch_api_acl_list_delete(device, (switch_handle_t) acl_table_id);
    return (sai_status_t) status;
}

typedef struct {
    tommy_node node;
    switch_handle_t handle;
} sai_handle_node_t;

/*
* Routine Description:
*   Create an ACL entry
*
* Arguments:
*   [out] acl_entry_id - the acl entry id
*   [in] attr_count - number of attributes
*   [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_create_acl_entry(
        _Out_ sai_object_id_t *acl_entry_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    uint32_t i=0;
    sai_object_id_t acl_table_id = 0ULL;
    int *match_fields = NULL;
    switch_acl_ip_key_value_pair_t *kvp=NULL;
    unsigned int field_size;
    switch_acl_action_t acl_action = 0;
    switch_acl_action_params_t         action_params;
    uint32_t priority = 0;
    switch_acl_info_t *acl_info = NULL;
    switch_acl_type_t acl_type;
    tommy_list handle_list;
    sai_handle_node_t *handle;

    *acl_entry_id = 0ULL;
    memset(&action_params, 0, sizeof(switch_acl_action_params_t));
    tommy_list_init(&handle_list);
    // get the table id
    for(i=0;i<attr_count;i++) {
        switch(attr_list[i].id) {
            // ACL table identifier
            case SAI_ACL_ENTRY_ATTR_TABLE_ID:
                acl_table_id = attr_list[i].value.aclfield.data.oid;
                break;
            // ACL entry priority
            case SAI_ACL_ENTRY_ATTR_PRIORITY:
                priority = attr_list[i].value.aclfield.data.u32;
                break;
            // ACL REFERENCE handling
            case SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS:
                {
                    uint32_t j=0;
                    sai_object_id_t *obj = attr_list[i].value.aclfield.data.objlist.list;
                    for(j=0;j<attr_list[i].value.aclfield.data.objlist.count;j++) {
                        // accumulate handle mask
                        handle = malloc(sizeof(sai_handle_node_t));
                        if(handle) {
                            memset(handle, 0, sizeof(sai_handle_node_t));
                            handle->handle = (switch_handle_t)*(obj + j);
                            tommy_list_insert_head(&handle_list, &(handle->node), handle);
                        }
                    }
                    free(attr_list[i].value.aclfield.data.objlist.list);
                }
                break;
            case SAI_ACL_ENTRY_ATTR_FIELD_IN_PORT:
                handle = malloc(sizeof(sai_handle_node_t));
                if(handle) {
                    memset(handle, 0, sizeof(sai_handle_node_t));
                    handle->handle = (switch_handle_t)attr_list[i].value.aclfield.data.oid;
                    tommy_list_insert_head(&handle_list, &(handle->node), handle);
                }
                break;
            /*
                    EGRESS ACL ONLY?
            case SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORTS:
            case SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORT:
                break;
            */
            case SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_ID:
                handle = malloc(sizeof(sai_handle_node_t));
                if(handle) {
                    memset(handle, 0, sizeof(sai_handle_node_t));
                    handle->handle = (switch_handle_t)attr_list[i].value.aclfield.data.oid;
                    tommy_list_insert_head(&handle_list, &(handle->node), handle);
                }
                break;
                 // TBD inner VLAN ID based filter in P4
            case SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_ID:
                break;
            // ACTION handling
            case SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT:
                {
                    switch_handle_t handle = (switch_handle_t)attr_list[i].value.aclfield.data.oid;
                    /*
                    if(SAI_CPU_PORT(port_handle)) {
                        acl_action = SWITCH_ACL_ACTION_REDIRECT_TO_CPU;
                    }
                    else
                    */
                    {
                        acl_action = SWITCH_ACL_ACTION_REDIRECT;
                        // set the action params
                        action_params.redirect.handle = handle;
                    }
                }
                break;
            case SAI_ACL_ENTRY_ATTR_PACKET_ACTION:
                acl_action = SWITCH_ACL_ACTION_DROP;
                break;
            case SAI_ACL_ENTRY_ATTR_ACTION_FLOOD:
                acl_action = SWITCH_ACL_ACTION_FLOOD_TO_VLAN;
                break;
        }
    }
    if(acl_table_id == 0ULL) {
        return SAI_STATUS_INVALID_PARAMETER;
    }
    if((acl_info = switch_acl_get((switch_handle_t) acl_table_id))) {
        acl_type = acl_info->type;
    }
    else
        return SAI_STATUS_INVALID_PARAMETER;
    
    // switch on type to get more values!
    field_size = SWITCH_ACL_IP_FIELD_MAX;   
    match_fields=malloc(sizeof(int)*field_size);
    if(match_fields) {
        int *actions = NULL;
        // init the array to unknown
        for(i=0;i<field_size;i++)
            match_fields[i] = -1;
        actions = malloc(sizeof(int)*SWITCH_ACL_ACTION_MAX);
        // get the match fields
        if(match_table_field(acl_type, attr_count, attr_list,
                    match_fields, actions) != -1) {
            // allocate to store key-value pairs
            kvp = malloc(sizeof(switch_acl_ip_key_value_pair_t)*field_size);
            if(kvp) {
                int j=0;
                // Translate the ATTR to field values
                for(i=0;i<field_size;i++) {
                    if(match_fields[i] != -1) {
                        if(match_fields[i] >= 0) {
                            kvp[j].field = match_fields[i];
                            xform_field_value(acl_type, match_fields[i], &kvp[j],
                                    &(attr_list[i].value.aclfield));
                            j++;
                        }
                    }
                }
                // add entry with kvp and j
                if(j > 0) {
                    tommy_node *node;
                    // create the rule
                    switch_api_acl_rule_create(device, acl_table_id,
                            priority, j, kvp, acl_action, &action_params, acl_entry_id);
                    // reference the ACL on handle
                    node = tommy_list_head(&handle_list);
                    while(node) {
                        handle = node->data;
                        if(handle) {
                            switch_api_acl_reference(device, acl_table_id, handle->handle);
                        }
                        node = node->next;
                    }

                    // free handle_list
                    tommy_list_foreach(&handle_list, free);
                }
            }
            free(kvp);
        }
        free(actions);
        free(match_fields);
    }

    return (sai_status_t) status;
}

/*
* Routine Description:
*   Delete an ACL entry
*
* Arguments:
*  [in] acl_entry_id - the acl entry id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_delete_acl_entry( 
        _In_ sai_object_id_t acl_entry_id) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    status = switch_api_acl_rule_delete(device, (switch_handle_t)0, (switch_handle_t)acl_entry_id);
    return (sai_status_t) status;
}

/*
*  ACL methods table retrieved with sai_api_query()
*/
sai_acl_api_t acl_api = {
    .create_acl_table                  =             sai_create_acl_table,
    .delete_acl_table                  =             sai_delete_acl_table,
    .create_acl_entry                  =             sai_create_acl_entry,
    .delete_acl_entry                  =             sai_delete_acl_entry
};

sai_status_t sai_acl_initialize(sai_api_service_t *sai_api_service) {
    sai_api_service->acl_api = acl_api;
    return SAI_STATUS_SUCCESS;
}
