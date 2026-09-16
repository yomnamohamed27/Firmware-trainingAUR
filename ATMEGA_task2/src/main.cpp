#include <avr/io.h>
int count = 0;
void PWM_brightness()
{ // F_PWM = 3906.25 ~= 3906HZ
  int temp = 0;
  if (TIFR & (1 << OCF0))
  {
    count++;
    if (count == 390)
    {
      temp = OCR0 + 10;
      count = 0;

      if (temp > 255)
        OCR0 = 64;

      else
        OCR0 = temp;
    }
  TIFR = (1 << OCF0);}
}

int main()
{
  TCNT0 = 0;
  DDRB |= (1 << PB3);
  TCCR0 = (1 << WGM01) | (1 << WGM00) | (1 << COM01) | (1 << CS01);
  OCR0 = 64;
  while (1)
  {
    PWM_brightness();
  }
}
