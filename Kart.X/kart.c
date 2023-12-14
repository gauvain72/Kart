//64MHz freq 
//PWM 62500Hz
//resolution 10bit :CCPR1L = 40; +CCP1CONbits.DC1B=0b00;
     

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xc.h>

#define CURRENT_sensor 2
#define COMMAND  3
void initTimerPWM(void)
{
    OSCCON = 0x72;//freq OSC a 16MHz pour 8MHz OSCCON=0b01100000;
    OSCTUNEbits.PLLEN=1; //multiple par 4 la freq on passe a 64MHz
    PR2 = 0xFF;// FREQ timer2 a 40KHz
    T2CON = 0b00000100;//Prediviseur � 1
    TMR2 = 0;
}

void initPWM(void)
{
    CCP1CON = 0x0C; //configuration ccp1 en PWM sortie sur RC2  
    CCPR1L = 40;// configuration du rapport cyclique
    CCP1CONbits.DC1B=0b00; //resolution 10bit 2bits LSB
    CCPTMRS0 = 0x00;//met le timer 2 sur la PWM
    TRISCbits.RC2 = 0; //config en sortie pour la PWM
}

void initTimer0_IT(float MSperiod) {
    // Configure Timer0 for 16-bit mode
    T0CON = 0b10001000;  // Prescaler 1:2, 16-bit mode 
    //T0CON = 0b10001010;// Prescaler 1:8 //PSA=1? 64MHz
    // Calculate the timer value for the specified period
    // Assuming F_osc = 16 MHz, prescaler = 2
    unsigned int timerValue = 65536 - (int)((MSperiod * 1000.0) / (4.0 * (1.0 / 16.0e6)));

    // Set the calculated timer value
    TMR0 = timerValue;

    // Enable Timer0 interrupt
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
}

void initADC(char IT)
{
	// Configurer PORT A comme entr�e analogique
	ANSELA = 0xFF;
    TRISA = 0xFF;

	// Configuration du module CAN
	ADCON0 = 0b00000101; // Active le CAN et s�lectionne AN0 comme entr�e
	ADCON1 = 0b00000000; // Tension de r�f�rence VDD et VSS
	ADCON2 = 0b00010010; // Fr�quence d'horloge ADC configur�e

    ADCON1 |= 1 << 7; //Justification a droite
    
    ADCON0bits.GO = 0; //S'assure qu'aucune conversion se lance

    if (IT==1)
        PIE1bits.ADIE = 1; // Activation de l'interruption du CAN
    else
        PIE1bits.ADIE = 0;
}

void initInterup(void)
{
    INTCONbits.GIE = 1;  // Activation globale des interruptions
    INTCONbits.PEIE = 1; // Activation des interruptions p�riph�riques
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
    
    readAn(CURRENT_sensor); //VALEUR A VERIFIER ----------------------------------------
    
    while (PIR1bits.ADIF == 0);
    
    float tensionCapteur = 5.0 * (float)(ADCResult()) / 1023.0;

    float mesureCapteur = tensionCapteur * 1.0; // Multiply by sensitivity (A/V)

    return mesureCapteur;
}

float readCommand(float commandMax){
    //1-changer ADC pour entree commande
    //2-faire la mesure
    //3-attendre que la mesure soit finie
    //4-transformer la mesure entre 0 et max

    readAn(COMMAND); //VALEUR A VERIFIER ----------------------------------------

    while (PIR1bits.ADIF == 0);
    //PIR1bits.ADIF = 0; inutil remis a 0 automatique

    float command = commandMax * (float)(ADCResult()) / 1023.0;

    return command;
}

void applyPWM(short newValue){
    if (newValue < 5)
        newValue = 0;
        
    if (newValue > 95)
        newValue = 100;
//neuValue valeur entre 0 et 1023,
    CCPR1L = newValue;
    //CCP1CONbits.DC1B
}

/*void applyPWM(float newValue){
    if (newValue < 5)
        newValue = 0;
        
    if (newValue > 95)
        newValue = 100;

    CCPR1L = (unsigned char)newValue;
}*/