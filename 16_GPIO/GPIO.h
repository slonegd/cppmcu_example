#pragma once

#include "registr.h"
#include "stm32f0xx.h"

// используем namespace, поскольку названия регистров повторяются
// от переферии к переферии
// регистры описываем по аналогии с SysTick прошлой части
namespace GPIO {

struct MODER_t {
   enum Mode { Input = 0b00, Output, Alternate, Analog };
   Mode MODER0  :2;
   Mode MODER1  :2;
   Mode MODER2  :2;
   Mode MODER3  :2;
   Mode MODER4  :2;
   Mode MODER5  :2;
   Mode MODER6  :2;
   Mode MODER7  :2;
   Mode MODER8  :2;
   Mode MODER9  :2;
   Mode MODER10 :2;
   Mode MODER11 :2;
   Mode MODER12 :2;
   Mode MODER13 :2;
   Mode MODER14 :2;
   Mode MODER15 :2;
};

struct OTYPER_t {
   enum OutType  {PushPull = 0b0, OpenDrain};
   OutType OT0  :1;
   OutType OT1  :1;
   OutType OT2  :1;
   OutType OT3  :1;
   OutType OT4  :1;
   OutType OT5  :1;
   OutType OT6  :1;
   OutType OT7  :1;
   OutType OT8  :1;
   OutType OT9  :1;
   OutType OT10 :1;
   OutType OT11 :1;
   OutType OT12 :1;
   OutType OT13 :1;
   OutType OT14 :1;
   OutType OT15 :1;
   uint32_t res1 :16;
};

struct OSPEEDR_t{
   enum OutSpeed { Low = 0b00, Medium, High,
   #if defined(STM32F4) // задел на будущее
      VeryHigh
   #endif
   };
   OutSpeed OSPEEDR0  :2;
   OutSpeed OSPEEDR1  :2;
   OutSpeed OSPEEDR2  :2;
   OutSpeed OSPEEDR3  :2;
   OutSpeed OSPEEDR4  :2;
   OutSpeed OSPEEDR5  :2;
   OutSpeed OSPEEDR6  :2;
   OutSpeed OSPEEDR7  :2;
   OutSpeed OSPEEDR8  :2;
   OutSpeed OSPEEDR9  :2;
   OutSpeed OSPEEDR10 :2;
   OutSpeed OSPEEDR11 :2;
   OutSpeed OSPEEDR12 :2;
   OutSpeed OSPEEDR13 :2;
   OutSpeed OSPEEDR14 :2;
   OutSpeed OSPEEDR15 :2;
};

struct PUPDR_t {
   enum PullResistor { No = 0b00, Up, Down };
   PullResistor PUPDR0  :2;
   PullResistor PUPDR1  :2;
   PullResistor PUPDR2  :2;
   PullResistor PUPDR3  :2;
   PullResistor PUPDR4  :2;
   PullResistor PUPDR5  :2;
   PullResistor PUPDR6  :2;
   PullResistor PUPDR7  :2;
   PullResistor PUPDR8  :2;
   PullResistor PUPDR9  :2;
   PullResistor PUPDR10 :2;
   PullResistor PUPDR11 :2;
   PullResistor PUPDR12 :2;
   PullResistor PUPDR13 :2;
   PullResistor PUPDR14 :2;
   PullResistor PUPDR15 :2;
};


// эта структура немного отличается
// потому что работа с этим регистром обычно это работа не с битами,
// но в отладчике надо видеть как установлен каждый бит,
// поэтому используется объединение
struct DR_t  { // для IDR ODR
   union {
      uint32_t reg;
      struct {
         bool _0  :1;
         bool _1  :1;
         bool _2  :1;
         bool _3  :1;
         bool _4  :1;
         bool _5  :1;
         bool _6  :1;
         bool _7  :1;
         bool _8  :1;
         bool _9  :1;
         bool _10 :1;
         bool _11 :1;
         bool _12 :1;
         bool _13 :1;
         bool _14 :1;
         bool _15 :1;
         uint32_t res1 :16;
      } bits;
   };
};

struct AFR_t {
   enum AF { _0 = 0b0000, _1, _2, _3, _4, _5, _6, _7,
   #if defined(STM32F4)
      _8, _9, _10, _11, _12, _13, _14, _15
   #endif
   };
   AF AFR0  : 4;
   AF AFR1  : 4;
   AF AFR2  : 4;
   AF AFR3  : 4;
   AF AFR4  : 4;
   AF AFR5  : 4;
   AF AFR6  : 4;
   AF AFR7  : 4;
   AF AFR8  : 4;
   AF AFR9  : 4;
   AF AFR10 : 4;
   AF AFR11 : 4;
   AF AFR12 : 4;
   AF AFR13 : 4;
   AF AFR14 : 4;
   AF AFR15 : 4;
};

} // namespace GPIO {


