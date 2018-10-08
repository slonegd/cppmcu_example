/**
 * 20 объекты вместо статических классов
 **/

#include "defines.h"
#include "stm32f0xx.h"
#include "pin.h"
#include "timers.h"

// в 01 части 2 последние итерации получились одинаковые по коду
// iter7 была в виде обычного класса, поля которого хранили всю нформацию о пине
// iter8 была в виде шаблона класса, информация о пине передаётся в качестве параметра шаблона
// практика показала, что синтаксис с шаблонами более трудочитаем
// , а раз нет никакой разницы, то попробуем перейти на классические объекты
// переписал всё что качается GPIO RCC на обычные классы (без шаблонов)
// пустая прога 536
// 19: 1104 -0s
// 20: 1544 -0s
// видно, что размер того же кода увеличился в 2 раза
// , компилятор не смог оптимизировать как в части 01, посмотрим почему

int main(void)
{
   Pin blueled  {Port::N::C, 8, PinMode::Output};
   Pin greenled {Port::N::C, 9, PinMode::Output};
   Pin none     {Port::N::A, 0, PinMode::Input};

   CyclicTask<200> _200ms;
   CyclicTask<300> _300ms;

   while (1)
   {
      _200ms([&]() {  blueled.toggle(); });
      _300ms([&]() { greenled.toggle(); });

      // делать в этом цикле пока нечего, потому спим до прерывания
      __WFI();
   }
}

// посмотрим, что выдаст компилятор в обоих случаях для set и toggle внутри лямбды
// видно, что компилятор сделал одинаковый код

// 19 часть (set):
// template_GPIO<1207961600ul, Pointer<GPIO_t<1207961600ul> > >::set<8u> () at GPIO.h:180
// 180	   template<size_t n> static void set()   { Pointer::get()->BSRR |= (1 << n);          }
// => 0x080001d2 <main()+122>:	99 69	ldr	r1, [r3, #24]
//    0x080001d4 <main()+124>:	21 43	orrs	r1, r4
//    0x080001d6 <main()+126>:	99 61	str	r1, [r3, #24]

// 20 часть (set):
// Port::set (n=8, this=0x48000800) at GPIO.h:191
// 191	   inline void set   (size_t n) { BSRR |= (1 << n);             }
// => 0x08000216 <main()+190>:	99 69	ldr	r1, [r3, #24]
//    0x08000218 <main()+192>:	21 43	orrs	r1, r4
//    0x0800021a <main()+194>:	99 61	str	r1, [r3, #24]


// 19 часть (toggle)
// template_GPIO<1207961600ul, Pointer<GPIO_t<1207961600ul> > >::isSet<8u> () at GPIO.h:182
// 182	   template<size_t n> static bool isSet() { return Pointer::get()->IDR.reg & (1 << n); }
// => 0x080001ae <main()+86>:	19 69	ldr	r1, [r3, #16]
// Pin<template_GPIO<1207961600ul, Pointer<GPIO_t<1207961600ul> > >, 8>::toggle () at pin.h:22
// 22	   static void toggle()      { isSet() ? clear() : set(); }
// => 0x080001b0 <main()+88>:	01 42	tst	r1, r0
//    0x080001b2 <main()+90>:	06 d0	beq.n	0x80001c2 <main()+106>
// template_GPIO<1207961600ul, Pointer<GPIO_t<1207961600ul> > >::set<8u> () at GPIO.h:180
// 180	   template<size_t n> static void set()   { Pointer::get()->BSRR |= (1 << n);          }
// => 0x080001c2 <main()+106>:	99 69	ldr	r1, [r3, #24]
//    0x080001c4 <main()+108>:	01 43	orrs	r1, r0
//    0x080001c6 <main()+110>:	f7 e7	b.n	0x80001b8 <main()+96>
// 180	   template<size_t n> static void set()   { Pointer::get()->BSRR |= (1 << n);          }
// => 0x080001b8 <main()+96>:	99 61	str	r1, [r3, #24]

// 20 часть (toggle)
// Port::isSet (n=8, this=0x48000800) at GPIO.h:193
// 193	   inline bool isSet (size_t n) { return IDR.reg & (1 << n);    }
// => 0x0800021c <main()+196>:	19 69	ldr	r1, [r3, #16]
// 0x08000220	194	   inline void toggle(size_t n) { isSet(n) ? clear(n) : set(n); }
//    0x0800021e <main()+198>:	01 42	tst	r1, r0
// => 0x08000220 <main()+200>:	06 d0	beq.n	0x8000230 <main()+216>
// 0x08000234	191	   inline void set   (size_t n) { BSRR |= (1 << n);             }
//    0x08000230 <main()+216>:	99 69	ldr	r1, [r3, #24]
//    0x08000232 <main()+218>:	01 43	orrs	r1, r0
// => 0x08000234 <main()+220>:	f7 e7	b.n	0x8000226 <main()+206>
// 191	   inline void set   (size_t n) { BSRR |= (1 << n);             }
// => 0x08000226 <main()+206>:	99 61	str	r1, [r3, #24]

// Выводы:
//    код работы с пином абсолютно одинаковый для обоих случаев
//    код инициализации в случае с объектом не смог быть спотимизирован, поскольку
//       нельзя использовать if constexpr, так как метаинформация хранится в полях класса

// Что надо сделать
//    Как вариант, надо попробовать оставить объекты, но метаинформацию передавать
//       через параметры шаблона конструктора, тогда опять можно будет использовать constexpr
//       хотя бы внутри конструктора.
//    Для случая, если эта информация нужна не только в конструкторе, надо ещё думать
//       как пока необзуманная идея constexpr конструктор как посредник