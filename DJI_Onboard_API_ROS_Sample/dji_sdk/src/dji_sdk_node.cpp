//
// Created by Hao Xu on 15/5/5.
// modified by Lichao Wang on 15/8/20
#include "dji_sdk_node.h"
#include <dji_sdk/dji_ros_modules.h>
#include <dji_sdk/motion_controls.h>
#include <sdk_lib/DJI_Pro_App.h>
#include <thread>
#include <functional>

//#define ROS_LOG_DETAILS 0
static gimbal_custom_control_angle_simple_t global_gimbal_data;
int	cmd_activation_flag = 0,cmd_take_off_flag = 0,cmd_do_task_flag=0;
void check_Rcmode(sdk_5_10B_data_t recv_rc);	
//----------------------------------------------------------
//table of sdk req data handler
//----------------------------------------------------------
int16_t sdk_std_msgs_handler(uint8_t cmd_id, uint8_t *pbuf, uint16_t len, req_id_t req_id);

int16_t nav_force_close_handler(uint8_t cmd_id, uint8_t *pbuf, uint16_t len, req_id_t req_id);

// cmd id table
cmd_handler_table_t cmd_handler_tab[] = {
        {0x00, sdk_std_msgs_handler},
        {0x01, nav_force_close_handler},
        {ERR_INDEX, NULL}
};
// cmd set table
set_handler_table_t set_handler_tab[] = {
        {0x02, cmd_handler_tab},
        {ERR_INDEX, NULL}
};




//----------------------------------------------------------
// sdk_req_data_callback
//----------------------------------------------------------

int16_t nav_force_close_handler(uint8_t cmd_id, uint8_t *pbuf, uint16_t len, req_id_t req_id)
{
    if (len != sizeof(uint8_t))
        return 0;
    uint8_t msg;
    memcpy(&msg, pbuf, sizeof(msg));
    // test session ack
    nav_force_close_req_id.sequence_number = req_id.sequence_number;
    nav_force_close_req_id.session_id = req_id.session_id;
    nav_force_close_req_id.reserve = 1;

    printf("WARNING nav close by app %d !!!!!!!!!!!!!! \n", msg);
    return 1;

}

#define _recv_std_msgs(_flag, _enable, _data, _buf, _datalen) \
    if( (_flag & _enable))\
    {\
        memcpy((uint8_t *)&(_data),(uint8_t *)(_buf)+(_datalen), sizeof(_data));\
        _datalen += sizeof(_data);\
    }

int16_t sdk_std_msgs_handler(uint8_t cmd_id, uint8_t *pbuf, uint16_t len, req_id_t req_id)
{   
	static uint16_t count;
    uint16_t *msg_enable_flag = (uint16_t *) pbuf;
    uint16_t data_len = MSG_ENABLE_FLAG_LEN;

    _recv_std_msgs(*msg_enable_flag, ENABLE_MSG_TIME, recv_sdk_std_msgs.time_stamp, pbuf, data_len);
    _recv_std_msgs(*msg_enable_flag, ENABLE_MSG_Q, recv_sdk_std_msgs.q, pbuf, data_len);
    _recv_std_msgs(*msg_enable_flag, ENABLE_MSG_A, recv_sdk_std_msgs.a, pbuf, data_len);
    _recv_std_msgs(*msg_enable_flag, ENABLE_MSG_V, recv_sdk_std_msgs.v, pbuf, data_len);
    _recv_std_msgs(*msg_enable_flag, ENABLE_MSG_W, recv_sdk_std_msgs.w, pbuf, data_len);
    _recv_std_msgs(*msg_enable_flag, ENABLE_MSG_POS, recv_sdk_std_msgs.pos, pbuf, data_len);
    _recv_std_msgs(*msg_enable_flag, ENABLE_MSG_MAG, recv_sdk_std_msgs.mag, pbuf, data_len);
    _recv_std_msgs(*msg_enable_flag, ENABLE_MSG_RC, recv_sdk_std_msgs.rc, pbuf, data_len);
    _recv_std_msgs(*msg_enable_flag, ENABLE_MSG_GIMBAL, recv_sdk_std_msgs.gimbal, pbuf, data_len);
    _recv_std_msgs(*msg_enable_flag, ENABLE_MSG_STATUS, recv_sdk_std_msgs.status, pbuf, data_len);
    _recv_std_msgs(*msg_enable_flag, ENABLE_MSG_BATTERY, recv_sdk_std_msgs.battery_remaining_capacity, pbuf, data_len);
    _recv_std_msgs(*msg_enable_flag, ENABLE_MSG_DEVICE, recv_sdk_std_msgs.ctrl_device, pbuf, data_len);
//    _recv_std_msgs(*msg_enable_flag, ENABLE_MSG_HACC, recv_sdk_std_msgs.hacc, pbuf, data_len);

    // testing reciever frequence
    if ((*msg_enable_flag & ENABLE_MSG_DEVICE)) {
        std_msgs::Float32 msg;
        msg.data = (float) recv_sdk_std_msgs.ctrl_device;
        publishers::test_fre_pub.publish(msg);

    }
    count++;
    if(count%50==0)
    {
    	printf("Rcmode: yaw %d  throttle %d  roll %d  pitch %d  gear %d \n",recv_sdk_std_msgs.rc.yaw,recv_sdk_std_msgs.rc.throttle,recv_sdk_std_msgs.rc.roll,recv_sdk_std_msgs.rc.pitch,recv_sdk_std_msgs.rc.gear);
    }
	
    check_Rcmode(recv_sdk_std_msgs.rc);

    return 0;

}

