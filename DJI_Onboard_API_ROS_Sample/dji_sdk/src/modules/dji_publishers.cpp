//
// Created by Hao Xu on 15/5/5.
//

#include <dji_sdk/dji_publishers.h>

#include "std_msgs/Float32.h"
#include <dji_sdk/attitude_quad.h>
#include <dji_sdk/global_position.h>
#include <dji_sdk/local_position.h>
#include <dji_sdk/velocity.h>
#include <dji_sdk/acc.h>
#include <dji_sdk/gimbal.h>
#include <dji_sdk/rc_channels.h>
#include <nav_msgs/Odometry.h>

namespace  publishers
{
    ros::Publisher battery_pub, nav_ctrl_status_pub,
            flight_status_pub, activation_status_pub, test_fre_pub, acc_pub;

    ros::Publisher gps_pub, att_quad_pub,
            vel_pub, local_pos_pub,rc_channels_pub;

    ros::Publisher odem_publisher;


    int init_publishers(ros::NodeHandle &nh)
    {
        // start ros publisher
        publishers::battery_pub = nh.advertise<std_msgs::Float32>("battery_status", 10);
        publishers::nav_ctrl_status_pub = nh.advertise<std_msgs::Float32>("nav_open_close_status", 10);
        publishers::flight_status_pub = nh.advertise<std_msgs::Float32>("flight_status", 10);
        publishers::activation_status_pub = nh.advertise<std_msgs::Float32>("activation_status", 10);
        publishers::test_fre_pub = nh.advertise<std_msgs::Float32>("test_fre", 10);

        publishers::acc_pub = nh.advertise<dji_sdk::acc>("acceleration", 10);

        //extend pub

        publishers::att_quad_pub = nh.advertise<dji_sdk::attitude_quad>("attitude_quad", 10);
        publishers::gps_pub = nh.advertise<dji_sdk::global_position>("global_position", 10);
        publishers::local_pos_pub = nh.advertise<dji_sdk::local_position>("local_position", 10);
        publishers::vel_pub = nh.advertise<dji_sdk::velocity>("velocity", 10);
        publishers::odem_publisher = nh.advertise<nav_msgs::Odometry>("odom",10);

        rc_channels_pub = nh.advertise<dji_sdk::rc_channels>("rc_channels",10);

        return 0;
    }
};
