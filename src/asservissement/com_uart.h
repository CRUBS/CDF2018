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

#ifndef LYCOM_UART28012018
#define LYCOM_UART28012018

#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "r_freertos.h"


#include "uart.h"
#include "variable_com_asserv.h"
#include "RPBRX210.h"
/**********		macro				**********************/
/**********		constants			**********************/
#define NB_BYTE_DATA 5
#define NB_BYTE_TRAME 9
#define START_BYTE 126
#define STOP_BYTE 236
/**********		global variables	**********************/
SemaphoreHandle_t xMutex_com_asser;
/**********		type & struct		**********************/
/**********		functions			**********************/

/* function design to be a task to read data on uart*/
void read_data_asserv(_s_uart *uart);
/* function to adapt data for our protocol and send it */
void write_data_asserv(_s_uart *uart);
#endif
