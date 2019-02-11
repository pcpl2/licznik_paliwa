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

ISR(ADC_vect)//obsługa przerwania po zakończeniu konwersji ADC
{
	ADCValue = ADC;
}

int main( void )
{

	lcd_init(LCD_DISP_ON);
	lcd_home();
	lcd_led(0);
	lcd_gotoxy(0, 0);
	lcd_puts("Fuel Counter by");
	lcd_gotoxy(0, 1);
	lcd_puts("pcpl2");

	initOhms();

	sei(); //Globalne uruchomienie przerwań

	_delay_ms(500);
	lcd_clrscr();

	while(1)
	{
		loop();
	}
}

void initOhms(void)
{
	ADCSRA |= _BV(ADEN) //uruchomienie ADC
			|_BV(ADFR) //włączenie trybu Free run
            |_BV(ADIE) //uruchomienie zgłaszania przerwań
            |_BV(ADSC) //rozpoczącie konwersji
			| _BV(ADPS2); //ADPS2: (ustawienie preskalera) preskaler= 16

	ADMUX |= _BV(REFS0) //VCC jako napięcie referencyjne
			| ADC3; //wybór kanału pomiarowego na pinie ADC3

	DDRC &=~ _BV(ADCINDATA); //Ustawienie Wejścia ADC
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