// app_example
//----------------------------------------------------------
// mode_test
void test_activation_ack_cmd_callback(ProHeader *header)
{
    uint16_t ack_data;
    printf("Sdk_ack_cmd0_callback,sequence_number=%d,session_id=%d,data_len=%d\n", header->sequence_number,
           header->session_id, header->length - EXC_DATA_SIZE);
    memcpy((uint8_t *) &ack_data, (uint8_t *) &header->magic, (header->length - EXC_DATA_SIZE));

    if (is_sys_error(ack_data)) {
        printf("[DEBUG] SDK_SYS_ERROR!!! ACTIVATION\n");
        std_msgs::Float32 msg;
        msg.data = NO_AUTHORITY;
        publishers::activation_status_pub.publish(msg);
    }
    else {
        char result[][50] = {{"ACTIVATION_SUCCESS"},
                             {"PARAM_ERROR"},
                             {"DATA_ENC_ERROR"},
                             {"NEW_DEVICE_TRY_AGAIN"},
                             {"DJI_APP_TIMEOUT"},
                             {" DJI_APP_NO_INTERNET"},
                             {"SERVER_REFUSED"},
                             {"LEVEL_ERROR"},
                             {"SDK_VERSION_ERROR"}};
        printf("[ACTIVATION] Activation result: %s \n", *(result + ack_data));
        if (ack_data == 0)
            dji_variable::activated = true;
        else {
            dji_variable::activated = false;
        }
        std_msgs::Float32 msg;
        msg.data = (float) ack_data;
        publishers::activation_status_pub.publish(msg);

        if (ack_data == 0) {
            Pro_Config_Comm_Encrypt_Key(key);
            printf("[ACTIVATION] set key %s\n", key);
        }
    }
}

void test_activation(void)
{
//	msg.app_id 		= 0;
//	msg.app_api_level 	= 1;
//	msg.app_ver		= 2;
//	msg.app_bundle_id[0]	= 4;
    App_Send_Data(2, 0, MY_ACTIVATION_SET, API_USER_ACTIVATION, (uint8_t *) &activation_msg, sizeof(activation_msg),
                  test_activation_ack_cmd_callback, 1000, 1);

    printf("[ACTIVATION] send acticition msg: %d %d %d %d \n", activation_msg.app_id, activation_msg.app_api_level,
           activation_msg.app_ver, activation_msg.app_bundle_id[0]);
}


void test_version_query_ack_cmd_callback(ProHeader *header)
{
    version_query_data_t ack;
    printf("Sdk_ack_cmd0_callback,sequence_number=%d,session_id=%d,data_len=%d\n", header->sequence_number, header->session_id, header->length - EXC_DATA_SIZE);
    memcpy((uint8_t *)&ack,(uint8_t *)&header->magic, sizeof(uint16_t));
    /* pay attention to memory holes */
    memcpy((uint8_t *)&ack.version_crc,(uint8_t *)&header->magic+sizeof(uint16_t), (header->length - EXC_DATA_SIZE)-sizeof(uint16_t));
    if(ack.version_ack == SDK_ERR_SUCCESS)
    {   
        printf("[ACTIVATION] Activation result:\n   ack SDK_ACT_SUCCESS\n");
    }
    else
    {
        printf("[ACTIVATION] Activation result:\n   ack SDK_NO_AUTHORIZED\n");
    }
    printf("    version_crc %X\n    version_name %s \n", ack.version_crc, ack.version_name);
}

