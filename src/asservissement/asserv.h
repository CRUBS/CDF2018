/**********************************************************
 *
 * title: asserv.h
 * description: fichier definisssant les fonctions d'asservissement
 *
 * date: 01/05/2018
 * author: mr tanche
 * version: v0.1
 *
 *********************************************************/

#ifndef ASSERVLY01052018
#define ASSERVLY01052018

#include "stdlib.h"
#include "stdio.h"

#include "RPBRX210.h"
#include "iodefine.h"
#include "typedefine.h"
#include "r_freertos.h"
#include "pwm_RX210.h"

#include "odometrie.h"

/**********		macro				**********************/
/**********		constants			**********************/
/**********		global variables	**********************/
extern _s_odo odo;
extern _s_odo cmd;
extern SemaphoreHandle_t xOdo_mutex;
extern SemaphoreHandle_t xCmd_mutex;
extern SemaphoreHandle_t xPid_mutex;

/**********		type & struct		**********************/
typedef struct
{
    uint16_t p;
    float    i;
    uint16_t d;
}_s_pid;
/**********		functions			**********************/
TaskHandle_t asserv(void);
void stop_deplacement(void);
#endif
