/**********************************************************
 *  title: asserv.c
 *
 *  Createded: 01/05/2018
 *  Author: mr tanche
 *********************************************************/

/*****************	includes	**************************/
#include "asserv.h"
/*****************	constants	**************************/
#define time pdMS_TO_TICKS(10)
#define SPEED_ASSERV
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
_s_pid asservD={1,0};
_s_pid asservO={1,0};
/*****************	public functions	******************/

TaskHandle_t asserv(void)
{
    /* init variable */
    TickType_t xLastWakeUp;
    /* variable sens =1 si + et -1 si - */
    int8_t sens_g = 1,sens_d=1;
    float p_dist=1,d_dist=0;
    float p_orient = 100, d_orient =0;
    float err_dist[3]={0,0,0};
    float err_orient[3]={0,0,0};
    float dErr_dist=0, dErr_orient=0;

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
        err_dist[2]=err_dist[1];
        err_dist[1]=err_dist[0];
        /* prend les deux semaphore odo et cmd */
        if(xSemaphoreTake(xOdo_mutex,portMAX_DELAY))
        {
            if(xSemaphoreTake(xCmd_mutex,portMAX_DELAY))
            {
                err_dist[0] = cmd.norme - odo.norme;
                dErr_dist = err_dist[0]-err_dist[1];
                xSemaphoreGive(xCmd_mutex);
            }
            xSemaphoreGive(xOdo_mutex);
        }

        err_orient[2]=err_orient[1];
        err_orient[1]=err_orient[0];
        err_orient[0] = cmd.orientation - odo.orientation;
        dErr_orient = err_orient[0]-err_orient[1];

        /* calcul correction */
        if(xSemaphoreTake(xPid_mutex,portMAX_DELAY))
        {
            commande_dist+= (int) ((asservD.p*dErr_dist)+\
                asservD.d*(err_dist[0]-2*err_dist[1]+\
                    err_dist[2]));

            commande_orient+= (int) ((asservO.p*dErr_orient)\
                +asservO.d*(err_orient[0]-2*err_orient[1]+\
                    err_orient[2]));
            xSemaphoreGive(xPid_mutex);
        }
        /* adaptation des resultats aux pwm */
        cmd_d = commande_dist - commande_orient;
        cmd_g = commande_dist + commande_orient;
        //printf("cmd_d: %i, cmd_g: %i \n\r",cmd_d,cmd_g);

        /* on tcheck s'il y a une inversion de sens */
        if(sens_g*cmd_g<0)
        {
            PWM_G=0;
            PWM_D=0;
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        else if(sens_d*cmd_d<0)
        {
            PWM_G=0;
            PWM_D=0;
           // vTaskDelay(pdMS_TO_TICKS(100));
        }
        /* balance les pwm man */
        if(cmd_g>=0)
        {
            if(cmd_g>MAX_PWM_ASSERV){cmd_g=MAX_PWM_ASSERV;}
            SENS_G = 1;
            PWM_G = cmd_g;
            sens_g = 1;
        }
        else if(cmd_g<0)
        {
            cmd_g *=-1;
            if(cmd_g>MAX_PWM_ASSERV){cmd_g=MAX_PWM_ASSERV;}
            SENS_G = 0;
            PWM_G = cmd_g;
            sens_g = -1;
        }
        if(cmd_d>=0)
        {
            if(cmd_d>MAX_PWM_ASSERV){cmd_d=MAX_PWM_ASSERV;}
            SENS_D = 1;
            PWM_D = cmd_d;
            sens_d = 1;
        }
        else if(cmd_d<0)
        {
            cmd_d = cmd_d*-1;
            if(cmd_d>MAX_PWM_ASSERV){cmd_d=MAX_PWM_ASSERV;}
            SENS_D = 0;
            PWM_D = cmd_d;
            sens_d = -1;
        }
        #ifdef SPEED_ASSERV
            PORT2.PODR.BIT.B2 =0;
        #endif
    }
}
