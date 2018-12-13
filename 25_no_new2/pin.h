#pragma once

#include "defines.h"
#include "GPIO.h"
#if not defined(TEST)
#include "no_mutex.h"
#endif


class Pin {
   Port& port;  // класс не шаблонный
   const int n;
public:
   // пришлось написать этот конструктор, поскольку direct-list-initialization
   // недоступен для приватных полей
   Pin (Port& port, int n) : port{port}, n{n} {}

   // фабрика принимает в качестве параметра шаблона всё, что нужно для инициализации
   // инициализирует и возвращает уже проиницализированный параметрами шаблона объект
   template<Periph p, int n, PinMode mode> static auto& make()
   {
      // создаём объект
      static Pin pin { Port::make<p>(), n };
      // инициализируем нужными параметрами
      pin.port.init<n,mode>();
      // возвращаем "забыв" о параметрах шаблона
      return pin;
   }
   // объекты позволяют использовать определения операторов
   bool operator= (bool v) { v ? set() : clear(); return v; }
   operator bool() { return isSet(); }

   void toggle() { port.toggle(n); }
   void set()    { port.set(n); }
   void clear()  { port.clear(n); }
   bool isSet()  { return port.isSet(n); }

};