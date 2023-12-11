 //init du timer2 pour la pwm
void initTimer(void);

//init de la PWM
void initPWM(void);

//config entre Analogique et du CAN
void initADC(void);

//autorise l'interruption sur le CAN
void initInterup(void);

//lis le Port analogique du CAN(avoir config la broche ds initADC(void))
void readAn(char port);