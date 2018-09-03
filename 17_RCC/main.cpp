/**
 * 17 абстракция над RCC
 **/

#include "defines.h"
#include "stm32f0xx.h"
#include "pin.h"
#include "timers.h"

// тут ничего не меняется, добавил файл RCC.h, где всё уже должно быть знакомо
// см RCC.h

// 1104 -0s
int main(void)
{
   using BlueLed  = PC8;
   using GreenLed = PC9;
   BlueLed ::init<PinMode::Output>();
   GreenLed::init<PinMode::Output>();
   PA0     ::init<PinMode::Input>();

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
















