#pragma once

// понадобится таймер, поскольку в задаче говорится об отсчёте времени
#include "timers.h"
#include "pin.h"

template<class Pin>
class Button : Subscriber
{
   // для отсчитывания 10 мс
   int time {0};
   // для выявления того, что кнопку уже опросили
   bool done {false};
   // метод, необходимый для Subscriber
   void notify()
   {
      if (Pin::isSet()) {
         time++;
      } else {
         // сброс всего, когда не нажата
         time = 0;
         done = false;
      }
   }
public:
   // конструктор должен только включить тактирование, поскольку
   // пин по умолчанию настроен, как на вход;
   // и не забыть подписаться на издателя
   Button() { Pin::clockEnable(); tickUpdater.subscribe (this); }

   operator bool()
   {
      if (time >= 10 and not done) {
         done = true; // чтобы вернуть true только 1 раз
         return true;
      }
      return false;
   }
};

// Как видно по тесту, класс спроектирован верно в соотвествии с ТЗ