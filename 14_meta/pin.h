#pragma once

#include "defines.h"
#include "stm32f0xx.h"

// класс, в который перенесём метод port
// сразу делаем параметром шаблонна тип переферии, поскольку работать далее будем не только с портами
template<class Periph, uint32_t adr>
struct Pointer
{
   static auto port() { return reinterpret_cast<Periph>(adr); }
};


template <uint32_t adr, int n, class Pointer = Pointer<decltype(GPIOA), adr>>
struct Pin {
   static void clockEnable()
   {
      // по адресу порта определяем маску разрешения тактирования
      constexpr auto mask =
         adr == GPIOA_BASE ? RCC_AHBENR_GPIOAEN :
         adr == GPIOB_BASE ? RCC_AHBENR_GPIOBEN :
         adr == GPIOC_BASE ? RCC_AHBENR_GPIOCEN :
         adr == GPIOD_BASE ? RCC_AHBENR_GPIODEN :
         adr == GPIOF_BASE ? RCC_AHBENR_GPIOFEN : 0;
      RCC->AHBENR |= mask;
   }
   static void setAsOut() { Pointer::port()->MODER |= 1 << (2*n); }
   static void set()      { Pointer::port()->BSRR = 1 << n; }
   static void clear()    { Pointer::port()->BSRR = 1 << (16 + n); }
   static bool isSet()    { return (Pointer::port()->IDR & (1 << n)) != 0;  }
   static void toggle()   { isSet() ? clear() : set(); }
};

using PC8 = Pin<GPIOC_BASE, 8>;
using PC9 = Pin<GPIOC_BASE, 9>;
