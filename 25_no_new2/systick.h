#pragma once

#include "registr.h"
#include "defines.h"

struct CTRL_t {
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


struct SysTick_t {
   __IOM CTRL_t   CTRL;  // Offset: 0x000 (R/W)  SysTick Control and Status Register
   __IOM uint32_t LOAD;  // Offset: 0x004 (R/W)  SysTick Reload Value Register
   __IOM uint32_t VAL;   // Offset: 0x008 (R/W)  SysTick Current Value Register
   __IM  CALIB_t  CALIB; // Offset: 0x00C (R/ )  SysTick Calibration Register
   SysTick_t() = delete;
   static constexpr uint32_t Base = SysTick_BASE; 
};


template<class Pointer = Pointer<SysTick_t>>
struct template_SysTick {
   using Clock = CTRL_t::Clock;
   
   static void load (uint32_t v)   { Pointer::get()->LOAD = v; }
   static void clear()             { Pointer::get()->VAL = 0; }
   static void setSource (Clock v) { Pointer::get()->CTRL.CLKSOURCE = v; }
   static void enableInterrupt()   { Pointer::get()->CTRL.TICKINT = true; }
   static void enable()            { Pointer::get()->CTRL.ENABLE = true; }
   static uint32_t get()           { return Pointer::get()->VAL; }
};

#undef SysTick
using SysTick = template_SysTick<>;

void systemTimerInit()
{
   constexpr uint32_t val = F_CPU/(1000)-1;
   SysTick::load (val);
   SysTick::clear();
   SysTick::setSource (SysTick::Clock::processor);
   SysTick::enableInterrupt();
   SysTick::enable();
}
