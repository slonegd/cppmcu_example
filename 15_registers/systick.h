#pragma once

#include "defines.h"

// для простоты рассмотрим одну из простейших структур регистров

// CMSIS определяет регистры системного таймера так
// typedef struct
// {
//   __IOM uint32_t CTRL; /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
//   __IOM uint32_t LOAD; /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register */
//   __IOM uint32_t VAL;  /*!< Offset: 0x008 (R/W)  SysTick Current Value Register */
//   __IM  uint32_t CALIB;/*!< Offset: 0x00C (R/ )  SysTick Calibration Register */
// } SysTick_Type;
// и далее определены (define) несколько масок и позиций по битам с длинными названиями
// например
// #define SysTick_CTRL_COUNTFLAG_Pos  16U                                 /*!< SysTick CTRL: COUNTFLAG Position */
// #define SysTick_CTRL_COUNTFLAG_Msk  (1UL << SysTick_CTRL_COUNTFLAG_Pos) /*!< SysTick CTRL: COUNTFLAG Mask */

// данной информации недостаточно для понимания работы с регистрами, приходится лезть в документацию
// в регистрах, как правило, управление проиходит побитно, поэтому напрашиваются, битовые поля

struct CTRL_t {
   // данное перечисление используется в битовом поле, чтобы яснее выразить назначение бита
   enum Clock { external = 0b0, processor = 0b1 };
   bool     ENABLE    :1;  // bit 0: Indicates the enabled status of the SysTick counter
   bool     TICKINT   :1;  // bit 1: Indicates whether counting to 0 causes the status of the SysTick exception to change to pending
   Clock    CLKSOURCE :1;  // bit 2: Indicates the SysTick clock source
   uint32_t res1      :13; // bit 15-3: reserved 
   bool     COUNTFLAG :1;  // bit 16: Indicates whether the counter has counted to 0 since the last read of this register
   uint32_t res2      :15; // bit 17-31: reserved
};

// регистры LOAD и VAL являются регистрами данных, в них нет никаких битов управления

struct CALIB_t {
   uint32_t TENMS :24; // Optionally, holds a reload value to be us ed for 10ms (100Hz) timing, subject to system clock
                       // skew errors. If this field is zero, the calibration value is not known.
   uint32_t res1  :6;  // bit 29-24: reserved 
   bool     SKEW  :1;  // bit 30 Indicates whether the 10ms calibration value is exact
   bool     NOREF :1;  // bit 31 Indicates whether the IMPLEMENTATION DEFINED reference clock is provided
};


// определим структуру переферии с помощью определённых ранее битовых полей вместо uint32_t
struct SysTick_t {
   __IOM CTRL_t   CTRL;  // Offset: 0x000 (R/W)  SysTick Control and Status Register
   __IOM uint32_t LOAD;  // Offset: 0x004 (R/W)  SysTick Reload Value Register
   __IOM uint32_t VAL;   // Offset: 0x008 (R/W)  SysTick Current Value Register
   __IM  CALIB_t  CALIB; // Offset: 0x00C (R/ )  SysTick Calibration Register
   SysTick_t() = delete;  // конструктор по умолчанию удалён намеренно, поскольку
                          // объект этой структуры может быть только по определённому адресу
   // сохраняем этот адрес в статическом поле (чтобы был доступ без объекта)
   static constexpr uint32_t Base = SysTick_BASE; 
};

// создадим фабрику объектов наборов регистров переферии
// (будем использовать сохранённый в самой структуре Base)
// фабрику оформляем в виде структуры, поскольку эту структуру надо делать параметром шаблона
// по аналогии с тем, как было сделано в классе Pin для тестируемости
template<class Periph> struct Pointer {
   static auto get() { return reinterpret_cast<Periph*>(Periph::Base); }
};

// когда работали с пинами, была похожая шаблонная структура, которая предоставляла
// указатель на переферию по адресу.
// Если адрес запоминать в Base, то структуру можно упростить и не передавать адрес в качестве параметра шаблона
// чтобы было единообразно, надо создать подобные структуры под всю переферию

// Разработаем класс работы с регистрами SysTick (реализуем только требуемый функционал)
// сразу определяем шаблоном класс с методом вывода указателя, для тестирумести, см Pin
template<class Pointer = Pointer<SysTick_t>>
struct template_SysTick {
   // как бы перебрасываем перечисление из одной структуры в другую
   using Clock = CTRL_t::Clock;
   
   static void load (uint32_t v)   { Pointer::get()->LOAD = v; }
   static void clear()             { Pointer::get()->VAL = 0; }
   static void setSource (Clock v) { Pointer::get()->CTRL.CLKSOURCE = v; }
   static void enableInterrupt()   { Pointer::get()->CTRL.TICKINT = true; }
   static void enable()            { Pointer::get()->CTRL.ENABLE = true; }
   static uint32_t get()           { return Pointer::get()->VAL; }
};

// избавляемся от неугодного define
#undef SysTick
// определяем псевдоним шаблонного класса с параметром по умолчанию
using SysTick = template_SysTick<>;

// перепишем функцию systemTimerInit под новую структуру (можно избавится от комментариев)
void systemTimerInit()
{
   constexpr uint32_t val = F_CPU/(1000)-1;
   SysTick::load (val);
   SysTick::clear();
   SysTick::setSource (SysTick::Clock::processor);
   SysTick::enableInterrupt();
   SysTick::enable();
}

// в действительности данная структура работать не будет,
// если обращаться с ней, как с битовым полем, потому что компилятор тогда
// может использовать инстукции доступа к байту, а не все регистры это поддерживают,
// многие только к слову или двойному слову.
// Например, если посмотреть в отладчике выполнение программы,
// команды gdb для отображения ассем блера при отладке
// set  disassemble-next-line on
// show disassemble-next-line
// то инструкция, на которую указывает => не выполняется, хотя написана впринципе верно
// и должен установится третий бит CTRL (в r2 на момент этой инструкции записано 4, в r3 - адрес регистра)
// static void enableInterrupt()   { Pointer::get()->CTRL.TICKINT = true; }
//    0x0800023a <systemTimerInit()+10>:	19 78	ldrb	r1, [r3, #0]
//    0x0800023c <systemTimerInit()+12>:	04 32	adds	r2, #4
//    0x0800023e <systemTimerInit()+14>:	0a 43	orrs	r2, r1
// => 0x08000240 <systemTimerInit()+16>:	1a 70	strb	r2, [r3, #0]
//    r2             0x4	
//    r3             0xe000e010

// если поставить специальный флаг компиляции -fno-strict-volatile-bitfields
// то подобные инструкции использоваться не будут
// вот что выдаёт в том же случае компилятор
// static void enableInterrupt()   { Pointer::get()->CTRL.TICKINT = true; }
//    0x08000242 <systemTimerInit()+18>:	02 22	movs	r2, #2
//    0x08000244 <systemTimerInit()+20>:	19 68	ldr	r1, [r3, #0]
//    0x08000246 <systemTimerInit()+22>:	0a 43	orrs	r2, r1
// => 0x08000248 <systemTimerInit()+24>:	1a 60	str	r2, [r3, #0]
//    r2             0x6	
//    r3             0xe000e010

// ещё одно приемущество данного подхода состоит в том, что если набрать в отладчике
// (SysTick_t*)SysTick_t::Base
// можно развернуть структуру и рассмотреть значение каждого бита
// в понятной форме

// в качестве закрепления могу предложить разработать по аналогии структуру для GPIO
// и переписать pin.h используя её до того, как преступить к следующей части

