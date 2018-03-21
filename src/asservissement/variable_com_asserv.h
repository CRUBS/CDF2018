/**********************************************************
 *
 * title: variable_com_asserv.h
 * description: fichier definissant les différentes 
 *              variables accessible et modifiable depuis
 *              l'interface uart...
 * date:28/01/2018
 * author: mr tanche
 * version: v0.1
 *
 *********************************************************/

#ifndef LYVAR_COM_ASSERV27012018
#define LYVAR_COM_ASSERV27012018

/**********		macro				**********************/
/**********		constants			**********************/
#define NB_ADR 16
/**********		global variables	**********************/
int acces_var[NB_ADR];
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

/**********		type & struct		**********************/
/**********		functions			**********************/
#endif
