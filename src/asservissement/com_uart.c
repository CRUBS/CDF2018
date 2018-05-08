/**********************************************************
 *  title: com_uart.h
 *
 *  Date: 28/01/2018
 *  Author: Mr tanche
 *  version: v0.1
 *********************************************************/

/*****************	includes	**************************/
#include "com_uart.h"
/*****************	constants	**************************/
/*****************	private variables	******************/
/***************** 	privates functions	declaration ******/

/* function to verify the checksum */
static int checksum_check(char *buff, char check);
/* calcul and return the checksum */
static char checksum(char *buff);
/* transform a int into a NB_BYTE_DATA char array */
static int int_to_char(int *var,char *buff);
/* initialisation des variable partage */
static void init_asserv_var(void);
/*****************  private type & struct	**************/
/**********************************************************
 *
 * 	function:checksum_check
 * 	description: function to verify the checksul value
 *
 *********************************************************/
static int checksum_check(char *buff, char check)
{
    char res=0;
    for(char i=0;i<NB_BYTE_DATA;i++){res+=*(buff+i);}
    if((char)res==check){return EXIT_SUCCESS;}
    return EXIT_FAILURE;
}

/**********************************************************
 *
 * 	function:checksum
 * 	description: function to calcul and return the chekcsum
 *
 *********************************************************/
static char checksum(char *buff)
{
    char res=0;
    for(char i=0;i<NB_BYTE_DATA;i++){res+=*(buff+i);}
    return res;
}
/**********************************************************
 *
 *	function: int_to_char()
 *	description:function to fit a int into a full
 *              array of 5 char
 *
 *********************************************************/
static int int_to_char(int *var,char *buff)
{
    /* buffer temporaire */
    char *temp;
   /* transform en string et check overflow*/
    sprintf(temp,"%5i",*var);
    /* le moindre travail en float plante ! */
    //if((int)abs(*var)>=pow(10,NB_BYTE_DATA))\
        {return EXIT_FAILURE;}
    strcpy(buff,temp);
    return EXIT_SUCCESS;
}
/**********************************************************
 *
 *	function: init_access_var()
 *	description:function to initialize var array
 *
 *********************************************************/

static void init_asserv_var(void)
{
    for(int i=0;i<NB_ADR;i++){asserv_var[i]=0;}
}
/*****************	public functions	******************/

/**********************************************************
 *
 *	functionata_asserv
 *  param: _s_uart
 *	description:
 *       read_data_from_uart_driver and treat it
 *
 *********************************************************/

void read_data_asserv(_s_uart *uart)
{
    unsigned char reading_temp[NB_BYTE_DATA]; 
    unsigned char buff,adr,crc;
    char *test;
    int res=0;
    for(;;)
    {
            if(xQueueReceive(uart->xReceptionQueue,&buff,\
                portMAX_DELAY))
            {
                /* verif byte de start */
                if(buff==START_BYTE)
                {
                    /* get adr destination */
                    xQueueReceive(uart->xReceptionQueue,&adr,\
                    portMAX_DELAY);
                /* get data */
                    for(char i=0;i<NB_BYTE_DATA;i++)
                    {
                        xQueueReceive(uart->xReceptionQueue,\
                        (reading_temp+i),portMAX_DELAY);
                    }
                /* check checksum */
                    xQueueReceive(uart->xReceptionQueue,&crc,\
                    portMAX_DELAY);

                    if(!checksum_check(reading_temp,crc))
                    {
                        /*verif stopping byte */
                        xQueueReceive(uart->xReceptionQueue,\
                        &buff,portMAX_DELAY);
                        if(buff==STOP_BYTE)
                        {
                        /* assembler les données et envoyer à
                           la bonne variable */
                            if(xSemaphoreTake(xComAsser_mutex,\
                            portMAX_DELAY)==pdTRUE)
                            {
                                asserv_var[adr]=atoi(reading_temp);
                                xSemaphoreGive(xComAsser_mutex);
                            }
                        }
                    }
                }
                if(xSemaphoreTake(xPid_mutex,portMAX_DELAY))
                {
                    asservO.p = asserv_var[1];
                    asservO.d = asserv_var[2];
                    asservD.p = asserv_var[3];
                    asservD.d = asserv_var[4];
                    xSemaphoreGive(xPid_mutex);
                }
            }
    }

}


/**********************************************************
 *
 *	function: write_data_asserv
 *  param: _s_uart
 *	description:
 *       treat data from other task and send it
 *       write_data_on_uart_driver
 *
 *********************************************************/
void write_data_asserv(_s_uart *uart)
{
    char trame[NB_BYTE_DATA];
    /* on init le tab de var partagées */
    init_asserv_var();
    for(;;)
    {
        LED1=~LED1;

        if(xSemaphoreTake(xOdo_mutex,portMAX_DELAY))
        {
            asserv_var[11]=(int)odo.orientation;
            asserv_var[12]=(int)odo.norme;
            asserv_var[13]=(int)odo.ang_vitesse;
            asserv_var[14]=(int)odo.vitesse;
            asserv_var[15]=(int)odo.ang_acceleration;
            asserv_var[16]=(int)odo.acceleration;
            xSemaphoreGive(xOdo_mutex);
        }

        if(xSemaphoreTake(xPid_mutex,portMAX_DELAY))
        {
            asserv_var[1]=(int) asservO.p;
            asserv_var[2]=(int) asservO.d;
            asserv_var[3]=(int) asservD.p;
            asserv_var[4]=(int) asservD.d;
            xSemaphoreGive(xPid_mutex);
        } 
        if(xSemaphoreTake(xComAsser_mutex,portMAX_DELAY)\
            ==pdTRUE)
        {
            for(char i=1;i<NB_ADR;i++)
            {
                /* envoyer vers traitement */
                /* envoyer sur uart */
                if(!int_to_char(&asserv_var[i],trame))
                {
					printf("%i%i%i%i%i",START_BYTE,i,asserv_var[i],\
						checksum(trame),STOP_BYTE);

                    put_char(START_BYTE,uart);
                    put_char(i,uart); /* i = adresse */
                    for(char j=0;j<NB_BYTE_DATA;j++){\
                        put_char(trame[j],uart);}
                    put_char(checksum(trame),uart);
                    put_char(STOP_BYTE,uart);
                }
                else    /* cas ou int_to_char fail */
                {
                    put_char(START_BYTE,uart);
                    put_char('b',uart); /* i = adresse */
                    /*transform i en char methode bofbof*/
                    /* car utilisation de int_to_char sans
                        verif du retour */
                    int_to_char(&i,trame);
                    for(char j=0;j<NB_BYTE_DATA;j++)\
                        {put_char(trame[j],uart);}
                    put_char(checksum(trame),uart);
                    put_char(STOP_BYTE,uart);
                }
            }
            xSemaphoreGive(xComAsser_mutex);
        }
        /* ne renvoi les data que toutes les 250 ms */
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

