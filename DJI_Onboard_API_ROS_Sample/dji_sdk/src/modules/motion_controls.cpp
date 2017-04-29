//
// Created by Hao Xu on 15/5/5.
// modified by Lichao Wang on 15/8/20

#include <dji_sdk/motion_controls.h>
#include <sdk_lib/sdk_lib.h>
#include <dji_sdk/dji_variable.h>
#include <dji_sdk/dji_gimbal.h>
#include <sdk_lib/DJI_Pro_App.h>
// MATH for_example
#include <math.h>

// parameter
#define C_EARTH (double) 6378137.0
#define C_PI    (double) 3.141592653589793
namespace motion_controls
{
    
    void DJI_Onboard_API_Ctr(unsigned char flagmode,unsigned int n,float x,float y,float z,float yaw)
   {
            api_ctrl_without_sensor_data_t send_data={0};
            unsigned int j;
     
      for (j=0;j<n;j++)
        { 
            send_data.ctrl_flag = flagmode;
            send_data.roll_or_x = x;
            send_data.pitch_or_y = y;
            send_data.thr_z = z; //m/s
            send_data.yaw = yaw;
            App_Send_Data(0, 0, MY_CTRL_CMD_SET, API_CTRL_REQUEST, (unsigned char*)&send_data, sizeof(send_data),NULL, 0, 0);
            usleep(8000);
        }
   }

    
   void DJI_Onboard_API_Ctr_drawcube(void)
   {
            DJI_Onboard_API_Ctr(0x48,300,2,0,0,0);
            sleep(4);
            DJI_Onboard_API_Ctr(0x48,300,0,2,0,0);
            sleep(4);
            DJI_Onboard_API_Ctr(0x48,300,-2,0,0,0);
            sleep(4);
            DJI_Onboard_API_Ctr(0x48,300,0,-2,0,0);
            sleep(4);
            DJI_Onboard_API_Ctr(0x48,300,0,0,2,0);
            sleep(4);
            DJI_Onboard_API_Ctr(0x48,300,2,0,0,0);
            sleep(4);
            DJI_Onboard_API_Ctr(0x48,300,0,0,-2,0);
            sleep(4);
            DJI_Onboard_API_Ctr(0x48,300,0,0,2,0);
            sleep(4);
            DJI_Onboard_API_Ctr(0x48,300,0,2,0,0);
            sleep(4);
            DJI_Onboard_API_Ctr(0x48,300,0,0,-2,0);
            sleep(4);
            DJI_Onboard_API_Ctr(0x48,300,0,0,2,0);
            sleep(4);
            DJI_Onboard_API_Ctr(0x48,300,-2,0,0,0);
            sleep(4);
            DJI_Onboard_API_Ctr(0x48,300,0,0,-2,0);
            sleep(4);
            DJI_Onboard_API_Ctr(0x48,300,0,0,2,0);
            sleep(4);
            DJI_Onboard_API_Ctr(0x48,300,0,-2,0,0);
            sleep(4);   
   }
    void DJI_Onboard_API_Ctr_drawcircle(void)
    {
        api_ctrl_without_sensor_data_t send_data={0};
        unsigned int j;
        for(j=0;j<2500;j++)
        {
            send_data.ctrl_flag = 0x50;
            send_data.roll_or_x = 1*cos(C_PI*0.002*j);
            send_data.pitch_or_y = 1*sin(C_PI*0.002*j);
            send_data.thr_z = 15; //m/s
            send_data.yaw = 0;
            App_Send_Data(0, 0, MY_CTRL_CMD_SET, API_CTRL_REQUEST, (unsigned char*)&send_data, sizeof(send_data),NULL, 0, 0);
            usleep(30000);
        }
    }

};
