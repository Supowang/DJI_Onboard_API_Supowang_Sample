/*
 * DJI_Pro_Link.h
 *
 *  Created on: Mar 12, 2015
 *      Author: wuyuwei
 */

#ifndef DJI_PRO_LINK_H_
#define DJI_PRO_LINK_H_

#define DJI_SDK_PRO_VER					0
#define PRO_DATA_MAX_SIZE				512 //TODO: for user define
#define PKG_MAX_SIZE					(PRO_DATA_MAX_SIZE + 16) //data,CRC32,header size
#define PRO_ACK_MAX_SIZE				8	//TODO: for user define
#define ACK_MAX_SIZE					(PRO_DATA_MAX_SIZE + 16)

#define SESSION_AND_MEM_COUNT			30

#define ACK_SESSION_IDLE				0
#define ACK_SESSION_PROCESS				1
#define ACK_SESSION_USING				2

#define POLL_TICK						20  //unit is ms

typedef struct ProHeader
{
	unsigned int sof : 8; 			// 1byte
	unsigned int length : 10;
	unsigned int version : 6; 		// 2byte
	unsigned int session_id : 5;
	unsigned int is_ack : 1;
	unsigned int reversed0 : 2; 	// always 0

	unsigned int padding : 5;
	unsigned int enc_type : 3;
	unsigned int reversed1 : 24;

	unsigned int sequence_number : 16;
	unsigned int head_crc : 16;
	unsigned int magic[];
}ProHeader;


typedef struct Memory_Manage_Unit
{
	unsigned char mmu_index;
	unsigned char usage_flag;
	unsigned short res;
	unsigned long start_addr;
	unsigned long end_addr;
}Memory_Manage_Unit;

typedef struct Session_Queue
{
	unsigned int session_id : 5;
	unsigned int usage_flag : 1;
	unsigned int sent_time : 5;
	unsigned int retry_send_time : 5;
	unsigned int ack_timeout : 16;
	Memory_Manage_Unit *mmu;
	unsigned int pre_seq_num;
	unsigned int pre_timestamp;
}Session_Queue;


typedef struct ProSendParameter
{
	unsigned short pkg_type : 2;
	unsigned short need_encrypt : 1;
	unsigned int length;
	unsigned char *buf;
}ProSendParameter;

typedef struct ProAckParameter
{
	unsigned short session_id : 8;
	unsigned short need_encrypt : 8;
	unsigned short seq_num;
	unsigned int length;
	unsigned char *buf;
}ProAckParameter;


void Pro_Link_Setup(void);

Session_Queue * Request_Send_Session(unsigned short size);
void Free_Send_Session(Session_Queue * session);

void Pro_Config_Comm_Encrypt_Key(const char *key);
int Pro_Send_Interface(ProSendParameter *parameter);


int  Pro_Send_Interface(ProSendParameter *parameter);


#endif /* DJI_PRO_LINK_H_ */
