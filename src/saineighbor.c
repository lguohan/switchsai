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

#include <saineighbor.h>
#include "saiinternal.h"
#include <switchapi/switch_neighbor.h>
#include <switchapi/switch_nhop.h>

static void sai_neighbor_entry_parse(
        const sai_neighbor_entry_t *neighbor_entry,
        switch_api_neighbor_t *api_neighbor) {
    api_neighbor->interface = (switch_handle_t) neighbor_entry->rif_id;
    if (neighbor_entry->ip_address.addr_family == SAI_IP_ADDR_FAMILY_IPV4) {
        api_neighbor->rw_type = SWITCH_API_NEIGHBOR_RW_TYPE_L3;
        api_neighbor->ip_addr.type = SWITCH_API_IP_ADDR_V4;
        api_neighbor->ip_addr.ip.v4addr = neighbor_entry->ip_address.addr.ip4;
        api_neighbor->ip_addr.prefix_len = 32;
    } else if (neighbor_entry->ip_address.addr_family == SAI_IP_ADDR_FAMILY_IPV6) {
        api_neighbor->rw_type = SWITCH_API_NEIGHBOR_RW_TYPE_L3;
        api_neighbor->ip_addr.type = SWITCH_API_IP_ADDR_V6;
        memcpy(api_neighbor->ip_addr.ip.v6addr, neighbor_entry->ip_address.addr.ip6, 16);
        api_neighbor->ip_addr.prefix_len = 128;
    }
}

static void sai_neighbor_entry_attribute_parse(
        uint32_t attr_count,
        const sai_attribute_t *attr_list,
        switch_api_neighbor_t *api_neighbor) {
    const sai_attribute_t *attribute;
    uint32_t index = 0;
    for (index = 0; index < attr_count; index++) {
        attribute = &attr_list[index];
        switch (attribute->id) {
            case SAI_NEIGHBOR_ATTR_DST_MAC_ADDRESS:
                memcpy(&api_neighbor->mac_addr, attribute->value.mac, sizeof(switch_mac_addr_t));
                break;
            case SAI_NEIGHBOR_ATTR_PACKET_ACTION:
                break;
        }
    }
}

static void sai_neighbor_entry_nexthop_get(
        switch_api_neighbor_t *api_neighbor) {
    switch_ip_addr_t ip_addr;
    switch_nhop_key_t nhop_key;
    memset(&ip_addr, 0, sizeof(switch_ip_addr_t));
    memset(&nhop_key, 0, sizeof(switch_nhop_key_t));
    nhop_key.intf_handle = api_neighbor->interface;
    memcpy(&nhop_key.ip_addr, &api_neighbor->ip_addr, sizeof(switch_ip_addr_t));
    nhop_key.ip_addr_valid = 1;
    api_neighbor->nhop_handle = switch_api_nhop_handle_get(&nhop_key);
}

/*
* Routine Description:
*    Create neighbor entry 
*
* Arguments:
*    [in] neighbor_entry - neighbor entry 
*    [in] attr_count - number of attributes
*    [in] attrs - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*
* Note: IP address expected in Network Byte Order.
*/
sai_status_t sai_create_neighbor_entry(
        _In_ const sai_neighbor_entry_t* neighbor_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    switch_api_neighbor_t api_neighbor;

    memset(&api_neighbor, 0, sizeof(switch_api_neighbor_t));
    sai_neighbor_entry_parse(neighbor_entry, &api_neighbor);
    sai_neighbor_entry_attribute_parse(attr_count, attr_list, &api_neighbor);
    sai_neighbor_entry_nexthop_get(&api_neighbor);
    switch_api_neighbor_entry_add(device, &api_neighbor);
    return (sai_status_t) status;
}

/*
* Routine Description:
*    Remove neighbor entry 
*
* Arguments:
*    [in] neighbor_entry - neighbor entry 
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*
* Note: IP address expected in Network Byte Order.
*/
sai_status_t sai_remove_neighbor_entry(
        _In_ const sai_neighbor_entry_t* neighbor_entry) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    switch_api_neighbor_t api_neighbor;
    switch_handle_t neighbor_handle;
    memset(&api_neighbor, 0, sizeof(switch_api_neighbor_t));
    sai_neighbor_entry_parse(neighbor_entry, &api_neighbor);
    sai_neighbor_entry_nexthop_get(&api_neighbor);
    neighbor_handle = switch_api_neighbor_handle_get(api_neighbor.nhop_handle);
    status = switch_api_neighbor_entry_remove(device, neighbor_handle);
    return (sai_status_t) status;
}

/*
* Routine Description:
*    Set neighbor attribute value
*
* Arguments:
*    [in] neighbor_entry - neighbor entry
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_set_neighbor_entry_attribute(
        _In_ const sai_neighbor_entry_t* neighbor_entry,
        _In_ const sai_attribute_t *attr) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    return (sai_status_t) status;
}

/*
* Routine Description:
*    Get neighbor attribute value
*
* Arguments:
*    [in] neighbor_entry - neighbor entry
*    [in] attr_count - number of attributes
*    [inout] attrs - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_get_neighbor_entry_attribute(
        _In_ const sai_neighbor_entry_t* neighbor_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    return (sai_status_t) status;
}

/*
*  Neighbor methods table retrieved with sai_api_query()
*/
sai_neighbor_api_t neighbor_api = {
    .create_neighbor_entry                  =             sai_create_neighbor_entry,
    .remove_neighbor_entry                  =             sai_remove_neighbor_entry,
    .set_neighbor_attribute                 =             sai_set_neighbor_entry_attribute,
    .get_neighbor_attribute                 =             sai_get_neighbor_entry_attribute
};

sai_status_t sai_neighbor_initialize(sai_api_service_t *sai_api_service) {
    sai_api_service->neighbor_api = neighbor_api;
    return SAI_STATUS_SUCCESS;
}
