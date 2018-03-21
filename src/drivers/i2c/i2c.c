/**********************************************************
 *  title: i2c.c
 *
 *  Createded: 02/02/2018
 *  Author: mr tanche
 *********************************************************/

/*****************	includes	**************************/
#include "i2c.h"
/*****************	constants	**************************/
/*****************	private variables	******************/
/*****************	privates functions	declaration ******/
static void init_hw_i2c_lidar();
/*****************  private type & struct	**************/
/**********************************************************
 *
 *	function: init_hw_i2c
 *	description: initialisation de l'hw pour i2c du lydar
 *
 *********************************************************/
static void init_hw_i2c_lidar()
{
    //unlock protection
    SYSTEM.PRCR.WORD=0xA502;
    //clear ACSE bit (all-module clock stop mode enable
    SYSTEM.MSTPCRA.BIT.ACSE=0;
   // cancel state of SCI5 peripheral to enable writing
    MSTP(SCI5)=0;
    SYSTEM.PRCR.WORD=0xA500;    // relocking

    /* seting IO */
    MPC.PC2PFS.BYTE = 0x0A;
    MPC.PC3PFS.BYTE = 0x0A;
    /* port c2 et c3 en mode function & output */
    PORTC.PMR.BYTE = PORTC.PMR.BYTE | 0x0C;
    PORTC.PDR.BYTE = PORTC.PDR.BYTE | 0x0C;
    PORTC.ODR0.BIT.B4 = 1;
    PORTC.ODR0.BIT.B6 = 1;

    /* reset SCR register */
    SCI5.SCR.BYTE = 0;
    /* place output at high impedence */
    SCI5.SIMR3.BIT.IICSDAS = 0b11;
    SCI5.SIMR3.BIT.IICSCLS = 0b11;

    /* set sci5 like i2c mode */
    SCI5.SCMR.BYTE = 0x80;
    /* cke =0 -> internal clock sck output */
    SCI5.SCR.BIT.CKE = 0;
    /* set Baudrate at 250 000 */
    SCI5.BRR = 3;
    SCI5.SEMR.BYTE = 0;
    SCI5.SIMR1.BYTE = 1;
    SCI5.SIMR2.BYTE=0x23;/*???*/
    SCI5.SPMR.BYTE = 0;

    /* enable transmit/receive and interrupt*/
    SCI5.SCR.BYTE |= 0xB0;
    IR(SCI5,TXI5)=0;
    IPR(SCI9,TXI9)=0x2;
    IEN(SCI5,TXI5)=1;       //enable interrupt 
}
/*****************	public functions	******************/
