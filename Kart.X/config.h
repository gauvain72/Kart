
// Remarque, liste des pragma config dans C:\Program Files\Microchip\xc8\v1.35\docs\chips
#pragma config WDTEN=OFF        //Watchdog controle par SWDTEN
#pragma config WDTPS=512        //Postdiviseur Watchdog -> periode 2s environ
#pragma config PWRTEN=OFF       //Power up timer desactive
#pragma config BOREN=OFF        //
#pragma config BORV=250         //Seuil BOR 2.5V
#pragma config MCLRE=EXTMCLR    //Master reset externe
#pragma config DEBUG=OFF        //Mode debug desactive
#pragma config LVP=OFF          //Mode programmation Low Voltage desactive

#pragma config IESO=OFF
#pragma config PRICLKEN=OFF     //primary clock
#pragma config FCMEN=OFF        //fail-safe clock monitor
#pragma config FOSC=INTIO67     //Oscillateur interne


//Frequence d'horloge choisie: (en MHz)
#define FCLK 16    
#define _XTAL_FREQ 1000000      //define necessaire pour l'utilisation des fonctions __delay

#if (FCLK==1)||(FCLK==2)||(FCLK==4)||(FCLK==8)||(FCLK==16)
#pragma config PLLCFG=OFF       //PLL desactivee
#endif

#if (FCLK==32)||(FCLK==64)
#pragma config PLLCFG=ON        //PLL activee
#endif

void InitPic(void);
