/*
 * main.h
 *
 * Created: 12.02.2019 13:30:15
 *  Author: pcpl2
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))
#define SetOutput(port, pin) port&=~pin

#define	R1 216
#define VREF 5
#define FullFuelTankOhms 171.5

#define ADC5 (1 << MUX2)|(1 << MUX0)
#define ADC4 (1 << MUX2)
#define ADC3 (1 << MUX1)|(1 << MUX0)
#define ADC2 (1 << MUX1)
#define ADC1 (1 << MUX0)

void loop(void);
void initOhms(void);
void printPrecent();
void buildProgressbar();
_Bool calculateOhms(void);


#endif /* MAIN_H_ */
