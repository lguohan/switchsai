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

#include <saihostintf.h>
#include "saiinternal.h"
#include <switchapi/switch_sup.h>

/*
* Routine Description:
*    Create host interface trap group
*
* Arguments:
*  [out] hostif_trap_group_id  - host interface trap group id
*  [in] attr_count - number of attributes
*  [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_create_hostif_trap_group(
    _Out_ sai_object_id_t *hostif_trap_group_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list) {
    const sai_attribute_t *attribute;
    switch_sup_group_t sup_group;
    int index = 0;
    memset(&sup_group, 0, sizeof(switch_sup_group_t));
    for (index = 0; index < attr_count; index++) {
        attribute = &attr_list[index];   
        switch (attribute->id) {
            case SAI_HOSTIF_TRAP_GROUP_ATTR_ADMIN_STATE:
                break;
            case SAI_HOSTIF_TRAP_GROUP_ATTR_PRIO:
                sup_group.priority = attribute->value.u32;
                break;
            case SAI_HOSTIF_TRAP_GROUP_ATTR_QUEUE:
                sup_group.egress_queue = attribute->value.u32;
                break;
            case SAI_HOSTIF_TRAP_GROUP_ATTR_POLICER:
                break;
        }
    }
    *hostif_trap_group_id = switch_api_sup_group_create(device, &sup_group);
    return SAI_STATUS_SUCCESS;
}

/*
* Routine Description:
*    Remove host interface trap group
*
* Arguments:
*  [in] hostif_trap_group_id - host interface trap group id
*
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_remove_hostif_trap_group(
    _In_ sai_object_id_t hostif_trap_group_id) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    status = switch_api_sup_group_delete(device, hostif_trap_group_id);
    return status;
}

/*
* Routine Description:
*   Set host interface trap group attribute value.
*
* Arguments:
*    [in] hostif_trap_group_id - host interface trap group id
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_set_hostif_trap_group_attribute(
    _In_ sai_object_id_t hostif_trap_group_id,
    _In_ const sai_attribute_t *attr) {
    return SAI_STATUS_SUCCESS;
}

/*
* Routine Description:
*   get host interface trap group attribute value.
*
* Arguments:
*    [in] hostif_trap_group_id - host interface trap group id
*    [in] attr_count - number of attributes
*    [in,out] attr_list - array of attributes
*
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_get_hostif_trap_group_attribute(
    _In_ sai_object_id_t hostif_trap_group_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list) {
    return SAI_STATUS_SUCCESS;
}

switch_sup_code_t switch_sai_to_switch_api_sup_code(
        sai_hostif_trap_id_t trap_id) {
    switch_sup_code_t sup_code = SWITCH_SUP_CODE_NONE;
    switch (trap_id) {
        case SAI_HOSTIF_TRAP_ID_STP:
            sup_code = SWITCH_SUP_CODE_STP;
            break;
        case SAI_HOSTIF_TRAP_ID_LACP:
            sup_code = SWITCH_SUP_CODE_LACP;
            break;
        case SAI_HOSTIF_TRAP_ID_EAPOL:
            sup_code = SWITCH_SUP_CODE_EAPOL;
            break;
        case SAI_HOSTIF_TRAP_ID_LLDP:
            sup_code = SWITCH_SUP_CODE_LLDP;
            break;
        case SAI_HOSTIF_TRAP_ID_PVRST:
            sup_code = SWITCH_SUP_CODE_PVRST;
            break;
        case SAI_HOSTIF_TRAP_ID_IGMP_TYPE_QUERY:
            sup_code = SWITCH_SUP_CODE_IGMP_TYPE_QUERY;
            break;
        case SAI_HOSTIF_TRAP_ID_IGMP_TYPE_LEAVE:
            sup_code = SWITCH_SUP_CODE_IGMP_TYPE_LEAVE;
            break;
        case SAI_HOSTIF_TRAP_ID_IGMP_TYPE_V1_REPORT:
            sup_code = SWITCH_SUP_CODE_IGMP_TYPE_V1_REPORT;
            break;
        case SAI_HOSTIF_TRAP_ID_IGMP_TYPE_V2_REPORT:
            sup_code = SWITCH_SUP_CODE_IGMP_TYPE_V2_REPORT;
            break;
        case SAI_HOSTIF_TRAP_ID_IGMP_TYPE_V3_REPORT:
            sup_code = SWITCH_SUP_CODE_IGMP_TYPE_V3_REPORT;
            break;
        case SAI_HOSTIF_TRAP_ID_SAMPLEPACKET:
            sup_code = SWITCH_SUP_CODE_SAMPLEPACKET;
            break;
        case SAI_HOSTIF_TRAP_ID_ARP_REQUEST:
            sup_code = SWITCH_SUP_CODE_ARP_REQUEST;
            break;
        case SAI_HOSTIF_TRAP_ID_ARP_RESPONSE:
            sup_code = SWITCH_SUP_CODE_ARP_RESPONSE;
            break;
        case SAI_HOSTIF_TRAP_ID_DHCP:
            sup_code = SWITCH_SUP_CODE_DHCP;
            break;
        case SAI_HOSTIF_TRAP_ID_OSPF:
            sup_code = SWITCH_SUP_CODE_OSPF;
            break;
        case SAI_HOSTIF_TRAP_ID_PIM:
            sup_code = SWITCH_SUP_CODE_PIM;
            break;
        case SAI_HOSTIF_TRAP_ID_VRRP:
            sup_code = SWITCH_SUP_CODE_VRRP;
            break;
        case SAI_HOSTIF_TRAP_ID_BGP:
            sup_code = SWITCH_SUP_CODE_BGP;
            break;
        case SAI_HOSTIF_TRAP_ID_DHCPV6:
            sup_code = SWITCH_SUP_CODE_DHCPV6;
            break;
        case SAI_HOSTIF_TRAP_ID_OSPFV6:
            sup_code = SWITCH_SUP_CODE_OSPFV6;
            break;
        case SAI_HOSTIF_TRAP_ID_VRRPV6:
            sup_code = SWITCH_SUP_CODE_VRRPV6;
            break;
        case SAI_HOSTIF_TRAP_ID_BGPV6:
            sup_code = SWITCH_SUP_CODE_BGPV6;
            break;
        case SAI_HOSTIF_TRAP_ID_IPV6_NEIGHBOR_DISCOVERY:
            sup_code = SWITCH_SUP_CODE_IPV6_NEIGHBOR_DISCOVERY;
            break;
        case SAI_HOSTIF_TRAP_ID_IPV6_MLD_V1_V2:
            sup_code = SWITCH_SUP_CODE_IPV6_MLD_V1_V2;
            break;
        case SAI_HOSTIF_TRAP_ID_IPV6_MLD_V1_REPORT:
            sup_code = SWITCH_SUP_CODE_IPV6_MLD_V1_REPORT;
            break;
        case SAI_HOSTIF_TRAP_ID_IPV6_MLD_V1_DONE:
            sup_code = SWITCH_SUP_CODE_IPV6_MLD_V1_DONE;
            break;
        case SAI_HOSTIF_TRAP_ID_MLD_V2_REPORT:
            sup_code = SWITCH_SUP_CODE_MLD_V2_REPORT;
            break;
        case SAI_HOSTIF_TRAP_ID_L3_MTU_ERROR:
            sup_code = SWITCH_SUP_CODE_L3_MTU_ERROR;
            break;
        case SAI_HOSTIF_TRAP_ID_TTL_ERROR:
            sup_code = SWITCH_SUP_CODE_TTL_ERROR;
            break;
        default:
            break;
    }
    return sup_code;
}

switch_acl_action_t switch_sai_action_to_switch_api_action(
        sai_packet_action_t packet_action) {
    switch_acl_action_t acl_action;
    switch (packet_action) {
        case SAI_PACKET_ACTION_DROP:
            acl_action = SWITCH_ACL_ACTION_DROP;
            break;
        case SAI_PACKET_ACTION_FORWARD:
            acl_action = SWITCH_ACL_ACTION_PERMIT;
            break;
        case SAI_PACKET_ACTION_TRAP:
            acl_action = SWITCH_ACL_ACTION_REDIRECT_TO_CPU;
            break;
        case SAI_PACKET_ACTION_LOG:
            acl_action = SWITCH_ACL_ACTION_COPY_TO_CPU;
            break;
        default:
            break;
    }
    return acl_action;
}

switch_sup_channel_t switch_sai_channel_to_switch_api_channel(
        sai_hostif_trap_channel_t trap_channel) {
    switch_sup_channel_t sup_channel = 0;
    switch (trap_channel) {
        case SAI_HOSTIF_TRAP_CHANNEL_FD:
            sup_channel = SWITCH_CHANNEL_CB;
            break;
        case SAI_HOSTIF_TRAP_CHANNEL_CB:
            sup_channel = SWITCH_CHANNEL_CB;
            break;
        case SAI_HOSTIF_TRAP_CHANNEL_NETDEV:
            sup_channel = SWITCH_CHANNEL_NETDEV;
            break;
        default:
            break;
    }
    return sup_channel;
}
/*
* Routine Description:
*    Create host interface trap
*
* Arguments:
*  [in] hostif_trap_id - host interface trap id
*  [in] attr_count - number of attributes
*  [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_create_hostif_trap(
    _In_ sai_hostif_trap_id_t hostif_trapid,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list) {
    switch_sup_code_info_t sup_code_info;
    sai_status_t status = SAI_STATUS_SUCCESS;
    const sai_attribute_t *attribute;
    int index = 0;
    memset(&sup_code_info, 0, sizeof(switch_sup_code_info_t));
    sup_code_info.sup_code = switch_sai_to_switch_api_sup_code(hostif_trapid);
    for (index = 0; index < attr_count; index++) {
        attribute = &attr_list[index];
        switch (attribute->id) {
            case SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION:
                sup_code_info.action = switch_sai_action_to_switch_api_action(attribute->value.u32);
                break;
            case SAI_HOSTIF_TRAP_ATTR_TRAP_PRIORITY:
                sup_code_info.priority = attribute->value.u32;
                break;
            case SAI_HOSTIF_TRAP_ATTR_TRAP_CHANNEL:
                sup_code_info.channel = switch_sai_channel_to_switch_api_channel(attribute->value.u32);
                break;
            case SAI_HOSTIF_TRAP_ATTR_TRAP_GROUP:
                sup_code_info.sup_group_id = attribute->value.oid;
                break;
            default:
                break;
        }
    }
    status = switch_api_sup_code_create(device, &sup_code_info);
    return status;
}

/*
* Routine Description:
*    Remove host interface trap
*
* Arguments:
*  [in] hostif_trap_id - host interface trap id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_remove_hostif_trap(
    _In_ sai_hostif_trap_id_t hostif_trapid) {
    switch_sup_code_t sup_code;
    sai_status_t status = SAI_STATUS_SUCCESS;
    sup_code = switch_sai_to_switch_api_sup_code(hostif_trapid);
    status = switch_api_sup_code_delete(device, sup_code);
    return status;
}

/*
* Routine Description:
*   Set trap attribute value.
*
* Arguments:
*    [in] hostif_trap_id - host interface trap id
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_set_hostif_trap_attribute(
    _In_ sai_hostif_trap_id_t hostif_trapid,
    _In_ const sai_attribute_t *attr) {
    switch_sup_code_info_t sup_code_info;
    sai_status_t status = SAI_STATUS_SUCCESS;
    memset(&sup_code_info, 0, sizeof(switch_sup_code_info_t));
    switch (attr->id) {
        case SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION:
            sup_code_info.action = switch_sai_action_to_switch_api_action(attr->value.u32);
            break;
        case SAI_HOSTIF_TRAP_ATTR_TRAP_PRIORITY:
            sup_code_info.priority = attr->value.u32;
            break;
        case SAI_HOSTIF_TRAP_ATTR_TRAP_CHANNEL:
            sup_code_info.channel = switch_sai_channel_to_switch_api_channel(attr->value.u32);
            break;
        case SAI_HOSTIF_TRAP_ATTR_TRAP_GROUP:
            sup_code_info.sup_group_id = attr->value.oid;
            break;
        default:
            break;
    }
    status = switch_api_sup_code_update(device, &sup_code_info);
    return status;
}

/*
* Routine Description:
*   Get trap attribute value.
*
* Arguments:
*    [in] hostif_trap_id - host interface trap id
*    [in] attr_count - number of attributes
*    [in,out] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_get_hostif_trap_attribute(
    _In_ sai_hostif_trap_id_t hostif_trapid,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list) {
    return SAI_STATUS_SUCCESS;
}

/*
* Routine Description:
*   Set user defined trap attribute value.
*
* Arguments:
*    [in] hostif_user_defined_trap_id - host interface user defined trap id
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_set_hostif_user_defined_trap_attribute(
    _In_ sai_hostif_user_defined_trap_id_t hostif_user_defined_trapid,
    _In_ const sai_attribute_t *attr) {
    return SAI_STATUS_SUCCESS;
}

/*
* Routine Description:
*   Get user defined trap attribute value.
*
* Arguments:
*    [in] hostif_user_defined_trap_id - host interface user defined trap id
*    [in] attr_count - number of attributes
*    [in,out] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_get_hostif_user_defined_trap_attribute(
    _In_ sai_hostif_user_defined_trap_id_t hostif_user_defined_trapid,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list) {
    return SAI_STATUS_SUCCESS;
}


/*
* Routine Description:
*   hostif receive function
*
* Arguments:
*    [in]  hif_id  - host interface id
*    [out] buffer - packet buffer
*    [in,out] buffer_size - [in] allocated buffer size. [out] actual packet size in bytes
*    [in,out] attr_count - [in] allocated list size. [out] number of attributes
*    [out] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    SAI_STATUS_BUFFER_OVERFLOW if buffer_size is insufficient,
*    and buffer_size will be filled with required size. Or
*    if attr_count is insufficient, and attr_count
*    will be filled with required count.
*    Failure status code on error
*/
sai_status_t sai_recv_hostif_packet(
    _In_ sai_object_id_t  hif_id,
    _Out_ void *buffer,
    _Inout_ sai_size_t *buffer_size,
    _Inout_ uint32_t *attr_count,
    _Out_ sai_attribute_t *attr_list) {
    return SAI_STATUS_SUCCESS;
}