void test_version_query(void)
{
    uint8_t msg = 1;
    App_Send_Data(2, 0, MY_ACTIVATION_SET, API_VERSION_QUERY, (uint8_t*)&msg,sizeof(msg), test_version_query_ack_cmd_callback, 1000, 0);
    printf("[ACTIVATION] send version_query msg \n");
}

//----------------------------------------------------------
// ros_callback function
//----------------------------------------------------------
static uint8_t cmd_send_flag = 1;

void cmd_callback_fun(uint16_t *ack)
{
    /*
    *	#define	REQ_TIME_OUT			0x0000
        #define REQ_REFUSE			0x0001
        #define CMD_RECIEVE			0x0002
        #define STATUS_CMD_EXECUTING		0x0003
        #define STATUS_CMD_EXE_FAIL		0x0004
        #define STATUS_CMD_EXE_SUCCESS		0x0005
    */
    uint16_t ack_data = *ack;

    if (is_sys_error(ack_data)) {
        printf("[DEBUG] SDK_SYS_ERROR!!! \n");
        std_msgs::Float32 msg;
        msg.data = NO_AUTHORITY;
        publishers::activation_status_pub.publish(msg);
    }
    else {
        printf("[DEBUG] recv_ack %#x \n", ack_data);
        char result[6][50] = {{"REQ_TIME_OUT"},
                              {"REQ_REFUSE"},
                              {"CMD_RECIEVE"},
                              {"STATUS_CMD_EXECUTING"},
                              {"STATUS_CMD_EXE_FAIL"},
                              {"STATUS_CMD_EXE_SUCCESS"}};
        printf("random_test Cmd result: %s \n", *(result + ack_data));
    }
    cmd_send_flag = 1;
}

void ros_cmd_data_callback(const std_msgs::Float32::ConstPtr &msg)
{
    uint8_t send_data = (uint8_t) msg->data;
//	printf("cmd %d\n", send_data);
    if (send_data > 21)
        return;

    if (cmd_send_flag) {
        App_Complex_Send_Cmd(send_data, cmd_callback_fun);
        cmd_send_flag = 0;
    }
    else {
//		printf("[CMD] wating! \n");
    }
}

void sdk_ack_nav_open_close_callback(ProHeader *header);

void try_to_open_control()
{
//    if (dji_variable::)
    printf("send open......\n");
    uint8_t send_data = (uint8_t) 1;
    App_Send_Data(1, 1, MY_CTRL_CMD_SET, API_OPEN_SERIAL, (uint8_t *) &send_data, sizeof(send_data),
                  sdk_ack_nav_open_close_callback, 1000, 0);
}

void sdk_ack_nav_open_close_callback(ProHeader *header)
{
    uint16_t ack_data;
    memcpy((uint8_t *) &ack_data, (uint8_t *) &header->magic, (header->length - EXC_DATA_SIZE));

    std_msgs::Float32 msg;
    if (is_sys_error(ack_data)) {
        printf("[DEBUG] SDK_SYS_ERROR!!! OPEN :%d \n" , dji_variable::opened);
        try_to_open_control();
        msg.data = NO_AUTHORITY;
        publishers::nav_ctrl_status_pub.publish(msg);
    }
    else {
        msg.data = (float) ack_data;
        if (msg.data == 2)
        {
            printf("opened!!!\n");
            dji_variable::opened = true;
            usleep(100000);

           // try_to_open_control();
        }
        else {
            dji_variable::opened = false;
            printf("closed!!! Retry\n");
            usleep(100000);
            publishers::nav_ctrl_status_pub.publish(msg);
            try_to_open_control();
        }
    }
}
void ros_nav_open_close_callback(const std_msgs::Float32::ConstPtr &msg)
{
    uint8_t send_data = (uint8_t) msg->data;
    printf("send open nav %d\n", send_data);
    App_Send_Data(1, 1, MY_CTRL_CMD_SET, API_OPEN_SERIAL, (uint8_t *) &send_data, sizeof(send_data),
                  sdk_ack_nav_open_close_callback, 1000, 0);
}


void ros_activation_callback(const std_msgs::Float32::ConstPtr &msg)
{
    printf("ros_activation_callback %f \n", msg->data);
    test_activation();
}



