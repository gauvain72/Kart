#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xc.h>

void initTimer(void)
{
    OSCCON=0x72;//freq OSC a 16MHz
    PR2=100;// FREQ timer2 a 40KHz
    T2CON=0b00000100;//Prediviseur à 1
    TMR2=0;
}

void initPWM(void)
{
   
    CCP1CON=0x0C; //configuration ccp1 en PWM sortie sur RC2  
    CCPR1L = 40;// configuration du rapport cyclique
    CCPTMRS0=0x00;//met le timer 2 sur la PWM
    TRISCbits.RC2=0; //config en sortie pour la PWM
}

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

void initInterup(void)
{
    INTCONbits.GIE = 1;  // Activation globale des interruptions
    INTCONbits.PEIE = 1; // Activation des interruptions périphériques
    PIE1bits.ADIE = 1; // Activation de l'interruption du CAN
    ADCON0bits.GO = 0; // Démarre la conversion

}

void readAn(char port)
{
    ADCON0bits.CHS = port;//config le port choisi a convertir
    ADCON0bits.GO = 1;//lance la conversion
}