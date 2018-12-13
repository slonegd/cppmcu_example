/**
 * 24 статическая локальная переменная как алтернатива new
 **/

#include "defines.h"
#include "stm32f0xx.h"
#include "pin.h"
#include "pin_old.h"


// Возникла идея не использовать new, а попробовать следующий приём
// https://wandbox.org/permlink/UlZPsUbezhThjZkW
// локальный статик конструируется только один раз и живёт до конца программы
// что вообщем то нам и было нужно, когда использовали new
// попробуем
// старый файл pin.h переименовал в pin_old, чтобы сравнить

int main()
{
   auto& pc8 = Pin    ::make<Periph::PC, 8, PinMode::Output>();
   // auto  pc8 = Pin_old::make<Periph::PC, 8, PinMode::Output>();
   pc8.set();
}


// результат компиляции -0s
// text    data     bss     dec     hex filename
//  576       8    1576    2160     870 build/main.elf
//  496       8    1568    2072     818 build/main.elf
// размер программы увеличился на 576-496=80 байт, это плата за статическую переменную

// посмотрим выполнение программы
// 0x0800011a	20	      static Pin pin { Port::make<p>(), n };
//    0x0800010a <main()+2>:	14 4d	ldr	r5, [pc, #80]	; (0x800015c <main()+84>)
//    0x0800010c <main()+4>:	2b 78	ldrb	r3, [r5, #0]
//    0x0800010e <main()+6>:	bf f3 5b 8f	dmb	ish
//    0x08000112 <main()+10>:	13 4c	ldr	r4, [pc, #76]	; (0x8000160 <main()+88>)
//    0x08000114 <main()+12>:	db 07	lsls	r3, r3, #31
//    0x08000116 <main()+14>:	11 d4	bmi.n	0x800013c <main()+52>
//    0x08000118 <main()+16>:	28 00	movs	r0, r5
// => 0x0800011a <main()+18>:	00 f0 50 f8	bl	0x80001be <__cxa_guard_acquire>
// => 0x080001be <__cxa_guard_acquire+0>:	01 23	movs	r3, #1
// => 0x080001c0 <__cxa_guard_acquire+2>:	02 68	ldr	r2, [r0, #0]
// => 0x080001c2 <__cxa_guard_acquire+4>:	1a 42	tst	r2, r3
// => 0x080001c4 <__cxa_guard_acquire+6>:	06 d1	bne.n	0x80001d4 <__cxa_guard_acquire+22>
// => 0x080001c6 <__cxa_guard_acquire+8>:	42 78	ldrb	r2, [r0, #1]
// => 0x080001c8 <__cxa_guard_acquire+10>:	00 2a	cmp	r2, #0
// => 0x080001ca <__cxa_guard_acquire+12>:	00 d0	beq.n	0x80001ce <__cxa_guard_acquire+16>
// => 0x080001ce <__cxa_guard_acquire+16>:	43 70	strb	r3, [r0, #1]
// => 0x080001d0 <__cxa_guard_acquire+18>:	18 00	movs	r0, r3
// => 0x080001d2 <__cxa_guard_acquire+20>:	70 47	bx	lr
//    0x0800011e <main()+22>:	00 28	cmp	r0, #0
// => 0x08000120 <main()+24>:	0c d0	beq.n	0x800013c <main()+52>
// 0x08000128	153	      else if constexpr (p == Periph::PC) AHBENR.IOPCEN = true;
//    0x08000122 <main()+26>:	80 23	movs	r3, #128	; 0x80
//    0x08000124 <main()+28>:	0f 4a	ldr	r2, [pc, #60]	; (0x8000164 <main()+92>)
//    0x08000126 <main()+30>:	1b 03	lsls	r3, r3, #12
// => 0x08000128 <main()+32>:	51 69	ldr	r1, [r2, #20]
// Pin::make<(Periph)2, 8, (PinMode)1> () at pin.h:20
// 20	      static Pin pin { Port::make<p>(), n };
// => 0x0800012a <main()+34>:	28 00	movs	r0, r5
// 0x0800012e	153	      else if constexpr (p == Periph::PC) AHBENR.IOPCEN = true;
//    0x0800012c <main()+36>:	0b 43	orrs	r3, r1
// => 0x0800012e <main()+38>:	53 61	str	r3, [r2, #20]
// 0x08000136	13	   Pin (Port& port, int n) : port{port}, n{n} {}
//    0x08000130 <main()+40>:	0d 4b	ldr	r3, [pc, #52]	; (0x8000168 <main()+96>)
//    0x08000132 <main()+42>:	23 60	str	r3, [r4, #0]
//    0x08000134 <main()+44>:	08 23	movs	r3, #8
// => 0x08000136 <main()+46>:	63 60	str	r3, [r4, #4]
// Pin::make<(Periph)2, 8, (PinMode)1> () at pin.h:20
// 20	      static Pin pin { Port::make<p>(), n };
// => 0x08000138 <main()+48>:	00 f0 4e f8	bl	0x80001d8 <__cxa_guard_release>
// => 0x080001d8 <__cxa_guard_release+0>:	01 23	movs	r3, #1
// => 0x080001da <__cxa_guard_release+2>:	03 60	str	r3, [r0, #0]
// => 0x080001dc <__cxa_guard_release+4>:	70 47	bx	lr
// Pin::make<(Periph)2, 8, (PinMode)1> () at pin.h:22
// 22	      pin.port.init<n,mode>();
// => 0x0800013c <main()+52>:	22 68	ldr	r2, [r4, #0]
// 0x08000140	209	      else if constexpr (n == 8)  MODER.MODER8  = mode;
//    0x0800013e <main()+54>:	0b 49	ldr	r1, [pc, #44]	; (0x800016c <main()+100>)
// => 0x08000140 <main()+56>:	13 68	ldr	r3, [r2, #0]
// 0x0800014a	209	      else if constexpr (n == 8)  MODER.MODER8  = mode;
//    0x08000142 <main()+58>:	19 40	ands	r1, r3
//    0x08000144 <main()+60>:	80 23	movs	r3, #128	; 0x80
//    0x08000146 <main()+62>:	5b 02	lsls	r3, r3, #9
//    0x08000148 <main()+64>:	0b 43	orrs	r3, r1
// => 0x0800014a <main()+66>:	13 60	str	r3, [r2, #0]
// 0x08000154	193	   void set   (size_t n) { BSRR |= (1 << n);             }
//    0x0800014c <main()+68>:	01 23	movs	r3, #1
//    0x0800014e <main()+70>:	60 68	ldr	r0, [r4, #4]
//    0x08000150 <main()+72>:	91 69	ldr	r1, [r2, #24]
//    0x08000152 <main()+74>:	83 40	lsls	r3, r0
// => 0x08000154 <main()+76>:	0b 43	orrs	r3, r1
// Port::set (n=8, this=0x48000800) at GPIO.h:193
// 193	   void set   (size_t n) { BSRR |= (1 << n);             }
// => 0x08000158 <main()+80>:	93 61	str	r3, [r2, #24]

// В выполнение вклинился вызов этих функций
// __cxa_guard_acquire и __cxa_guard_release
// они и вызвали такое увеличение времени выполнения
// данные функции предоставлены компилятором
// Поскольку компилятор должен учитывать выполнение программы
// в многопоточной среде, он создаёт мьютекс, чтобы
// статическая переменная не конструировалась 2 раза

// Но у нас не многопоточная среда, поэтому эти мьютексы не нужны
// поэтому предлагаю переопределить эти функции на пустые в следующей части