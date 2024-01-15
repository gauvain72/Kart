#pragma once

#define CURRENT_sensor  2
#define ADC_Command 3


// Initialise le timer2 pour la PWM
void initTimerPWM(void);

// Initialise la PWM
void initPWM(void);

// Initialise le timer0 pour les interruptions
void initTimer0_IT();

// Calcule la valeur de réinitialisation du timer0
unsigned int calculateTimer0Value(float MSperiod);

// Configure entre le mode analogique et le CAN
void initADC(char IT);

// Autorise l'interruption sur le CAN
void initInterup(void);

// Lit le port analogique du CAN (après avoir configuré la broche dans initADC(void))
void readAn(char port);

// Retourne la valeur de la conversion sur 10 bits
unsigned short ADCResult(void);

// Lit le port du capteur de courant (Ampères)
float readCurrentSensor(void);

// Lit le port du potentiomètre de commande et retourne une valeur entre 0 et Max
float readCommand(float commandMax);

// Nouvelle valeur entre 0 et 1023
void applyPWM(unsigned short newValue);

// Entrée en % (par exemple, 68 ou 79.435)
void applyPWM_f(float newValue);

// Entrée en volts
void applyTension(float newValue);
