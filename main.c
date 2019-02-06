#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "main.h"

#include "lcdpcf8574/lcdpcf8574.h"
#include "pcf8574/pcf8574.h"

#define INPUT_DATA			PC0
#define	r1					216

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

	_delay_ms(500);
	lcd_clrscr();

	while(1)
	{
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
  lcd_gotoxy(0, 0);
  buildProgressbar(precent);
  lcd_gotoxy(0, 1);
  char ohmsMsg[12];
  if((int)ohms < 100) {
	  sprintf(ohmsMsg, "R: = %d ", (int)ohms);
  }
  else
  {
	  sprintf(ohmsMsg, "R: = %d", (int)ohms);
  }
  lcd_puts(ohmsMsg);
  lcd_gotoxy(12, 1);
  char precentMsg[6];
  if(precent < 100)
  {
	  sprintf(precentMsg, " %d%%", precent);
  }
  else
  {
	  sprintf(precentMsg, "%d%%", precent);
  }
  lcd_puts(precentMsg);
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

  lcd_puts(progress);
}

float getOhms(void)
{
    ADCSRA |= _BV(ADSC);
    while(ADCSRA & _BV(ADSC)) {}
	float uWyj =(float)(ADCL | (ADCH<<8))/1024*5;
	float r = r1 * ( 5.0f/uWyj - 1);
	return r;
}
