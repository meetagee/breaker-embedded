#include "fsm.h"
#include "port.h"
#include "message.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "zb_zcl.h"
#include "zb_znp.h"

#include "app.h"
#include "app_dbg.h"

#include "task_list.h"
#include "task_zigbee.h"


//https://github.com/athombv/homey/issues/2165
//https://github.com/Frans-Willem/AqaraHub/blob/master/documentation/devices/lumi.sensor_ht.md
//#define DBG_ZB_FRAME

uint16_t control_switch_address;

int zb_znp::zigbee_message_handler(zigbee_msg_t& zigbee_msg) {
	/* zigbee start debug message */
#if defined (DBG_ZB_FRAME)
	APP_DBG("[ZB msg] len: %d cmd0: %x cmd1: %x \n", zigbee_msg.len, zigbee_msg.cmd0, zigbee_msg.cmd1);
	APP_DBG(" data: ");
	for (int i = 0; i < zigbee_msg.len; i++) {
		APP_DBG("%x ", (uint8_t)zigbee_msg.data[i]);
	}
	APP_DBG("\n");
#endif
	/* zigbee stop debug message */

	uint16_t zigbee_cmd = BUILD_UINT16(zigbee_msg.cmd1, zigbee_msg.cmd0);

	switch(zigbee_cmd) {
	case ZDO_MGMT_LEAVE_REQ: {
		APP_DBG("ZDO_MGMT_LEAVE_REQ");
	}
		break;

	case ZB_RECEIVE_DATA_INDICATION: {
		APP_DBG("ZB_RECEIVE_DATA_INDICATION");
	}
		break;

	case AF_INCOMING_MSG: {
		af_incoming_msg_t* st_af_incoming_msg = (af_incoming_msg_t*)zigbee_msg.data;

		zclProcMsgStatus_t zclProcMsgStatus = zcl_ProcessMessageMSG(st_af_incoming_msg);
		switch (zclProcMsgStatus) {
		case ZCL_PROC_SUCCESS: {
			APP_DBG("[zcl_ProcessMessageMSG] Message was processed\n");
		}
			break;

		case ZCL_PROC_INVALID: {
			APP_DBG("[zcl_ProcessMessageMSG] Format or parameter was wrong\n");
		}
			break;

		case ZCL_PROC_EP_NOT_FOUND: {
			APP_DBG("[zcl_ProcessMessageMSG] Endpoint descriptor not found\n");
		}
			break;

		case ZCL_PROC_NOT_OPERATIONAL: {
			APP_DBG("[zcl_ProcessMessageMSG] Can't respond to this command\n");
		}
			break;

		case ZCL_PROC_INTERPAN_FOUNDATION_CMD: {
			APP_DBG("[zcl_ProcessMessageMSG] INTER-PAN and Foundation Command (not allowed)\n");
		}
			break;

		case ZCL_PROC_NOT_SECURE: {
			APP_DBG("[zcl_ProcessMessageMSG] Security was required but the message is not secure\n");
		}
			break;

		case ZCL_PROC_MANUFACTURER_SPECIFIC: {
			APP_DBG("[zcl_ProcessMessageMSG] Manufacturer Specific command - not handled\n");
		}
			break;

		case ZCL_PROC_MANUFACTURER_SPECIFIC_DR: {
			APP_DBG("[zcl_ProcessMessageMSG] Manufacturer Specific command - not handled, but default response sent\n");
		}
			break;

		case ZCL_PROC_NOT_HANDLED: {
			APP_DBG("[zcl_ProcessMessageMSG] No default response was sent and the message was not handled\n");
		}
			break;

		case ZCL_PROC_NOT_HANDLED_DR: {
			APP_DBG("[zcl_ProcessMessageMSG] default response was sent and the message was not handled\n");
		}

		default:
			APP_DBG("zclProcMsgStatus: %d\n", zclProcMsgStatus);
			break;
		}

#if defined (DBG_ZB_FRAME)
		APP_DBG("AF_INCOMING_MSG:\n");
		APP_DBG("\tgroup_id: %04x \n", st_af_incoming_msg->group_id);
		APP_DBG("\tcluster_id: %04x\n", st_af_incoming_msg->cluster_id);
		APP_DBG("\tsrc_addr: %04x\n", st_af_incoming_msg->src_addr);
		APP_DBG("\tsrc_endpoint: %x\n", st_af_incoming_msg->src_endpoint);
		APP_DBG("\tdst_endpoint: %x\n", st_af_incoming_msg->dst_endpoint);
		APP_DBG("\twas_broadcast: %x\n", st_af_incoming_msg->was_broadcast);
		APP_DBG("\tlink_quality: %x\n", st_af_incoming_msg->link_quality);
		APP_DBG("\tsecurity_use: %x\n", st_af_incoming_msg->security_use);
		APP_DBG("\ttime_stamp: %08x\n", st_af_incoming_msg->time_stamp);
		APP_DBG("\ttrans_seq_num: %x\n", st_af_incoming_msg->trans_seq_num);
		APP_DBG("\tlen: %d\n", st_af_incoming_msg->len);
		APP_DBG("\tdata: ");
		for (int i = 0 ; i < st_af_incoming_msg->len ; i++) {
			APP_DBG("%02x ", st_af_incoming_msg->payload[i]);
		}
		APP_DBG("\n");
#endif
	}
		break;

	case ZDO_MGMT_LEAVE_RSP: {
		APP_DBG("ZDO_MGMT_LEAVE_RSP\n");
	}
		break;

	case ZDO_MGMT_PERMIT_JOIN_RSP: {
		APP_DBG("ZDO_MGMT_PERMIT_JOIN_RSP\n");
	}
		break;

	case ZDO_TC_DEV_IND: {
		APP_DBG("ZDO_TC_DEV_IND\n");
	}
		break;

	case ZDO_END_DEVICE_ANNCE_IND: {
#if defined (DBG_ZB_FRAME)
		af_incoming_msg_t* st_af_incoming_msg = (af_incoming_msg_t*)zigbee_msg.data;
		APP_DBG("ZDO_END_DEVICE_ANNCE_IND\n");
		APP_DBG("\tgroup_id: %04x \n", st_af_incoming_msg->group_id);
		APP_DBG("\tcluster_id: %04x\n", st_af_incoming_msg->cluster_id);
		APP_DBG("\tsrc_addr: %04x\n", st_af_incoming_msg->src_addr);
		APP_DBG("\tsrc_endpoint: %x\n", st_af_incoming_msg->src_endpoint);
		APP_DBG("\tdst_endpoint: %x\n", st_af_incoming_msg->dst_endpoint);
		APP_DBG("\twas_broadcast: %x\n", st_af_incoming_msg->was_broadcast);
		APP_DBG("\tlink_quality: %x\n", st_af_incoming_msg->link_quality);
		APP_DBG("\tsecurity_use: %x\n", st_af_incoming_msg->security_use);
		APP_DBG("\ttime_stamp: %08x\n", st_af_incoming_msg->time_stamp);
		APP_DBG("\ttrans_seq_num: %x\n", st_af_incoming_msg->trans_seq_num);
		APP_DBG("\tlen: %d\n", st_af_incoming_msg->len);
		APP_DBG("\tdata: ");
		for (int i = 0 ; i < st_af_incoming_msg->len ; i++) {
			APP_DBG("%02x ", st_af_incoming_msg->payload[i]);
		}
		APP_DBG("\n");
#endif
	}
		break;

	default: {
		APP_DBG("zigbee_cmd: 0x%02X\n", zigbee_cmd);
	}
		break;
	}

	return 0;
}


