/**
 * 09 рефакторинг CyclicTask, ListIterator, тест на лямбдах
 **/

#include "defines.h"
#include "stm32f0xx.h"
#include "pin.h"
#include "timers.h"

// тут ничего не меняется
// см. timers.h: изменения CyclicTask так, чтобы оператор() мог принимать лямбду с захватом
// см. test_cyclic_task.cpp: тот же тест, но на лямбдах
// см. list.h: перенос класса ListIterator внутрь класса List
//

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
















