#ifndef __DJI_SDK_GIMBAL_H__
#define __DJI_SDK_GIMBAL_H__

#include <sdk_lib/sdk_lib.h>
#include <dji_sdk/local_position.h>

namespace gimbal
{

    void DJI_Onboard_API_CtrGimbal_speed(int16_t yaw,int16_t x,int16_t y);
    void DJI_Onboard_API_CtrGimbal_angle(int16_t yaw,int16_t x,int16_t y);
    void DJI_Onboard_API_CtrGimbal_angle_simple(int16_t yaw,int16_t x,int16_t y);
};

#endif