void check_Rcmode(sdk_5_10B_data_t recv_rc)
{
	uint8_t send_data = 1;
	if(recv_rc.yaw == -10000 && recv_rc.throttle == -10000 && recv_rc.roll == -10000 && recv_rc.pitch == -10000 && cmd_take_off_flag== 0)
	    {
		    App_Complex_Send_Cmd(4, cmd_callback_fun);
		    cmd_take_off_flag = 1;
		    cmd_activation_flag = 0;
			sleep(4);
            motion_controls::DJI_Onboard_API_Ctr(0x48,300,0,0,2,0);
           			
	    }

	if(recv_rc.yaw == 10000 && recv_rc.throttle == -10000 && recv_rc.roll == 10000 &&recv_rc.pitch == -10000  && cmd_take_off_flag == 1&&cmd_do_task_flag==0)
	    {
           
		    motion_controls::DJI_Onboard_API_Ctr_drawcube();
		    cmd_do_task_flag=1;
		
	    }
	
	if(recv_sdk_std_msgs.rc.gear == -10000 &&cmd_do_task_flag==1)
		{
            
			cmd_do_task_flag=2;
			motion_controls::DJI_Onboard_API_Ctr(0x48,300,1,-0.414,0,0);
			sleep(4);
			motion_controls::DJI_Onboard_API_Ctr_drawcircle();
			sleep(1);
		   
		}
	if(recv_sdk_std_msgs.rc.gear == -4545&& cmd_do_task_flag==2)
		 {  
		 	cmd_do_task_flag=3;
		 	gimbal::DJI_Onboard_API_CtrGimbal_speed(0,0,-1000);
		 	sleep(1);
		 	
		 }
    if (recv_sdk_std_msgs.rc.gear == -10000&& cmd_do_task_flag==3)
		 {  
		 	cmd_do_task_flag=4;
		 	gimbal::DJI_Onboard_API_CtrGimbal_speed(0,0,1500);
		 	sleep(1);
		 }
	if (recv_sdk_std_msgs.rc.gear == -4545&& cmd_do_task_flag==4)
		 {  
		 	cmd_do_task_flag=0;
		 	cmd_take_off_flag = 0;
		    cmd_activation_flag = 0;
		 	printf("all Finish");
		 	App_Complex_Send_Cmd(6, cmd_callback_fun);
		 }

}


using namespace dji_variable;
void update_ros_vars()
{

    static int frame_id = 0;
     frame_id ++;
    auto current_time = ros::Time::now();

    attitude_quad.q0 = recv_sdk_std_msgs.q.q0;
    attitude_quad.q1 = recv_sdk_std_msgs.q.q1;
    attitude_quad.q2 = recv_sdk_std_msgs.q.q2;
    attitude_quad.q3 = recv_sdk_std_msgs.q.q3;
    attitude_quad.wx = recv_sdk_std_msgs.w.x;
    attitude_quad.wy = recv_sdk_std_msgs.w.y;
    attitude_quad.wz = recv_sdk_std_msgs.w.z;
    attitude_quad.ts = recv_sdk_std_msgs.time_stamp;

    global_position.lat = recv_sdk_std_msgs.pos.lati;
    global_position.lon = recv_sdk_std_msgs.pos.longti;
    global_position.height = recv_sdk_std_msgs.pos.height;
    global_position.alti = recv_sdk_std_msgs.pos.alti;
    global_position.ts = recv_sdk_std_msgs.time_stamp;

    global_position_degree = global_position;

    global_position_degree.lat = global_position.lat * 180.0f /M_PI;
    global_position_degree.lon = global_position.lon * 180.0f /M_PI;

    static int seted = 0;
    //TODO:
    // FIX BUG about flying at lat = 0
    if (global_position.ts != 0 && seted == 0 && global_position.lat != 0) {
        dji_variable::global_position_ref = global_position;
        seted = 1;
    }

    velocity.ts = recv_sdk_std_msgs.time_stamp;
    velocity.velx = recv_sdk_std_msgs.v.x;
    velocity.vely = recv_sdk_std_msgs.v.y;
    velocity.velz = recv_sdk_std_msgs.v.z;

    acc.ax = recv_sdk_std_msgs.a.x;
    acc.ay = recv_sdk_std_msgs.a.y;
    acc.az = recv_sdk_std_msgs.a.z;

    dji_variable::gps_convert_ned(
            local_position.x,
            local_position.y,
            global_position.lon,
            global_position.lat,
            dji_variable::global_position_ref.lon,
            dji_variable::global_position_ref.lat
    );


    local_position.height = global_position.height;
    local_position.ts = global_position.ts;
    dji_variable::local_position_ref = local_position;

    odem.header.frame_id = "dji_sys_0";
    odem.header.stamp = current_time;

    odem.pose.pose.position.x = local_position.x;
    odem.pose.pose.position.y = local_position.y;
    odem.pose.pose.position.z = local_position.height;

    odem.pose.pose.orientation.w = attitude_quad.q0;
    odem.pose.pose.orientation.x = attitude_quad.q1;
    odem.pose.pose.orientation.y = attitude_quad.q2;
    odem.pose.pose.orientation.z = attitude_quad.q3;

    odem.twist.twist.angular.x = attitude_quad.wx;
    odem.twist.twist.angular.y = attitude_quad.wy;
    odem.twist.twist.angular.z = attitude_quad.wz;

    odem.twist.twist.linear.x = velocity.velx;
    odem.twist.twist.linear.y = velocity.vely;
    odem.twist.twist.linear.z = velocity.velz;

    publishers::odem_publisher.publish(odem);


    rc_channels.pitch = recv_sdk_std_msgs.rc.pitch;
    rc_channels.roll = recv_sdk_std_msgs.rc.roll;
    rc_channels.mode = recv_sdk_std_msgs.rc.mode;
    rc_channels.gear_up = recv_sdk_std_msgs.rc.gear;
    rc_channels.throttle = recv_sdk_std_msgs.rc.throttle;
    rc_channels.yaw = recv_sdk_std_msgs.rc.yaw;

    ctrl_device = recv_sdk_std_msgs.ctrl_device;
    flight_status = recv_sdk_std_msgs.status;

    battery = recv_sdk_std_msgs.battery_remaining_capacity;
//    recv_sdk_std_msgs.status

    publishers::local_pos_pub.publish(local_position);

    publishers::att_quad_pub.publish(attitude_quad);

    publishers::gps_pub.publish(global_position);

    publishers::vel_pub.publish(velocity);

    publishers::acc_pub.publish(acc);

    publishers::rc_channels_pub.publish(rc_channels);
}


