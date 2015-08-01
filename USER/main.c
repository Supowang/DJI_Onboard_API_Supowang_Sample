//////////////////////////////////////////////////////////////////////////////////	 
//DJI M100 STM32 移植Demo
//使用STM32 3.5官方库文件和正点原子的底层库
//参考DJI官方DJI_Onboard_API_Cmdline_Sample例程移植
//技术论坛: 官方http://forum.dev.dji.com/forum-73-1.html
//修改日期:2015/8/1
//版本：V1.0
//作者 Supowang（Lichao wang）
//All rights reserved	
//程序主要使用STM32完成对经纬 M100的激活、获取控制权、起飞、降落等控制，其他功能请参考官方SDK文档
// 并使用Onboard SDK的姿态控制命定完成两个基本的task：画正方体 画圆
//////////////////////////////////////////////////////////////////////////////////  
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	
#include "DJI_Pro_Test.h" 
extern  u8 RecBuff[160];//串口接收缓存数组，串口中断服务程序中调用
extern  u8 i;//接收到的数据，i累加
unsigned char PrintAck[]="";//转换接收到的16进制数据，存在此数组中用于液晶显示
unsigned char Dji_welcome[]="Welcome to Use Dji SDK!";//DJI欢迎词
//----------------------------------------------------------------------
// 打印飞控发回的应答数据
//----------------------------------------------------------------------
void display(u16 line)
	{
		LCD_ShowString(2,line,216,16,12,PrintAck);
  }
//----------------------------------------------------------------------
// 主函数
// 主要完成系统初始化，飞机激活、起飞、完成飞行任务，降落或返航
//----------------------------------------------------------------------
 int main(void)
 {	 
  u8 t;
	delay_init();	  //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(230400);	 	//串口初始化为230400，M100支持115200和230400，请结合N1调参软件修改
 	LED_Init();		//LED端口初始化
	LCD_Init();	  //液晶屏初始化
	KEY_Init();	 	//按键初始化  本例程使用 PE4启动飞机
  DJI_Pro_Test_Setup(); // 配置DJI SDK序列号 密钥 初始化通信链路
	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(20,10,200,16,16,Dji_welcome);	
	POINT_COLOR=BLUE;//设置字体为蓝色  YELLOW
	
	 //下面循环用于等待用户触发PE4按键启动飞机
	 	while(1)
		{
			t=KEY_Scan(0);
			if(t==1)
			break;
    }
	 delay_s(8);//按键后延时，防止实飞时飞机伤到人。按键开始激活飞机、起飞，请立即远离飞机，以免造成伤害！！！！！！可以适当加长延时时间
		
		//----------------------------------------------------------------------
// 此while循环完成飞机激活、获取控制权，如果没有激活成功或者获取到控制权，本次循环将终止跳到下次循环继续激活并获取控制权
// 直到成功获取后 执行起飞命定,break跳出循环继续执行飞行任务
//----------------------------------------------------------------------

	while(1)
	{	
		DJI_Onboard_API_Activation();//激活API
		delay_ms(500);
		if(i>3)
			{	
				HexToStr(PrintAck, RecBuff, i);
				display(40);
				if(RecBuff[0]==0xAA&RecBuff[12]==0x00&RecBuff[13]==0x00)
				  {
					   LCD_ShowString(20,55,200,16,12,"Activation Success            ");
          }
				else 
						 LCD_ShowString(20,55,200,16,12,"Activation error              ");
        i=0;				
      }
		else continue;
		delay_s(1);
		DJI_Onboard_API_Control(1);//获取控制权	
		delay_ms(500);
	  if(i>3)
			{
				
				HexToStr(PrintAck, RecBuff, i);
				display(80);
				if(RecBuff[0]==0xAA&RecBuff[12]==0x02&RecBuff[13]==0x00)
				{
					LCD_ShowString(20,95,200,16,12,"Obtain Success                ");
        }
				else
					LCD_ShowString(20,95,200,16,12,"Obtain error                  ");
				i=0;				
      }	
   else continue;	
   delay_s(1);			
	 DJI_Onboard_API_UAV_Control(4);//起飞
	 delay_ms(500);
			if(i>3)
			{
				HexToStr(PrintAck, RecBuff, i);
				display(120);
				if(RecBuff[0]==0xAA&RecBuff[12]==0x02&RecBuff[13]==0x00)
				{
					LCD_ShowString(20,135,200,16,12,"Begin to Take off");
        }
				i=0;				
      }	
	 else continue;
      break; 
  }
	
// 开始执行任务
				delay_s(5);	
	      LCD_ShowString(20,155,200,16,12,"Take off Success");
				LCD_ShowString(20,175,200,16,12,"Begin to fly cube");
				DJI_Onboard_API_Ctr_drawcube();//画正方体
	      delay_s(1);
				DJI_Onboard_API_Ctr(0x48,100,1,-0.414,0,0);//调整位置，规划圆心，准备画圆
				delay_s(4);
				LCD_ShowString(20,195,200,16,12,"Begin to fly circle");
				DJI_Onboard_API_Ctr_drawcircle();//在正方体上方绘制外接圆
				delay_s(4);
//				LCD_ShowString(20,215,200,16,12,"Begin to landing");
//				DJI_Onboard_API_UAV_Control(6);//降落	
			
       	DJI_Onboard_API_Ctr(0x48,200,6,0,0,0);//拉开距离，使飞机飞出起飞点20m外，方便测试返航功能，实飞时请不要执行该命定。
				LCD_ShowString(20,215,200,16,12,"Begin to go home");
				DJI_Onboard_API_UAV_Control(1);//自动返航
	      while(1)
				{
					LCD_ShowString(20,235,200,16,12,"All task finished ");
        }
}


