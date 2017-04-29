#ifndef __DJI_SDK_MOTION_CONTROLS_H__
#define __DJI_SDK_MOTION_CONTROLS_H__

#include "dji_sdk/velocity.h"
#include <ros/ros.h>
#include <dji_sdk/local_position.h>
#include <dji_sdk/global_position.h>
namespace motion_controls
{
    void DJI_Onboard_API_Ctr(unsigned char flagmode,unsigned int n,float x,float y,float z,float yaw);
    void DJI_Onboard_API_Ctr_drawcube(void);
    void DJI_Onboard_API_Ctr_drawcircle(void);
};

#endif