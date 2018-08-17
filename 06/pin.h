#pragma once

#include "defines.h"
#include "stm32f0xx.h"

template <uint32_t adr, int n>
struct Pin {
	static auto port() { return reinterpret_cast<decltype(GPIOA)>(adr); }
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
	static void setAsOut() { port()->MODER |= 1 << (2*n); }
	static void set()      { port()->BSRR = 1 << n; }
	static void clear()    { port()->BSRR = 1 << (16 + n); }
   static bool isSet()    { return (port()->IDR & (1 << n)) != 0;  }
   static void toggle()   { isSet() ? clear() : set(); }

};

// инстанцирование шаблонов, пишу не все, а только необходимые
using PC8 = Pin<GPIOC_BASE, 8>;
using PC9 = Pin<GPIOC_BASE, 9>;