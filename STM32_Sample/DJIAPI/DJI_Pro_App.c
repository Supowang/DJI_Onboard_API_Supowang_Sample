/*
 * DJI_Pro_App.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: Ying Jiahang
 */

#include "DJI_Pro_Link.h"
#include "DJI_Pro_App.h"
#include <string.h>
#include <stdio.h>
static unsigned char Pro_Encode_Data[1024];
static dji_sdk_cmd_unit cmd_unit = {0};

void App_Send_Data(unsigned char flag, unsigned char is_enc, unsigned char  cmd_set, unsigned char cmd_id,unsigned char *pdata,int len)
{
	ProSendParameter param;
	unsigned char *ptemp = (unsigned char *)Pro_Encode_Data;
	*ptemp++ = cmd_set;
	*ptemp++ = cmd_id;

	memcpy(Pro_Encode_Data + SET_CMD_SIZE,pdata,len);

	param.pkg_type = flag;
	param.length = len + SET_CMD_SIZE;
	param.buf = Pro_Encode_Data;
	param.need_encrypt = is_enc;
	
	Pro_Send_Interface(&param);
}

//----------------------------------------------------------------------
// cmd agency
//----------------------------------------------------------------------


void App_Complex_Send_Cmd(unsigned char cmd)
{
	cmd_unit.cmd.cmd_data = cmd;
	cmd_unit.is_send_cmd = 1;	
	cmd_unit.cmd.cmd_sequence ++;
	App_Send_Data(2,1,MY_CTRL_CMD_SET, API_CMD_REQUEST,(unsigned char*)&cmd_unit.cmd,sizeof(cmd_unit.cmd));
}


