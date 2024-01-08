//64MHz freq 
//PWM 62500Hz
//resolution 10bit :CCPR1L = 40; +CCP1CONbits.DC1B=0b00;
     

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xc.h>
#include "kart.h"

void initTimerPWM(void)
{
    // Initialise le timer PWM en configurant le registre PR2 pour obtenir une fréquence de timer2 de 40 KHz
    //OSCCON = 0x72;//freq OSC a 16MHz pour 8MHz OSCCON=0b01100000;
    //OSCTUNEbits.PLLEN=1; //multiple par 4 la freq on passe a 64MHz
    PR2 = 0xFF;// FREQ timer2 a 40KHz
    T2CON = 0b00000100;//Prediviseur à 1
    TMR2 = 0;
}

void initPWM(void)
{
    // Initialise le module PWM en configurant CCP1 pour générer un signal PWM sur la sortie RC2 avec un rapport cyclique de 10 bits
    CCP1CON = 0x0C; //configuration ccp1 en PWM sortie sur RC2  
    CCPR1L = 40;// configuration du rapport cyclique
    CCP1CONbits.DC1B=0b00; //resolution 10bit 2bits LSB
    CCPTMRS0 = 0x00;//met le timer 2 sur la PWM
    TRISCbits.RC2 = 0; //config en sortie pour la PWM
}

void initTimer0_IT() {
    // Configure le Timer0 en mode 16 bits avec un prédiviseur de 1:8 et active l'interruption Timer0
    //T0CON = 0b10001000;  // Prescaler 1:2, 16-bit mode 
    T0CON = 0b10001101;// Prescaler 1:64 //PSA=1? 64MHz

    // Enable Timer0 interrupt
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
}
unsigned int calculateTimer0Value(float MSperiod){
    // Calcule la valeur du timer pour le Timer0 en fonction de la période spécifiée en millisecondes
    //PSC = 8 : Max = 30ms period
	
    // Calculate the timer value for the specified period
    const float FREQ_IN_HZ = 64000000.0;
    const float PRESCALER = 64.0;
    unsigned int timerValue = 65536 - (unsigned int)((MSperiod * FREQ_IN_HZ) / (1000.0 * PRESCALER * 4.0));
    
    return timerValue;
}

void initADC(char IT)
{
    // Initialise le module ADC en configurant PORT A comme entrée analogique, active le CAN et sélectionne AN0 comme entrée
	// Configurer PORT A comme entrée analogique
	ANSELA = 0xFF;
    TRISA = 0xFF;

	// Configuration du module CAN
	ADCON0 = 0b00000101; // Active le CAN et sélectionne AN0 comme entrée
	ADCON1 = 0b00000000; // Tension de référence VDD et VSS
	ADCON2 = 0b00010010; // Fréquence d'horloge ADC configurée

    ADCON2 |= 1 << 7; // Justification a droite
    
    ADCON0bits.GO = 0; // S'assure qu'aucune conversion se lance

    if (IT==1)
        PIE1bits.ADIE = 1; // Activation de l'interruption du CAN
    else
        PIE1bits.ADIE = 0;
}

void initInterup(void)
{
    // Active globalement les interruptions et les interruptions périphériques
    INTCONbits.GIE = 1;  // Activation globale des interruptions
    INTCONbits.PEIE = 1; // Activation des interruptions périphériques
}

void readAn(char port)
{
    // Selectionne le port choisi à convertir et lance la conversion
    ADCON0bits.CHS = port; // Config le port choisi a convertir
    ADCON0bits.GO = 1; // Lance la conversion
}

unsigned short ADCResult(void){
    // Lit le résultat de la conversion ADC avec justification à droite
	
    //unsigned short res = ADRESH << 2; Justification a gauche
    //res += ADRESL >> 6;	
    unsigned short res = ADRESH << 8;  // Justification a droite
    res += ADRESL;
	
    return res;
}

float readCurrentSensor(void){
    // Lit la tension du capteur de courant, convertit en Ampères et renvoie la valeur

    readAn(CURRENT_sensor);
    
    while (PIR1bits.ADIF == 0);
    
    float tensionCapteur = 5.0 * (float)(ADCResult()) / 1023.0;

    float mesureCapteur = tensionCapteur * 1.0; // Multiply by sensitivity (A/V)

    return mesureCapteur;
}

float readCommand(float commandMax){
    // Lit la commande analogique, la convertit entre 0 et max, et renvoie la valeur
	
    readAn(ADC_Command);

    while (PIR1bits.ADIF == 0);
    //PIR1bits.ADIF = 0; Inutile: remise a 0 automatique

    float command = commandMax * (float)(ADCResult()) / 1023.0;

    return command;
}

void applyPWM(unsigned short newValue){
    // Applique la nouvelle valeur PWM en ajustant les bits CCP1L et DC1B
	
    if (newValue > 1023)
        newValue = 1023;
    
    CCPR1L = (unsigned char)(newValue >> 2);
    CCP1CONbits.DC1B = newValue & 3;
}

void applyPWM_f(float newValue){
    // Applique la nouvelle valeur PWM (en pourcentage) en la convertissant en valeur numérique et appelle applyPWM()
    if (newValue < 5)
        newValue = 0;
        
    if (newValue > 95)
        newValue = 100;

    unsigned short newShortValue = (unsigned short)( newValue*1024.0/100.0 );
    applyPWM(newShortValue);
}

void applyTension(float newValue){
    // Applique la nouvelle valeur de tension en la convertissant en valeur PWM et appelle applyPWM()
    if (newValue < 3)
        newValue = 0;
        
    if (newValue > 45)
        newValue = 48;

    unsigned short newShortValue = (unsigned short)( newValue*1024.0/48.0 );
    applyPWM(newShortValue);
}
