
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "uart.h"
#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7
#include "lcd.h"
#pragma config FOSC = HS        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program M

#define _XTAL_FREQ 4000000
void ADCinit (void);
unsigned int ReadADC (void);

unsigned int ADCvalue = 0;
float f;
char s[20];
unsigned int voltage;
int q = 0;

void main(void) {
    ADCinit();
    UART_Init(9600);

    RA0 = 1;
    TRISA0 = 1;
    TRISB0 = 1;// dat B0 la input
    TRISB1 = 0;
    TRISB2 = 0;
    TRISB3 = 0;
    TRISD = 0x00; 
    Lcd_Init();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("Mach Bao Chay");
    __delay_ms(1000);
    Lcd_Clear();
    
    while(1){
        
        RB1 = 0;
        RB2 = 0;
        RB3 = 0;
        ADCvalue = ReadADC();
        voltage = 5000.0f / 1023 * ADCvalue;
        f = voltage / 10;
        sprintf(s, "NHIET DO = %f\n\r", f); 
        if(f!=0){
           UART_Write_Text(s);  
        }
           
         Lcd_Set_Cursor(2,1);
         Lcd_Write_String(s);
         __delay_ms(1000); 
         if(q == 0) {
                __delay_ms(5000); 
                q++;
            }
         
        if(f>59){
            
            RB1 = 1;
            RB2 = 1;
            UART_Write_Text("NHIET DO CAO!\n\r"); 
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("NHIET DO CAO!");
            __delay_ms(1000);
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("                ");
        }
    }
}
void ADCinit (void)// adc
{
    // chon tan so clock cho bo adc
    ADCON1bits.ADCS2 = 0, ADCON0bits.ADCS1 = 0,ADCON0bits.ADCS0 = 1;
    // chon kenh adc la kenh an0
    ADCON0bits.CHS2 = 0, ADCON0bits.CHS1 = 0, ADCON0bits.CHS0 = 0;
    // chon cach luu data
    ADCON1bits.ADFM = 1;
    // cau hinh cong vao
    ADCON1bits.PCFG3 = 1,  ADCON1bits.PCFG2 = 1,  ADCON1bits.PCFG1 = 1,  ADCON1bits.PCFG0 = 0;
    // cap nguon cho khoi adc
    ADCON0bits.ADON = 1;
}
unsigned int ReadADC (void)// doc len 7 doan
{
    unsigned int TempValue = 0;
    ADCON0bits.GO_nDONE = 1;
    while(ADCON0bits.GO_nDONE);
    TempValue = ADRESH*256 + ADRESL;
    return (TempValue);
}
