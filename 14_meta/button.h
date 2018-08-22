#pragma once

#include "timers.h"
#include "pin.h"
#include "meta.h"

// определение первого типа в вариадике несколько грамоздко и плохочитаемо в прошлой вариации
// в метод isPush можно передать любой Pin, нет проверки на вхождение
// Pin-а в список вариадика, а это явно ошибка, нужно выявить static_assert
// Чтобы сделать так, как описано выше, необходимо написать несколько мета функций
// о том, как их написать см. meta.h
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
   // метафункция first_t определяет первый тип из вариадика Pins...
   template<class Pin = first_t<Pins...>>
   bool isPush()
   {
      static_assert (
         is_inlist_v<Pin, Pins...>, // метафункция определяет, входит ли Pin в список Pins
         "Pin is not in list of Pins"
      );
      auto tmp { time >= 10 and not done and Pin::isSet() };
      if (tmp)
         done = true;
      return tmp;
   }
};
