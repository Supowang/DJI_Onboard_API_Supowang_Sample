//
// Created by Hao Xu on 15/5/5.
// Modified by Lichao Wang on 15/8/20

#include <dji_sdk/dji_services.h>
#include <dji_sdk/dji_gimbal.h>
#include <dji_sdk/motion_controls.h>
#include <dji_sdk/dji_commands.h>
#include <dji_sdk/test_simple_task.h>
#include <dji_sdk/test_basic_command.h>
#include <dji_sdk/test_gimbal_control.h>
#include <dji_sdk/test_movement_control.h>
namespace service_handles
{

    bool test_basic_command_cb(
         dji_sdk::test_basic_commandRequest & request,
         dji_sdk::test_basic_commandResponse & response
        )
    {
        uint8_t senddata =(uint8_t)request.send_data;
        if(senddata == 4)
        {
            dji_commands::set_takeoff();
            response.success = true;
        }   
        else if(senddata == 6)
        {
            dji_commands::set_return2home();
            response.success = true;
        }   
        else if(senddata == 1)
        {
            dji_commands::set_land();
            response.success = true;
        }   
        else
        response.success = false;
        return true;
    }

    bool test_movement_control_cb(
            dji_sdk::test_movement_controlRequest & request,
            dji_sdk::test_movement_controlResponse & response
    )
    {
        motion_controls::DJI_Onboard_API_Ctr(request.flag,request.n,request.x,request.y,request.z,request.yaw);
        response.success = true;


        return true;
    }

    bool test_gimbal_control_cb(
            dji_sdk::test_gimbal_controlRequest & request,
            dji_sdk::test_gimbal_controlResponse & response
    )
    {
        uint8_t flag =(uint8_t)request.flag;
        if(flag == 1)
        {
            gimbal::DJI_Onboard_API_CtrGimbal_angle(
                request.yaw,
                request.x,
                request.y
        );
            response.success = true;
        }
        
        else if (flag == 2)
        {
            gimbal::DJI_Onboard_API_CtrGimbal_speed(
                request.yaw,
                request.x,
                request.y
        );
            response.success = true;

        }

        else if (flag == 3)
        {
            /* code */
            gimbal::DJI_Onboard_API_CtrGimbal_angle_simple(
                request.yaw,
                request.x,
                request.y
        );
            response.success = true;
        }
        
        else
            response.success = false;

        return true;
    }

    bool test_simple_task_cb(
        dji_sdk::test_simple_taskRequest & request,
        dji_sdk::test_simple_taskResponse & response
        )
    {
       
       uint8_t senddata =(uint8_t)request.task_num;
       if(senddata==1)
       {
            motion_controls::DJI_Onboard_API_Ctr(0x48,300,0,0,2,0);
            sleep(4);
            motion_controls::DJI_Onboard_API_Ctr_drawcube();
            response.success = true;
       }
       else if(senddata==2)
       {
            motion_controls::DJI_Onboard_API_Ctr(0x48,300,1,-0.414,0,0);
            sleep(4);
            motion_controls::DJI_Onboard_API_Ctr_drawcircle();
            sleep(1);
            response.success = true;
       }

       else
         response.success = false;
         return true;
    }

    ros::ServiceServer test_basic_command,test_movement_control,test_gimbal_control,test_simple_task;

    int init_services(ros::NodeHandle & n)
    {
       

      
        test_basic_command = n.advertiseService(
            "test_basic_command",
            test_basic_command_cb
        );
        test_movement_control=n.advertiseService(
            "test_movement_control",
            test_movement_control_cb
        );
        test_gimbal_control = n.advertiseService(
                "test_gimbal_control",
                test_gimbal_control_cb
        );      
        test_simple_task = n.advertiseService(
            "test_simple_task",
            test_simple_task_cb
        );

        ROS_INFO("Init services\n");

        return 0;
    }
}