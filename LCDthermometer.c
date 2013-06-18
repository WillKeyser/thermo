/* AVR Studio:
 * LCDthermometer.c
 *
 * Author:  Will Keyser
 * Compiler: AVR GNU C Compiler (GCC)
 */ 
 
#define F_CPU  8000000UL
#include <avr/delay.h>
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
 
/*Global Variables Declarations*/
 
/*LCD function declarations */
void LCD_send_command(unsigned char cmnd);
void LCD_send_data(unsigned char data);
void LCD_init();
void LCD_goto(unsigned char y, unsigned char x);
void LCD_print(char *string);
 
/*ADC Conversion Complete Interrupt Service Routine (ISR)*/
ISR(ADC_vect);
 
#define LCD_DATA_PORT	PORTB
#define LCD_DATA_DDR	DDRB
#define LCD_DATA_PIN	PINB
 
#define LCD_CNTRL_PORT	PORTC
#define LCD_CNTRL_DDR	DDRC
#define LCD_CNTRL_PIN	PINC
 
#define LCD_RS_PIN		5
#define LCD_RW_PIN		6
#define LCD_ENABLE_PIN	7
#define SET_HOUR		3
#define SET_MINUTE		4
 
int main(void)
{
	unsigned char i; 
 
    LCD_init();
	LCD_goto(1,2);
	LCD_print("Temperature is");
 
 
	LCD_CNTRL_PORT = (1<<SET_HOUR | 1<<SET_MINUTE);
 
	DDRA = 0x00;			// Configure PortA as input
 
	ADCSRA = 0x8F;			// Enable the ADC and its interrupt feature
					// and set the ACD clock pre-scalar to clk/128
	ADMUX = 0xE0;			// Select internal 2.56V as Vref, left justify 
					// data registers and select ADC0 as input channel 
 
	sei();				// Enable Global Interrupts
	ADCSRA |= 1<<ADSC;		// Start Conversion
 
	while(1);
}
 
/* This function sends a command 'cmnd' to the LCD module*/
void LCD_send_command(unsigned char cmnd)
{
	LCD_DATA_PORT = cmnd;
	LCD_CNTRL_PORT &= ~(1<<LCD_RW_PIN);
	LCD_CNTRL_PORT &= ~(1<<LCD_RS_PIN);
 
	LCD_CNTRL_PORT |= (1<<LCD_ENABLE_PIN);
	_delay_us(2);
	LCD_CNTRL_PORT &= ~(1<<LCD_ENABLE_PIN);
	_delay_us(100);
}
 
/* This function sends the data 'data' to the LCD module*/
void LCD_send_data(unsigned char data)
{
	LCD_DATA_PORT = data;
	LCD_CNTRL_PORT &= ~(1<<LCD_RW_PIN);
	LCD_CNTRL_PORT |= (1<<LCD_RS_PIN);
 
	LCD_CNTRL_PORT |= (1<<LCD_ENABLE_PIN);
	_delay_us(2);
	LCD_CNTRL_PORT &= ~(1<<LCD_ENABLE_PIN);
	_delay_us(100);
}
 
void LCD_init()
{
	LCD_CNTRL_DDR = 0xFF;
	LCD_CNTRL_PORT = 0x00;
	LCD_DATA_DDR = 0xFF;
	LCD_DATA_PORT = 0x00;
 
	_delay_ms(10);
	LCD_send_command(0x38);
	LCD_send_command(0x0C);
	LCD_send_command(0x01);
	_delay_ms(10);
	LCD_send_command(0x06);
}
 
/* This function moves the cursor the line y column x on the LCD module*/
void LCD_goto(unsigned char y, unsigned char x)
{
	unsigned char firstAddress[] = {0x80,0xC0,0x94,0xD4};
 
	LCD_send_command(firstAddress[y-1] + x-1);
	_delay_ms(10);	
}
 
void LCD_print(char *string)
{
	unsigned char i=0;
 
	while(string[i]!=0)
	{
		LCD_send_data(string[i]);
		i++;
	}
}
 
/*ADC Conversion Complete Interrupt Service Routine (ISR)*/
ISR(ADC_vect)
{	
	char tempC, tempF, display;
	float tempff;
 
	tempC = ADCH;
	tempff = (float)tempC;
	tempff = (tempff*9)/5 + 32;	
	tempF = tempff;
 
	LCD_goto(2,4);	
	itoa(tempC/10,display,10);
	LCD_print(display);
	itoa(tempC%10,display,10);
	LCD_print(display);
	LCD_send_data(0xDF);
	LCD_print("C   ");
	itoa(tempF/10,display,10);
	LCD_print(display);
	itoa(tempF%10,display,10);
	LCD_print(display);
	LCD_send_data(0xDF);
	LCD_print("F");
	_delay_ms(500);
	ADCSRA |= 1<<ADSC;		// Start Conversion
}
