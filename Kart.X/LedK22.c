
//Projet exemple pour carte évaluation 26K22

//01/10/2023: V1.00 - Initialisation du projet

//Fichiers d'include génériques
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xc.h>                 //Fichier include générique pour tous les Pics

#include "Uart.h"


// Remarque, liste des pragma config dans C:\Program Files\Microchip\xc8\v1.35\docs\chips
#pragma config WDTEN=OFF        //Watchdog contrôlé par SWDTEN
#pragma config WDTPS=512        //Postdiviseur Watchdog -> période 2s environ
#pragma config PWRTEN=OFF       //Power up timer désactivé
#pragma config BOREN=OFF        //
#pragma config BORV=250         //Seuil BOR 2.5V
#pragma config MCLRE=EXTMCLR    //Master reset externe
#pragma config DEBUG=OFF        //Mode debug désactivé
#pragma config LVP=OFF          //Mode programmation Low Voltage désactivé

#pragma config IESO=OFF
#pragma config PRICLKEN=OFF     //primary clock
#pragma config FCMEN=OFF        //fail-safe clock monitor
#pragma config FOSC=INTIO67     //Oscillateur interne


//Fréquence d'horloge choisie: (en MHz)
#define FCLK 1    
#define _XTAL_FREQ 1000000      //define nécessaire pour l'utilisation des fonctions __delay

#if (FCLK==1)||(FCLK==2)||(FCLK==4)||(FCLK==8)||(FCLK==16)
#pragma config PLLCFG=OFF       //PLL désactivée
#endif

#if (FCLK==32)||(FCLK==64)
#pragma config PLLCFG=ON        //PLL désactivée
#endif


//Définition des entrées/sorties du système
#define Led1      LATCbits.LC0   //sortie de visualisation
#define Led2      LATCbits.LC1

#define RelaiOn   LATCbits.LC2   //commande relais ON
#define RelaiOff  LATCbits.LC3   //commande relais OFF

//Double définition pour la fonction chenillard
#define Led3      LATCbits.LC2   
#define Led4      LATCbits.LC3   


const char version[] @ 0xF00000 = "V1.00 - LedK22 - 01/10/2023";


//Registre d'état global
volatile unsigned char STATM @ 0x060;   
typedef union {
  struct {
    unsigned TEF    : 1;           //B0: à 1, indique un top d'échabtillonnage
    unsigned VaON   : 1;           //B1: à 1, relai alim activé
    unsigned        : 6;           //
  };
} STATMbits_t;  
STATMbits_t STATMbits @ 0x060;     //Déclaration de STATM bit à bit

#define  TEF_bit     STATMbits.TEF
#define  VaON_bit    STATMbits.VaON


//Variables de fonctionnement du module en Banque 0:
unsigned short cpt        @0x061;



//-----------------------------------------------------------------------------------------------------------------
//Commande ON du relai n
void RelaiON(unsigned char n)
{
}


//-----------------------------------------------------------------------------------------------------------------
//Commande OFF du relai n
void RelaiOFF(unsigned char n)
{
}


//-----------------------------------------------------------------------------------------------------------------
//Initialisation générale du Pic
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



void __interrupt() ISR(void); //interuption pour le CAN
void init_timer(void); //init du timer2 pour la pwm
void initPWM(void); //init de la PWM
void initADC(void); //config entre Analogique et du CAN
void init_interup(void); //autorise l'interruption sur le CAN
void lecturAn(char port);//lit le Port analogique du CAN(avoir config la broche ds initADC(void))
//-----------------------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------------------
void main (void)
{
//-----------------------------------------------------------------------------------------------------------------
    //appel des differentes configurations 
    init_interup();
    InitPic();
    init_timer();
    initPWM();
    initADC();
    
 //-----------------------------------------------------------------------------------------------------------------
    
    
    
    while(1)
    {
        lecturAn(3);//lecture sur le port Analogique3 = RA3;2=RA2;1=RA1;4=RA5,Le port 5,6et 7 n'existe pas, le rest sur PORTB et PORTC;19=RC7
    }
//-----------------------------------------------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------------------

void init_timer(void)
{
    OSCCON=0x72;//freq OSC a 16MHz
    PR2=100;// FREQ timer2 a 40KHz
    T2CON=0b00000100;//Prediviseur à 1
    TMR2=0;
}

//-----------------------------------------------------------------------------------------------------------------

void initPWM(void)
{
   
    CCP1CON=0x0C; //configuration ccp1 en PWM sortie sur RC2  
    CCPR1L = 40;// configuration du rapport cyclique
    CCPTMRS0=0x00;//met le timer 2 sur la PWM
    TRISCbits.RC2=0; //config en sortie pour la PWM
}

//-----------------------------------------------------------------------------------------------------------------

void initADC(void)
{
	// Configurer PORT A comme entrée analogique
	ANSELA = 0xFF;
    TRISA = 0xFF;
	// Configuration du module CAN
	ADCON0 = 0b00000101; // Active le CAN et sélectionne AN0 comme entrée
	ADCON1 = 0b00000000; // Tension de référence VDD et VSS
	ADCON2 = 0b00010010; // Fréquence d'horloge ADC configurée
}

//-----------------------------------------------------------------------------------------------------------------

void init_interup(void)
{
    INTCONbits.GIE = 1;  // Activation globale des interruptions
    INTCONbits.PEIE = 1; // Activation des interruptions périphériques
    PIE1bits.ADIE = 1; // Activation de l'interruption du CAN
    ADCON0bits.GO = 0; // Démarre la conversion

}

//-----------------------------------------------------------------------------------------------------------------

void lecturAn(char port)
{
    ADCON0bits.CHS = port;//config le port choisi a convertir
    ADCON0bits.GO = 1;//lance la conversion
}