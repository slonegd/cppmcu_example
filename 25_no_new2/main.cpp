/**
 * 25 переопределение блокирующих функций 
 * при конструировании статической локальной переменной
 **/

#include "defines.h"
#include "stm32f0xx.h"
#include "pin.h"


// отсюда http://indrekis2.blogspot.com/2015/11/c-arm-gcc-stm32-coide.html
// узнал, что функции мьютексов, которые вызываются
// при создании статических локальных переменных можно переопределить
// что я и сделал в файле no_mutex.h
// в pin.h заинклюдил его
// НО, чуть позже я узнал, что есть флаг компиляции -fno-threadsafe-statics
// , отключающий эти функции, добавил их в makefile
// , no_mutex.h оставил просто чтоб был


int main()
{
   auto& pc8 = Pin::make<Periph::PC, 8, PinMode::Output>();
   pc8.set();
}


// результат компиляции -0s
// text    data     bss     dec     hex filename
//  576       8    1576    2160     870 build/main.elf  // standart static
//  576      12    1824    2412     96c build/main.elf  // наш new с проверкой на переполнение кучи
//  568      12    1824    2404     964 build/main.elf  // наш new без проверки
//  528       8    1576    2112     840 build/main.elf  // no mutex static
//  496       8    1568    2072     818 build/main.elf  // stack variable


// посмотрим выполнение программы и убедимся что функции исчезли
// 0x08000114	23	      static Pin pin { Port::make<p>(), n };
//    0x08000108 <main()+0>:	10 4b	ldr	r3, [pc, #64]	; (0x800014c <main()+68>)
//    0x0800010a <main()+2>:	1b 78	ldrb	r3, [r3, #0]
//    0x0800010c <main()+4>:	bf f3 5b 8f	dmb	ish
//    0x08000110 <main()+8>:	0f 49	ldr	r1, [pc, #60]	; (0x8000150 <main()+72>)
//    0x08000112 <main()+10>:	db 07	lsls	r3, r3, #31
// => 0x08000114 <main()+12>:	09 d4	bmi.n	0x800012a <main()+34>
// 0x08000120	153	      else if constexpr (p == Periph::PC) AHBENR.IOPCEN = true;
//    0x08000116 <main()+14>:	80 23	movs	r3, #128	; 0x80
//    0x08000118 <main()+16>:	0e 4a	ldr	r2, [pc, #56]	; (0x8000154 <main()+76>)
//    0x0800011a <main()+18>:	1b 03	lsls	r3, r3, #12
//    0x0800011c <main()+20>:	50 69	ldr	r0, [r2, #20]
//    0x0800011e <main()+22>:	03 43	orrs	r3, r0
// => 0x08000120 <main()+24>:	53 61	str	r3, [r2, #20]
// 0x08000128	16	   Pin (Port& port, int n) : port{port}, n{n} {}
//    0x08000122 <main()+26>:	0d 4b	ldr	r3, [pc, #52]	; (0x8000158 <main()+80>)
//    0x08000124 <main()+28>:	0b 60	str	r3, [r1, #0]
//    0x08000126 <main()+30>:	08 23	movs	r3, #8
// => 0x08000128 <main()+32>:	4b 60	str	r3, [r1, #4]
// Pin::make<(Periph)2, 8, (PinMode)1> () at pin.h:25
// 25	      pin.port.init<n,mode>();
// => 0x0800012a <main()+34>:	0a 68	ldr	r2, [r1, #0]
// 0x0800012e	209	      else if constexpr (n == 8)  MODER.MODER8  = mode;
//    0x0800012c <main()+36>:	0b 48	ldr	r0, [pc, #44]	; (0x800015c <main()+84>)
// => 0x0800012e <main()+38>:	13 68	ldr	r3, [r2, #0]
// 0x08000138	209	      else if constexpr (n == 8)  MODER.MODER8  = mode;
//    0x08000130 <main()+40>:	18 40	ands	r0, r3
//    0x08000132 <main()+42>:	80 23	movs	r3, #128	; 0x80
//    0x08000134 <main()+44>:	5b 02	lsls	r3, r3, #9
//    0x08000136 <main()+46>:	03 43	orrs	r3, r0
// => 0x08000138 <main()+48>:	13 60	str	r3, [r2, #0]
// 0x08000144	193	   void set   (size_t n) { BSRR |= (1 << n);             }
//    0x0800013a <main()+50>:	01 23	movs	r3, #1
//    0x0800013c <main()+52>:	49 68	ldr	r1, [r1, #4]
//    0x0800013e <main()+54>:	90 69	ldr	r0, [r2, #24]
//    0x08000140 <main()+56>:	8b 40	lsls	r3, r1
//    0x08000142 <main()+58>:	03 43	orrs	r3, r0
// => 0x08000144 <main()+60>:	93 61	str	r3, [r2, #24]