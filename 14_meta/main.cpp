/**
 * 14 немного о метафункциях
 **/

#include "defines.h"
#include "stm32f0xx.h"
#include "pin.h"
#include "timers.h"

// тут ничего не меняется
// см. button.h

// 1036 байт -0s
int main(void)
{
   using BlueLed  = PC8;
   using GreenLed = PC9;
   BlueLed::clockEnable();
   BlueLed::setAsOut();
   GreenLed::setAsOut();

   CyclicTask<200> _200ms;
   CyclicTask<300> _300ms;

   while(1)
   {
      _200ms ([](){ BlueLed::toggle();  });

      _300ms ([](){ GreenLed::toggle(); });

      // делать в этом цикле пока нечего, потому спим до прерывания
      __WFI();
   }
}
















