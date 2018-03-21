/************************************************************
* Autor			:Lebansais yo 
*
* File Name		: uart.c
* Version		: 1
* Device		: rx210_freertos
* H/W Platform	:
* Description	: drivers d'uart pour rx210 avec freertos
************************************************************/
/************************************************************
* History : DD.MM.YYYY     Version     Description
*           28.12.2017     Ver. 1      création
************************************************************/

/************************************************************
Includes   <System Includes> , "Project Includes"
************************************************************/
#include "uart.h"
/************************************************************
Macro definitions
************************************************************/


volatile _s_uart uart9;
/************************************************************
Private function
************************************************************/
static void init_hw_uart9();


/************************************************************
* Function Name	: init_hw_uart9
* Description	: initialisation du drivers de l'uart9
* Arguments     : none
* Return value	: none
************************************************************/
static void init_hw_uart9()
{
    int i=0;
   //unlock protection
    SYSTEM.PRCR.WORD=0xA502;
    //clear ACSE bit (all-module clock stop mode enable
    SYSTEM.MSTPCRA.BIT.ACSE=0;
   // cancel state of SCI5 peripheral to enable writing
    MSTP(SCI9)=0;
    SYSTEM.PRCR.WORD=0xA500;    // relocking

    //mise à 0 du registre de gestion de l'uart
    // stop TIE, RXI, TX, RX and set cke to internal
    SCI9.SCR.BYTE=0x00;

    //decla des ports
    // port7 en uart TX
    MPC.PB7PFS.BYTE=0x0A;
    PORTB.PMR.BIT.B7=1;
    PORTB.PDR.BIT.B7=1;
    MPC.PB6PFS.BYTE=0x0A;       // port6 en uart RX
    PORTB.PMR.BIT.B6 = 1;
    PORTB.PDR.BIT.B6 = 0;

    SCI9.SIMR1.BIT.IICM=0;      //clear to use uart
    SCI9.SPMR.BYTE=0xC0;        // clock polarity not invert
    SCI9.SMR.BYTE=0x00;         //b[0:1] =0b00 . pclk/1
                                //b2 no multiprocess mode
                                //b3 1 stop bit
                                //b4,b5 no parity
                                //b6 8 bits data
                                //b7 asynchrone mode
            //b0 serial communication interface mode
    SCI9.SCMR.BYTE=0;
                    //b1 should be xrite at 1
                    //b2 no invert data
                    //b3 MSB send first
                    //b[4:6] should be write at 1
                    // BCP2 =1 to have clock diviser 32
    SCI9.SEMR.BIT.ABCS=1;       // pour division par 32 de BRR
    SCI9.BRR=207;               // 9600 pour le moment
                                // set a transmission at 80kBds
    for(i=0;i<100;i++){}        //attente pour s'assurer de la bonne mise en place des paramètres
    SCI9.SCR.BYTE=0b11110000;   //b0,b1 internal clock select
                    //b2 transmit end interrupt enable
                    //b3 multipro unable
                    //b4 reception enable
                    //b5 transmission enable
                    //b6 recept interrupt enable
                    //b7 transmission interrupt enable

    IR(SCI9,RXI9)=0;        //on efface le flag rxi
    IR(SCI9,TXI9)=0;
    IPR(SCI9,RXI9)=0x2;     // priorité en fonction de freertos (inf 4)
    IPR(SCI9,TXI9)=0x2;
    IEN(SCI9,RXI9)=1;       //enable interrupt from controller
    IEN(SCI9,TXI9)=1;       //enable interrupt from controller
}


/* ENTRY POINTS ===========================================*/

/************************************************************
* Function Name	: init_uart9
* Description	: function to initialize struct uart and hw
* Arguments     : none
* Return value	: none
************************************************************/

