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
#include "switchapi/switch_handle.h"
#include "switchapi/switch_nhop.h"
#include <p4_sim/rmt.h>
#include <BMI/bmi_port.h>
#include "sailog.h"

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

        case SAI_API_QOS_MAPS:
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

/*
* Routine Description:
*     Query sai object type.
*
* Arguments:
*     [in] sai_object_id_t
*
* Return Values:
*    Return SAI_OBJECT_TYPE_NULL when sai_object_id is not valid.
*    Otherwise, return a valid sai object type SAI_OBJECT_TYPE_XXX
*/
sai_object_type_t 
sai_object_type_query(
    _In_ sai_object_id_t sai_object_id) {
    sai_object_type_t object_type = SAI_OBJECT_TYPE_NULL;
    switch_nhop_index_type_t nhop_type = 0;
    switch_handle_type_t handle_type = SWITCH_HANDLE_TYPE_NONE;
    handle_type = switch_handle_get_type(sai_object_id);
    switch (handle_type) {
        case SWITCH_HANDLE_TYPE_PORT:
            object_type = SAI_OBJECT_TYPE_PORT;
            break;
        case SWITCH_HANDLE_TYPE_LAG:
            object_type = SAI_OBJECT_TYPE_LAG;
            break;
        case SWITCH_HANDLE_TYPE_INTERFACE:
            object_type = SAI_OBJECT_TYPE_ROUTER_INTERFACE;
            break;
        case SWITCH_HANDLE_TYPE_VRF:
            object_type = SAI_OBJECT_TYPE_VIRTUAL_ROUTER;
            break;
        case SWITCH_HANDLE_TYPE_NHOP:
            nhop_type = switch_api_nhop_type_get(sai_object_id);
            if (nhop_type == SWITCH_NHOP_INDEX_TYPE_ONE_PATH) {
                object_type = SAI_OBJECT_TYPE_NEXT_HOP;
            } else if (nhop_type == SWITCH_NHOP_INDEX_TYPE_ECMP) {
                object_type = SAI_OBJECT_TYPE_NEXT_HOP_GROUP;
            } else {
                object_type = SAI_OBJECT_TYPE_NULL;
            }
            break;
        case SWITCH_HANDLE_TYPE_STP:
            object_type = SAI_OBJECT_TYPE_STP_INSTANCE;
            break;
        case SWITCH_HANDLE_TYPE_ACL:
            object_type = SAI_OBJECT_TYPE_ACL_TABLE;
            break;
        case SWITCH_HANDLE_TYPE_HOSTIF_GROUP:
            object_type = SAI_OBJECT_TYPE_TRAP_GROUP;
            break;
        default:
            object_type = SAI_OBJECT_TYPE_NULL;
            break;
    }
    return object_type;
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
    sai_hostif_initialize(&sai_api_service);
    sai_acl_initialize(&sai_api_service);
    return SAI_STATUS_SUCCESS;
}

const char* sai_profile_get_value(_In_ sai_switch_profile_id_t profile_id,
                                     _In_ const char* variable)
{
    return NULL;
}


    /* Enumerate all the K/V pairs in a profile. 
       Pointer to NULL passed as variable restarts enumeration.
       Function returns 0 if next value exists, -1 at the end of the list. */
int sai_profile_get_next_value(_In_ sai_switch_profile_id_t profile_id,
                                  _Out_ const char** variable,
                                  _Out_ const char** value)
{
    return -1;
}


const service_method_table_t sai_services = {
    .profile_get_value = sai_profile_get_value,
    .profile_get_next_value = sai_profile_get_next_value
};

static unsigned int initialized = 0;
extern int start_switch_api_packet_driver(void);
static bmi_port_mgr_t *port_mgr;

static void
transmit_wrapper(p4_port_t egress, void *pkt, int len) {
    if (bmi_port_send(port_mgr, egress, pkt, len) < 0) {
        printf("Error sending packet\n");
    }
}


