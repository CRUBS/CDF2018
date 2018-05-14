/**********************************************************
 *
 * title: robot.h 
 * description: definis les différentes caractéristiques du robots
 *
 * date: 04/05/2018
 * author:  mr tanche
 * version: v0.1
 *
 *********************************************************/

#ifndef LYROBOT04052018
#define LYROBOT04052018

#include "odometrie.h"
/**********		macro				**********************/
/**********		constants			**********************/
#define RATIO_RD 23.34 /*tck par mm roue droite */
#define RATIO_RG 23.42

#define L_ROBOT 331.0 /* largeur entre roue codeuse robot */

#define TPS_ECH 10  /* 10x0.1mS */
#define DT TPS_ECH/10000.0

#define SEUIL_15 1260
#define SEUIL_50 1000
/* definition asserv */
#define MIN_PWM_ASSERV 0
#define MAX_SOMME_ERREUR 3000.0
#define MAX_PWM_ASSERV 900

/* definition leds et switch robots */
#define LANCEUR PORTB.PIDR.BIT.B1
#define BP_INIT PORTA.PIDR.BIT.B5
#define COTE_ORANGE PORTB.PIDR.BIT.B0
#define COTE_VERT PORTA.PIDR.BIT.B7

/* definition du bit de puissance */
#define PUISSANCE PORTB.PODR.BIT.B4

/**********		global variables	**********************/
uint8_t couleur_traj;
/**********		type & struct		**********************/
/**********		functions			**********************/
#endif
