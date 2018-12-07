#pragma once

#include "registr.h"
#include "stm32f0xx.h"

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

enum class PinMode { Input, Output, Alternate_1 };

class Port {
   __IO GPIO::MODER_t   MODER;   // mode register,                offset: 0x00
   __IO GPIO::OTYPER_t  OTYPER;  // output type register,         offset: 0x04
   __IO GPIO::OSPEEDR_t OSPEEDR; // output speed register,        offset: 0x08
   __IO GPIO::PUPDR_t   PUPDR;   // pull-up/pull-down register,   offset: 0x0C
   __IO GPIO::DR_t      IDR;     // input data register,          offset: 0x10
   __IO GPIO::DR_t      ODR;     // output data register,         offset: 0x14
   __IO uint32_t        BSRR;    // bit set/reset register,       offset: 0x18
   __IO uint32_t        LCKR;    // configuration lock register,  offset: 0x1C
   __IO GPIO::AFR_t     AFR;     // alternate function registers, offset: 0x20-0x24
public:
   using Mode         = GPIO::MODER_t  ::Mode;
   using AF           = GPIO::AFR_t    ::AF;

   Port() = default;

   // шаблонный фабричный метод, чтобы получить правильный объект
   // сам класс не шаблонный, потому его не надо инстанцировать
   // RCC как параметр шаблона для дальнейшего тестирования этого метода
   template<Periph p, class RCC = RCC> static Port& make()
   {
      // тут же включаем тактирование переферии
      RCC::make().template clockEnable<p>();
      if      constexpr (p == Periph::PA) return *reinterpret_cast<Port*>(GPIOA_BASE);
      else if constexpr (p == Periph::PB) return *reinterpret_cast<Port*>(GPIOB_BASE);
      else if constexpr (p == Periph::PC) return *reinterpret_cast<Port*>(GPIOC_BASE);
      else if constexpr (p == Periph::PD) return *reinterpret_cast<Port*>(GPIOD_BASE);
      else if constexpr (p == Periph::PF) return *reinterpret_cast<Port*>(GPIOF_BASE);
   }

   void set   (size_t n) { BSRR |= (1 << n);             }
   void clear (size_t n) { BSRR |= (1 << (n + 16));      }
   bool isSet (size_t n) { return IDR.reg & (1 << n);    }
   void toggle(size_t n) { isSet(n) ? clear(n) : set(n); }

   // переписываем в шаблонные параметры то, что компилятор не мог соптимизировать
   template<size_t n> void set (Mode mode)
   {
      if      constexpr (n == 0)  MODER.MODER0  = mode;
      else if constexpr (n == 1)  MODER.MODER1  = mode;
      else if constexpr (n == 2)  MODER.MODER2  = mode;
      else if constexpr (n == 3)  MODER.MODER3  = mode;
      else if constexpr (n == 4)  MODER.MODER4  = mode;
      else if constexpr (n == 5)  MODER.MODER5  = mode;
      else if constexpr (n == 6)  MODER.MODER6  = mode;
      else if constexpr (n == 7)  MODER.MODER7  = mode;
      else if constexpr (n == 8)  MODER.MODER8  = mode;
      else if constexpr (n == 9)  MODER.MODER9  = mode;
      else if constexpr (n == 10) MODER.MODER10 = mode;
      else if constexpr (n == 11) MODER.MODER11 = mode;
      else if constexpr (n == 12) MODER.MODER12 = mode;
      else if constexpr (n == 13) MODER.MODER13 = mode;
      else if constexpr (n == 14) MODER.MODER14 = mode;
      else if constexpr (n == 15) MODER.MODER15 = mode;
   }
   template<size_t n>  void set (AF af)
   {
      if      constexpr (n == 0)  AFR.AFR0  = af;
      else if constexpr (n == 1)  AFR.AFR1  = af;
      else if constexpr (n == 2)  AFR.AFR2  = af;
      else if constexpr (n == 3)  AFR.AFR3  = af;
      else if constexpr (n == 4)  AFR.AFR4  = af;
      else if constexpr (n == 5)  AFR.AFR5  = af;
      else if constexpr (n == 6)  AFR.AFR6  = af;
      else if constexpr (n == 7)  AFR.AFR7  = af;
      else if constexpr (n == 8)  AFR.AFR8  = af;
      else if constexpr (n == 9)  AFR.AFR9  = af;
      else if constexpr (n == 10) AFR.AFR10 = af;
      else if constexpr (n == 11) AFR.AFR11 = af;
      else if constexpr (n == 12) AFR.AFR12 = af;
      else if constexpr (n == 13) AFR.AFR13 = af;
      else if constexpr (n == 14) AFR.AFR14 = af;
      else if constexpr (n == 15) AFR.AFR15 = af;
   }
   template<size_t n, PinMode mode> void init()
   {
      if        constexpr (mode == PinMode::Input) {
         set<n> (Mode::Input);

      } else if constexpr (mode == PinMode::Output) {
         set<n> (Mode::Output);

      } else if constexpr (mode == PinMode::Alternate_1) {
         set<n> (Mode::Alternate);
         set<n>   (AF::_1);
      }

   }
};

// далее см. pin.h