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
#ifndef DEF_ODOMETRIE// multiple inclusion guard
#define DEF_ODOMETRIE

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
#define compteur_d MTU1.TCNT
#define compteur_g MTU2.TCNT
    //reset value of codeur register
#define INIT_COD    0x8000

/************************************************************
Exported global functions (to be accessed by other files)
************************************************************/

// struct to define parametre en odometrie
//extern typedef struct odometrie odometrie;
struct odometrie{
/* distance of each side in tck */
    int drg_tck[2];
    int drd_tck[2];
// calcul in tck of the absolute position
    unsigned int x_tck;
    unsigned int y_tck;

/* angle of the robot between origin */
    float theta_tck;    //angle en tck

/* calcul with tck  */
    float dist_tck;
    int angl_tck;
};

struct speed{
/* speed by wheel */
    int vrd_tck[2];
    int vrg_tck[2];
/* speed of robot */
    int v_tck;
    int w_tck;
};

struct acc{
/* acc by wheel */
    int ard_tck;
    int arg_tck;
/* acc of robot */
    int a_tck;
    int a_ang_tck;
};

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
#endif
