#include "timers.h"
#include <iostream>

int main()
{
   bool good_test {false};
   bool bad_test  {false};
   CyclicTask<5> test_task;
   test_task ( [&](){ bad_test = true; } );
   auto n {5};
   while (n--) {
      test_task ( [&](){ bad_test = true; } );
      tickUpdater.notify();
   }
   test_task ( [&](){ good_test = true; } );
   test_task ( [&](){ bad_test = true; } );
   tickUpdater.notify();
   test_task ( [&](){ bad_test = true; } );

   std::cout << '\n' << "Тесты класса Cyclic_task:" << std::endl;

   // \033[32mТЕКСТ\033[0m - для цвета
   std::cout << (good_test and not bad_test ? "Cyclic_task::operator() \033[32mпрошёл\033[0m" : "Cyclic_task::operator() \033[31mпровален\033[0m") << std::endl;
   std::cout << std::endl;
}


// некоторое пояснение по лямбдам с захватом
// когда указывается знак [&] - это означает, что внутри лямбды
// доступны все, видимые в месте создания лямбды, переменные по ссылке,
// то есть измененные внутри лямбды, именятся и вне
// компилятор посмотрит, какие переменные используются и создаст следующий класс
// и выполнит конструктор в месте создания лямбды
// к примеру на строке 9
struct lambda
{
   bool& bad_test;                                 // захваченный по ссылке объект
   lambda (bool& bad_test) : bad_test(bad_test) {} // конструктор объекта
   void operator() () { bad_test == true; }        // функция, которая выполнится
};
// согласитесь, что запись [&](){ bad_test = true; } короче