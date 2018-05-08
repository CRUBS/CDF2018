/***********************************************************
				DESCRIPTION

************************************************************/
/***********************************************************
* Autor			: leconte mickael & lebansais yoann	
*
* File Name		: odometrie.h
* Version 		: 1 
* Device 		:rx210
* Description 		: file of functions to manage odometrie
************************************************************/
/************************************************************
* History       : DD.MM.YYYY     Version     Description
*                 17.01.2017     Ver. 1 
************************************************************/
#ifndef DECODER_QUADRALY04052018// multiple inclusion guard
#define DECODER_QUADRALY04052018

/***********************************************************
INCLUDE
************************************************************/
#include "iodefine.h"
#include "RPBRX210.h"
/***********************************************************
Macro definitions
************************************************************/
    //flag over et underflow MTU1
#define flag_over_MTU1 IR(MTU1,TCIV1)
#define flag_under_MTU1 IR(MTU1,TCIU1)
    //flag over et underflow MTU2
#define flag_over_MTU2 IR(MTU2,TCIV2)
#define flag_under_MTU2 IR(MTU2,TCIV2)
    // changement de sens de rotation MTU1
#define sens_rot_g  MTU1.TSR.BIT.TCFD
#define sens_rot_d  MTU2.TSR.BIT.TCFD

    // les compteurs maintenant
#define COMPTEUR_D MTU1.TCNT
#define COMPTEUR_G MTU2.TCNT
    //reset value of codeur register
#define INIT_COD    0x8000

/************************************************************
Exported global functions (to be accessed by other files)
************************************************************/

/*******************************************************************************
*
* Function
*
*******************************************************************************/
/**********************************************************
*		DECODER
**********************************************************/

void	init_mtclk(void);		// fonction d'initialisation du module de comptage
void	mtclk_start(void);		// démarre le décodage en quadrature
void mtclk_reset(void);
#endif