void init_uart9(void)
{
    /* on commence par creer les queues */
    uart9.xReceptionQueue = xQueueCreate(100,DATA_SIZE_UART);
    uart9.xEmissionQueue = xQueueCreate(100,DATA_SIZE_UART);
    if(uart9.xEmissionQueue == NULL || \
        uart9.xReceptionQueue == NULL){LED_ERROR;}
    uart9.busy=0;
    /* on recupère les registre */
    uart9.p_registre = &SCI9;//0x8A120;
    /* on recup les resgistres d'interruption */
    uart9.tx_ien = ICU.IER[IER_SCI9_TXI9].BIT.IEN_SCI9_TXI9;
    uart9.tx_ir = ICU.IR[IR_SCI9_TXI9].BIT.IR;
    uart9.tx_ipr = ICU.IPR[IPR_SCI9_TXI9].BIT.IPR;

    uart9.rx_ien = ICU.IER[IER_SCI9_RXI9].BIT.IEN_SCI9_RXI9;
    uart9.rx_ir = ICU.IR[IR_SCI9_RXI9].BIT.IR;
    uart9.rx_ipr = ICU.IPR[IPR_SCI9_RXI9].BIT.IPR;
    /* init du hard de l'uart 9 */
    init_hw_uart9();
}
/************************************************************
* Function Name	: putChar_uart9
* Description	: function allow to send a char by uart9
*                 with freertos
* Arguments     : none
* Return value	: none
************************************************************/
void put_char(unsigned char mess, _s_uart *uart)
{
    //si jamais l'uart est libre on envoie
    if(uart->busy==0)
    {
        uart->busy=1;
        //on envoi les datas
        uart->p_registre->TDR=mess;
        //activation des interruptions de transmission
        uart->p_registre->SCR.BIT.TIE=1;
        uart->tx_ien = 1;
    }
    else
    {   /* sinon on ajoute à la queue d'envoi */
        xQueueSendToBack(uart->xEmissionQueue,&mess\
            ,pdMS_TO_TICKS(2));
       // uart.tx_ien = 1;
    }
}



/************************************************************
* Function Name	:put_string 
* Description	: function to send string on uart
*                 this function need an \n char to finish
*
* Arguments     : char * string for the message
                  _s_uart to define which uart using
* Return value	: none
************************************************************/
void put_string(unsigned char *mess, _s_uart *uart)
{
    do{
        put_char(*mess, uart);
        mess++;
    }while(*mess !='\n');
    put_char(*mess, uart);
}
/************************************************************
* Function Name	: EXECP_SCI9_ERI9
* Description	: uart interruption to tell us
*                   reception is end
************************************************************/
void Excep_SCI9_ERI9(void) {  }

/************************************************************
* Function Name	: interruption de reception uart9
* Description	: all in title
* Arguments	:none
* Return value	: void
************************************************************/
void Excep_SCI9_RXI9(void)
{
    BaseType_t  higher_prio = pdFALSE;
    /* get value from hardware reg on uart9 */
    xQueueSendToBackFromISR(uart9.xReceptionQueue, \
            &uart9.p_registre->RDR, &higher_prio);
        /* clean the flag */
    uart9.rx_ir = 0;
    /* ask for switching context */
    portYIELD_FROM_ISR(higher_prio);


    /*TODO take care about error of full queue */
}
/************************************************************
* Function Name	: interruption d'envoi via uart9
* Description	: all in title
* Arguments	:none
* Return value	: void
***********************************************************/

void Excep_SCI9_TXI9(void)
{
    BaseType_t higher_prio= pdFALSE;
    unsigned char data;

    if(uxQueueMessagesWaiting(uart9.xEmissionQueue))
    {
            xQueueReceiveFromISR(uart9.xEmissionQueue, \
                &data, &higher_prio);
        uart9.p_registre->TDR= data;

        portYIELD_FROM_ISR(higher_prio);
    }
    /* si rien à transmettre on desactive le tx */
    else{
        uart9.busy = 0;
        uart9.tx_ien = 0;
    }
    /* on remet le flag à 0 */
    uart9.tx_ir = 0;
}
/************************************************************
* Function Name	: Excep_SCI9_TEI9
* Description	: interruption end of emission
* Arguments	:none
* Return value	: void
************************************************************/

void Excep_SCI9_TEI9(void){}
/*===============================================
       TEST FUNCTIONS
===============================================*/

/************************************************************
* debug function
* resend what it recieve on uart
************************************************************/

void renvoi_le_recu(_s_uart *uart)
{
    unsigned char data;
    if(xQueueReceive(uart->xReceptionQueue,&data,0)!= \
                errQUEUE_EMPTY)
    {
        put_char(data,uart);
    }
}