bool switch_sai_tx_type_to_switch_api_tx_type(
        sai_hostif_tx_type_t tx_type) {
    bool bypass = TRUE;
    switch (tx_type) {
        case SAI_HOSTIF_TX_TYPE_PIPELINE_BYPASS:
            bypass = TRUE;
            break;
        case SAI_HOSTIF_TX_TYPE_PIPELINE_LOOKUP:
            bypass = FALSE;
            break;
        default:
            break;
    }
    return bypass;
}

/*
* Routine Description:
*   hostif send function
*
* Arguments:
*    [in] hif_id  - host interface id. only valid for send through FD channel. Use SAI_NULL_OBJECT_ID for send through CB channel.
*    [In] buffer - packet buffer
*    [in] buffer size - packet size in bytes
*    [in] attr_count - number of attributes
*    [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_send_hostif_packet(
    _In_ sai_object_id_t  hif_id,
    _In_ void *buffer,
    _In_ sai_size_t buffer_size,
    _In_ uint32_t attr_count,
    _In_ sai_attribute_t *attr_list) {
    switch_sup_packet_t sup_packet;
    sai_status_t status = SAI_STATUS_SUCCESS;
    memset(&sup_packet, 0, sizeof(switch_sup_packet_t));
    sup_packet.pkt = buffer;
    sup_packet.pkt_size = buffer_size;
    const sai_attribute_t *attribute;
    int index = 0;
    for (index = 0; index < attr_count; index++) {
        attribute = &attr_list[index];
        switch (attribute->id) {
            case SAI_HOSTIF_PACKET_TX_TYPE:
                sup_packet.tx_bypass = switch_sai_tx_type_to_switch_api_tx_type(attribute->value.u32);
                break;
            case SAI_HOSTIF_PACKET_EGRESS_PORT_OR_LAG:
                sup_packet.handle = attribute->value.oid;
                //Set is_lag flag if oid is lag
                break;
            default:
                break;
        }
    }
    status = switch_api_sup_tx_packet_to_hw(device, &sup_packet);
    return status;
}

/*
* Routine Description:
*   hostif receive callback
*
* Arguments:
*    [in] buffer - packet buffer
*    [in] buffer_size - actual packet size in bytes
*    [in] attr_count - number of attributes
*    [in] attr_list - array of attributes
*
* Return Values:
*/
void sai_recv_hostif_packet_cb(
    switch_sup_packet_t *sup_packet) {
    int max_attr_count = 2;
    int attr_count = 0;
    sai_attribute_t attr_list[max_attr_count];
    sai_attribute_t *attribute;
    attribute = &attr_list[attr_count];
    attribute->id = SAI_HOSTIF_PACKET_TRAP_ID;
    attribute->value.u32 = sup_packet->sup_code;
    attr_count++;
    attribute = &attr_list[attr_count];
    attribute->id = SAI_HOSTIF_PACKET_INGRESS_PORT;
    attribute->value.oid = sup_packet->handle;
    attr_count++;
    sai_switch_notifications.on_packet_event(sup_packet->pkt,
                                             sup_packet->pkt_size,
                                             attr_count,
                                             attr_list);
    return;
}

