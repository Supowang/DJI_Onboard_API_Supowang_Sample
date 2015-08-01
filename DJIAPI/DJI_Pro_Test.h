/*
 * DJI_Pro_Test.h
 *
 *  Created on: 7 Apr, 2015
 *      Author: wuyuwei
 */

#ifndef DJI_PRO_TEST_H_
#define DJI_PRO_TEST_H_
#include "DJI_Pro_Codec.h"
#include "DJI_Pro_Link.h"
#include "DJI_Pro_App.h"
/* external functions */
void DJI_Pro_Test_Setup(void);
void DJI_Onboard_API_Activation(void);
void DJI_Onboard_API_Control(unsigned char arg);
void DJI_Onboard_API_UAV_Control(unsigned char data);
void DJI_Onboard_API_Ctr(unsigned char flagmode,unsigned int n,float a,float b,float c,float d);
void DJI_Onboard_API_Ctr_drawcube(void);
void DJI_Onboard_API_Ctr_drawcircle(void);
#endif /* DJI_PRO_TEST_H_ */
