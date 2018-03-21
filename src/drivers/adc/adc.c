/**********************************************************
 *  title: adc.c
 *
 *  Createded: 23/01/2018
 *  Author: mr tanche
 *********************************************************/

/*****************	includes	**************************/
#include "adc.h"
/*****************	constants	**************************/
/*****************	private variables	******************/
/***************** 	privates functions	declaration ******/
/*****************  private type & struct	**************/
/*****************	public functions	******************/
/**********************************************************
 *
 *	function:init_adc() 
 *	description:initialize hardware of the adc
 *
 *********************************************************/
void init_adc()
{
    /* enable adc on clock management */
    SYSTEM.PRCR.WORD=0xA503;
    /* verif le prcr le bon reglage */
    SYSTEM.MSTPCRA.BIT.ACSE=0;
    MSTP(S12AD)=0;
    SYSTEM.PRCR.WORD=0xA500;

    /* set the pin out */
    /* set pin 40 like ADC (potar) */
    //droits d'écriture des registres PFS
    MPC.PWPR.BIT.B0WI=0;
    MPC.PWPR.BIT.PFSWE=1;
    // parametrage port ADC
    PORT4.PODR.BYTE = 0;
    PORT4.PMR.BYTE = 0;
    MPC.P40PFS.BIT.ASEL = 1;
    MPC.P41PFS.BIT.ASEL = 1;
    MPC.P42PFS.BIT.ASEL = 1;
    MPC.P43PFS.BIT.ASEL = 1;
    MPC.P44PFS.BIT.ASEL = 1;
    MPC.P45PFS.BIT.ASEL = 1;
    MPC.P46PFS.BIT.ASEL = 1;
    MPC.P47PFS.BIT.ASEL = 1;

    PORT4.PMR.BYTE = 0xFF;
    PORT4.PDR.BYTE= 0;
    //Fin des droits d'écriture des registres PFS
    MPC.PWPR.BIT.B0WI=1;
    MPC.PWPR.BIT.PFSWE=0;


    S12AD.ADCSR.WORD = 1;
    S12AD.ADCSR.BIT.ADIE = 1;
    S12AD.ADADS.WORD=0;
    S12AD.ADCER.WORD = 0;
    S12AD.ADEXICR.WORD = 0x0;
    S12AD.ADSHCR.BIT.SHANS = 0;

    /* set sampling value */
    S12AD.ADSSTR0=0x8F;

    IR(S12AD,S12ADI0)=0;
    IPR(S12AD,S12ADI0)=1;
    IEN(S12AD,S12ADI0)=0;
    S12AD.ADCSR.BIT.ADST=1;
}
/**********************************************************
 *
 *	function:get_adc() 
 *  return: short
 *	description:get value of the adc0 channel
 *
 *********************************************************/
 unsigned short get_adc(unsigned char i)
 {
    /* select the channel to convert */
    S12AD.ADANSA.WORD=1<<i;
    /* start conversion */
    S12AD.ADCSR.BIT.ADST=1;
    /* wait end conversion signal */
    while(!ICU.IR[IR_S12AD_S12ADI0].BIT.IR);
    ICU.IR[IR_S12AD_S12ADI0].BIT.IR = 0;
    /* return value */
    return *((&S12AD.ADDR0)+i);
}
