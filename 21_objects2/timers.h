#pragma once

#include "defines.h"
// #include "stm32f0xx.h"
#include "subscriber.h"
#include "systick.h"
#include <cstddef>


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
   template<class Function> void operator() (Function function)
   { 
      if (time_count == time)
         function();
      time_count %= time;
   }
};
