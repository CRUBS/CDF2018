/**********************************************************
 *  title: asserv.c
 *
 *  Createded: 01/05/2018
 *  Author: mr tanche
 *********************************************************/

/*****************	includes	**************************/
#include "asserv.h"
/*****************	constants	**************************/
//#define SPEED_ASSERV
/*****************	private variables	******************/
/*****************	privates functions	declaration ******/
/*****************  private type & struct	**************/
/**********************************************************
 *
 * 	function:
 * 	description:
 *
 *********************************************************/
/***************** public variables **********************/
_s_pid asservD={50,35,150};
//_s_pid asservD={0,0,0};
//_s_pid asservO={28,0,41200.0};
_s_pid asservO={300,90,65000};
/*****************	public functions	******************/

TaskHandle_t asserv(void)
{
    /* init variable */
    TickType_t xLastWakeUp;
    /* variable sens =1 si + et -1 si - */
    int8_t sens_g = 1,sens_d=1;
    float p_dist=1,d_dist=0;
    //float p_orient = 100, d_orient =0;
    float err_dist[2]={0,0};
    float err_orient[2]={0,0};
    float dErr_dist=0, dErr_orient=0;
    float sErr_dist=0, sErr_orient;


    int32_t commande_dist=0;
    int32_t commande_orient=0;
    int32_t cmd_g=0,cmd_d=0;

    init_pwm_asser();
    PWM_G =0;
    PWM_D =0;

    /* init variable mesure vitesse */
    PORT2.PDR.BIT.B2 = 1;
    PORT2.PMR.BIT.B2 = 0;
    PORT2.PODR.BIT.B2 =0;
    /* get time */
    xLastWakeUp = xTaskGetTickCount();
    for(;;)
    {
        /* echantillonnage freertos */
        vTaskDelayUntil(&xLastWakeUp, TPS_ECH);

        #ifdef SPEED_ASSERV
            PORT2.PODR.BIT.B2 =1;
        #endif
        /* mise à jours des erreurs */
        if(xSemaphoreTake(xCmd_mutex,0))
        {
            xSemaphoreGive(xCmd_mutex);
        }
        err_dist[1]=err_dist[0];
        err_orient[1]=err_orient[0];


        /* prend les deux semaphore odo et cmd */
        if(xSemaphoreTake(xOdo_mutex,portMAX_DELAY))
        {
            if(xSemaphoreTake(xCmd_mutex,portMAX_DELAY))
            {
                err_dist[0] = (cmd.norme) - odo.norme;
                err_orient[0] = (cmd.orientation) - \
                    odo.orientation;
                xSemaphoreGive(xCmd_mutex);
            }
           xSemaphoreGive(xOdo_mutex);
        }

        /* i distance */
        sErr_dist+=err_dist[0];
        if(sErr_dist>MAX_SOMME_ERREUR)
        {
            sErr_dist=MAX_SOMME_ERREUR;
        }
        else if(sErr_dist<-MAX_SOMME_ERREUR)
        {
            sErr_dist=-MAX_SOMME_ERREUR;
        }
        /* orientation */
        sErr_orient+=err_orient[0];
        if(sErr_orient>MAX_SOMME_ERREUR)
        {
            sErr_orient=MAX_SOMME_ERREUR;
        }
        else if(sErr_orient<-MAX_SOMME_ERREUR)
        {
            sErr_orient=-MAX_SOMME_ERREUR;
        }

        dErr_dist = err_dist[0]-err_dist[1];
        dErr_orient = err_orient[0]-err_orient[1];

//        printf("err_orient:%0.1f\n\r",dErr_orient);
        /* calcul correction */
        if(xSemaphoreTake(xPid_mutex,portMAX_DELAY))
        {
            //commande_dist+=(int) ((asservD.p*dErr_dist/10.0)\
                -asservD.d*(dist[0]-(2*dist[1])\
                     +dist[2])*2000.0);
            commande_dist=(int) ((asservD.p*err_dist[0]/10.0)\
                +(asservD.i*sErr_dist/1000.0)+\
                    (asservD.d*dErr_dist/DT));
            /*  orientation */
            commande_orient=(int) ((asservO.p*err_orient[0]/10.0)\
                +(asservO.i*sErr_orient/1000.0)+\
                    (asservO.d*dErr_orient/DT));
            //commande_orient+=(int) ((asservO.p*dErr_orient/10.0)\
                -asservO.d*(orient[0]-2*orient[1]+\
                orient[2])*2000.0)
            xSemaphoreGive(xPid_mutex);
        }
        /* adaptation des resultats aux pwm */
        cmd_d = commande_dist - commande_orient;
        cmd_g = commande_dist + commande_orient;

        /* on tcheck s'il y a une inversion de sens */
        if(cmd_g<0){sens_g=-1;}
        else {sens_g =1;}
        if(cmd_d<0){sens_d = -1;}
        else{sens_d = 1;}

        /*if(sens_g*cmd_g<0)
        {
            PWM_G=0;
            PWM_D=0;
            vTaskDelay(pdMS_TO_TICKS(100));
        }*/
        /*else if(sens_d*cmd_d<0)
        {
            PWM_G=0;
            PWM_D=0;
            vTaskDelay(pdMS_TO_TICKS(100));
        }*/
        /* balance les pwm man */
        if(cmd_g>=0)
        {
            if(cmd_g>MAX_PWM_ASSERV){cmd_g=MAX_PWM_ASSERV;}
            else if(cmd_g < MIN_PWM_ASSERV){cmd_g=0;}
            SENS_G = 1;
        }
        else if(cmd_g<0)
        {
            cmd_g *=-1;
            if(cmd_g>MAX_PWM_ASSERV){cmd_g=MAX_PWM_ASSERV;}
            else if(cmd_g < MIN_PWM_ASSERV){cmd_g=0;}
            SENS_G = 0;
        }
        if(cmd_d>=0)
        {
            if(cmd_d>MAX_PWM_ASSERV){cmd_d=MAX_PWM_ASSERV;}
            else if(cmd_d < MIN_PWM_ASSERV){cmd_d=0;}
            SENS_D = 1;
        }
        else if(cmd_d<0)
        {
            cmd_d = cmd_d*-1;
            if(cmd_d>MAX_PWM_ASSERV){cmd_d=MAX_PWM_ASSERV;}
            else if(cmd_d < MIN_PWM_ASSERV){cmd_d=0;}
            SENS_D = 0;
        }
        //printf("cmd_d:%i err:%.1f\n\r",cmd_d,sErr_dist);
        PWM_G=cmd_g;
        PWM_D=cmd_d;
        PWM_D=cmd_d;
        #ifdef SPEED_ASSERV
            PORT2.PODR.BIT.B2 =0;
        #endif
    }
}

void stop_deplacement(void)
{
    taskENTER_CRITICAL();
            cmd.orientation = odo.orientation;
            cmd.norme = odo.norme;
            xSemaphoreGive(xCmd_mutex);
    taskEXIT_CRITICAL();
}
