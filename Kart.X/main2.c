//Fichiers d'include génériques
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xc.h>

#include "Uart.h"
#include "kart.h"
#include "config.h"

void __interrupt() ISR(void)
{
    if (PIR1bits.ADIF) // Vérifiez si l'interruption du CAN a été déclenchée
    {
        unsigned short valccp = ADCResult();
        if(ADCON0bits.CHS==3)
        {
            //Do something
        }
        if(ADCON0bits.CHS==4)
        {
            //Do something else
        }
        
        PIR1bits.ADIF = 0;                
    }
}

void main (void)
{
    //appel des differentes configurations 
    InitPic();
    initTimerPWM();
    initPWM();
    initADC(true);
    initInterup();
    
    while(1)
    {
       //readAn(4);
       //readAn(3);
    }
}