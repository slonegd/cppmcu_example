#pragma once

#include "defines.h"
#include "GPIO.h" // включаем новую абстракцию

// теперь пин принимает, не адрес порта, а сам порт
// практически все методы повторяют методы порта с пробросом информации о номере пина
template <class Port, int n>
struct Pin {
   // новый обобщённый метод инициализации вместо setAsOut
   // конфигурация никогда не изменяется динамически во время работы,
   // поэтому этот параметр (режим работы) - шаблонный
   // слово template перед вызовом метода обязательно,
   // без него ошибка компиляции
   template<PinMode c>
   static void init()        { Port::template init<n,c>(); }

   // static void clockEnable() { Port::clockEnable(); } // больше не нужен, тактирование включается в init
   static void set()         { Port::template set<n>(); }
   static void clear()       { Port::template clear<n>(); }
   static bool isSet()       { return Port::template isSet<n>(); }
   static void toggle()      { isSet() ? clear() : set(); }
};

using PA0 = Pin<PA,0>;
using PC8 = Pin<PC, 8>;
using PC9 = Pin<PC, 9>;

// Можно заметить что данный шаблон класса совсем не зависит от типа микроконтроллера
// сделана полная абстракция от железа
// ничто не мешает определить его и для любого другого микроконтроллера (Atmega8 к примеру)
// таким образом достигается единообразие работы с пинами в main в разных программах