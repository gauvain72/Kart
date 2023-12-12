#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xc.h>

void initTimerPWM(void)
{
    OSCCON = 0x72;//freq OSC a 16MHz
    PR2 = 100;// FREQ timer2 a 40KHz
    T2CON = 0b00000100;//Prediviseur à 1
    TMR2 = 0;
}

void initPWM(void)
{
    CCP1CON = 0x0C; //configuration ccp1 en PWM sortie sur RC2  
    CCPR1L = 40;// configuration du rapport cyclique
    CCPTMRS0 = 0x00;//met le timer 2 sur la PWM
    TRISCbits.RC2 = 0; //config en sortie pour la PWM
}

void initTimer0_IT(float MSperiod) {
    // Configure Timer0 for 16-bit mode
    T0CON = 0b10001000;  // Prescaler 1:2, 16-bit mode

    // Calculate the timer value for the specified period
    // Assuming F_osc = 16 MHz, prescaler = 2
    unsigned int timerValue = 65536 - (int)((MSperiod * 1000.0) / (4.0 * (1.0 / 16.0e6)));

    // Set the calculated timer value
    TMR0 = timerValue;

    // Enable Timer0 interrupt
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
}

void initADC(bool IT)
{
	// Configurer PORT A comme entrée analogique
	ANSELA = 0xFF;
    TRISA = 0xFF;

	// Configuration du module CAN
	ADCON0 = 0b00000101; // Active le CAN et sélectionne AN0 comme entrée
	ADCON1 = 0b00000000; // Tension de référence VDD et VSS
	ADCON2 = 0b00010010; // Fréquence d'horloge ADC configurée

    ADCON1 |= 1 << 7; //Justification a droite
    
    ADCON0bits.GO = 0; //S'assure qu'aucune conversion se lance

    if (IT)
        PIE1bits.ADIE = 1; // Activation de l'interruption du CAN
    else
        PIE1bits.ADIE = 0;
}

void initInterup(void)
{
    INTCONbits.GIE = 1;  // Activation globale des interruptions
    INTCONbits.PEIE = 1; // Activation des interruptions périphériques
}

void readAn(char port)
{
    ADCON0bits.CHS = port;//config le port choisi a convertir
    ADCON0bits.GO = 1;//lance la conversion
}

unsigned short ADCResult(void){
    return ((unsigned short)ADRESH << 8) | ADRESL;
}

float readCurrentSensor(void){
    //1-changer ADC pour entree capteur
    //2-faire la mesure
    //3-attendre que la mesure soit finie
    //4-transformer en valeur Amps
    
    readAn(4); //VALEUR A VERIFIER ----------------------------------------

    float tensionCapteur = 5.0 * (float)(ADCResult()) / 1023.0;

    float mesureCapteur = tensionCapteur * 1.0; // Multiply by sensitivity (A/V)

    return mesureCapteur;
}

float readCommand(float commandMax){
    //1-changer ADC pour entree commande
    //2-faire la mesure
    //3-attendre que la mesure soit finie
    //4-transformer la mesure entre 0 et max

    readAn(3); //VALEUR A VERIFIER ----------------------------------------

    while (PIR1bits.ADIF == 0);
    PIR1bits.ADIF = 0;

    float command = commandMax * (float)(ADCResult()) / 1023.0;

    return command;
}

void applyPWM(short newValue){
    if (newValue < 5)
        newValue = 0;
        
    if (newValue > 95)
        newValue = 100;

    CCPR1L = newValue;
}

void applyPWM(float newValue){
    if (newValue < 5)
        newValue = 0;
        
    if (newValue > 95)
        newValue = 100;

    CCPR1L = (unsigned char)newValue;
}