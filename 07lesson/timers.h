#pragma once

#include "defines.h"
#include "stm32f0xx.h"
#include "subscriber.h"
#include <cstddef>


void systemTimerInit()
{
   constexpr uint32_t val = F_CPU/(1000)-1;
   SysTick->LOAD = val;                        // Загрузка значения
   SysTick->VAL  = val;                        // Обнуляем таймеры и флаги 
   SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk  // processor clock
                 | SysTick_CTRL_TICKINT_Msk;   // разрешение прерывания
   SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   // запускает таймер
}


// обычный издатель, у которого добавляется только конструктор с инициализацией системного таймера
// все методы определены в Publisher
struct TickUpdater : Publisher
{
   TickUpdater() { systemTimerInit(); }
} tickUpdater;

extern "C" void SysTick_Handler()
{
   tickUpdater.notify();
}



template<std::size_t time>
class CyclicTask : Subscriber
{
private:
   std::size_t time_count {0};
   void notify() override { time_count++; }
public:
   CyclicTask() { tickUpdater.subscribe(this); }
   void operator ()(void(function)())
   { 
      if (time_count == time)
         function();
      time_count %= time;
   }
};
