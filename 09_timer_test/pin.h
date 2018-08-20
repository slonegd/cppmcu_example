#pragma once

#include "defines.h"
#include "stm32f0xx.h"

// для того, чтобы добиться тестируемости этого класса
// необходимо заставить его работать не на микроконтроллере
// очевидно, что если запустить это класс на компе,
// он работать не будет, поскольку тут есть жесткая привязка к адресу
// Обычно в таких случаях создают мок-объекты (mock - поддельный),
// которые подменяют собой другие объекты, которые должны реагировать
// на методы класса, или которые должны заствалять работать тестируемый объект
// Объекты класса Pin создавать не предполагается
// Единственный метод, который жестко завязан с памятью - port() (пока не рассматриваем clockEnable)
// Перенесём его в отдельный класс, сделаем его параметром шаблона по умолчанию.
// Тогда можно создавать инстанцированные классы, где вместо
// класса по умолчанию передавать любой другой класс (мок-класс)
// clockEnable - чужеродный метод. При дальнейшем развитии его необходимо перенести
// в другой класс RCC, а здесь вызывать метод этого класса. Его тестированием тут не занимаемся.
namespace old {

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

} // namespace old {


// класс, в который перенесём метод port
// сразу делаем параметром шаблонна тип переферии, поскольку работать далее будем не только с портами
template<class Periph, uint32_t adr>
struct Pointer
{
   static auto port() { return reinterpret_cast<Periph>(adr); }
};

// переписываем Pin
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
   // теперь надо явно указать откуда метод port
   static void setAsOut() { Pointer::port()->MODER |= 1 << (2*n); }
   static void set()      { Pointer::port()->BSRR = 1 << n; }
   static void clear()    { Pointer::port()->BSRR = 1 << (16 + n); }
   static bool isSet()    { return (Pointer::port()->IDR & (1 << n)) != 0;  }
   static void toggle()   { isSet() ? clear() : set(); }
};

using PC8 = Pin<GPIOC_BASE, 8>;
using PC9 = Pin<GPIOC_BASE, 9>;


// сами тесты там ./test/test_pin.cpp