//----------------------------------------------------------
// timer spin_function 50Hz
//----------------------------------------------------------
void spin_callback(const ros::TimerEvent &e)
{

    update_ros_vars();
    //dji_variable::wp_m.loop();

    static unsigned int count = 0;
    count++;
    if (count % 50 == 0) {
        std_msgs::Float32 msg;

        msg.data = (float) recv_sdk_std_msgs.status;
        publishers::flight_status_pub.publish(msg);

        msg.data = (float) recv_sdk_std_msgs.battery_remaining_capacity;
        publishers::battery_pub.publish(msg);

#ifdef ROS_LOG_DETAILS
        ROS_INFO("STD_MSGS:");
        printf("[STD_MSGS] time_stamp %d \n", recv_sdk_std_msgs.time_stamp);
        printf("[STD_MSGS] q %f %f %f %f \n", recv_sdk_std_msgs.q.q0, recv_sdk_std_msgs.q.q1, recv_sdk_std_msgs.q.q2,
               recv_sdk_std_msgs.q.q3);
        printf("[STD_MSGS] a %f %f %f\n", recv_sdk_std_msgs.a.x, recv_sdk_std_msgs.a.y, recv_sdk_std_msgs.a.z);
        printf("[STD_MSGS] v %f %f %f\n", recv_sdk_std_msgs.v.x, recv_sdk_std_msgs.v.y, recv_sdk_std_msgs.v.z);
        printf("[STD_MSGS] w %f %f %f\n", recv_sdk_std_msgs.w.x, recv_sdk_std_msgs.w.y, recv_sdk_std_msgs.w.z);
        printf("[STD_MSGS] pos %f %f %f %f \n", recv_sdk_std_msgs.pos.lati, recv_sdk_std_msgs.pos.longti,
               recv_sdk_std_msgs.pos.alti, recv_sdk_std_msgs.pos.height);
        printf("[STD_MSGS] mag %d %d %d \n", recv_sdk_std_msgs.mag.x, recv_sdk_std_msgs.mag.y, recv_sdk_std_msgs.mag.z);
        printf("[STD_MSGS] rc %d %d %d %d %d %d\n", recv_sdk_std_msgs.rc.roll, recv_sdk_std_msgs.rc.pitch,
               recv_sdk_std_msgs.rc.yaw, recv_sdk_std_msgs.rc.throttle, recv_sdk_std_msgs.rc.mode,
               recv_sdk_std_msgs.rc.gear
        );
        printf("[STD_MSGS] gimbal %f %f %f\n", recv_sdk_std_msgs.gimbal.x, recv_sdk_std_msgs.gimbal.y,
               recv_sdk_std_msgs.gimbal.z);
        printf("[STD_MSGS] status %d\n", recv_sdk_std_msgs.status);
        printf("[STD_MSGS] battery %d\n", recv_sdk_std_msgs.battery_remaining_capacity);
        printf("[STD_MSGS] ctrl_device %d\n", recv_sdk_std_msgs.ctrl_device);

//        printf("[STD_MSGS] hacc %d\n", recv_sdk_std_msgs.hacc);
#endif
    }

    // test session ack for force close
    if (nav_force_close_req_id.reserve == 1) {
        std_msgs::Float32 msg2;
        msg2.data = 4;
        publishers::nav_ctrl_status_pub.publish(msg2);
        nav_force_close_req_id.reserve = 0;

        uint16_t ack = 0x0001;
        printf("Ack close send %d !!!!!!!!!!! \n", ack);
        App_Send_Ack(nav_force_close_req_id, (uint8_t *) &ack, sizeof(ack));
    }

}

 ros::Subscriber cmd_data_sub, nav_open_close_sub,
            activation_sub;


    int init_subscibers(ros::NodeHandle &nh)
    {
        cmd_data_sub = nh.subscribe("/sdk_request_cmd", 10, ros_cmd_data_callback);
        activation_sub = nh.subscribe("/sdk_request_activation", 10, ros_activation_callback);
        nav_open_close_sub = nh.subscribe("/nav_open_close_request", 10, ros_nav_open_close_callback);

        return 0;
    }
