#include <LiquidCrystal.h>

float r1= 10000.0;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
void setup()
{
  lcd.begin(16, 2);
}

void loop()
{
  lcd.setCursor(0, 1);
  printPrecent(getOhms());
  delay(3000);
}

float calculatePrecent(float ohms) 
{
 return (ohms / 171.5) * 100; 
}

void printPrecent(float ohms) 
{
  int precent = (int) calculatePrecent(ohms);
  lcd.setCursor(0, 0);
  String progress;
  progress += F("[");
  for(int i = 0; i <= 6; i++)
  {
    progress += F("#");
  }
  progress += F("]");
  lcd.print(progress);
  lcd.setCursor(0, 1);
  //char voltageMsg[25];
  //sprintf(voltageMsg, "WR: = %f", ohms);
  lcd.print(ohms);
  //lcd.setCursor(12, 1);
  lcd.print(String(precent, 2) + "%");
}

float getOhms() 
{
  float Uwyj = (5.0/1024.0) * float(analogRead(0));
  float R = r1 * ( (5.0/Uwyj) - 1);
  return R;
}
