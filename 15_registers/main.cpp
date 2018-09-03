/**
 * 15 абстракция над регистрами, битовые поля
 **/

#include "defines.h"
#include "stm32f0xx.h"
#include "pin.h"
#include "timers.h"

// тут ничего не меняется
// см. systick.h

// 1048 байт -0s
// размер немного увеличился, потому что при инициализации
// 2 бита устанавливали по отдельности, а не за раз, как в CMSIS
// при большом желании можно добавить метод установки сразу двух битов,
// но я не вижу в этом большой проблеммы, так как скорость выполнения инициализации не так важна
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
















