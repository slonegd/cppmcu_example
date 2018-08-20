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


// класс CyclicTask никак не взаимодействет напрямую с памятью, пожтому для
// него никаких мок создавать не надо.
// Но поведением этим классом полностью управляет издатель, который является
// глобальным объектом, конструктор кторого напрямую с памятью работает.
// Предлагаю добавить опцию компиляции для тестов -DTEST, и спрятать неугодный
// конструктор за макропоределением
struct TickUpdater : Publisher
{
#if not defined(TEST) 
   TickUpdater() { systemTimerInit(); }
#endif
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
