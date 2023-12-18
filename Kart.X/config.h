//Fichiers d'include g�n�riques
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xc.h>                 //Fichier include g�n�rique pour tous les Pics

#include "Uart.h"
void InitPic(void);

// Remarque, liste des pragma config dans C:\Program Files\Microchip\xc8\v1.35\docs\chips
#pragma config WDTEN=OFF        //Watchdog contr�l� par SWDTEN
#pragma config WDTPS=512        //Postdiviseur Watchdog -> p�riode 2s environ
#pragma config PWRTEN=OFF       //Power up timer d�sactiv�
#pragma config BOREN=OFF        //
#pragma config BORV=250         //Seuil BOR 2.5V
#pragma config MCLRE=EXTMCLR    //Master reset externe
#pragma config DEBUG=OFF        //Mode debug d�sactiv�
#pragma config LVP=OFF          //Mode programmation Low Voltage d�sactiv�

#pragma config IESO=OFF
#pragma config PRICLKEN=OFF     //primary clock
#pragma config FCMEN=OFF        //fail-safe clock monitor
#pragma config FOSC=INTIO67     //Oscillateur interne


//Fr�quence d'horloge choisie: (en MHz)
#define FCLK 32    
#define _XTAL_FREQ 32000000      //define n�cessaire pour l'utilisation des fonctions __delay

#if (FCLK==1)||(FCLK==2)||(FCLK==4)||(FCLK==8)||(FCLK==16)
#pragma config PLLCFG=OFF       //PLL d�sactiv�e
#endif

#if (FCLK==32)||(FCLK==64)
#pragma config PLLCFG=ON        //PLL d�sactiv�e
#endif


//D�finition des entr�es/sorties du syst�me
#define Led1      LATCbits.LC0   //sortie de visualisation
#define Led2      LATCbits.LC1

#define RelaiOn   LATCbits.LC2   //commande relais ON
#define RelaiOff  LATCbits.LC3   //commande relais OFF

//Double d�finition pour la fonction chenillard
#define Led3      LATCbits.LC2   
#define Led4      LATCbits.LC3   


const char version[] @ 0xF00000 = "V1.00 - LedK22 - 01/10/2023";


//Registre d'�tat global
volatile unsigned char STATM @ 0x060;   
typedef union {
  struct {
    unsigned TEF    : 1;           //B0: � 1, indique un top d'�chabtillonnage
    unsigned VaON   : 1;           //B1: � 1, relai alim activ�
    unsigned        : 6;           //
  };
} STATMbits_t;  
STATMbits_t STATMbits @ 0x060;     //D�claration de STATM bit � bit

#define  TEF_bit     STATMbits.TEF
#define  VaON_bit    STATMbits.VaON


//Variables de fonctionnement du module en Banque 0:
unsigned short cpt        @0x061;



//-----------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------
//Initialisation g�n�rale du Pic
