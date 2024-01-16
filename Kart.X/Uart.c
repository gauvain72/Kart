// Communication UART
// Bibliothèque de base

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#include "Uart.h"                 //Fichier include générique pour tous les Pics

//Initialisation du module UART n°1
void InitUart9600(unsigned char Fclk)
{
  TRISCbits.RC7=1;                 //RC7=RX en entrée
  TRISCbits.RC6=1;                 //RC6=TX en entrée (controlée par le PIC)
  ANSELCbits.ANSC7=0;              //désactivation du mode analogique de RC7
  ANSELCbits.ANSC6=0;              //désactivation du mode analogique de RC6
   
  PIE1bits.TXIE=0;                 //Désactive les interruptions en transmission
  PIE1bits.RCIE=1;                 //Désactive les interruptions en réception
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

  TXSTA1=0b00100100;               //TX9=0 émission sur 8 bits, TXEN=1 validation de l'émetteur
                                   //SYNC=0 mode asynchrone, SENDB=0 pas d'émission de break, BRGH=1 
  RCSTA1=0b10010000;               //SPEN=1 validation de l'UART, RX9=0 réception sur 8 bits
                                   //CREN=1 validation du récepteur ADDEN=0 dévalidation de la détection d'adresses
}

//Initialisation du module UART n°1
void InitUart38400(unsigned char Fclk)
{
  TRISCbits.RC7=1;                 //RC7=RX en entrée
  TRISCbits.RC6=1;                 //RC6=TX en entrée (controlée par le PIC)
  ANSELCbits.ANSC7=0;              //désactivation du mode analogique de RC7
  ANSELCbits.ANSC6=0;              //désactivation du mode analogique de RC6
   
  PIE1bits.TXIE=0;                 //Désactive les interruptions en transmission
  PIE1bits.RCIE=0;                 //Désactive les interruptions en réception
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
  
  TXSTA1=0b00100100;               //TX9=0 émission sur 8 bits, TXEN=1 validation de l'émetteur
                                   //SYNC=0 mode asynchrone, SENDB=0 pas d'émission de break, BRGH=1 
  RCSTA1=0b10010000;               //SPEN=1 validation de l'UART, RX9=0 réception sur 8 bits
                                   //CREN=1 validation du récepteur ADDEN=0 dévalidation de la détection d'adresses
}

void InitUart115200(unsigned char Fclk)
{
  TRISCbits.RC7 = 1;    // RC7=RX en entrée
  TRISCbits.RC6 = 1;    // RC6=TX en entrée (controlée par le PIC)
  ANSELCbits.ANSC7 = 0;  // désactivation du mode analogique de RC7
  ANSELCbits.ANSC6 = 0;  // désactivation du mode analogique de RC6

  PIE1bits.TXIE = 0;  // Désactive les interruptions en transmission
  PIE1bits.RCIE = 0;  // Désactive les interruptions en réception
  BAUDCONbits.BRG16 = 1;

  unsigned int n = Fclk * 1000000 / (4UL * 115200) - 1; //16 or 4

  SPBRG1 = n;            // Load the low byte of the baud rate value
  SPBRGH1 = (n >> 8);    // Load the high byte of the baud rate value

  TXSTA1 = 0b00100100;   // TX9=0 émission sur 8 bits, TXEN=1 validation de l'émetteur
                         // SYNC=0 mode asynchrone, SENDB=0 pas d'émission de break, BRGH=1
  RCSTA1 = 0b10010000;   // SPEN=1 validation de l'UART, RX9=0 réception sur 8 bits
                         // CREN=1 validation du récepteur ADDEN=0 dévalidation de la détection d'adresses
}

//Emission d'un charactère sur le module UART n°1
void UartWriteChar(unsigned char c)
{
  while (!TXSTA1bits.TRMT);        //éventuellement, attend la fin d'émission précédente
  TXREG1=c;                        //lance l'émission
}

void UartWriteStr (unsigned char* c, short unsigned int lenght)
{
    for(short unsigned int i=0; i<lenght;i++){
        UartWriteChar(c[i]);    
        if (c[i] == '\0')
            break;    
    }
}

//Réception d'un caractère sur le module UART n°1
unsigned char UartReadChar(void)
{
  
  while (!PIR1bits.RCIF);           //attend la réception d'un caractère
  return(RCREG1);                  //lecture du tampon UART et retour
}
