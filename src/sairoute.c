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

#include <sairoute.h>
#include "saiinternal.h"
#include <switchapi/switch_l3.h>
#include <switchapi/switch_hostif.h>
#include <arpa/inet.h>

unsigned int sai_v4_prefix_length(sai_ip4_t ip4)  {
    int x = 0;
    unsigned int prefix_length = 0;
    while (ip4) {
        x = ip4 & 0x1;
        if (x) prefix_length++;
        ip4 = ip4 >> 1;
    }
    return prefix_length;
}

unsigned int sai_v6_prefix_length(const sai_ip6_t ip6)  {
    int i = 0, x = 0;
    unsigned int prefix_length = 0;
    sai_ip6_t ip6_temp;
    memcpy(ip6_temp, ip6, 16);
    for (i = 0; i < 16; i++) {
        if (ip6_temp[i] == 0xFF) {
            prefix_length += 8;
        } else {
            while (ip6_temp[i]) {
                x = ip6_temp[i] & 0x1;
                if (x) prefix_length++;
                ip6_temp[i] = ip6_temp[i] >> 1;
            }
        }
    }
    return prefix_length;
}

static void sai_route_entry_parse(
        const sai_unicast_route_entry_t* unicast_route_entry,
        switch_handle_t *vrf_handle,
        switch_ip_addr_t *ip_addr) {
    unsigned int prefix_length = 0;
    const sai_ip_prefix_t *sai_ip_addr;
    memset(ip_addr, 0, sizeof(switch_ip_addr_t));
    sai_ip_addr = &unicast_route_entry->destination;
    *vrf_handle = (switch_handle_t) unicast_route_entry->vr_id;
    if (sai_ip_addr->addr_family == SAI_IP_ADDR_FAMILY_IPV4) {
        prefix_length = sai_v4_prefix_length(ntohl(sai_ip_addr->mask.ip4));
        ip_addr->type = SWITCH_API_IP_ADDR_V4;
        ip_addr->ip.v4addr = ntohl(sai_ip_addr->addr.ip4);
        ip_addr->prefix_len = prefix_length;
    } else if (sai_ip_addr->addr_family == SAI_IP_ADDR_FAMILY_IPV6) {
        prefix_length = sai_v6_prefix_length(sai_ip_addr->mask.ip6);
        ip_addr->type = SWITCH_API_IP_ADDR_V6;
        memcpy(ip_addr->ip.v6addr, sai_ip_addr->addr.ip6, 16);
        ip_addr->prefix_len = prefix_length;
    }
}

static void sai_route_entry_attribute_parse(
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list,
        switch_handle_t *nhop_handle,
        int *action, int *pri) {
    const sai_attribute_t *attribute;
    uint32_t index = 0;
    for (index = 0; index < attr_count; index++) {
        attribute = &attr_list[index];
        switch (attribute->id) {
            case SAI_ROUTE_ATTR_NEXT_HOP_ID:
                *nhop_handle = (switch_handle_t) attribute->value.oid;
                break;
            case SAI_ROUTE_ATTR_TRAP_PRIORITY:
                *pri = attribute->value.u8;
                break;
            case SAI_ROUTE_ATTR_PACKET_ACTION:
                *action = attribute->value.s32;
                break;
        }
    }
}

/*
* Routine Description:
*    Create Route
*
* Arguments:
*    [in] unicast_route_entry - route entry
*    [in] attr_count - number of attributes
*    [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
* 
* Note: IP prefix/mask expected in Network Byte Order.
*   
*/
sai_status_t sai_create_route_entry(
        _In_ const sai_unicast_route_entry_t* unicast_route_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    switch_ip_addr_t ip_addr;
    switch_handle_t nhop_handle = 0;
    switch_handle_t vrf_handle = 0;
    int action=-1, pri=-1;
    sai_route_entry_parse(unicast_route_entry, &vrf_handle, &ip_addr);
    sai_route_entry_attribute_parse(attr_count, attr_list, &nhop_handle, &action, &pri);
    if(!nhop_handle && action != -1) {
        switch(action) {
            case SAI_PACKET_ACTION_DROP:
                nhop_handle = switch_api_cpu_nhop_get(SWITCH_HOSTIF_REASON_CODE_NULL_DROP);
                break;
            case SAI_PACKET_ACTION_FORWARD:
                break;
            case SAI_PACKET_ACTION_TRAP:
                // set the nhop_handle to the cpu nhop
                nhop_handle = switch_api_cpu_nhop_get(SWITCH_HOSTIF_REASON_CODE_GLEAN);
                break;
            default:
                break;
        }
    }
    if (nhop_handle) {
        status = switch_api_l3_route_add(device, vrf_handle, &ip_addr, nhop_handle);
    }
    return (sai_status_t) status;
}

/*
* Routine Description:
*    Remove Route
*
* Arguments:
*    [in] unicast_route_entry - route entry
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*
* Note: IP prefix/mask expected in Network Byte Order.
*/
sai_status_t sai_remove_route_entry(
        _In_ const sai_unicast_route_entry_t* unicast_route_entry) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    switch_ip_addr_t ip_addr;
    switch_handle_t vrf_handle = 0;
    switch_handle_t nhop_handle = 0;
    sai_route_entry_parse(unicast_route_entry, &vrf_handle, &ip_addr);
    status = switch_api_l3_route_delete(device, vrf_handle, &ip_addr, nhop_handle);
    return (sai_status_t) status;
}

/*
* Routine Description:
*    Set route attribute value
*
* Arguments:
*    [in] unicast_route_entry - route entry
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_set_route_entry_attribute(
        _In_ const sai_unicast_route_entry_t* unicast_route_entry,
        _In_ const sai_attribute_t *attr) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    return (sai_status_t) status;
}

/*
* Routine Description:
*    Get route attribute value
*
* Arguments:
*    [in] unicast_route_entry - route entry
*    [in] attr_count - number of attributes
*    [inout] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_get_route_entry_attribute(
        _In_ const sai_unicast_route_entry_t* unicast_route_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    return (sai_status_t) status;
}

/*
*  Router entry methods table retrieved with sai_api_query()
*/
sai_route_api_t route_api = {
    .create_route                  =             sai_create_route_entry,
    .remove_route                  =             sai_remove_route_entry,
    .set_route_attribute           =             sai_set_route_entry_attribute,
    .get_route_attribute           =             sai_get_route_entry_attribute,
};

sai_status_t sai_route_initialize(sai_api_service_t *sai_api_service) {
    sai_api_service->route_api = route_api;
    return SAI_STATUS_SUCCESS;
}
