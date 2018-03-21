/**********************************************************
				DESCRIPTION

************************************************************/
/************************************************************
* Autor			:Lebansais yo & Leconte Micki
*
* File Name		: uart.h
* Version		:1
* Device		:rx210
* Description		: fichier d'en tete
************************************************************/
/************************************************************
* History       : DD.MM.YYYY     Version     Description
*                 23.11.2016     Ver. 1
                  10.07.2017     Ver. 2     improve float
************************************************************/
#ifndef UART_H // multiple inclusion guard
#define UART_H

#include "iodefine.h"
#include "RPBRX210.h"
#include "interrupt_handlers.h"
#include "typedefine.h"
#include "r_freertos.h"
/************************************************************
CONSTANTS
************************************************************/
    /* nb de byte dans une trame de donnée */
#define DATA_SIZE_UART 1

/************************************************************
Macro definitions
************************************************************/
/************************************************************
Definition des structures
************************************************************/
typedef struct
{
    QueueHandle_t xReceptionQueue;
    QueueHandle_t xEmissionQueue;
    char busy;
    volatile struct st_sci0 *p_registre;
    /*************************************/
    volatile unsigned char tx_ien;
    volatile unsigned char tx_ir;
    volatile unsigned char tx_ipr;
    /************************************/
    volatile unsigned char rx_ien;
    volatile unsigned char rx_ir;
    volatile unsigned char rx_ipr;
}_s_uart;

/************************************************************
Exported global functions (to be accessed by other files)
************************************************************/
//hardware function
//fonction d'initialisation de l'uart n°9
void init_uart9(void);

//fonction permettant d'envoyer ou lire 
void put_string(unsigned char *mess, _s_uart *uart);
void put_char(unsigned char mess,_s_uart *uart);
void renvoi_le_recu(_s_uart *uart);

#endif
