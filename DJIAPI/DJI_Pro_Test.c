/*
 ============================================================================
 Name        : DJI_Pro_Test.cpp
 Author      : Wu Yuwei
 Version     :
 Copyright   : Your copyright notice
 Description :
 ============================================================================
 */

/* SDK */
#include <stdio.h>
#include <stdlib.h>
#include "DJI_Pro_Test.h"
#include "delay.h"
/* MATH for_example */
#include <math.h>

/* parameter */
#define C_EARTH (double) 6378137.0
#define C_PI	(double) 3.141592653589793
static const char *key;
static activation_data_t activation_msg = {14,2,1,""};

void DJI_Pro_Test_Setup(void)
{

	activation_msg.app_id =1002242;
	activation_msg.app_api_level = 2;
	activation_msg.app_ver = 1;
	memcpy(activation_msg.app_bundle_id,"1234567890123456789012", 32);
	key = "be8631fb6d726c96f5b935df3cc64510dd9e74febe60400192e0b8608593828e";
	Pro_Config_Comm_Encrypt_Key(key);
	Pro_Link_Setup();
}

void DJI_Onboard_API_Activation(void)
{
	App_Send_Data( 2, 0, MY_ACTIVATION_SET, API_USER_ACTIVATION,(unsigned char*)&activation_msg,sizeof(activation_msg));
}

void DJI_Onboard_API_Control(unsigned char arg)
{
		unsigned char send_data = arg;
		App_Send_Data(1,1,MY_CTRL_CMD_SET,API_OPEN_SERIAL,(unsigned char*)&send_data,sizeof(send_data));
}

void DJI_Onboard_API_UAV_Control(unsigned char arg)
{
	unsigned char send_data = arg;
	App_Complex_Send_Cmd(send_data);	
}

void DJI_Onboard_API_Ctr(unsigned char flagmode,unsigned int n,float a,float b,float c,float d)
{
			api_ctrl_without_sensor_data_t send_data={0};
			unsigned char j;
      send_data.ctrl_flag = flagmode;
			send_data.roll_or_x = a;
			send_data.pitch_or_y = b;
			send_data.thr_z = c; //m/s
			send_data.yaw = d;
      for (j=0;j<n;j++)
		{ 
			App_Send_Data(0, 0, MY_CTRL_CMD_SET, API_CTRL_REQUEST, (unsigned char*)&send_data, sizeof(send_data));
			delay(1);
		}
}

void DJI_Onboard_API_Ctr_drawcube(void)
{
	DJI_Onboard_API_Ctr(0x48,100,2,0,0,0);
	delay(400);
	DJI_Onboard_API_Ctr(0x48,100,0,2,0,0);
	delay(400);
	DJI_Onboard_API_Ctr(0x48,100,-2,0,0,0);
	delay(400);
	DJI_Onboard_API_Ctr(0x48,100,0,-2,0,0);
	delay(400);
	DJI_Onboard_API_Ctr(0x48,100,0,0,2,0);
	delay(400);
	DJI_Onboard_API_Ctr(0x48,100,2,0,0,0);
	delay(400);
	DJI_Onboard_API_Ctr(0x48,100,0,0,-2,0);
	delay(400);
	DJI_Onboard_API_Ctr(0x48,100,0,0,2,0);
	delay(400);
	DJI_Onboard_API_Ctr(0x48,100,0,2,0,0);
	delay(400);
	DJI_Onboard_API_Ctr(0x48,100,0,0,-2,0);
	delay(400);
	DJI_Onboard_API_Ctr(0x48,100,0,0,2,0);
	delay(400);
	DJI_Onboard_API_Ctr(0x48,100,-2,0,0,0);
	delay(400);
	DJI_Onboard_API_Ctr(0x48,100,0,0,-2,0);
	delay(400);
	DJI_Onboard_API_Ctr(0x48,100,0,0,2,0);
	delay(400);
	DJI_Onboard_API_Ctr(0x48,100,0,-2,0,0);
	delay(400);		
}

void DJI_Onboard_API_Ctr_drawcircle(void)
{
	api_ctrl_without_sensor_data_t send_data={0};
  unsigned int j;
     for(j=0;j<3000;j++)
    {
			send_data.ctrl_flag = 0x50;
			send_data.roll_or_x = 1*cos(C_PI*0.002*j);
			send_data.pitch_or_y = 1*sin(C_PI*0.002*j);
			send_data.thr_z = 10; //m/s
			send_data.yaw = 0;
			App_Send_Data(0, 0, MY_CTRL_CMD_SET, API_CTRL_REQUEST, (unsigned char*)&send_data, sizeof(send_data));
			delay(1);
		}
}

