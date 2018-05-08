/**********************************************************
 *
 * title: odometrie.h
 * description: fichier de fonction d'odometrie
 *
 * date: 01/05/2018
 * author: mr tanche
 * version: v0.1
 *
 *********************************************************/

#ifndef ODOMETRIELY01052018
#define ODOMETRIELY01052018

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "r_freertos.h"
#include "RPBRX210.h"
#include "iodefine.h"
#include "typedefine.h"

#include "decoder_quadra.h"
#include "robot.h"

/**********		macro				**********************/
/**********		constants			**********************/
#define LARGEUR_ROBOT 333.0 /* largeur en mm */
#define RATIO_CODEUR 24.0     /* nb ticks/mm */
/**********		global variables	**********************/
extern SemaphoreHandle_t xOdo_mutex;

/**********		type & struct		**********************/
typedef struct 
{
    float x; /* position x en mm */
    float y; /* postion y en mm */
    int vitesse;
    int acceleration;
    int ang_vitesse;
    int ang_acceleration;
    float orientation; /* angle en degré */
    float norme;

}_s_odo;
/*---------------------------------dessin de la table ---*/
/*<----------------3 m = x -------------------------->
______________________________________________________
|                                                    | ^
|                                                    | |
|                                                    | |
|                                                    | |
|                                                    | y
|                                                    | |
|                                                    | |
|                                                    | |
|                                                    | |
|0___________________________________________________| v


/**********		functions			**********************/
TaskHandle_t odometrie(void);
#endif
