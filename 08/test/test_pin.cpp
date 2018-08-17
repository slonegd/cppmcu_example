#include "pin.h"
#include <iostream>
#include <type_traits>

// для того, чтобы протестировать методы класса, надо создать mock-class,
// главная обязанность которого предоставить метод port()
struct MockPointer
{
   static decltype(GPIOA) p;
   static auto port() { return p; }
};

// создаём Mock структуру (для remove_pointer нужен заголовочник type_traits)
std::remove_pointer_t<decltype(GPIOA)> mock;

// указываем в статическом поле мок-класса созданный мок-объект
decltype(GPIOA) MockPointer::p = &mock;

// создаём псевдоним тестируемого типа с мок-классом (адрес не важен)
using T5 = Pin<0, 5, MockPointer>;

// тесты возвращают true, если прошли
bool test_setAsOut()
{
   mock.MODER = 0;
   T5::setAsOut();
   return mock.MODER == (1 << (5*2)); 
}

bool test_set()
{
   mock.BSRR = 0;
   T5::set();
   return mock.BSRR == (1 << 5); 
}

bool test_clear()
{
   mock.BSRR = 0;
   T5::clear();
   return mock.BSRR == (1 << (5+16)); 
}

bool test_isSet1()
{
   mock.IDR = 0;
   return not T5::isSet();
}

bool test_isSet2()
{
   mock.IDR = (1 << 5);
   return T5::isSet();
}

bool test_toogle1()
{
   mock.BSRR = 0;
   mock.IDR = 0;
   T5::toggle();
   return mock.BSRR == (1 << 5);
}

bool test_toogle2()
{
   mock.BSRR = 0;
   mock.IDR = 1 << 5;
   T5::toggle();
   return mock.BSRR == (1 << (5+16));
}

// добавил специально провальный тест
bool test_bad()
{
   mock.BSRR = 0;
   T5::set();
   return (mock.BSRR == (1 << 6)); 
}


int main()
{
   std::cout << '\n' << "Тесты класса Pin:" << std::endl;

   // \033[32mТЕКСТ\033[0m - для цвета
   std::cout << (test_setAsOut() ? "Pin::setAsOut \033[32mпрошёл\033[0m" : "Pin::setAsOut \033[31mпровален\033[0m") << std::endl;
   std::cout << (test_set()      ? "Pin::set      \033[32mпрошёл\033[0m" : "Pin::set      \033[31mпровален\033[0m") << std::endl;
   std::cout << (test_clear()    ? "Pin::clear    \033[32mпрошёл\033[0m" : "Pin::clear    \033[31mпровален\033[0m") << std::endl;
   std::cout << (test_isSet1()   ? "Pin::isSet1   \033[32mпрошёл\033[0m" : "Pin::isSet1   \033[31mпровален\033[0m") << std::endl;
   std::cout << (test_isSet2()   ? "Pin::isSet2   \033[32mпрошёл\033[0m" : "Pin::isSet2   \033[31mпровален\033[0m") << std::endl;
   std::cout << (test_toogle1()  ? "Pin::toogle1  \033[32mпрошёл\033[0m" : "Pin::toogle1  \033[31mпровален\033[0m") << std::endl;
   std::cout << (test_toogle2()  ? "Pin::toogle2  \033[32mпрошёл\033[0m" : "Pin::toogle2  \033[31mпровален\033[0m") << std::endl;
   std::cout << (test_bad()      ? "Pin::bad      \033[32mпрошёл\033[0m" : "Pin::bad      \033[31mпровален\033[0m") << std::endl;

}