/*
 *
 * Created: 2019-03-22 09:32:30
 * Author : SHENYING
 */ 
//****main.c****//
#define F_CPU 16000000UL
#include "lcd.h"
#include "i2c.h"
#include "font.h"

//int y,x;

int x_pos=10;
int y_pos=4;
char doconversion (int ch){
	//Configure the ADC module
	ADMUX = 0b01100000;// ADC Multiplexer Selection Register
	//REFS1:0 = 01 -> AVCC as reference,
	//ADLAR = 1 -> Left adjust
	//MUX4:0 = 00100 -> ADC4 as input (Analog pin4 as input)
	ADCSRA = 0b10000110;//ADC CTRL and Status Register (ADCSRA)ADCSRA – ADC Control and Status Register A
	//ADEN = 1: enable ADC,
	//ADSC = 0: don't start conversion yet
	//ASPS2:0 = 001: prescaler = 2
	//ADATE = 0: disable auto trigger,
	//ADIE = 0: disable ADC interrupt
	ch = ch&0b00000111;    //help to change the pin of analog in
	ADMUX |= ch;
	//Start conversion by setting flag ADSC
	ADCSRA |= (1 << ADSC);/*Using the ADC interrupt flag as a trigger source makes the ADC start a new conversion as soon as the ongoing conversion
	has finished. The ADC then operates in free running mode, constantly sampling and updating the ADC data register. The
	first conversion must be started by writing a logical one to the ADSC bit in ADCSRA. In this mode the ADC will perform
	successive conversions independently of whether the ADC interrupt flag, ADIF is cleared or not.
	//Wait until conversion is completed*/
	while (ADCSRA & (1 << ADSC)){;}
	//Read the top 8 bits, output to
	unsigned char result = ADCH;//ADCH 8-BIT
	return result;
}

/************************************************************************
*Namn: Print_pos                                                        *
*                                                                       *
*Syfte: Prints to the oled at pos x,y						            *
*                                                                       *
*Indata: The row and the column it shall be whiten on	                *
*                                                                       *
*Utdata: void								                            *
*                                                                       *
*Anropar:lcd_clrscr(),lcd_gotoxy(),i2c_start(), i2c_byte(),i2c_stop()   *
************************************************************************/
void Print_pos(int x,int y){

		lcd_clrscr();
			
	lcd_gotoxy(x,y);  //line x column y
	i2c_start(LCD_I2C_ADR << 1);
	i2c_byte(0x40);
	for (uint8_t i = 0; i < 7; i++)
	{
		i2c_byte(pgm_read_byte(&(FONT[33][i])));
	}	
	i2c_stop();
}
void Print_pos1(int x,int y){

	lcd_clrscr();
	
	lcd_gotoxy(x,y);  //line x column y
	i2c_start(LCD_I2C_ADR << 1);
	i2c_byte(0x40);
	for (uint8_t i = 0; i < 7; i++)
	{
		i2c_byte(pgm_read_byte(&(FONT[10][i])));
	}
	i2c_stop();
}


/************************************************************************
*Namn: main                                                             *
*                                                                       *
*Syfte: needs to be here, init the oled and print a "A" on it			*
*	    changed on the joystick		     	                            *
*Indata:void											                *
*								                                        *
*Utdata:a int								                            *
*                                                                       *
*Anropar:lcd_init(),Print_pos()								            *
************************************************************************/
int main(void){
	int a=0;//YCHANG20 KUAN8
	int b=0;//X
	int c=0;
	//lcd_init(0xAF); 
		for(;;){//eternal loop
			a=doconversion(0);
			b=doconversion(1);
		    c=doconversion(3);
			x_pos=10;
			y_pos=4;
			if(c>200){
				Print_pos1(x_pos,y_pos);
			}
			
			else if(a>125 && a<130 && b>125 && b<130){
				//lcd_init(0xAF);    // init lcd and turn on
				Print_pos(10,4);
				//_delay_ms(100);//wait for 5 second
				}
			
			if(a>135){
				//x_pos=x_pos+9;
				x_pos=x_pos+(a-125)/28;
				if(x_pos>20 || x_pos==20){
					x_pos=20;
				}
				//lcd_init(0xAF);    // init lcd and turn on
				Print_pos(x_pos,y_pos);
				
				//_delay_ms(100);//wait for 5 second
			}
			else if (a<120)
			{
				//x_pos=x_pos-9;
				x_pos=x_pos-(125-a)/28;
				if(x_pos<0 || x_pos==0){
					x_pos=0;
				}
				//lcd_init(0xAF);    // init lcd and turn on
				Print_pos(x_pos,y_pos);
			}
			else if (b<120)
			{
				y_pos=y_pos+(125-b)/28;
				//y_pos=y_pos+3;
				if(y_pos>7 || y_pos==7){
					y_pos=7;
				}
				//lcd_init(0xAF);    // init lcd and turn on
				Print_pos(x_pos,y_pos);
				//_delay_ms(100);//wait for 5 second
			}
			 else if(b>135){
				//y_pos=y_pos-3;
				y_pos=y_pos-(b-125)/28;
				if(y_pos<0 || y_pos==0){
					y_pos=0;
				}
				//lcd_init(0xAF);    // init lcd and turn on
				Print_pos(x_pos,y_pos);
			}
			/*int cal=0;
			else if
			{
				y_pos=b/28;x_pos=a/28;
				Print_pos(x_pos,y_pos);
				_delay_ms(10);
			}
			
	}*/}
	return 0;
}


