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

#include "saiapi.h"
#include "saiinternal.h"

sai_api_service_t sai_api_service;
switch_device_t device = 0;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

sai_status_t sai_api_query(
        _In_ sai_api_t sai_api_id,
        _Out_ void ** api_method_table)
{

    sai_status_t status =  SAI_STATUS_SUCCESS;

    switch (sai_api_id) {
        case SAI_API_SWITCH:
            *api_method_table = &sai_api_service.switch_api;
            break;

        case SAI_API_PORT:
            *api_method_table = &sai_api_service.port_api;
            break;

        case SAI_API_FDB:
            *api_method_table = &sai_api_service.fdb_api;
            break;

        case SAI_API_VLAN:
            *api_method_table = &sai_api_service.vlan_api;
            break;

        case SAI_API_VIRTUAL_ROUTER:
            *api_method_table = &sai_api_service.vr_api;
            break;

        case SAI_API_ROUTER_INTERFACE:
            *api_method_table = &sai_api_service.rif_api;
            break;

        case SAI_API_ROUTE:
            *api_method_table = &sai_api_service.route_api;
            break;

        case SAI_API_NEIGHBOR:
            *api_method_table = &sai_api_service.neighbor_api;
            break;

        case SAI_API_NEXT_HOP:
            *api_method_table = &sai_api_service.nhop_api;
            break;

        case SAI_API_NEXT_HOP_GROUP:
            *api_method_table = &sai_api_service.nhop_group_api;
            break;

        case SAI_API_QOS:
            *api_method_table = &sai_api_service.qos_api;
            break;

        case SAI_API_ACL:
            *api_method_table = &sai_api_service.acl_api;
            break;

        case SAI_API_LAG:
            *api_method_table = &sai_api_service.lag_api;
            break;

        case SAI_API_STP:
            *api_method_table = &sai_api_service.stp_api;
            break;

        case SAI_API_HOST_INTERFACE:
            *api_method_table = &sai_api_service.hostif_api;
            break;

        case SAI_API_MIRROR:
            *api_method_table = &sai_api_service.mirror_api;
            break;

        case SAI_API_SAMPLEPACKET:
            *api_method_table = &sai_api_service.samplepacket_api;
            break;

        default:
            status = SAI_STATUS_UNINITIALIZED;
    }
    return status;
}

sai_status_t sai_initialize() {
    sai_switch_initialize(&sai_api_service);
    sai_port_initialize(&sai_api_service);
    sai_fdb_initialize(&sai_api_service);
    sai_vlan_initialize(&sai_api_service);
    sai_lag_initialize(&sai_api_service);
    sai_router_interface_initialize(&sai_api_service);
    sai_next_hop_initialize(&sai_api_service);
    sai_next_hop_group_initialize(&sai_api_service);
    sai_route_initialize(&sai_api_service);
    sai_virtual_router_initialize(&sai_api_service);
    sai_stp_initialize(&sai_api_service);
    sai_neighbor_initialize(&sai_api_service);
    return SAI_STATUS_SUCCESS;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
