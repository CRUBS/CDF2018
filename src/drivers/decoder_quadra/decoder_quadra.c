#include "decoder_quadra.h"

void init_mtclk(void)
{
	SYSTEM.PRCR.WORD=0xA502; //write enable for the register related to operating mode

	MSTP(MTU) = 0; // MTU(MTU0 to MTU5) module stop state is canceled

	// on désactive toutes les intérruptions de MTU1 et 2
	MTU1.TIER.BYTE=0x00;
	MTU2.TIER.BYTE=0x00;
	while(MTU2.TIER.BYTE!=0x00){} // on attend la fin de la désactivation
	MTU1.TIER.BIT.TCIEU=1;	//activation des interrup under et over flow
	MTU1.TIER.BIT.TCIEV=1;
	MTU2.TIER.BIT.TCIEU=1;
	MTU2.TIER.BIT.TCIEV=1;

	// mise en plase du MTU
	// les compteurs fonctionnent de manière indépendantes
	MTU.TSYR.BYTE=0x00;

	// on efface les compteurs
	COMPTEUR_G=INIT_COD;		//mise a la moitier du compteur gauche pour eviter le problème d'over/underflow dans un premier temps
	COMPTEUR_D=INIT_COD;		//mise à la moitier du compteur droite pour eviter le problème d'over/underflow dans un premier temps

	// netoyage des TGRA et TGRB
	MTU1.TGRA=0;
	MTU1.TGRB=0;

	//on désactive le clearing
	MTU1.TCR.BYTE=0x0;		// clearing TCNT is disable
	MTU2.TCR.BYTE=0x0;

	// sélection du phase counting mode 1
	MTU1.TMDR.BYTE=0x04;
	MTU2.TMDR.BYTE=0x04;

	SYSTEM.PRCR.WORD=0xA500; //write disable for all register
}


void mtclk_start(void)
{
	//démarrer le comptage
	MTU.TSTR.BYTE = 0x06; // demarrer compteur MTU1.TCNT et MTU2.TCNT
}
/**********************************************************************************
*	Function wich reinit register of decoder in quadra
**********************************************************************************/
void mtclk_reset(void)
{
	COMPTEUR_G =INIT_COD;
	COMPTEUR_D =INIT_COD;
}
