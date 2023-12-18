//Fichiers d'include génériques
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xc.h>

#include "Uart.h"
#include "kart.h"
#include "config.h"

//Interrupt CAN
void __interrupt() ISR(void)
{
    if (PIR1bits.ADIF) // Vérifiez si l'interruption du CAN a été déclenchée
    {
        unsigned short res = ADCResult();
        applyPWM(res);
        
        char str[20];
        sprintf(str, "mes:%d\n", res);  
        UartWriteStr(str,20);
        
        PIR1bits.ADIF = 0;  //Réinitialisez le drapeau d'interruption du CAN
    }
}

void main (void)
{
    //appel des differentes configurations 
    InitPic();
    initTimerPWM();
    initPWM();
    initADC(1);
    initInterup();
    
    InitUart38400(FCLK);
    
    while(1)
    {
        __delay_ms(100);
        readAn(ADC_Command);
        ////lecture sur le port Analogique3 = RA3;2=RA2;1=RA1;4=RA5,Le port 5,6et 7 n'existe pas, le rest sur PORTB et PORTC;19=RC7
    }
}
