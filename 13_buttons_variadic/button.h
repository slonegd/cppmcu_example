#pragma once

#include "timers.h"
#include "pin.h"
#include <tuple>
#include <type_traits>

// обычно кнопок несколько
// можно создавать несколько объектов класса Button и нормально работать
// но тогда каждая кнопка будет занимать sizeof(Button)
// а переменные состояний у них несут абсолютно одинаковые функции
// происходит дублирование данных.
// Второй отрицательный момент это подписка на прерывание каждую мс
// Как известно, в прерывании следует находиться как можно меньше времени
// чтобы не пропустить другие важные прерывания, а в случае 10 кнопок
// будет создано 10 подписчиков, и в прерывании у каждого будет вызываться notify()
// С++ позволяет передавать переменное число параметров шаблона (variadic templates)
namespace old {

template<class Pin>
class Button : Subscriber
{
   int time  {0};
   bool done {false};
   void notify() override
   {
      if (Pin::isSet()) {
         time++;
      } else {
         time = 0;
         done = false;
      }
   }

public:
   Button() { Pin::clockEnable(); tickUpdater.subscribe (this); }
   operator bool()
   {
      auto tmp { time >= 10 and not done };
      if (tmp)
         done = true;
      return tmp;
   }
};

} // namespace old {


// синтаксис переменного числа параметров шаблона следующий
// изменим имя на Buttons
template<class ... Pins>
class Buttons : Subscriber
{
   int  time {0};
   bool done {false};
   void notify() override
   {
      // для работы с переменным числом параметров в с++17 появились выражения свёртки (fold expressions)
      // очевидно, что условие должно быть записано так: если хотя бы одна кнопка нажата
      // схемотично можно описать это так Pin1::isSet() or Pin2::isSet() or Pin3::isSet() or ...
      // выражение свёртки для этого следующее (Pins::isSet() or ...) (скобки вокруг обязательны)
      // это автоматически заменяется компилятором на выражение выше по числу параметров
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
      // вызов метода всех пинов из списка в выражении свёртки (оператор ,)
      (Pins::clockEnable(), ...);
      tickUpdater.subscribe (this);
   }

   // а вот тут всё сложнее, оставим этот метод для первого из списка параметров шаблона
   // это позволит не переписывать уже написаный тест
   // необходимо вывести первый класс параметр из списка
   // далее каждую строку опишу отдельно, но читать надо с конца (нужно заинклудить <tuple> и <type_traits>)
   using First = std::remove_reference_t < // std::get возвращает ссылку на элемент, поэтому убрать ссылку нужно
      decltype (                  // выводится тип выражения (структура на самом деле не создаётся, get не выполняется!)
         std::get<0> (            // берётся первый элемент этой структуры
            std::tuple<Pins...>{} // создаётся пустой tuple (кортеж) - структура, поля которой безымянны
         )
      )
   >;
   operator bool()
   {
      return isPush(); // используем метод, что описан дальше
   }

   // но ещё нужны методы для каждой кнопки из списка (шаблонный метод)
   template<class Pin = First> // пин по умолчанию First
   bool isPush()
   {
      // необходимо добавить проверку на конкретный пин
      auto tmp { time >= 10 and not done and Pin::isSet() };
      if (tmp)
         done = true;
      return tmp;
   }
};

// так же напишем тесты для двух кнопок в файле test_button.cpp