// Тип переферии шаблонный, поскольку портов несколько, в отличии ои SysTick
template<uint32_t adr>
struct GPIO_t {
   __IO GPIO::MODER_t   MODER;   // mode register,                offset: 0x00
   __IO GPIO::OTYPER_t  OTYPER;  // output type register,         offset: 0x04
   __IO GPIO::OSPEEDR_t OSPEEDR; // output speed register,        offset: 0x08
   __IO GPIO::PUPDR_t   PUPDR;   // pull-up/pull-down register,   offset: 0x0C
   __IO GPIO::DR_t      IDR;     // input data register,          offset: 0x10
   __IO GPIO::DR_t      ODR;     // output data register,         offset: 0x14
   __IO uint32_t        BSRR;    // bit set/reset register,       offset: 0x18
   __IO uint32_t        LCKR;    // configuration lock register,  offset: 0x1C
   __IO GPIO::AFR_t     AFR;     // alternate function registers, offset: 0x20-0x24
   GPIO_t() = delete; 
   static constexpr uint32_t Base = adr; 
};

// Данное перечисление отображает все возможные конфигурации пина
// Сейчас не пишу все, только 2 для примера
enum PinMode { Input, Output, Alternate_1 };

// Шаблон методов работы с переферией
// однострочные методы определяю сразу, многострочные, оставляю на потом
template<uint32_t adr, class Pointer = Pointer<GPIO_t<adr>>>
struct template_GPIO {
   using Mode         = GPIO::MODER_t  ::Mode;
   using OutType      = GPIO::OTYPER_t ::OutType;
   using OutSpeed     = GPIO::OSPEEDR_t::OutSpeed;
   using PullResistor = GPIO::PUPDR_t  ::PullResistor;
   using AF           = GPIO::AFR_t    ::AF;

   // метод инициализации, принимает номер пина порта и режим работы пина в виде перечисления
   template<size_t, PinMode> static void init();

   static void clockEnable();

   // методы установки конкретных регистров, будут использоваться в init
   // описываю пока не все
   template<size_t, Mode> static void setMode();
   template<size_t, AF>   static void setAF();

   template<size_t n> static void set()   { Pointer::get()->BSRR |= (1 << n);          }
   template<size_t n> static void clear() { Pointer::get()->BSRR |= (1 << (n + 16));   }
   // работаем с регистром по маске (для этого делали объединение)
   template<size_t n> static bool isSet() { return Pointer::get()->IDR.reg & (1 << n); }


};


using PA = template_GPIO<GPIOA_BASE>;
using PB = template_GPIO<GPIOB_BASE>;
using PC = template_GPIO<GPIOC_BASE>;
using PD = template_GPIO<GPIOD_BASE>;
using PF = template_GPIO<GPIOF_BASE>;
#if defined(STM32F4) // задел на будущее
using PE = template_GPIO<GPIOE_BASE>;
using PG = template_GPIO<GPIOG_BASE>;
using PH = template_GPIO<GPIOH_BASE>;
using PI = template_GPIO<GPIOI_BASE>;
#endif





template<uint32_t adr, class Pointer>
void template_GPIO<adr,Pointer>::clockEnable()
{
   // RCC::clockEnable<template_GPIO<adr,Pointer>>(); // задел на будущее
   constexpr auto mask =
      adr == GPIOA_BASE ? RCC_AHBENR_GPIOAEN :
      adr == GPIOB_BASE ? RCC_AHBENR_GPIOBEN :
      adr == GPIOC_BASE ? RCC_AHBENR_GPIOCEN :
      adr == GPIOD_BASE ? RCC_AHBENR_GPIODEN :
      adr == GPIOF_BASE ? RCC_AHBENR_GPIOFEN : 0;
   RCC->AHBENR |= mask;
}


