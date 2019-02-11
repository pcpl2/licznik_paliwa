/*
 * main.h
 *
 *  Created on: 26 gru 2018
 *      Author: pcpl2
 */

#ifndef MAIN_H_
#define MAIN_H_

#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

#define	R1 216
#define VREF 5
#define ADCINDATA PC3   //definicja ADCINDATA (wejście ADC)

#define ADC5 _BV(MUX2)|_BV(MUX0)
#define ADC4 _BV(MUX2)
#define ADC3 _BV(MUX1)|_BV(MUX0)
#define ADC2 _BV(MUX1)
#define ADC1 _BV(MUX0)

#define FullFuelTankOhms 171.5

void loop(void);
void initOhms(void);
void printPrecent();
void buildProgressbar();
_Bool calculateOhms(void);


#endif /* MAIN_H_ */
