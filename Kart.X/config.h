//Fichiers d'include génériques
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xc.h>                 //Fichier include générique pour tous les Pics

#include "Uart.h"
void InitPic(void);

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
#define FCLK 32    
#define _XTAL_FREQ 32000000      //define nécessaire pour l'utilisation des fonctions __delay

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


//-----------------------------------------------------------------------------------------------------------------
//Initialisation générale du Pic
