/*
 * main.c
 *
 * Created: 08.02.2019 22:07:00
 * Author : pcpl2
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "main.h"

#include "lcdpcf8574/lcdpcf8574.h"
#include "pcf8574/pcf8574.h"

volatile uint16_t ADCValue = 0;
volatile uint16_t Calculated = 0;
volatile uint16_t PrecentFuelTank = 0;

ISR(ADC_vect)
{
	ADCValue = ADC;
}

void startText(void)
{
	lcd_gotoxy(0, 0);
	lcd_puts("Fuel Counter by");
	lcd_gotoxy(0, 1);
	lcd_puts("pcpl2");
}

int main(void)
{
	lcd_init(LCD_DISP_ON);
	lcd_home();
	lcd_led(0);

	startText();

	initOhms();

	sei();

	_delay_ms(500);
	lcd_clrscr();

	while(1)
	{
		loop();
	}
}

void initOhms(void)
{
	#if defined (__AVR_ATtiny85__)
		ADCSRA = (1 << ADEN) | //uruchomienie ADC
				(1 << ADATE) | //w³¹czenie trybu Free run
				(1 << ADIE) | //uruchomienie zg³aszania przerwañ
				(1 << ADSC) | //rozpocz¹cie konwersji
				(1 << ADPS2) |
				(1 << ADPS1) |
				(1 << ADPS0); //(ustawienie preskalera) preskaler= 64


		  ADMUX = (0 << REFS1) |     // Sets ref. voltage to VCC, bit 1
				(0 << REFS0) |     // Sets ref. voltage to VCC, bit 0
				(0 << MUX3)  |     // use ADC2 for input (PB4), MUX bit 3
				(0 << MUX2)  |     // use ADC2 for input (PB4), MUX bit 2
				(1 << MUX1)  |     // use ADC2 for input (PB4), MUX bit 1
				(0 << MUX0);       // use ADC2 for input (PB4), MUX bit 0

		SetOutput(DDRB, (1 << PB4)); //Ustawienie Wejœcia ADC

	#elif defined (__AVR_ATmega8__)
	ADCSRA |= (1 << ADEN) //uruchomienie ADC
			|(1 << ADFR) //w³¹czenie trybu Free run
			|(1 << ADIE) //uruchomienie zg³aszania przerwañ
			|(1 << ADSC) //rozpocz¹cie konwersji
			|(1 << ADPS2); //ADPS2: (ustawienie preskalera) preskaler= 16

	ADMUX |= (1 << REFS0) //VCC jako napiêcie referencyjne
			| ADC3; //wybór kana³u pomiarowego na pinie ADC3

	SetOutput(DDRC, (1 << PC3)); //Ustawienie Wejœcia ADC
	#endif
}

void loop(void)
{
	_Bool printNewData = calculateOhms();
	if(printNewData)
	{
		printPrecent(Calculated);
	}
}

void printPrecent()
{
	lcd_gotoxy(0, 0);
	buildProgressbar();

	lcd_gotoxy(0, 1);
	char ohmsMsg[16];
	sprintf(ohmsMsg, "%d%% R: = %d", PrecentFuelTank, Calculated);
	lcd_puts(ohmsMsg);
}

void buildProgressbar()
{
	const size_t PROGRESS_BAR_LENGTH = 16;
	uint8_t progressCount = ((PrecentFuelTank * 14) / 100);
	char progress[PROGRESS_BAR_LENGTH+1];
	progress[0] = '[';
	for (uint8_t i = 1; i < PROGRESS_BAR_LENGTH - 1; ++i)
	{
		progress[i] = (i < progressCount) ? '#' : ' ';
	}
	progress[PROGRESS_BAR_LENGTH - 1] = ']';

	lcd_puts(progress);
}

_Bool calculateOhms(void)
{
	uint16_t ohms = R1 * ( VREF / ( (float)(ADCValue) / 1024 * VREF) - 1);
	if (ohms != Calculated)
	{
		Calculated = ohms;
		float precent = (ohms / FullFuelTankOhms) * 100;
		PrecentFuelTank = FLOAT_TO_INT(precent);
		return 1;
	}
	return 0;
}
