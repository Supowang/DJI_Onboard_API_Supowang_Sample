//============================================================================
// Name        : DJI_Onboard_Sdk_Test.cpp
// Author      : wuyuwei
// Version     :
// Copyright   : DJI Inc
// Description : DJI Onboard SDK test in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "DJI_Pro_Test.h"
#include <unistd.h>
using namespace std;



int main(int argc,char **argv)
{
	

	if(argc == 2 && strcmp(argv[1],"-v") == 0)
	{
		printf("\nDJI Onboard API Cmdline Test,Ver 1.0.0\n\n");
		return 0;
	}
	printf("\nDJI Onboard API Cmdline Test,Ver 1.0.0\n\n");

	if(DJI_Pro_Test_Setup() < 0)
	{
		printf("Serial Port Open ERROR\n");
		return 0;
	}
	
		
			/* api activation */
			DJI_Onboard_API_Activation();
			sleep(4);
			DJI_Onboard_API_Control(1);
			sleep(4);
			printf("OK\n");
			printf("OK\n");
			//DJI_Onboard_API_UAV_Control(4);
			while(1);
			
		

	return 0;
}
