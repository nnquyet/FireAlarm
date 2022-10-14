#define _XTAL_FREQ 4000000 
char UART_Init(const long int baudrate)
{
	 // Baud rate configuration
    BRGH = 1;
    SPBRG = 25;
    // Enable Asynchronous Serial Port
    SYNC = 0;
    SPEN = 1;
    // Configure Rx-Tx pin for UART 
    TRISC6 = 1;
    TRISC7 = 1;
    // Enable UART Transmission
    TXEN = 1;
    // Enable UART Receiver
    CREN = 1;//  cho phep nhan klien tuc
    //
    RCIE = 1;
    PEIE = 1;
    GIE = 1;
}
char UART_TX_Empty()
{
  return TRMT;
}

char UART_Data_Ready()
{  
   return RCIF;
}
char UART_Read()
{
  while(!RCIF);
  return RCREG;
}

void UART_Read_Text(char *Output, unsigned int length)
{
	unsigned int i;
	for(int i=0;i<length;i++)
		Output[i] = UART_Read();
}

void UART_Write(char data)
{
  while(!TRMT);
  TXREG = data;
}

void UART_Write_Text(char *text)
{
  int i;
  for(i=0;text[i]!='\0';i++)
	  UART_Write(text[i]);
}
