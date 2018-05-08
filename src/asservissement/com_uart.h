/**********************************************************
 *
 * title: com_uart.h
 * description: get information from computer to set asserv
 *               (coef p and d)
 *              send info from robot about displacement
 *
 * date: 28/01/2018
 * author: mr tanche
 * version: v0.1
 *
 *********************************************************/

#ifndef LYCOM_ASSERV28012018
#define LYCOM_ASSERV28012018

#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "r_freertos.h"


#include "uart.h"
#include "asserv.h"
#include "RPBRX210.h"
/**********		macro				**********************/
/**********		constants			**********************/
#define NB_BYTE_DATA 5
#define NB_BYTE_TRAME 9
#define START_BYTE 126
#define STOP_BYTE 236
#define NB_ADR 17
/**********		global variables	**********************/
extern SemaphoreHandle_t xComAsser_mutex;
int asserv_var[NB_ADR];
/* descrition du tableau
-----------------------------------------------------------
    ADR     |       Valeur
    0           error overflow
    1           correcteur P orientation
    2           correcteur D orientation
    3           correcteur P distance
    4           correcteur D distance
    5           consigne orientation
    6           consigne distance
    7           consigne vitesse orientation
    8           consigne vitesse distance
    9           consigne accélération orientation
    10          consigne accélération distance
	11			orientation
	12			distance
	13			vitesse angulaire
	14			vitesse linéaire
	15			acceleration orient
	16			acceleration lineaire
*/

extern SemaphoreHandle_t xOdo_mutex;
extern SemaphoreHandle_t xCmd_mutex;
extern SemaphoreHandle_t xPid_mutex;
extern _s_odo odo;
extern _s_odo cmd;
extern _s_pid asservD;
extern _s_pid asservO;
/**********		type & struct		**********************/
/**********		functions			**********************/

/* function design to be a task to read data on uart*/
void read_data_asserv(_s_uart *uart);
/* function to adapt data for our protocol and send it */
void write_data_asserv(_s_uart *uart);
#endif
