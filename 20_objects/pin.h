#pragma once

#include "defines.h"
#include "GPIO.h"

class Pin {
   Port& port;
   const int n;
public:
   Pin (Port::N p, int n, PinMode mode)
      : port {Port::make(p)}
      , n    {n}
   {
      port.init(n,mode);
   }
   inline void toggle() { port.toggle(n); }
   inline void set()    { port.set(n); }
   inline void clear()  { port.clear(n); }
   inline bool isSet()  { return port.isSet(n); }
};