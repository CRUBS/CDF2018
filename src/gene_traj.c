/**********************************************************
 *  title:
 *
 *  Createded: 
 *  Author: 
 *********************************************************/

/*****************	includes	**************************/
#include "gene_traj.h"
#define TRAJ1
/*****************	constants	**************************/
/*****************	private variables	******************/
/***************** 	privates functions	declaration ******/
/*****************  private type & struct	**************/
/**********************************************************
 *
 * 	function:
 * 	description:
 *
 *********************************************************/
/*****************	public functions	******************/
void gene_traj(void)
{
    int16_t point_orient[25];
    /* cube puis interrupteur traj 1*/
#ifdef TRAJ1
   //int16_t  point_norme[20]={0,650,0,200,0,695,-400,0,220,0,480,150,-200,0};
   int16_t  point_norme[25]={0,650,0,200,0,700,-410,0,250,0,350,250,-300,-550,0,1000,360,0,140,0,0};
#endif
#ifdef TRAJ2
    /* interrupteur cube traj2 */
   int16_t  point_norme[25]={0,650,0,470,0,640,315,-300,0};
#endif
    if(COTE_ORANGE)
    {
       /* traj 1 */
#ifdef TRAJ1
        point_orient[0]=0;
        point_orient[1]=0;
        point_orient[2]=45;
        point_orient[3]=45;
        point_orient[4]=140;
        point_orient[5]=140;
        point_orient[6]=140;
        point_orient[7]=50;
        point_orient[8]=50;
        point_orient[9]=140;
        point_orient[10]=140;
        point_orient[11]=140;
        point_orient[12]=140;
        point_orient[13]=140;
        point_orient[14]=274;
        point_orient[15]=274;
        point_orient[16]=274;
        point_orient[17]=318;
        point_orient[18]=318;
        point_orient[19]=318;
        point_orient[20]=318;
        point_orient[21]=318;
#endif
#ifdef TRAJ2
    /* traj 2 */
        point_orient[0]=0;
        point_orient[1]=0;
        point_orient[2]=45;
        point_orient[3]=45;
        point_orient[4]=135;
        point_orient[5]=135;
        point_orient[6]=135;
        point_orient[7]=135;
        point_orient[8]=135;
#endif
        LED2=0;
    }
    else if(COTE_VERT)
    {
#ifdef TRAJ1
        point_orient[0]=0;
        point_orient[1]=0;
        point_orient[2]=-45;
        point_orient[3]=-45;
        point_orient[4]=-140;
        point_orient[5]=-140;
        point_orient[6]=-140;
        point_orient[7]=-50;
        point_orient[8]=-50;
        point_orient[9]=-140;
        point_orient[10]=-140;
        point_orient[11]=-140;
        point_orient[12]=-140;
        point_orient[13]=-140;
        point_orient[14]=-274;
        point_orient[15]=-274;
        point_orient[16]=-274;
        point_orient[17]=-318;
        point_orient[18]=-318;
        point_orient[19]=-318;
        point_orient[20]=-318;
        point_orient[21]=-318;

#endif
#ifdef TRAJ2
        point_orient[0]=0;
        point_orient[1]=0;
        point_orient[2]=-45;
        point_orient[3]=-45;
        point_orient[4]=-135;
        point_orient[5]=-135;
        point_orient[6]=-135;
        point_orient[7]=-135;
        point_orient[8]=-135;
#endif
        LED1=0;
    }
    else{LED1=0;LED2=0;}
        /* on suspend la tâche en attendant le départ */
     vTaskSuspend(NULL);

   TickType_t xDead;
    uint8_t time_mesure=0;
   uint8_t i =0;
   for(;;)
   {
//        printf("erreur:%.0f\n\r",odo.norme-point_norme[i]);
        if(xSemaphoreTake(xOdo_mutex,portMAX_DELAY))
        {
            if((abs(odo.norme-point_norme[i])<10) && \
                abs(odo.orientation-point_orient[i])<4)
            {
                if(xSemaphoreTake(xCmd_mutex,portMAX_DELAY))
                {
                    xDead= xTaskGetTickCount();
                    i++;
                    /*TRAJ 1*/
#ifdef TRAJ1
                    if(i>20){i=20;}
                    switch(i)
                    {
                        case 5:
                            xDead = xTaskGetTickCount();
                            time_mesure = 1;
                            break;
                        case 6:
                            time_mesure = 0;
                            break;
                        case 10:
                            taskENTER_CRITICAL();
                            sharp_avant = 0;
                            sharp_arriere = 1;
                            taskEXIT_CRITICAL();
                            xDead = xTaskGetTickCount();
                            time_mesure = 1;
                            break;
                        case 12:
                            time_mesure = 0;
                            break;
                        case 13:
                            taskENTER_CRITICAL();
                            sharp_avant = 1;
                            sharp_arriere = 1;
                            taskEXIT_CRITICAL();
                            break;
                        case 16:
                            taskENTER_CRITICAL();
                            sharp_avant = 0;
                            sharp_arriere = 1;
                            taskEXIT_CRITICAL();
                            break;
                        case 17:
                            vTaskResume(vServo);
                            if(COTE_ORANGE){servo_ouvert();}
                            else if(COTE_VERT){servo_fermee();}
                            vTaskDelay(pdMS_TO_TICKS(100));
                        case 18:
                            xDead = xTaskGetTickCount();
                            time_mesure=1;
                            break;

                            break;
                        case 19:
                                                        //vTaskSuspend(vAsserv);
                            time_mesure=0;
                            vTaskPrioritySet(vServo,7);
                            if(COTE_ORANGE){servo_fermee();}
                            else if (COTE_VERT){servo_ouvert();}
                            vTaskDelay(pdMS_TO_TICKS(5000));
                            vTaskSuspend(vServo);
                            break;
                        case 20:
                            PUISSANCE = 0;
                            break;
                  }
#endif
#ifdef TRAJ2
                    if(i>8){i=8;}
                    switch(i)
                    {
                        case 6:
                            taskENTER_CRITICAL();
                            sharp_avant = 0;
                            sharp_arriere = 1;
                            taskEXIT_CRITICAL();
                            xDead = xTaskGetTickCount();
                            time_mesure = 1;
                            break;
                        case 8:
                            time_mesure = 0;
                            taskENTER_CRITICAL();
                            sharp_avant = 0;
                            sharp_arriere = 1;
                            taskEXIT_CRITICAL();
                            break;
                  }
#endif
                    odo.norme = 0;
                    cmd.norme = point_norme[i];
                    cmd.orientation = point_orient[i];
//                    printf("on rentre dedans\n\r");
                    xSemaphoreGive(xCmd_mutex);
  //                  printf("on sort dedans\n\r");
                }
            }
            xSemaphoreGive(xOdo_mutex);
        }
        if(time_mesure)
        {
            if(xTaskGetTickCount()-xDead>pdMS_TO_TICKS(3000))
            {
                if(xSemaphoreTake(xCmd_mutex,portMAX_DELAY))
                {
                    i++;
                    odo.norme = 0;
                    cmd.norme = point_norme[i];
                    cmd.orientation = point_orient[i];
                    xSemaphoreGive(xCmd_mutex);
                    time_mesure =0;
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(500));
        }
}
