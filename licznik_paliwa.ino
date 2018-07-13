#include <LiquidCrystal.h>

float r1= 10000.0;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
void setup()
{
  lcd.begin(16, 2);
}

void loop()
{
  printPrecent(getOhms());
  delay(5000);
}

float calculatePrecent(float ohms) 
{
 return (ohms / 171.5) * 100; 
}

void printPrecent(float ohms) 
{
  int precent = (int) calculatePrecent(ohms);
  lcd.setCursor(0, 0);
  lcd.print(buildProgressbar(precent));
  lcd.setCursor(0, 1);
  char ohmsMsg[12];
  sprintf(ohmsMsg, "R: = %d", (int)ohms);
  lcd.print(ohmsMsg);
  lcd.setCursor(12, 1);
  char precentMsg[6];
  sprintf(precentMsg, "%d%%", precent);
  lcd.print(precentMsg);
}

String buildProgressbar(int precent)
{
  int progressCount = ((precent * 14) / 100);
  String progress;
  progress += F("[");
  for(int i = 0; i <= 13; i++)
  {
    if(i < (progressCount - 1)) 
    {
      progress += F(" ");
    } else
    {
      progress += F("#");
    }
  }
  progress += F("]");
  return progress;
}

float getOhms() 
{
  float Uwyj = (5.0/1024.0) * float(analogRead(0));
  float R = r1 * ( (5.0/Uwyj) - 1);
  return R;
}