//----------------------------------------------------------
// main_function
//----------------------------------------------------------
int main(int argc, char **argv)
{

    printf("SDK Protocol\n");
    // initialize ros
    ros::init(argc, argv, "SDK_serial");
    ros::NodeHandle nh;
    ros::NodeHandle nh_private("~");

    nh_private.param("serial_name", serial_name, std::string("/dev/ttyACM0"));    // /dev/ttySAC0 or /dev/ttyUSB0
    nh_private.param("baud_rate", baud_rate, 230400);

    nh_private.param("app_id", app_id,1009412);
    nh_private.param("app_api_level", app_api_level, 2);
    nh_private.param("app_version", app_version, 1);
    nh_private.param("app_bundle_id", app_bundle_id, std::string("12345678901234567890123456789012"));

    nh_private.param("enc_key", enc_key,
                     std::string("5e758c5f4e76da52a202550e7923884f760decea78587f21d1ea91c1f15d1d7b"));

    activation_msg.app_id = (uint32_t) app_id;
    activation_msg.app_api_level = (uint32_t) app_api_level;
    activation_msg.app_ver = SDK_VERSION ;
    memcpy(activation_msg.app_bundle_id, app_bundle_id.c_str(), 32);

    key = (char *) enc_key.c_str();


    printf("[INIT] SET serial_port	: %s \n", serial_name.c_str());
    printf("[INIT] SET baud_rate	: %d \n", baud_rate);
    printf("[INIT] ACTIVATION INFO	: \n");
    printf("[INIT] 	  app_id     	  %d \n", activation_msg.app_id);
    printf("[INIT]    app_api_level	  %d \n", activation_msg.app_api_level);
    printf("[INIT]    app_version     %d \n", activation_msg.app_ver);
    printf("[INIT]    app_bundle_id	  %s \n", activation_msg.app_bundle_id);
    printf("[INIT]    enc_key	  %s \n", key);


    publishers::init_publishers(nh);
    service_handles::init_services(nh);
    init_subscibers(nh);

    // ros timer 50Hz
    simple_task_timer = nh.createTimer(ros::Duration(1.0 / 50.0), (const TimerCallback &) spin_callback);
    // open serial port
    Pro_Hw_Setup((char *) serial_name.c_str(), baud_rate);
    Pro_Link_Setup();
    App_Recv_Set_Hook(App_Recv_Req_Data);
    App_Set_Table(set_handler_tab, cmd_handler_tab);


    CmdStartThread();

    Pro_Config_Comm_Encrypt_Key(key);
    // ros spin for timer
    test_version_query();
    test_activation();
    try_to_open_control();

    ros::spin();

    return 0;
}
