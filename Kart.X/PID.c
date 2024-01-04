#include "Uart.h"
#include "kart.h"
#include "config.h"


// -- PID --
//Constantes PID (Ae : Amp erreur)
static const float PID_Kp = 10.0;      //Constante P (V/Ae)
static const float PID_Ki = 0.0;      //Constante I (V/(Ae/ms))
static const float PID_Kd = 0.0;      //Constante D (V/(Ae*ms))
static const float PID_deltaT = 10.0; //Période T (ms)
//Variables PID
static float PID_PrevError = 0; //Erreur précédente (Amps)
static float PID_SumError = 0;  //Somme des erreurs (Amps*ms)
static float PID_CurrentError;  //Erreur (Amps)
static float PID_Derivative;    //Derivee (Amps/ms)
static float PID_Out;           //Sortie du PID (% de tension max)

//Variables statiques
static const float CommandeMax = 8.0; //Le maximum de courant du moteur (Amps)
static float Commande;                //Pour la lecture de la commande (Amps)
static float Mesure;                  //Pour la lecture de la mesure (Amps)


void PID(void);

//Interrupt Timer pour T = PID_deltaT
void __interrupt() ISR(void)
{
    // Check if Timer0 overflow caused the interrupt
    if (INTCONbits.TMR0IF) {
        PID();

        // Clear the Timer0 interrupt flag
        INTCONbits.TMR0IF = 0;
    }
}
void PID(void){
    // 1-Lecture des entrees
    // 2-Calcul erreur
    // 3-Traitement PID
    // 4-Ecriture PWM

    //Lecture des entrees
    Mesure = readCurrentSensor();
    Commande = readCommand(CommandeMax);

    //Calcul erreur (Amps)
    PID_CurrentError = Commande - Mesure;

    //Calcul integrale (Amps * ms)
    PID_SumError += PID_CurrentError * PID_deltaT;

    //Calcul derivee (Amps/ms)
    PID_Derivative = (PID_CurrentError - PID_PrevError) / PID_deltaT;

    //Calcul PID
    PID_Out = (PID_Kp * PID_CurrentError) + (PID_Ki * PID_SumError) + (PID_Kd * PID_Derivative);

    applyTension(PID_Out);

    PID_PrevError = PID_CurrentError;
}

void main (void)
{
    //appel des differentes configurations 
   // InitPic();
    initTimerPWM();
    initPWM();
    initADC(0);
    initTimer0_IT(PID_deltaT);
    initInterup();
    
    while(1)
    {

    }
}
