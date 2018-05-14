/**********************************************************
 *
 * title:
 * description:
 *
 * date: 
 * author: 
 * version: v0.1
 *
 *********************************************************/

#ifndef LYGENETRAJ09052018
#define LYGENETRAJ09052018

#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"

#include "iodefine.h"
#include "r_freertos.h"
#include "robot.h"
#include "RPBRX210.h"
#include "SERVO.h"

/**********		macro				**********************/
/**********		constants			**********************/
/**********		global variables	**********************/
extern _s_odo odo;
extern _s_odo cmd;
extern SemaphoreHandle_t xOdo_mutex;
extern SemaphoreHandle_t xCmd_mutex;
extern uint8_t sharp_avant;
extern uint8_t sharp_arriere;
extern TaskHandle_t vAsserv;
extern TaskHandle_t vDetection;
extern TaskHandle_t vServo;
/**********		type & struct		**********************/
/**********		functions			**********************/
void gene_trag(void);
#endif
