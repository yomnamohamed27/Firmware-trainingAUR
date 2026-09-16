#include <avr/io.h>
#include <util/delay.h>

int main()

{
  int counter = 0;
  TCNT0 = 0;
  DDRB |= (1 << PB0);
  TCCR0 = (1 << WGM01) | (1 << CS01) | (1 << CS00);
  OCR0 = 124;
  while (1)

  {
    if (TIFR & (1 << OCF0))
    {
      counter++;
      TIFR = (1 << OCF0);

      if (counter == 500)

      {
        PORTB ^= (1 << PB0);
        counter = 0;
      }
    }
  }
}  