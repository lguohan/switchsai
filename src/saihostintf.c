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
#include "sailog.h"
#include <switchapi/switch_hostif.h>

/*
* Routine Description:
*    Create host interface
*
* Arguments:
*    [out] hif_id - host interface id
*    [in] attr_count - number of attributes
*    [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_create_hostif(
    _Out_ sai_object_id_t * hif_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list) {

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    const sai_attribute_t *attribute;
    uint32_t index = 0;
    switch_hostif_t hostif;
    for (index = 0; index < attr_count; index++) {
        attribute = &attr_list[index];
        switch (attribute->id) {
            case SAI_HOSTIF_ATTR_TYPE:
                if (attribute->value.u32 != SAI_HOSTIF_TYPE_NETDEV) {
                    return SAI_STATUS_FAILURE;
                }
                break;
            case SAI_HOSTIF_ATTR_RIF_OR_PORT_ID:
                hostif.handle = attribute->value.oid;
                break;
            case SAI_HOSTIF_ATTR_NAME:
                memcpy(hostif.intf_name, attribute->value.chardata, HOSTIF_NAME_SIZE);
                break;
            default:
                break;
        }
    }
    *hif_id = switch_api_hostif_create(device, &hostif);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);

    return SAI_STATUS_SUCCESS;
}

/*
* Routine Description:
*    Remove host interface
*
* Arguments:
*    [in] hif_id - host interface id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_remove_hostif(
    _In_ sai_object_id_t hif_id) {

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    sai_status_t status = SAI_STATUS_SUCCESS;
    status = switch_api_hostif_delete(device, hif_id);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);
    return status;
}

/*
* Routine Description:
*    Set host interface attribute
*
* Arguments:
*    [in] hif_id - host interface id
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_set_hostif_attribute(
    _In_ sai_object_id_t hif_id,
    _In_ const sai_attribute_t *attr) {

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);

    return SAI_STATUS_SUCCESS;
}

/*
* Routine Description:
*    Get host interface attribute
*
* Arguments:
*    [in] hif_id - host interface id
*    [in] attr_count - number of attributes
*    [inout] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_get_hostif_attribute(
    _In_ sai_object_id_t  hif_id,
    _In_ uint32_t attr_count,
    _Inout_ sai_attribute_t *attr_list) {

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);

    return SAI_STATUS_SUCCESS;
}

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

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    const sai_attribute_t *attribute;
    switch_hostif_group_t hostif_group;
    uint32_t index = 0;
    memset(&hostif_group, 0, sizeof(switch_hostif_group_t));
    for (index = 0; index < attr_count; index++) {
        attribute = &attr_list[index];   
        switch (attribute->id) {
            case SAI_HOSTIF_TRAP_GROUP_ATTR_ADMIN_STATE:
                break;
            case SAI_HOSTIF_TRAP_GROUP_ATTR_PRIO:
                hostif_group.priority = attribute->value.u32;
                break;
            case SAI_HOSTIF_TRAP_GROUP_ATTR_QUEUE:
                hostif_group.egress_queue = attribute->value.u32;
                break;
            case SAI_HOSTIF_TRAP_GROUP_ATTR_POLICER:
                break;
        }
    }
    *hostif_trap_group_id = switch_api_hostif_group_create(device, &hostif_group);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);

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

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    sai_status_t status = SAI_STATUS_SUCCESS;
    status = switch_api_hostif_group_delete(device, hostif_trap_group_id);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);

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

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);

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

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);

    return SAI_STATUS_SUCCESS;
}

switch_hostif_reason_code_t switch_sai_to_switch_api_reason_code(
        sai_hostif_trap_id_t trap_id) {
    switch_hostif_reason_code_t reason_code = SWITCH_HOSTIF_REASON_CODE_NONE;
    switch (trap_id) {
        case SAI_HOSTIF_TRAP_ID_STP:
            reason_code = SWITCH_HOSTIF_REASON_CODE_STP;
            break;
        case SAI_HOSTIF_TRAP_ID_LACP:
            reason_code = SWITCH_HOSTIF_REASON_CODE_LACP;
            break;
        case SAI_HOSTIF_TRAP_ID_EAPOL:
            reason_code = SWITCH_HOSTIF_REASON_CODE_EAPOL;
            break;
        case SAI_HOSTIF_TRAP_ID_LLDP:
            reason_code = SWITCH_HOSTIF_REASON_CODE_LLDP;
            break;
        case SAI_HOSTIF_TRAP_ID_PVRST:
            reason_code = SWITCH_HOSTIF_REASON_CODE_PVRST;
            break;
        case SAI_HOSTIF_TRAP_ID_IGMP_TYPE_QUERY:
            reason_code = SWITCH_HOSTIF_REASON_CODE_IGMP_TYPE_QUERY;
            break;
        case SAI_HOSTIF_TRAP_ID_IGMP_TYPE_LEAVE:
            reason_code = SWITCH_HOSTIF_REASON_CODE_IGMP_TYPE_LEAVE;
            break;
        case SAI_HOSTIF_TRAP_ID_IGMP_TYPE_V1_REPORT:
            reason_code = SWITCH_HOSTIF_REASON_CODE_IGMP_TYPE_V1_REPORT;
            break;
        case SAI_HOSTIF_TRAP_ID_IGMP_TYPE_V2_REPORT:
            reason_code = SWITCH_HOSTIF_REASON_CODE_IGMP_TYPE_V2_REPORT;
            break;
        case SAI_HOSTIF_TRAP_ID_IGMP_TYPE_V3_REPORT:
            reason_code = SWITCH_HOSTIF_REASON_CODE_IGMP_TYPE_V3_REPORT;
            break;
        case SAI_HOSTIF_TRAP_ID_SAMPLEPACKET:
            reason_code = SWITCH_HOSTIF_REASON_CODE_SAMPLEPACKET;
            break;
        case SAI_HOSTIF_TRAP_ID_ARP_REQUEST:
            reason_code = SWITCH_HOSTIF_REASON_CODE_ARP_REQUEST;
            break;
        case SAI_HOSTIF_TRAP_ID_ARP_RESPONSE:
            reason_code = SWITCH_HOSTIF_REASON_CODE_ARP_RESPONSE;
            break;
        case SAI_HOSTIF_TRAP_ID_DHCP:
            reason_code = SWITCH_HOSTIF_REASON_CODE_DHCP;
            break;
        case SAI_HOSTIF_TRAP_ID_OSPF:
            reason_code = SWITCH_HOSTIF_REASON_CODE_OSPF;
            break;
        case SAI_HOSTIF_TRAP_ID_PIM:
            reason_code = SWITCH_HOSTIF_REASON_CODE_PIM;
            break;
        case SAI_HOSTIF_TRAP_ID_VRRP:
            reason_code = SWITCH_HOSTIF_REASON_CODE_VRRP;
            break;
        case SAI_HOSTIF_TRAP_ID_BGP:
            reason_code = SWITCH_HOSTIF_REASON_CODE_BGP;
            break;
        case SAI_HOSTIF_TRAP_ID_DHCPV6:
            reason_code = SWITCH_HOSTIF_REASON_CODE_DHCPV6;
            break;
        case SAI_HOSTIF_TRAP_ID_OSPFV6:
            reason_code = SWITCH_HOSTIF_REASON_CODE_OSPFV6;
            break;
        case SAI_HOSTIF_TRAP_ID_VRRPV6:
            reason_code = SWITCH_HOSTIF_REASON_CODE_VRRPV6;
            break;
        case SAI_HOSTIF_TRAP_ID_BGPV6:
            reason_code = SWITCH_HOSTIF_REASON_CODE_BGPV6;
            break;
        case SAI_HOSTIF_TRAP_ID_IPV6_NEIGHBOR_DISCOVERY:
            reason_code = SWITCH_HOSTIF_REASON_CODE_IPV6_NEIGHBOR_DISCOVERY;
            break;
        case SAI_HOSTIF_TRAP_ID_IPV6_MLD_V1_V2:
            reason_code = SWITCH_HOSTIF_REASON_CODE_IPV6_MLD_V1_V2;
            break;
        case SAI_HOSTIF_TRAP_ID_IPV6_MLD_V1_REPORT:
            reason_code = SWITCH_HOSTIF_REASON_CODE_IPV6_MLD_V1_REPORT;
            break;
        case SAI_HOSTIF_TRAP_ID_IPV6_MLD_V1_DONE:
            reason_code = SWITCH_HOSTIF_REASON_CODE_IPV6_MLD_V1_DONE;
            break;
        case SAI_HOSTIF_TRAP_ID_MLD_V2_REPORT:
            reason_code = SWITCH_HOSTIF_REASON_CODE_MLD_V2_REPORT;
            break;
        case SAI_HOSTIF_TRAP_ID_L3_MTU_ERROR:
            reason_code = SWITCH_HOSTIF_REASON_CODE_L3_MTU_ERROR;
            break;
        case SAI_HOSTIF_TRAP_ID_TTL_ERROR:
            reason_code = SWITCH_HOSTIF_REASON_CODE_TTL_ERROR;
            break;
        default:
            break;
    }
    return reason_code;
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

switch_hostif_channel_t switch_sai_channel_to_switch_api_channel(
        sai_hostif_trap_channel_t trap_channel) {
    switch_hostif_channel_t hostif_channel = 0;
    switch (trap_channel) {
        case SAI_HOSTIF_TRAP_CHANNEL_FD:
            hostif_channel = SWITCH_HOSTIF_CHANNEL_CB;
            break;
        case SAI_HOSTIF_TRAP_CHANNEL_CB:
            hostif_channel = SWITCH_HOSTIF_CHANNEL_CB;
            break;
        case SAI_HOSTIF_TRAP_CHANNEL_NETDEV:
            hostif_channel = SWITCH_HOSTIF_CHANNEL_NETDEV;
            break;
        default:
            break;
    }
    return hostif_channel;
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

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    switch_api_hostif_rcode_info_t rcode_api_info;
    sai_status_t status = SAI_STATUS_SUCCESS;
    const sai_attribute_t *attribute;
    uint32_t index = 0;
    memset(&rcode_api_info, 0, sizeof(switch_api_hostif_rcode_info_t));
    rcode_api_info.reason_code = switch_sai_to_switch_api_reason_code(hostif_trapid);
    for (index = 0; index < attr_count; index++) {
        attribute = &attr_list[index];
        switch (attribute->id) {
            case SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION:
                rcode_api_info.action = switch_sai_action_to_switch_api_action(attribute->value.u32);
                break;
            case SAI_HOSTIF_TRAP_ATTR_TRAP_PRIORITY:
                rcode_api_info.priority = attribute->value.u32;
                break;
            case SAI_HOSTIF_TRAP_ATTR_TRAP_CHANNEL:
                rcode_api_info.channel = switch_sai_channel_to_switch_api_channel(attribute->value.u32);
                break;
            case SAI_HOSTIF_TRAP_ATTR_TRAP_GROUP:
                rcode_api_info.hostif_group_id = attribute->value.oid;
                break;
            default:
                break;
        }
    }
    status = switch_api_hostif_reason_code_create(device, &rcode_api_info);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);

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

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    switch_hostif_reason_code_t reason_code;
    sai_status_t status = SAI_STATUS_SUCCESS;
    reason_code = switch_sai_to_switch_api_reason_code(hostif_trapid);
    status = switch_api_hostif_reason_code_delete(device, reason_code);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);

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

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    switch_api_hostif_rcode_info_t rcode_api_info;
    sai_status_t status = SAI_STATUS_SUCCESS;
    memset(&rcode_api_info, 0, sizeof(switch_api_hostif_rcode_info_t));
    rcode_api_info.reason_code = switch_sai_to_switch_api_reason_code(hostif_trapid);
    switch (attr->id) {
        case SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION:
            rcode_api_info.action = switch_sai_action_to_switch_api_action(attr->value.u32);
            break;
        case SAI_HOSTIF_TRAP_ATTR_TRAP_PRIORITY:
            rcode_api_info.priority = attr->value.u32;
            break;
        case SAI_HOSTIF_TRAP_ATTR_TRAP_CHANNEL:
            rcode_api_info.channel = switch_sai_channel_to_switch_api_channel(attr->value.u32);
            break;
        case SAI_HOSTIF_TRAP_ATTR_TRAP_GROUP:
            rcode_api_info.hostif_group_id = attr->value.oid;
            break;
        default:
            break;
    }
    status = switch_api_hostif_reason_code_update(device, &rcode_api_info);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);

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

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);

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

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);

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

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);

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

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);

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

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    switch_hostif_packet_t hostif_packet;
    sai_status_t status = SAI_STATUS_SUCCESS;
    memset(&hostif_packet, 0, sizeof(switch_hostif_packet_t));
    hostif_packet.pkt = buffer;
    hostif_packet.pkt_size = buffer_size;
    const sai_attribute_t *attribute;
    uint32_t index = 0;
    for (index = 0; index < attr_count; index++) {
        attribute = &attr_list[index];
        switch (attribute->id) {
            case SAI_HOSTIF_PACKET_TX_TYPE:
                hostif_packet.tx_bypass = switch_sai_tx_type_to_switch_api_tx_type(attribute->value.u32);
                break;
            case SAI_HOSTIF_PACKET_EGRESS_PORT_OR_LAG:
                hostif_packet.handle = attribute->value.oid;
                //Set is_lag flag if oid is lag
                break;
            default:
                break;
        }
    }
    status = switch_api_hostif_tx_packet(device, &hostif_packet);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);

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
    switch_hostif_packet_t *hostif_packet) {

    SAI_LOG_ENTER(SAI_API_HOST_INTERFACE);

    int max_attr_count = 2;
    int attr_count = 0;
    sai_attribute_t attr_list[max_attr_count];
    sai_attribute_t *attribute;
    attribute = &attr_list[attr_count];
    attribute->id = SAI_HOSTIF_PACKET_TRAP_ID;
    attribute->value.u32 = hostif_packet->reason_code;
    attr_count++;
    attribute = &attr_list[attr_count];
    attribute->id = SAI_HOSTIF_PACKET_INGRESS_PORT;
    attribute->value.oid = hostif_packet->handle;
    attr_count++;
    if( sai_switch_notifications.on_packet_event)
        sai_switch_notifications.on_packet_event(hostif_packet->pkt,
                                             hostif_packet->pkt_size,
                                             attr_count,
                                             attr_list);

    SAI_LOG_EXIT(SAI_API_HOST_INTERFACE);

    return;
}

/*
* hostif methods table retrieved with sai_api_query()
*/
sai_hostif_api_t hostif_api = {
    .create_hostif                            =                sai_create_hostif,
    .remove_hostif                            =                sai_remove_hostif,
    .set_hostif_attribute                     =                sai_set_hostif_attribute,
    .get_hostif_attribute                     =                sai_get_hostif_attribute,
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
    switch_api_hostif_register_rx_callback(device, &sai_recv_hostif_packet_cb);
    return SAI_STATUS_SUCCESS;
}
