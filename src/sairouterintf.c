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

#include <sairouterintf.h>
#include "saiinternal.h"
#include <switchapi/switch_interface.h>

/*
* Routine Description:
*    Create router interface. 
*
* Arguments:
*    [out] rif_id - router interface id
*    [in] attr_count - number of attributes
*    [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_create_router_interface(
        _Out_ sai_object_id_t* rif_id,
        _In_ uint32_t attr_count,
        _In_ sai_attribute_t *attr_list) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    switch_api_interface_info_t intf_info;
    const sai_attribute_t *attribute;
    sai_router_interface_type_t sai_intf_type;
    int index = 0;
    memset(&intf_info, 0, sizeof(switch_api_interface_info_t));
    for (index = 0; index < attr_count; index++) {
        attribute = &attr_list[index];
        switch (attribute->id) {
            case SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID:
                intf_info.vrf_handle = (switch_handle_t) attribute->value.oid;
                break;
            case SAI_ROUTER_INTERFACE_ATTR_TYPE:
                sai_intf_type = attribute->value.u8;
                break;
            case SAI_ROUTER_INTERFACE_ATTR_PORT_ID:
                assert(sai_intf_type == SAI_ROUTER_INTERFACE_TYPE_PORT);
                intf_info.type = SWITCH_API_INTERFACE_L3;
                intf_info.u.port_lag_handle = attribute->value.oid;
                break;
            case SAI_ROUTER_INTERFACE_ATTR_VLAN_ID:
                assert(sai_intf_type == SAI_ROUTER_INTERFACE_TYPE_VLAN);
                intf_info.type = SWITCH_API_INTERFACE_L3_VLAN;
                intf_info.u.vlan_id = attribute->value.u16;
                break;
            case SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS:
                break;
            case SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE:
                intf_info.ipv4_unicast_enabled = attribute->value.booldata;
                break;
            case SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE:
                intf_info.ipv6_unicast_enabled = attribute->value.booldata;
                break;
            default:
                return SAI_STATUS_INVALID_PARAMETER; 
        }
    }
    *rif_id = (sai_object_id_t) switch_api_interface_create(device, &intf_info);
    return (sai_status_t) status;
}

/*
* Routine Description:
*    Remove router interface
*
* Arguments:
*    [in] rif_id - router interface id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_remove_router_interface(
        _In_ sai_object_id_t rif_id) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    status = switch_api_interface_delete((switch_handle_t)rif_id);
    return (sai_status_t) status;
}

/*
* Routine Description:
*    Set router interface attribute
*
* Arguments:
*    [in] rif_id - router interface id
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_set_router_interface_attribute(
        _In_ sai_object_id_t rif_id,
        _In_ const sai_attribute_t *attr) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    return (sai_status_t) status;
}

/*
* Routine Description:
*    Get router interface attribute
*
* Arguments:
*    [in] rif_id - router interface id
*    [in] attr_count - number of attributes
*    [inout] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_get_router_interface_attribute(
        _In_ sai_object_id_t rif_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    return (sai_status_t) status;
}

/*
*  Routing interface methods table retrieved with sai_api_query()
*/
sai_router_interface_api_t rif_api = {
    .create_router_interface                =             sai_create_router_interface,
    .remove_router_interface                =             sai_remove_router_interface,
    .set_router_interface_attribute         =             sai_set_router_interface_attribute,
    .get_router_interface_attribute         =             sai_get_router_interface_attribute,
};

sai_status_t sai_router_interface_initialize(sai_api_service_t *sai_api_service) {
    sai_api_service->rif_api = rif_api;
    return SAI_STATUS_SUCCESS;
}
