/*
 * Timer4Exam.c
 *
 * Created: 9/6/2020 10:57:36 PM
 * Author : Abhilekh
 */ 
#define F_CPU 1000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#define BUZZERPORT PORTC
#define BUZZERDDR DDRC
#define INDDR DDRB
#define INPIN PINB
#define INPORT PORTB
#define switch1pin 0   // input pin for setting firsttimervalue
#define switch2pin  1  // input pin for setting secondtimervalue
#define firsttimervalue 40 //40 secs
#define secondtimervalue 65  //65 secs
int seconds=0;  // incremented by timer from startup
int overflowseconds=-1;
void buzzshort(){
	BUZZERPORT=0xFF;
	_delay_ms(50);
	BUZZERPORT=0x00;
}
void buzzlong(){
	BUZZERPORT=0xFF;
	_delay_ms(600);
	BUZZERPORT=0x00;
}
ISR(TIMER1_OVF_vect)
{	
	seconds++;
	TCNT1H=(-977)>>8;
	TCNT1L=(-977)&0xFF;	
}
int main(void)
{
	BUZZERDDR=0xFF;
	INDDR=0x00;
	INPORT=0xFF;
	buzzshort();
	TCNT1H=(-977)>>8;
	TCNT1L=(-977)&0xFF;// over flow after 977 clocks__works for 1MHZ clock,[1/1000000x1024x977=1 second]
	TCCR1A=0x00;//normal mode
	TCCR1B=0x05;//internal clock ,prescaler 1:1024
	TIMSK=(1<<TOIE1);// enable timer1 interrupt
	sei();
    while (1) 
    {
		if(!(INPIN&(1<<switch1pin)))
		{
			_delay_ms(50);//debounce
			if(!(INPIN&(1<<switch1pin)))
			{
				buzzshort();
				overflowseconds=seconds+firsttimervalue;
			}
		}
		if(!(INPIN&(1<<switch2pin)))
		{
			_delay_ms(50);//debounce
			if(!(INPIN&(1<<switch2pin)))
			{
				buzzshort();
				overflowseconds=seconds+secondtimervalue;
			}
		}
		if(seconds==overflowseconds){
			buzzlong();
		}
		if((seconds-overflowseconds)>600) // if device not used for more than 1 hour, sound alarm to switch off the device
		{
			cli();
			for(int i=0;i<2000;i++)
			{
				buzzlong();
			}		
		}
	}
}

