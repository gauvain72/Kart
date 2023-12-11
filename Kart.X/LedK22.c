//Fichiers d'include génériques
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xc.h>

#include "Uart.h"
#include "kart.h"
#include "config.h"


//Définition des entrees/sorties du systeme
#define Led1      LATCbits.LC0   //sortie de visualisation
#define Led2      LATCbits.LC1

#define RelaiOn   LATCbits.LC2   //commande relais ON
#define RelaiOff  LATCbits.LC3   //commande relais OFF

//Double definition pour la fonction chenillard
#define Led3      LATCbits.LC2   
#define Led4      LATCbits.LC3   


const char version[] @ 0xF00000 = "V1.00 - LedK22 - 01/10/2023";


//Registre d'etat global
volatile unsigned char STATM @ 0x060;   
typedef union {
  struct {
    unsigned TEF    : 1;           //B0: a 1, indique un top d'echantillonnage
    unsigned VaON   : 1;           //B1: a 1, relai alim active
    unsigned        : 6;           //
  };
} STATMbits_t;  
STATMbits_t STATMbits @ 0x060;     //Declaration de STATM bit a bit

#define  TEF_bit     STATMbits.TEF
#define  VaON_bit    STATMbits.VaON


//Variables de fonctionnement du module en Banque 0:
unsigned short cpt        @0x061;

//Initialisation generale du Pic
void InitPic(void)
{
    /*
    //Initialisation du vecteur d'interruption
    #asm
    PSECT VectInterrupt,class=CODE,abs
    ORG 008h
      goto _TraitementIT
    ORG 018h
      goto _TraitementIT
    #endasm
    */
                                       //Au reset INTSRC=0 et MFIOSEL=0

    #if FCLK==1                        //Tcyc=4µs
      OSCCON=0b00110010;               //IRCF=%011 (1 MHz) et SCS=%10: horloge interne réglée sur 1 MHz
    #endif

    #if FCLK==2                        //Tcyc=2µs
      OSCCON=0b01000010;               //IRCF=%100 (2 MHz) et SCS=%10: horloge interne réglée sur 2 MHz
    #endif

    #if FCLK==4                        //Tcyc=1µs
      OSCCON=0b01010010;               //IRCF=%101 (4 MHz) et SCS=%10: horloge interne réglée sur 4 MHz
    #endif

    #if FCLK==8                        //Tcyc=500ns
      OSCCON=0b01100010;               //IRCF=%110 (8 MHz) et SCS=%10: horloge interne réglée sur 8 MHz
    #endif

    #if FCLK==16                       //Tcyc=250ns
      OSCCON=0b01110010;               //IRCF=%111 (16 MHz) et SCS=%10: horloge interne réglée sur 16 MHz
    #endif

    #if FCLK==20                       //Tcyc=200ns
      OSCCON=0b00000000;               //IRCF=%000 horloge interne réglée sur 31kHz dévalidée
    #endif

    #if FCLK==32                       //Tcyc=125ns
      OSCCON=0b01100000;               //IRCF=%110 (8 MHz) et SCS=%00: horloge interne réglée sur 8 MHz
      OSCTUNEbits.PLLEN=1;             //active la PLL
    #endif

    #if FCLK==64                       //Tcyc=62.5ns
      OSCCON=0b01110000;               //IRCF=%111 (16 MHz) et SCS=%00: horloge interne réglée sur 16 MHz
      OSCTUNEbits.PLLEN=1;             //active la PLL
    #endif

    //  INTCONbits.PEIE=1;               //validation des interruptions périphériques

      STATM=0;


    //Initialisation des ports d'entrée/sortie

    //RC0 (Led1), RC1 (Led2), RC2 (RelaiOn), RC3 (ReleiOff) en sortie logique
    //  ANSEL=0b01110000;
      /*TRISCbits.RC0=0;                 //RC0 en sortie
      TRISCbits.RC1=0;                 //RC1 en sortie
      TRISCbits.RC2=0;                 //RC2 en sortie
      TRISCbits.RC3=0;                 //RC3 en sortie
      //ANSELCbits.ANSC0=0;              Pas de mode analogique pour RC0
      //ANSELCbits.ANSC1=0;              Pas de mode analogique pour RC1
      ANSELCbits.ANSC2=0;              //désactivation du mode analogique de RC2
      ANSELCbits.ANSC3=0;              //désactivation du mode analogique de RC3

      Led1=0;
      Led2=0;
      RelaiOn=1;
      RelaiOff=1;

      TEF_bit=0;
      VaON_bit=0;

    */
}

//Interrupt CAN
void __interrupt() ISR(void)
{
    if (PIR1bits.ADIF) // Vérifiez si l'interruption du CAN a été déclenchée
    {
        //char dutycycle;
        char valccp1;
    
        valccp1 = ADRESH/2.55;//dutycycle = ADRESH/2.55;
                              //valccp1 = dutycycle;
        CCPR1L = valccp1;
        
        PIR1bits.ADIF = 0;  //Réinitialisez le drapeau d'interruption du CAN
    }
}

void main (void)
{
    //appel des differentes configurations 
    initInterup();
    InitPic();
    initTimer();
    initPWM();
    initADC();
    
    while(1)
    {
        readAn(3);//lecture sur le port Analogique3 = RA3;2=RA2;1=RA1;4=RA5,Le port 5,6et 7 n'existe pas, le rest sur PORTB et PORTC;19=RC7
    }
}
