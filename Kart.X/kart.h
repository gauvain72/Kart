 //init du timer2 pour la pwm
void initTimerPWM(void);

//init de la PWM
void initPWM(void);

//init timer0 pour avoir IT pour T=10ms
void initTimer0_IT(float MSperiod);

//config entre Analogique et du CAN
void initADC(bool IT);

//autorise l'interruption sur le CAN
void initInterup(void);

//lis le Port analogique du CAN(avoir config la broche ds initADC(void))
void readAn(char port);

//Retourne la valeur de la conversion sur 10 bits
unsigned short ADCResult(void);

//Lis le port du capteur de courant (Amps)
float readCurrentSensor(void);

//Lis le port du potentiometre commande et retourne entre 0 et Max
float readCommand(float commandMax);


//input in % (Ex 68 or 79.435)
void applyPWM(short newValue);
void applyPWM(float newValue);