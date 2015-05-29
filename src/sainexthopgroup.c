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

#include <sainexthop.h>
#include "saiinternal.h"
#include <switchapi/switch_nhop.h>

sai_status_t sai_create_next_hop_group_entry(
        _Out_ sai_object_id_t* next_hop_group_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

sai_status_t sai_remove_next_hop_group_entry(
        _In_ sai_object_id_t next_hop_group_id);

sai_status_t sai_add_next_hop_to_group(
        _In_ sai_object_id_t next_hop_group_id,
        _In_ uint32_t next_hop_count,
        _In_ const sai_object_id_t* nexthops);

sai_status_t sai_remove_next_hop_from_group(
        _In_ sai_object_id_t next_hop_group_id,
        _In_ uint32_t next_hop_count,
        _In_ const sai_object_id_t* nexthops);

/*
* Routine Description:
*    Create next hop group
*
* Arguments:
*    [out] next_hop_group_id - next hop group id
*    [in] attr_count - number of attributes
*    [in] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_create_next_hop_group_entry(
        _Out_ sai_object_id_t* next_hop_group_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    sai_attribute_t attribute;
    sai_object_id_t *nhop_list;
    uint32_t nhop_count = 0;
    int index = 0;
    *next_hop_group_id = (sai_object_id_t) switch_api_ecmp_create(device);
    for (index = 0; index < attr_count; index++) {
        attribute = attr_list[index];
        switch(attribute.id) {
            case SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_COUNT:
            case SAI_NEXT_HOP_GROUP_ATTR_TYPE:
                break;
            case SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_LIST:
                nhop_list = attribute.value.objlist.list;
                nhop_count = attribute.value.objlist.count;
                status = sai_add_next_hop_to_group(*next_hop_group_id,
                                          nhop_count,
                                          nhop_list);
                break;
        }
    }
    return (sai_status_t) status;
}

/*
* Routine Description:
*    Remove next hop group 
*
* Arguments:
*    [in] next_hop_group_id - next hop group id
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_remove_next_hop_group_entry(
        _In_ sai_object_id_t next_hop_group_id) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    status = switch_api_ecmp_delete(device, (switch_handle_t) next_hop_group_id);
    return (sai_status_t) status;
}

/*
* Routine Description:
*    Set Next Hop Group attribute
*
* Arguments:
*    [in] sai_object_id_t - next_hop_group_id
*    [in] attr - attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_set_next_hop_group_entry_attribute(
        _In_ sai_object_id_t next_hop_group_id,
        _In_ const sai_attribute_t *attr) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    return (sai_status_t) status;
}

/*
* Routine Description:
*    Get Next Hop Group attribute
*
* Arguments:
*    [in] sai_object_id_t - next_hop_group_id
*    [in] attr_count - number of attributes
*    [inout] attr_list - array of attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_get_next_hop_group_entry_attribute(
        _In_ sai_object_id_t next_hop_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    return (sai_status_t) status;
}

/*
* Routine Description:
*    Add next hop to a group 
*
* Arguments:
*    [in] next_hop_group_id - next hop group id
*    [in] next_hop_count - number of next hops
*    [in] nexthops - array of next hops
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_add_next_hop_to_group(
        _In_ sai_object_id_t next_hop_group_id,
        _In_ uint32_t next_hop_count,
        _In_ const sai_object_id_t* nexthops) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    int index = 0;
    for (index = 0; index < next_hop_count; index++) {
        status = switch_api_ecmp_member_add(device,
                              (switch_handle_t) next_hop_group_id,
                              (switch_handle_t) nexthops[index]);
    }
    return (sai_status_t) status;
}

/*
* Routine Description:
*    Remove next hop from a group 
*
* Arguments:
*    [in] next_hop_group_id - next hop group id
*    [in] next_hop_count - number of next hops
*    [in] nexthops - array of next hops
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_remove_next_hop_from_group(
        _In_ sai_object_id_t next_hop_group_id,
        _In_ uint32_t next_hop_count,
        _In_ const sai_object_id_t* nexthops) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    int index = 0;
    for (index = 0; index < next_hop_count; index++) {
        status = switch_api_ecmp_member_delete(device,
                              (switch_handle_t) next_hop_group_id,
                              (switch_handle_t) nexthops[index]);
    }
    return (sai_status_t) status;
}

/*
*  Next Hop group methods table retrieved with sai_api_query()
*/
sai_next_hop_group_api_t nhop_group_api = {
    .create_next_hop_group                  =             sai_create_next_hop_group_entry,
    .remove_next_hop_group                  =             sai_remove_next_hop_group_entry,
    .set_next_hop_group_attribute           =             sai_set_next_hop_group_entry_attribute,
    .get_next_hop_group_attribute           =             sai_get_next_hop_group_entry_attribute,
    .add_next_hop_to_group                  =             sai_add_next_hop_to_group,
    .remove_next_hop_from_group             =             sai_remove_next_hop_from_group
};

sai_status_t sai_next_hop_group_initialize(sai_api_service_t *sai_api_service) {
    sai_api_service->nhop_group_api = nhop_group_api;
    return SAI_STATUS_SUCCESS;
}
