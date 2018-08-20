#include "timers.h"
#include <iostream>

// CyclicTask написан так, что нельзя передать лямбду с захватом
// только функцию типа void(), к которой лямбда без захвата неявно приводится
// поэтому создадим несколько глобальных сущностей
bool good_test {false};
bool bad_test  {false};
// если выполняется эта функция, тест провален
void bad()  { bad_test = true; }
// если не выполняется эта функция, тест провален
void good() { good_test = true; }

int main()
{
   CyclicTask<5> test_task;
   test_task ( bad );
   auto n {5};
   while (n--) {
      test_task ( bad );
      tickUpdater.notify();
   }
   test_task ( good );
   test_task ( bad );
   tickUpdater.notify();
   test_task ( bad );

   std::cout << '\n' << "Тесты класса Cyclic_task:" << std::endl;

   // \033[32mТЕКСТ\033[0m - для цвета
   std::cout << (good_test and not bad_test ? "Cyclic_task::operator() \033[32mпрошёл\033[0m" : "Cyclic_task::operator() \033[31mпровален\033[0m") << std::endl;
   std::cout << std::endl;
}