/*
* hostif methods table retrieved with sai_api_query()
*/
sai_hostif_api_t hostif_api = {
    .create_hostif                            =                NULL,
    .remove_hostif                            =                NULL,
    .set_hostif_attribute                     =                NULL,
    .get_hostif_attribute                     =                NULL,
    .create_hostif_trap_group                 =                sai_create_hostif_trap_group,
    .remove_hostif_trap_group                 =                sai_remove_hostif_trap_group,
    .set_trap_group_attribute                 =                sai_set_hostif_trap_group_attribute,
    .get_trap_group_attribute                 =                sai_get_hostif_trap_group_attribute, 
    .set_trap_attribute                       =                sai_set_hostif_trap_attribute,
    .get_trap_attribute                       =                sai_get_hostif_trap_attribute,
    .set_user_defined_trap_attribute          =                sai_set_hostif_user_defined_trap_attribute,
    .get_user_defined_trap_attribute          =                sai_get_hostif_user_defined_trap_attribute,
    .recv_packet                              =                sai_recv_hostif_packet,
    .send_packet                              =                sai_send_hostif_packet
};

sai_status_t sai_hostif_initialize(sai_api_service_t *sai_api_service) {
    sai_api_service->hostif_api = hostif_api;
    switch_api_sup_register_rx_callback(device, &sai_recv_hostif_packet_cb);
    return SAI_STATUS_SUCCESS;
}
