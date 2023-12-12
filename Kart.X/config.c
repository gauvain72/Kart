#include "config.h"
#include <xc.h>

//Variables de fonctionnement du module en Banque 0:
unsigned short cpt        @0x061;

//Registre d'etat global
volatile unsigned char STATM @ 0x060;   
typedef union {
  struct {
    unsigned TEF    : 1;           //B0: a 1, indique un top d'echantillonnage
    unsigned VaON   : 1;           //B1: a 1, relai alim active
    unsigned        : 6;           //
  };
} STATMbits_t;  
STATMbits_t STATMbits @ 0x060; 

//Initialisation generale du Pic
void InitPic(void)
{
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

    STATM=0;
}