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
 * 	function: int_to_char()
 * 	description:function to fit a int into a full
 *              array of 5 char
 *
 *********************************************************/
static int int_to_char(int *var,char *buff)
{
    /* buffer temporaire */
    char *temp;
   /* transform en string et check overflow*/
    sprintf(temp,"%i",*var);
    if((int)abs(*var)>=pow(10,NB_BYTE_DATA))\
        {return EXIT_FAILURE;}
    strcpy(buff,temp);
    return EXIT_SUCCESS;
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
            xQueueReceive(uart->xReceptionQueue,&buff,\
                portMAX_DELAY);
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
                        if(xSemaphoreTake(xMutex_com_asser,\
                            portMAX_DELAY)==pdTRUE)
                        {
                            acces_var[adr]=atoi(reading_temp);
                            xSemaphoreGive(xMutex_com_asser);
                        }
                    }
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
    char trm_nb=0;
    char trame[NB_BYTE_DATA];
    for(;;)
    {
        if(xSemaphoreTake(xMutex_com_asser,portMAX_DELAY)\
            ==pdTRUE)
        {
            for(char i=1;i<NB_ADR;i++)
            {
                /* envoyer vers traitement */
                /* envoyer sur uart */ 
                if(!int_to_char(&acces_var[i],trame))
                {
                    put_char(START_BYTE,uart);
                    put_char(i,uart); /* i = adresse */
                    put_char(trm_nb,uart);
                    for(char j=0;i<NB_BYTE_DATA;i++){\
                        put_char(trame[i],uart);}
                    put_char(checksum(trame),uart);
                    put_char(STOP_BYTE,uart);
                }
                else    /* cas ou int_to_char fail */
                {
                    put_char(START_BYTE,uart);
                    put_char(0,uart); /* i = adresse */
                    put_char(trm_nb,uart);
                    /*transform i en char methode bofbof*/
                    /* car utilisation de int_to_char sans
                        verif du retour */
                    int_to_char(&i,trame);
                    for(char j=0;i<NB_BYTE_DATA;i++)\
                        {put_char(trame[i],uart);}
                    put_char(checksum(trame),uart);
                    put_char(STOP_BYTE,uart);
                }
            }
        }
    }
}

