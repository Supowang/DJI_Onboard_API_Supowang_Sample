//
// Created by Hao Xu on 15/5/5.
//

#ifndef DJI_SDK_DJI_SERVICES_H
#define DJI_SDK_DJI_SERVICES_H

#include "dji_variable.h"
#include <ros/ros.h>

namespace service_handles
{
    int init_services(ros::NodeHandle & n);
};

#endif //DJI_SDK_DJI_SERVICES_H
