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

#include <saistp.h>
#include "saiinternal.h"
#include <switchapi/switch_stp.h>
#include <switchapi/switch_vlan.h>

/**
 * @brief Create stp instance with default port state as forwarding.
 *
 * @param[out] stp_id stp instance id
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Value of attributes
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
sai_status_t sai_create_stp_entry(
        _Out_ sai_object_id_t *stp_id,
        _In_  uint32_t attr_count,
        _In_  const sai_attribute_t *attr_list) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    const sai_attribute_t *attribute;
    const sai_vlan_list_t *vlans;
    sai_vlan_id_t vlan_id = 0;
    int index1 = 0, index2 = 0;
    switch_handle_t *vlan_handle;
    *stp_id = (sai_object_id_t) switch_api_stp_group_create(device, 0);
    for (index1 = 0; index1 < attr_count; index1++) {
        attribute = &attr_list[index1];
        if (attribute->id == SAI_STP_ATTR_VLAN) {
            vlans = &attribute->value.vlanlist;
            vlan_handle = (switch_handle_t *) malloc(sizeof(switch_handle_t) * vlans->vlan_count);
            for (index2 = 0; index2 < vlans->vlan_count; index2++) {
                vlan_id = vlans->vlan_list[index2];
                switch_api_vlan_id_to_handle_get(vlan_id, &vlan_handle[index2]);
            }
            status = switch_api_stp_group_vlans_add(device, *stp_id, vlans->vlan_count, vlan_handle);
            free(vlan_handle);
        }
    }
    return (sai_status_t) status;
}

/**
 * @brief Remove stp instance.
 *
 * @param[in] stp_id stp instance id
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
sai_status_t sai_remove_stp_entry(
        _In_ sai_object_id_t stp_id) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    status = switch_api_stp_group_delete(device, (switch_handle_t)stp_id);
    return (sai_status_t) status;
}

/**
 * @brief Update stp state of a port in specified stp instance.
 *
 * @param[in] stp_id stp instance id
 * @param[in] port_id port id
 * @param[in] stp_port_state stp state of the port
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
sai_status_t sai_set_stp_entry_attribute(
        _In_ sai_object_id_t stp_id,
        _In_ const sai_attribute_t *attr) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    return (sai_status_t) status;
}

/**
 * @brief Retrieve stp state of a port in specified stp instance.
 *
 * @param[in] stp_id stp instance id
 * @param[in] port_id port id
 * @param[out] stp_port_state stp state of the port
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
sai_status_t sai_get_stp_entry_attribute(
        _In_ sai_object_id_t stp_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list) {
    return 0;
}

/**
 * @brief Set the attribute of STP instance.
 *
 * @param[in] stp_id stp instance id
 * @param[in] attr attribute value
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
sai_status_t sai_set_stp_port_state(
        _In_ sai_object_id_t stp_id,
        _In_ sai_object_id_t port_id,   
        _In_ sai_port_stp_port_state_t stp_port_state) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    switch_stp_state_t switch_stp_state = SWITCH_PORT_STP_STATE_NONE;
    switch (stp_port_state) {
        case SAI_PORT_STP_STATE_LEARNING:
            switch_stp_state = SWITCH_PORT_STP_STATE_LEARNING;
            break;
        case SAI_PORT_STP_STATE_FORWARDING:
            switch_stp_state = SWITCH_PORT_STP_STATE_FORWARDING;
            break;
        case SAI_PORT_STP_STATE_BLOCKING:
            switch_stp_state = SWITCH_PORT_STP_STATE_BLOCKING;
            break;
    }
    status = switch_api_stp_port_state_set(device, stp_id,
                                           port_id,
                                           switch_stp_state);
    return (sai_status_t) status;
}

/**
 * @brief Get the attribute of STP instance.
 *
 * @param[in] stp_id stp instance id
 * @param[in] attr_count number of the attribute
 * @param[in] attr_list attribute value
 * @return SAI_STATUS_SUCCESS if operation is successful otherwise a different
 *  error code is returned.
 */
sai_status_t sai_get_stp_port_state(
        _In_ sai_object_id_t stp_id,
        _In_ sai_object_id_t port_id,   
        _Out_ sai_port_stp_port_state_t *stp_port_state) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    switch_stp_state_t switch_stp_state = SWITCH_PORT_STP_STATE_NONE;
    status = switch_api_stp_port_state_get(device, stp_id, port_id, &switch_stp_state);
    switch (switch_stp_state) {
        case SWITCH_PORT_STP_STATE_LEARNING:
            *stp_port_state = SAI_PORT_STP_STATE_LEARNING;
            break;
        case SWITCH_PORT_STP_STATE_FORWARDING:
            *stp_port_state = SAI_PORT_STP_STATE_FORWARDING;
            break;
        case SWITCH_PORT_STP_STATE_BLOCKING:
            *stp_port_state = SAI_PORT_STP_STATE_BLOCKING;
            break;
        default:
            *stp_port_state = 0;
    }
    return (sai_status_t) status;
}

/**
 * @brief STP method table retrieved with sai_api_query()
 */
sai_stp_api_t stp_api = {
    .create_stp                        =             sai_create_stp_entry,
    .remove_stp                        =             sai_remove_stp_entry,
    .set_stp_attribute                 =             sai_set_stp_entry_attribute,
    .get_stp_attribute                 =             sai_get_stp_entry_attribute,
    .set_stp_port_state                =             sai_set_stp_port_state,
    .get_stp_port_state                =             sai_get_stp_port_state
};

sai_status_t sai_stp_initialize(sai_api_service_t *sai_api_service) {
    sai_api_service->stp_api = stp_api;
    return SAI_STATUS_SUCCESS;
}
