#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "HD44780.h"

#include "main.h"


#define INPUT_DATA			PC0
#define	r1					216

const unsigned short ohm[8] = {
	0b00000,
	0b01110,
	0b10001,
	0b10001,
	0b10001,
	0b01010,
	0b11011,
	0b00000
};

int main( void )
{
	LCD_Initalize();

	LCD_WriteText("Fuel Counter By");
	LCD_GoTo(0, 1);
	LCD_WriteText("pcpl2");

	initOhms();

	_delay_ms(500);
	LCD_Clear();
	while(1)
	{
		LCD_Clear();
		loop();
	}
}

void initOhms(void)
{
	ADMUX |= _BV(REFS0);

	ADMUX |= _BV(MUX1)|_BV(MUX0);
	ADCSRA |= _BV(ADEN);
	ADCSRA |= _BV(ADPS0);
	ADCSRA |= _BV(ADPS1);
	ADCSRA |= _BV(ADPS2);
}


void loop(void)
{
  printPrecent(getOhms());
  _delay_ms(200);
}

float calculatePrecent(float ohms)
{
 return (ohms / 171.5) * 100;
}

void printPrecent(float ohms)
{
  int precent = (int) calculatePrecent(ohms);
  LCD_GoTo(0, 0);
  buildProgressbar(precent);
  //LCD_WriteText(buildProgressbar(precent));
  LCD_GoTo(0, 1);
  char ohmsMsg[12];
  sprintf(ohmsMsg, "R: = %d", (int)ohms);
  LCD_WriteText(ohmsMsg);
 // LCD_WriteData(ohm);
  LCD_GoTo(12, 1);
  char precentMsg[6];
  sprintf(precentMsg, "%d%%", precent);
  LCD_WriteText(precentMsg);
}

void buildProgressbar(int precent)
{
  const size_t PROGRESS_BAR_LENGTH = 16;
  int progressCount = ((precent * 14) / 100);
  char progress[PROGRESS_BAR_LENGTH+1];
  progress[0] = '[';
  for (int i = 1; i < PROGRESS_BAR_LENGTH - 1; ++i)
  {
      progress[i] = (i < progressCount) ? '#' : ' ';
  }
  progress[PROGRESS_BAR_LENGTH - 1] = ']';

  LCD_WriteText(progress);
}

float getOhms(void)
{
    ADCSRA |= _BV(ADSC);
    while(ADCSRA & _BV(ADSC)) {}
	float uWyj =(float)(ADCL | (ADCH<<8))/1024*5;
	float r = r1 * ( 5.0f/uWyj - 1);
	return r;
}

