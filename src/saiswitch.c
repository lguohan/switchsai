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

#include <saivlan.h>
#include "saiinternal.h"
#include <switchapi/switch_vlan.h>
#include <switchapi/switch_capability.h>

sai_switch_notification_t sai_switch_notifications;

sai_status_t sai_initialize_switch(
        _In_ sai_switch_profile_id_t profile_id,
        _In_reads_z_(SAI_MAX_HARDWARE_ID_LEN) char* switch_hardware_id,
        _In_reads_opt_z_(SAI_MAX_FIRMWARE_PATH_NAME_LEN) char* firmware_path_name,
        _In_ sai_switch_notification_t* switch_notifications) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    memcpy(&sai_switch_notifications, switch_notifications, sizeof(sai_switch_notification_t));
    return (sai_status_t) status;
}

void sai_shutdown_switch(
        _In_ bool warm_restart_hint) {
}

sai_status_t sai_connect_switch(
        _In_ sai_switch_profile_id_t profile_id,
        _In_reads_z_(SAI_MAX_HARDWARE_ID_LEN) char* switch_hardware_id,
        _In_ sai_switch_notification_t* switch_notifications) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    return (sai_status_t) status;
}

void sai_disconnect_switch(void) {
}

/*
* Routine Description:
*    Set switch attribute value
*
* Arguments:
*    [in] attr - switch attribute
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_set_switch_attribute(
        _In_ const sai_attribute_t *attr) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    switch_api_capability_t api_switch_info;

    switch (attr->id) {
        case SAI_SWITCH_ATTR_SRC_MAC_ADDRESS:
            memcpy(&api_switch_info.switch_mac, &attr->value.mac, 6);
            break;
    }
    switch_api_capability_set(device, &api_switch_info);
    return (sai_status_t) status;
}

/*
* Routine Description:
*    Get switch attribute value
*
* Arguments:
*    [in] attr_count - number of switch attributes
*    [inout] attr_list - array of switch attributes
*
* Return Values:
*    SAI_STATUS_SUCCESS on success
*    Failure status code on error
*/
sai_status_t sai_get_switch_attribute(
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    int index1 = 0, index2 = 0;
    sai_object_list_t *objlist = NULL;
    switch_api_capability_t api_switch_info;
    sai_attribute_t attribute;

    switch_api_capability_get(device, &api_switch_info);
    for (index1 = 0; index1 < attr_count; index1++) {
        attribute = attr_list[index1];
        switch (attribute.id) {
            case SAI_SWITCH_ATTR_PORT_NUMBER:
                attr_list->value.u32 = api_switch_info.max_ports;
                break;
            case SAI_SWITCH_ATTR_PORT_LIST:
                objlist = &attr_list->value.objlist;
                objlist->count = api_switch_info.max_ports;
                for (index2 = 0; index2 < objlist->count; index2++) {
                    objlist->list[index2] = api_switch_info.port_list[index2];
                }
                break;
            case SAI_SWITCH_ATTR_DEFAULT_PORT_VLAN_ID:
                break;
            case SAI_SWITCH_ATTR_LAG_HASH_ALGO:
            case SAI_SWITCH_ATTR_LAG_HASH_SEED:
            case SAI_SWITCH_ATTR_LAG_HASH_FIELDS:
                break;
            case SAI_SWITCH_ATTR_ECMP_HASH_ALGO:
            case SAI_SWITCH_ATTR_ECMP_HASH_SEED:
            case SAI_SWITCH_ATTR_ECMP_HASH_FIELDS:
                break;
            case SAI_SWITCH_ATTR_MAX_VIRTUAL_ROUTERS:
                break;
            case SAI_SWITCH_ATTR_DEFAULT_STP_INST_ID:
                break;
            case SAI_SWITCH_ATTR_SRC_MAC_ADDRESS:
                memcpy(attribute.value.mac, &api_switch_info.switch_mac, 6);
                break;

        }
    }
    return (sai_status_t) status;
}

/*
* Switch method table retrieved with sai_api_query() 
*/
sai_switch_api_t switch_api = {
    .initialize_switch                 =             sai_initialize_switch,
    .shutdown_switch                   =             sai_shutdown_switch,
    .connect_switch                    =             sai_connect_switch,
    .disconnect_switch                 =             sai_disconnect_switch,
    .set_switch_attribute              =             sai_set_switch_attribute,
    .get_switch_attribute              =             sai_get_switch_attribute
};

sai_status_t sai_switch_initialize(sai_api_service_t *sai_api_service) {
    sai_api_service->switch_api = switch_api;
    return SAI_STATUS_SUCCESS;
}
