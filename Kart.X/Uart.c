// Communication UART
// Biblioth�que de base

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>                 //Fichier include g�n�rique pour tous les Pics

//Initialisation du module UART n�1
void InitUart9600(unsigned char Fclk)
{
  TRISCbits.RC7=1;                 //RC7=RX en entr�e
  TRISCbits.RC6=1;                 //RC6=TX en entr�e (control�e par le PIC)
  ANSELCbits.ANSC7=0;              //d�sactivation du mode analogique de RC7
  ANSELCbits.ANSC6=0;              //d�sactivation du mode analogique de RC6
   
  PIE1bits.TXIE=0;                 //D�sactive les interruptions en transmission
  PIE1bits.RCIE=1;                 //D�sactive les interruptions en r�ception
  BAUDCONbits.BRG16=1;
  
  switch (Fclk)
  {
    case  1: SPBRG1=25; SPBRGH1=0; break;
    case  2: SPBRG1=51; SPBRGH1=0; break;
    case  4: SPBRG1=103; SPBRGH1=0; break;
    case  8: SPBRG1=207; SPBRGH1=0; break;
    case 16: SPBRG1=160; SPBRGH1=1; break;
    case 32: SPBRG1=64; SPBRGH1=3; break;
    case 64: SPBRG1=130; SPBRGH1=6; break;
    default: SPBRG1=207; SPBRGH1=0; break;
  }

  TXSTA1=0b00100100;               //TX9=0 �mission sur 8 bits, TXEN=1 validation de l'�metteur
                                   //SYNC=0 mode asynchrone, SENDB=0 pas d'�mission de break, BRGH=1 
  RCSTA1=0b10010000;               //SPEN=1 validation de l'UART, RX9=0 r�ception sur 8 bits
                                   //CREN=1 validation du r�cepteur ADDEN=0 d�validation de la d�tection d'adresses
}

//Initialisation du module UART n�1
void InitUart38400(unsigned char Fclk)
{
  TRISCbits.RC7=1;                 //RC7=RX en entr�e
  TRISCbits.RC6=1;                 //RC6=TX en entr�e (control�e par le PIC)
  ANSELCbits.ANSC7=0;              //d�sactivation du mode analogique de RC7
  ANSELCbits.ANSC6=0;              //d�sactivation du mode analogique de RC6
   
  PIE1bits.TXIE=0;                 //D�sactive les interruptions en transmission
  PIE1bits.RCIE=0;                 //D�sactive les interruptions en r�ception
  BAUDCONbits.BRG16=1;
  
  switch (Fclk)
  {
    case  1: SPBRG1=6; SPBRGH1=0; break;
    case  2: SPBRG1=12; SPBRGH1=0; break;
    case  4: SPBRG1=25; SPBRGH1=0; break;
    case  8: SPBRG1=51; SPBRGH1=0; break;
    case 16: SPBRG1=103; SPBRGH1=0; break;
    case 32: SPBRG1=207; SPBRGH1=0; break;
    case 64: SPBRG1=160; SPBRGH1=1; break;
    default: SPBRG1=51; SPBRGH1=0; break;
  }
  
  TXSTA1=0b00100100;               //TX9=0 �mission sur 8 bits, TXEN=1 validation de l'�metteur
                                   //SYNC=0 mode asynchrone, SENDB=0 pas d'�mission de break, BRGH=1 
  RCSTA1=0b10010000;               //SPEN=1 validation de l'UART, RX9=0 r�ception sur 8 bits
                                   //CREN=1 validation du r�cepteur ADDEN=0 d�validation de la d�tection d'adresses
}

//Emission d'un charact�re sur le module UART n�1
void UartWriteChar(unsigned char c)
{
  while (!TXSTA1bits.TRMT);        //�ventuellement, attend la fin d'�mission pr�c�dente
  TXREG1=c;                        //lance l'�mission
}

//R�ception d'un caract�re sur le module UART n�1
unsigned char UartReadChar(void)
{
  
  while (!PIR1bits.RCIF);           //attend la r�ception d'un caract�re
  return(RCREG1);                  //lecture du tampon UART et retour
}