// Communication UART

//Initialisation du module UART n°1
void InitUart9600(unsigned char Fclk);

//Initialisation du module UART n°1
void InitUart38400(unsigned char Fclk);

//Emission d'un caractère sur le module UART n°1
void UartWriteChar(unsigned char c);

//Emission d'un messqge sur le module UART n°1
void UartWriteStr(unsigned char* c, short unsigned int lenght);


//Attente bloquante d'un caractère sur le module UART n°1
unsigned char UartReadChar(void);

