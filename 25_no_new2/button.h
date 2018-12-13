#pragma once

#include "timers.h"
#include "pin.h"
#include "meta.h"

template<class ... Pins>
class Buttons : Subscriber
{
   int  time {0};
   bool done {false};
   void notify() override
   {
      if ((Pins::isSet() or ...)) {
         time++;
      } else {
         time = 0;
         done = false;
      }
   }

public:
   Buttons()
   {
      (Pins::clockEnable(), ...);
      tickUpdater.subscribe (this);
   }

   operator bool()
   {
      return isPush();
   }

   template<class Pin = meta::first_t<Pins...>>
   bool isPush()
   {
      static_assert (
         meta::is_inlist_v<Pin, Pins...>,
         "Pin is not in list of Pins"
      );
      auto tmp { time >= 10 and not done and Pin::isSet() };
      if (tmp)
         done = true;
      return tmp;
   }
};
