/**
 * 07lesson выделение паттерна в отдельный файл и итераторы
 **/

#include "defines.h"
#include "stm32f0xx.h"
#include "pin.h"
#include "timers.h"

// в этом файле оставил только main
// в subscriber.h выделил всё, что касается подписчика,
//    ещё пригодится в прерываниях
//    так же использовал там автоцикл для прохода по подписчикам
// в timers.h выделил всё что касается программного таймера
//    (переписал на сущности из subscriber.h)
// в list.h ввёл итераторы для того, чтобы работал автоцикл

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
















