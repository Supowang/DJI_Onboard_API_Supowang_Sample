//
// Created by  Lichao Wang on 15/8/20
//
#include <ros/ros.h>
#include <dji_sdk/dji_services.h>
#include <dji_sdk/test_basic_command.h>
#include <dji_sdk/test_movement_control.h>
#include <dji_sdk/test_gimbal_control.h>
#include <dji_sdk/test_simple_task.h>
#include <cstdlib>

static void Display_Main_Menu(void)
{
  printf("\r\n");
  printf("-----------   < Main menu >  ----------\r\n\r\n");
  printf("[a]  Takeoff\n");
  printf("[b]  Landing\n");
  printf("[c]  Go home\n");
  printf("[d]  Control the UAV with speed\n");
  printf("[e]  Control the UAV with position\n");
  printf("[f]  Control the gimbal with angle\n");
  printf("[g]  Control the gimbal with speed\n");
  printf("[h]  Control the gimbal with simple angle\n");
  printf("[i]  test_simple_task with flying cube\n");
  printf("[j]  test_simple_task with flying circle\n");
  printf("\ninput a/b/c etc..then press enter key\r\n");
  printf("----------------------------------------\r\n");
  printf("input: ");
}

int main(int argc, char **argv)
{
  int main_operate_code = 0;
  int temp32;
  bool valid_flag = false;
  bool err_flag = false;
  ros::init(argc, argv, "sdk_client");
  ROS_INFO("sdk_service_test");
  ros::NodeHandle n;
  ros::ServiceClient test_basic_command_client = n.serviceClient<dji_sdk::test_basic_command>("test_basic_command");
  ros::ServiceClient test_movement_control_client = n.serviceClient<dji_sdk::test_movement_control>("test_movement_control");
  ros::ServiceClient test_gimbal_control_client = n.serviceClient<dji_sdk::test_gimbal_control>("test_gimbal_control");
  ros::ServiceClient test_simple_task_client = n.serviceClient<dji_sdk::test_simple_task>("test_simple_task");
  
  dji_sdk::test_basic_command srv_tbc;
  dji_sdk::test_movement_control srv_tmc;
  dji_sdk::test_gimbal_control srv_tgc;
  dji_sdk::test_simple_task srv_tst;

  Display_Main_Menu();

  while(1)
  {
    temp32 = getchar();
    if(temp32 != 10)
    {
      if(temp32 >= 'a' && temp32 <= 'j' && valid_flag == false)
      {
        main_operate_code = temp32;
        valid_flag = true;
      }
      else
      {
        err_flag = true;
      }
      continue;
    }
    else
    {
      if(err_flag == true)
      {
        printf("input: ERROR\n");
        Display_Main_Menu();
        err_flag = valid_flag = false;
        continue;
      }
    }

    switch(main_operate_code)
    {
    case 'a':
      /* takeoff */
      srv_tbc.request.send_data=4;
      test_basic_command_client.call(srv_tbc);
      break;
    case 'b':
        /* landing */
      srv_tbc.request.send_data=1;
      test_basic_command_client.call(srv_tbc);
      break;
    case 'c':
      /* go home */
      srv_tbc.request.send_data=6;
      test_basic_command_client.call(srv_tbc);
      break;
    case 'd':
      /* Control the UAV */
      srv_tmc.request.flag=0x48;
      srv_tmc.request.n=200;
      srv_tmc.request.x=2;
      srv_tmc.request.y=-2;
      srv_tmc.request.z=2;
      srv_tmc.request.yaw=500;
      test_movement_control_client.call(srv_tmc);
      break;
    case 'e':
      /* Control the UAV */
      srv_tmc.request.flag=0x90;
      srv_tmc.request.n=200;
      srv_tmc.request.x=4;
      srv_tmc.request.y=-4;
      srv_tmc.request.z=20;
      srv_tmc.request.yaw=500;
      test_movement_control_client.call(srv_tmc);
      break;
    case 'f':
     /* control the gimbal with angle*/
     srv_tgc.request.flag=1;
     srv_tgc.request.yaw=1000;
     srv_tgc.request.x=0;
     srv_tgc.request.y=0;
     test_gimbal_control_client.call(srv_tgc);
      break;
    case 'g':
      /* control the gimbal with speed*/
     srv_tgc.request.flag=2;
     srv_tgc.request.yaw=0;
     srv_tgc.request.x=0;
     srv_tgc.request.y=-1000;
     test_gimbal_control_client.call(srv_tgc);
      break;
    case 'h':
      /* control the gimbal with simple angle*/
     srv_tgc.request.flag=3;
     srv_tgc.request.yaw=0;
     srv_tgc.request.x=1000;
     srv_tgc.request.y=0;
     test_gimbal_control_client.call(srv_tgc);
      break;
    case 'i':
      srv_tst.request.task_num=1;
      test_simple_task_client.call(srv_tst);
      break;
    case 'j':
      srv_tst.request.task_num=2;
      test_simple_task_client.call(srv_tst);
      break;

    }
    main_operate_code = -1;
    err_flag = valid_flag = false;
    Display_Main_Menu();
  }
  return 0;
  
}


  
