/**********************************************************
 *  title: i2c.c
 *
 *  Createded: 02/02/2018
 *  Author: mr tanche
 *********************************************************/

/*****************	includes	**************************/
#include "i2c.h"
#include "RPBRX210.h"
/*****************	constants	**************************/
#define I2CDEBUG 1
#define WRITE 0xC4
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

    /* reset SCR register */
    SCI5.SCR.BYTE = 0;

    /* seting IO */
    MPC.PC2PFS.BYTE = 0x0A;
    MPC.PC3PFS.BYTE = 0x0A;
    /* port c2 et c3 en mode function & output */
    PORTC.PMR.BYTE = PORTC.PMR.BYTE | 0x0C;
    PORTC.PDR.BYTE = PORTC.PDR.BYTE | 0x0C;
    PORTC.ODR0.BIT.B4 = 1;
    PORTC.ODR0.BIT.B6 = 1;
    #ifdef I2CDEBUG
        printf("PMR:%x\n\rPDR:%x\n\r",PORTC.PMR.BYTE,PORTC.PDR.BYTE);
    #endif

    /* place output at high impedence */
    SCI5.SIMR3.BIT.IICSDAS = 0b11;
    SCI5.SIMR3.BIT.IICSCLS = 0b11;

    /* set SCI5 like i2c mode */
    SCI5.SMR.BYTE = 0x80;
    SCI5.SCMR.BYTE = 0x08;
    /* cke =0 -> internal clock sck output */
    SCI5.SCR.BIT.CKE = 0;
    /* set Baudrate at 250 000 */
    SCI5.BRR = 3;
    SCI5.SEMR.BYTE = 0;
    SCI5.SNFR.BYTE = 0;
    SCI5.SIMR1.BYTE = 1;
    SCI5.SIMR2.BYTE=0x23;/*???*/
    SCI5.SPMR.BYTE = 0;

    /* enable transmit/receive and interrupt*/
    SCI5.SCR.BYTE = 0xF4;
    for(int i=0;i<100;i++);
    IR(SCI5,TXI5)=0;
    IPR(SCI5,TXI5)=0x2;
    IEN(SCI5,TXI5)=1;       //enable interrupt 
    #ifdef I2CDEBUG
        printf("SMR: %x\n\rSCMR: %x\n\rSIMR1: %x\n\rSIMR2:\
                 %x\n\rSIMR3: %x\n\rSCR:%x\n\r",\
                SCI5.SMR.BYTE,SCI5.SCMR.BYTE,SCI5.SIMR1.BYTE,\
                SCI5.SIMR2.BYTE,SCI5.SIMR3.BYTE\
                ,SCI5.SCR.BYTE);
    #endif
}
/*****************	public functions	******************/
int start_i2c(void)
{
    /* init hardxare serial tr in i2c mode */
    init_hw_i2c_lidar();
    #ifdef I2CDEBUG
        printf("init i2c fini\n\r");
    #endif
    SCI5.SIMR3.BYTE |= 0x51;
    while(SCI5.SIMR3.BIT.IICSTIF!=1);

    SCI5.SIMR3.BIT.IICSTIF=0;
    SCI5.TDR= 0x62;
    while(IR(SCI5,TXI5==0));

    IR(SCI5,TXI5)=0;
    while(SCI5.SISR.BIT.IICACKR!=0);

    SCI5.TDR = 0x01;
    SCI5.SIMR3.BYTE |= 0x54;
    while(SCI5.SIMR3.BIT.IICSTIF!=1);
    SCI5.SIMR3.BIT.IICSTIF=0;
/* reception */
    SCI5.SIMR3.BYTE |= 0x51;
    while(SCI5.SIMR3.BIT.IICSTIF!=1);
    SCI5.SIMR3.BYTE |= 0x00;
    SCI5.TDR= 0x62;
    while(IR(SCI5,TXI5==0));

    while(SCI5.SISR.BIT.IICACKR!=0);
    SCI5.SIMR2.BIT.IICACKT = 0;
    SCI5.TDR = 0xFF;
    IR(SCI5,TXI5)=0;
    #ifdef I2CDEBUG
        printf("iic recept\n\r");
    #endif
    while(IR(SCI5,RXI5)!=1)
    IR(SCI5,RXI5)=0;
    printf("slave adr:%x\n\r",SCI5.RDR);
    while(IR(SCI5,TXI5==0));
    SCI5.SIMR2.BIT.IICACKT = 1;
    SCI5.TDR = 0xFF;
    while(IR(SCI5,RXI5)!=1)
    IR(SCI5,RXI5)=0;
    printf("slave data:%x\n\r",SCI5.RDR);
    SCI5.SIMR3.BYTE= 0x54;
    while(SCI5.SIMR3.BIT.IICSTIF!=1);
    SCI5.SIMR3.BIT.IICSTIF=0;
    SCI5.SIMR3.BYTE= 0xF0;
    while(1);

    LED1=~LED1;
    return 0;
}

