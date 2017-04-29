//
// Created by Hao Xu on 15/5/5.
// modified by Lichao Wang on 15/8/20
#include <dji_sdk/dji_gimbal.h>
#include "math.h"
#include <dji_sdk/local_position.h>
#include "stdio.h"


namespace gimbal
{


    void DJI_Onboard_API_CtrGimbal_speed(int16_t yaw,int16_t x,int16_t y)
   {
            gimbal_custom_control_speed_t send_data={0};
            send_data.ctrl_byte.ctrl_switch= 1;
            send_data.ctrl_byte.reserve = 0;
            send_data.yaw_angular_rate= yaw;
            send_data.roll_angular_rate= x;
            send_data.pitch_angular_rate= y;
            App_Send_Data(0, 0, MY_CTRL_CMD_SET, API_CTRL_GIMBAL_SPEED, (unsigned char*)&send_data, sizeof(send_data),NULL, 0, 0);
   }

    void DJI_Onboard_API_CtrGimbal_angle(int16_t yaw,int16_t x,int16_t y)
   {
            gimbal_custom_control_angle_t send_data={0};
            
            send_data.yaw_angle = yaw;
            send_data.roll_angle = x;
            send_data.pitch_angle = y;
            send_data.tolerance = 50;    // unit 0.01

            send_data.ctrl_byte.ctrl_switch = 1;
            send_data.ctrl_byte.reference = 1;
            send_data.ctrl_byte.frame = 1;
            send_data.ctrl_byte.yaw_follow = 1;
            send_data.timeout = 20;  // unit 0.1s
            App_Send_Data(0, 0, MY_CTRL_CMD_SET, API_CTRL_GIMBAL_ANGLE, (unsigned char*)&send_data, sizeof(send_data),NULL, 0, 0);
   }

     void DJI_Onboard_API_CtrGimbal_angle_simple(int16_t yaw,int16_t x,int16_t y)
   {
            gimbal_custom_control_angle_simple_t send_data = {0};
            send_data.yaw_angle = yaw;
            send_data.roll_angle = x;
            send_data.pitch_angle = y;
            send_data.ctrl_byte.base = 1;
            send_data.duration = 20;
            App_Send_Data(0, 0, MY_CTRL_CMD_SET, API_CTRL_GIMBAL_ANGLE, (unsigned char*)&send_data, sizeof(send_data), NULL, 0, 0);
    }
};