// испольуется возможность 17 стандарта - if constexpr
// она работает только начиная с 7 версии gcc, поэтому стоит обновить компилятор
// все ветки, которые не проходят условия - убираются из компиляции
// в результате останется установка только нужного бита
// чтобы не повторять кучу строк, есть возможности препроцессора
// https://www.boost.org/doc/libs/1_67_0/libs/preprocessor/doc/ref/local_macro.html
// но мне не очень нравится игра с препроцессором
// Хотя, если нало на этапе компиляции просчитать и инициализировать
// какой-нибудь массив с конст данными, то самое то
// В старом коде можно увидеть следующие вариации, которые делают тоже самое, но не так явно:
// 1 полная специализация шаблонной функции
// 2 частичная специализация структуры, имеющей метод (круглые скобки), который определяет поведение
// например тут полную специализацию применить сложно, потому что пинов 16 и режимов 4 (итого 64 специализации)
// поэтому можно применить частичную специализацию дополнительной структуры по пину
// 3 или же работать с регистром не как с битовым полем, а как с числом (масками)
template<uint32_t adr, class Pointer>
template<size_t n, typename template_GPIO<adr,Pointer>::Mode mode>
void template_GPIO<adr,Pointer>::setMode()
{
   if      constexpr (n == 0)  Pointer::get()->MODER.MODER0  = mode;
   else if constexpr (n == 1)  Pointer::get()->MODER.MODER1  = mode;
   else if constexpr (n == 2)  Pointer::get()->MODER.MODER2  = mode;
   else if constexpr (n == 3)  Pointer::get()->MODER.MODER3  = mode;
   else if constexpr (n == 4)  Pointer::get()->MODER.MODER4  = mode;
   else if constexpr (n == 5)  Pointer::get()->MODER.MODER5  = mode;
   else if constexpr (n == 6)  Pointer::get()->MODER.MODER6  = mode;
   else if constexpr (n == 7)  Pointer::get()->MODER.MODER7  = mode;
   else if constexpr (n == 8)  Pointer::get()->MODER.MODER8  = mode;
   else if constexpr (n == 9)  Pointer::get()->MODER.MODER9  = mode;
   else if constexpr (n == 10) Pointer::get()->MODER.MODER10 = mode;
   else if constexpr (n == 11) Pointer::get()->MODER.MODER11 = mode;
   else if constexpr (n == 12) Pointer::get()->MODER.MODER12 = mode;
   else if constexpr (n == 13) Pointer::get()->MODER.MODER13 = mode;
   else if constexpr (n == 14) Pointer::get()->MODER.MODER14 = mode;
   else if constexpr (n == 15) Pointer::get()->MODER.MODER15 = mode;
}

template<uint32_t adr, class Pointer>
template<size_t n, typename template_GPIO<adr,Pointer>::AF af>
void template_GPIO<adr,Pointer>::setAF()
{
   if      constexpr (n == 0)  Pointer::get()->AFR.AFR0  = af;
   else if constexpr (n == 1)  Pointer::get()->AFR.AFR1  = af;
   else if constexpr (n == 2)  Pointer::get()->AFR.AFR2  = af;
   else if constexpr (n == 3)  Pointer::get()->AFR.AFR3  = af;
   else if constexpr (n == 4)  Pointer::get()->AFR.AFR4  = af;
   else if constexpr (n == 5)  Pointer::get()->AFR.AFR5  = af;
   else if constexpr (n == 6)  Pointer::get()->AFR.AFR6  = af;
   else if constexpr (n == 7)  Pointer::get()->AFR.AFR7  = af;
   else if constexpr (n == 8)  Pointer::get()->AFR.AFR8  = af;
   else if constexpr (n == 9)  Pointer::get()->AFR.AFR9  = af;
   else if constexpr (n == 10) Pointer::get()->AFR.AFR10 = af;
   else if constexpr (n == 11) Pointer::get()->AFR.AFR11 = af;
   else if constexpr (n == 12) Pointer::get()->AFR.AFR12 = af;
   else if constexpr (n == 13) Pointer::get()->AFR.AFR13 = af;
   else if constexpr (n == 14) Pointer::get()->AFR.AFR14 = af;
   else if constexpr (n == 15) Pointer::get()->AFR.AFR15 = af;
}


template<uint32_t adr, class Pointer>
template<size_t n, PinMode c>
void template_GPIO<adr,Pointer>::init()
{
   // включаем тактировании в инициализации, больше не забудем это сделать
   clockEnable();

   if constexpr (c == Input) {
      setMode<n, Mode::Input>();

   } else if constexpr (c == Output) {
      setMode<n, Mode::Output>();

   } else if constexpr (c == Alternate_1) {
      setMode<n, Mode::Alternate>();
      setAF  <n, AF  ::_1>();
   }
}

// далее надо переписать pin.h



