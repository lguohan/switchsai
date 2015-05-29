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
#include <saitypes.h>
#include <assert.h>
#include <switchapi/switch_base_types.h>

#ifndef __SAIINTERNAL_H_
#define __SAIINTERNAL_H_

extern switch_device_t device;

sai_status_t sai_switch_initialize(sai_api_service_t *sai_api_service);
sai_status_t sai_port_initialize(sai_api_service_t *sai_api_service);
sai_status_t sai_fdb_initialize(sai_api_service_t *sai_api_service);
sai_status_t sai_vlan_initialize(sai_api_service_t *sai_api_service);
sai_status_t sai_lag_initialize(sai_api_service_t *sai_api_service);
sai_status_t sai_router_interface_initialize(sai_api_service_t *sai_api_service);
sai_status_t sai_next_hop_initialize(sai_api_service_t *sai_api_service);
sai_status_t sai_next_hop_group_initialize(sai_api_service_t *sai_api_service);
sai_status_t sai_route_initialize(sai_api_service_t *sai_api_service);
sai_status_t sai_virtual_router_initialize(sai_api_service_t *sai_api_service);
sai_status_t sai_stp_initialize(sai_api_service_t *sai_api_service);
sai_status_t sai_neighbor_initialize(sai_api_service_t *sai_api_service);

unsigned int sai_v4_prefix_length(sai_ip4_t ip4);
unsigned int sai_v6_prefix_length(const sai_ip6_t ip6);

#endif  // __SAIINTERNAL_H_
