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

#include <sailag.h>
#include "saiinternal.h"
#include <switchapi/switch_lag.h>

sai_status_t sai_create_lag_entry(
        _Out_ sai_object_id_t* lag_id,
        _In_ uint32_t attr_count,
        _In_ sai_attribute_t *attr_list);

sai_status_t sai_remove_lag_entry(
        _In_ sai_object_id_t lag_id);

sai_status_t sai_set_lag_entry_attribute(
        _In_ sai_object_id_t lag_id,
        _In_ const sai_attribute_t *attr);

sai_status_t sai_add_ports_to_lag(
        _In_ sai_object_id_t lag_id,
        _In_ const sai_object_list_t *port_list);

/*
    \brief Create LAG
    \param[out] lag_id LAG id
    \param[in] attr_count number of attributes
    \param[in] attr_list array of attributes
    \return Success: SAI_STATUS_SUCCESS
            Failure: Failure status code on error
*/
sai_status_t sai_create_lag_entry(
        _Out_ sai_object_id_t* lag_id,
        _In_ uint32_t attr_count,
        _In_ sai_attribute_t *attr_list) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    sai_attribute_t attribute;
    int index = 0;
    *lag_id = (sai_object_id_t) switch_api_lag_create(device);
    for (index = 0; index < attr_count; index++) {
        attribute = attr_list[index];
        switch(attribute.id) {
            case SAI_LAG_ATTR_PORT_LIST:
                status = sai_add_ports_to_lag(*lag_id, &attr_list[index].value.objlist);
                break;
        }
    }
    return (sai_status_t) status;
}

/*
    \brief Remove LAG
    \param[in] lag_id LAG id
    \return Success: SAI_STATUS_SUCCESS
            Failure: Failure status code on error
*/
sai_status_t sai_remove_lag_entry(
        _In_ sai_object_id_t lag_id) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    status = switch_api_lag_delete((switch_handle_t) lag_id);
    return (sai_status_t) status;
}

/*
    \brief Set LAG Attribute
    \param[in] lag_id LAG id
    \param[in] attr Structure containing ID and value to be set
    \return Success: SAI_STATUS_SUCCESS
            Failure: Failure status code on error
*/
sai_status_t sai_set_lag_entry_attribute(
        _In_ sai_object_id_t lag_id,
        _In_ const sai_attribute_t *attr) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    return (sai_status_t) status;
}

/*
    \brief Get LAG Attribute
    \param[in] lag_id LAG id
    \param[in] attr_count Number of attributes to be get
    \param[in,out] attr_list List of structures containing ID and value to be get
    \return Success: SAI_STATUS_SUCCESS
            Failure: Failure status code on error
*/
sai_status_t sai_get_lag_entry_attribute(
        _In_ sai_object_id_t lag_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    return (sai_status_t) status;
}

/*
    \brief Add ports to LAG.
    \param[in] lag_id LAG id
    \param[in] port_count number of ports
    \param[in] port_list pointer to membership structures
    \return Success: SAI_STATUS_SUCCESS
            Failure: Failure status code on error
*/
sai_status_t sai_add_ports_to_lag(
        _In_ sai_object_id_t lag_id,
        _In_ const sai_object_list_t *port_list) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    switch_direction_t direction = SWITCH_API_DIRECTION_BOTH;
    int index = 0;
    for (index = 0; index < port_list->count; index++) {
        status = switch_api_lag_member_add(device,
                        (switch_handle_t) lag_id,
                        direction,
                        port_list->list[index]);
    }
    return (sai_status_t) status;
}

/*
    \brief Remove ports from LAG.
    \param[in] lag_id LAG id
    \param[in] port_count number of ports
    \param[in] port_list pointer to membership structures
    \return Success: SAI_STATUS_SUCCESS
            Failure: Failure status code on error
*/
sai_status_t sai_remove_ports_from_lag(
        _In_ sai_object_id_t lag_id,
        _In_ const sai_object_list_t *port_list) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    switch_direction_t direction = SWITCH_API_DIRECTION_BOTH;
    int index = 0;
    for (index = 0; index < port_list->count; index++) {
        status = switch_api_lag_member_delete(device,
                        (switch_handle_t) lag_id,
                        direction,
                        port_list->list[index]);
    }
    return (sai_status_t) status;
}

/*
*  LAG methods table retrieved with sai_api_query()
*/
sai_lag_api_t lag_api = {
    .create_lag                        =             sai_create_lag_entry,
    .remove_lag                        =             sai_remove_lag_entry,
    .set_lag_attribute                 =             sai_set_lag_entry_attribute,
    .get_lag_attribute                 =             sai_get_lag_entry_attribute,
    .add_ports_to_lag                  =             sai_add_ports_to_lag,
    .remove_ports_from_lag             =             sai_remove_ports_from_lag,
};

sai_status_t sai_lag_initialize(sai_api_service_t *sai_api_service) {
    sai_api_service->lag_api = lag_api;
    return SAI_STATUS_SUCCESS;
}
