#include "timers.h"


// CyclicTask написан так, что нельзя передать лямбду с захватом
// поэтому создадим несколько глобальных сущностей
bool good_test {true};
// если выполняется эта функция, тест провален
void bad()  { good_test = false; }
// если не выполняется эта функция, тест провален
void good() { good_test = true; }

int main()
{
   CyclicTask<10> test_task;
   
   test_task ([](){ bad(); });
   tickUpdater.notify();
}