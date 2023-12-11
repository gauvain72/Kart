// Communication UART
// BibliothÃ¨que de base


//-----------------------------------------------------------------------------------------------------------------
//Initialisation du module UART n°1
void InitUart9600(unsigned char Fclk);

//Initialisation du module UART n°1
void InitUart38400(unsigned char Fclk);

//-----------------------------------------------------------------------------------------------------------------
//Emission d'un caractère sur le module UART n°1
void UartWriteChar(unsigned char c);

//-----------------------------------------------------------------------------------------------------------------
//Attente bloquante d'un caractère sur le module UART n°1
unsigned char UartReadChar(void);