static void
packet_handler(int port_num, const char *buffer, int length)
{
    /* @fixme log vector */
    printf("Packet in on port %d length %d; first bytes:\n", port_num, length);
    int i = 0;
    for (i = 0; i < 16; i++) {
        if (i && ((i % 4) == 0)) {
            printf(" ");
        }
        printf("%02x", (uint8_t) buffer[i]);
    }
    printf("\n");
    printf("rmt proc returns %d\n", rmt_process_pkt(port_num, (char*)buffer, length));
}

static int load_config(char *fname, unsigned int *num_ports)
{
    char s[256];
    int port;
    char veth[32];
    char pcap[36];
    char tmp[32];
    char *pcap_file = NULL;
    FILE *fp = fopen(fname, "r");
    if(fp) {
        while(fgets(s, 256, fp)) {
            int r=0;
            pcap[0] = 0;
            pcap_file = NULL;
            if(!strncmp(s, "num_ports", 9)) {
                sscanf(s, "%s = %d", tmp, num_ports);
            }
            else {
                if((r= sscanf(s, "%d:%s %s", &port, veth, pcap)) >= 2) {
                    pcap_file = pcap;
                }
                if(bmi_port_interface_add(port_mgr, veth, port, pcap_file) != 0) {
                    fclose(fp);
                    return -1;
                }
            }
        }
        fclose(fp);
        return 0;
    }
    return -1;
}

static int api_log_level[SAI_API_SCHEDULER_GROUP+1];
static const char *module[] = {
    "UNSPECIFIED",
    "SWITCH",
    "PORT",
    "FDB",
    "VLAN",
    "VIRTUAL_ROUTER",
    "ROUTE",
    "NEXT_HOP",
    "NEXT_HOP_GROUP",
    "ROUTER_INTERFACE",
    "NEIGHBOR",
    "ACL",
    "HOST_INTERFACE",
    "MIRROR",
    "SAMPLEPACKET",
    "STP",
    "LAG",
    "POLICER",
    "WRED",
    "QOS_MAPS",
    "QUEUE",
    "SCHEDULER",
    "SCHEDULER_GROUP"
  };

sai_status_t
sai_api_initialize(
    _In_ uint64_t flags,
    _In_ const service_method_table_t* services) {
    sai_status_t status =  SAI_STATUS_SUCCESS;
    unsigned int num_ports = 32;
    if(!initialized) {
        SAI_LOG(SAI_LOG_WARN, SAI_API_UNSPECIFIED, "INIT device");
        bmi_port_create_mgr(&port_mgr);
        rmt_init();
        rmt_logger_set((p4_logging_f) printf);
        rmt_log_level_set(P4_LOG_LEVEL_TRACE);
        rmt_transmit_register(transmit_wrapper);
        status = load_config("port.cfg", &num_ports);
        if(status != 0)
            return status;
        switch_api_init(0, num_ports);
        start_switch_api_packet_driver();
        initialized = 1;
        sai_initialize();
        bmi_set_packet_handler(port_mgr, packet_handler);
    }

    services = &sai_services;
    return status;
}


sai_status_t
sai_api_uninitialize(void) {
    sai_status_t status =  SAI_STATUS_SUCCESS;
    return status;
}


sai_status_t sai_log_set(
    _In_ sai_api_t sai_api_id,
    _In_ sai_log_level_t log_level) {
    sai_status_t status =  SAI_STATUS_SUCCESS;
    api_log_level[sai_api_id] = log_level;
    return status;
}

#define MAX_LOG_BUFFER 1000

char log_buf[MAX_LOG_BUFFER+1];

void my_log(int level, sai_api_t api, char *fmt, ...)
{
    va_list args;
    // compare if level of each API here?
    if(level < api_log_level[api])
        return;
    va_start(args, fmt);
    vsnprintf(log_buf, MAX_LOG_BUFFER, fmt, args);
    va_end(args);
#if 1
    printf("%s: %s\n", module[api], log_buf);
#else
    syslog(LOG_DEBUG-level, "%s: %s", module[api], log_buf);
#endif
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