zb_znp zigbee_network;

void task_zigbee(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_ZIGBEE_INIT: {
		APP_DBG_SIG("AC_ZIGBEE_INIT\n");
	}
		break;

	case AC_ZIGBEE_FORCE_START_COODINATOR: {
		APP_DBG_SIG("AC_ZIGBEE_FORCE_START_COODINATOR\n");
		if (zigbee_network.start_coordinator(1) == 0) {
			APP_DBG("force start_coordinator successfully\n");
		}
		else {
			APP_DBG("force start_coordinator error\n");
		}
	}
		break;

	case AC_ZIGBEE_START_COODINATOR: {
		APP_DBG_SIG("AC_ZIGBEE_START_COODINATOR\n");
		if (zigbee_network.start_coordinator(0) == 0) {
			APP_DBG("start_coordinator successfully\n");
		}
		else {
			APP_DBG("start_coordinator error\n");
		}
	}
		break;

	case AC_ZIGBEE_PERMIT_JOINING_REQ: {
		APP_DBG_SIG("AC_ZIGBEE_PERMIT_JOINING_REQ\n");
		zigbee_network.set_permit_joining_req(ALL_ROUTER_AND_COORDINATOR, 60, 1);
	}
		break;

	case AC_ZIGBEE_ZCL_CMD_HANDLER: {
		APP_DBG_SIG("AC_ZIGBEE_ZCL_CMD_HANDLER\n");

		zclOutgoingMsg_t* pOutgoingMsg = (zclOutgoingMsg_t*)get_data_common_msg(msg);

		APP_DBG("[pOutgoingMsg] short_addr = 0x%04x\n", pOutgoingMsg->short_addr);
		APP_DBG("[pOutgoingMsg] cluster_id = 0x%04x\n", pOutgoingMsg->cluster_id);
		APP_DBG("[pOutgoingMsg] group_id = 0x%04x\n", pOutgoingMsg->group_id);
		APP_DBG("[pOutgoingMsg] cmd = 0x%02x\n", pOutgoingMsg->cmd);
		APP_DBG("[pOutgoingMsg] attrID = 0x%04x\n", pOutgoingMsg->attrID);
		APP_DBG("[pOutgoingMsg] dataType = 0x%02x\n", pOutgoingMsg->dataType);
		APP_DBG("[pOutgoingMsg] dataLen = %d\n", pOutgoingMsg->dataLen);
		APP_DBG("[pOutgoingMsg] data: ");
		for (int i = 0 ; i < pOutgoingMsg->dataLen ; i++) {
			APP_DBG("%02x ", pOutgoingMsg->data[i]);
		}
		APP_DBG("\n");

		// Handle data message incoming.
		switch (pOutgoingMsg->cluster_id) {
		case ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY: {
			APP_DBG("ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY\n");
			uint16_t retHum = (uint16_t)(pOutgoingMsg->data[pOutgoingMsg->dataLen] + pOutgoingMsg->data[pOutgoingMsg->dataLen - 1] * 256);
			(void)retHum;
			// Ví dụ: retHum = 6789, thì giá trị trả về là 67,89 %
			APP_DBG("HUMIDITY: %0.2f\n", (float)(retHum / 100));
		}
			break;

		case ZCL_CLUSTER_ID_MS_TEMPERATURE_MEASUREMENT: {
			APP_DBG("ZCL_CLUSTER_ID_MS_TEMPERATURE_MEASUREMENT\n");
			uint16_t retTemp = (uint16_t)(pOutgoingMsg->data[pOutgoingMsg->dataLen] + pOutgoingMsg->data[pOutgoingMsg->dataLen - 1] * 256);
			(void)retTemp;
			// Ví dụ: retTemp = 2723, thì giá trị trả về là 27,23 *C
			APP_DBG("TEMPERATURE: %0.2f\n", (float)(retTemp / 100));
		}
			break;

		case ZCL_CLUSTER_ID_GEN_BASIC: {
			APP_DBG("ZCL_CLUSTER_ID_GEN_BASIC\n");
			switch (pOutgoingMsg->attrID) {
			case ATTRID_XIAOMI_SENS_STATUS_REPORT: {
				if (pOutgoingMsg->dataLen == sizeof(xiaomi_sens_status_report_t) &&
						pOutgoingMsg->data[1] == 1) {

					uint8_t battery_level = 0;
					(void)battery_level;
					xiaomi_sens_status_report_t xiaomi_sens_status_report;
					memcpy(&xiaomi_sens_status_report, pOutgoingMsg->data, sizeof(xiaomi_sens_status_report_t));
					APP_DBG("<SENS> BATTERY VOLTAGE: %d\n", xiaomi_sens_status_report.battery_value);

					//https://devzone.nordicsemi.com/f/nordic-q-a/28101/how-to-calculate-battery-voltage-into-percentage-for-aa-2-batteries-without-fluctuations
					if (xiaomi_sens_status_report.battery_value >= 3000) {
						xiaomi_sens_status_report.battery_value  = 100;
					}
					else if (xiaomi_sens_status_report.battery_value > 2900) {
						battery_level = 100 - ((3000 - xiaomi_sens_status_report.battery_value ) * 60) / 100;
					}
					else if (xiaomi_sens_status_report.battery_value > 2740) {
						battery_level = 60 - ((2900 - xiaomi_sens_status_report.battery_value) * 40) / 150;
					}
					else {
						battery_level = 0;
					}

					APP_DBG("<SENS> BATTERY PERCENT: %d\n", battery_level);
					APP_DBG("<SENS> TEMPERATURE: %d\n", xiaomi_sens_status_report.temperatemure_value / 100);
					APP_DBG("<SENS> HUMIDITY: %d\n", xiaomi_sens_status_report.humidity_value / 100);
				}
			}
				break;

			default:
				break;
			}
		}
			break;

		default:
			break;
		}

		// free message.
		if (pOutgoingMsg) {
			ak_free(pOutgoingMsg);
		}

		if (pOutgoingMsg->data) {
			ak_free(pOutgoingMsg->data);
		}
	}
		break;

	default:
		break;
	}
}
