#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

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
  LCD_WriteText(buildProgressbar(precent));
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

char* buildProgressbar(int precent)
{
  int progressCount = ((precent * 14) / 100);
  char *progress = calloc(16, 1);
  char *buffer = calloc(14, 1);
  for(int i = 0; i <= 13; i++)
  {
	  buffer[i] = i < (progressCount - 1) ? '#' : ' ';

/*    if(i < (progressCount - 1))
    {
    	sprintf(&progress, "%s ", progress);
    } else
    {
    	sprintf(&progress, "%s#", progress);
    }*/
  }
  sprintf(progress, "[%s]", buffer);
  free(buffer);
  return progress;
}

float getOhms(void)
{
    ADCSRA |= _BV(ADSC);
    while(ADCSRA & _BV(ADSC)) {}
	float uWyj =(float)(ADCL | (ADCH<<8))/1024*5;
	float r = r1 * ( 5.0f/uWyj - 1);
	return r;
}

