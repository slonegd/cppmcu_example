#include "button.h"
#include <iostream>

// нужен мок класс пина, чтобы подсунуть тестируемому классу
struct MockPin
{
   // будем менять состояние Pinа с помощтю переменной
   static bool pressed;
   // по логике классу Button необходимы только эти методы класса Pin
   static bool clockEnable() { }
   static bool isSet()   { return pressed;     }
   static bool isClear() { return not pressed; }
};
bool MockPin::pressed {false};

int main()
{
   Button<MockPin> button;
   bool good_test {true};
   good_test &= not button;
   MockPin::pressed = true;
   auto n {10};
   while (n--) {
      good_test &= not button;
      tickUpdater.notify();
   }
   // прошло 10 мс
   good_test &= button;
   // возвращает true лишь раз
   good_test &= not button;
   // отпустили кнопку
   MockPin::pressed = false;
   tickUpdater.notify();
   good_test &= not button;

   // и снова нажали, но на 5 мс
   MockPin::pressed = true;
   n = 5;
   while (n--) {
      good_test &= not button;
      tickUpdater.notify();
   }
   MockPin::pressed = false;
   tickUpdater.notify();
   good_test &= not button;

   // повтор теста на время большее 10 мс
   MockPin::pressed = true;
   n = 10;
   while (n--) {
      good_test &= not button;
      tickUpdater.notify();
   }
   // добавим ещё 10 мс, но опроса кнопки не делаем
   n = 10;
   while (n--)
      tickUpdater.notify();
   // первый опрос кнопки после 20 мс
   good_test &= button;
   good_test &= not button;


   std::cout << '\n' << "Тесты класса Button:" << std::endl;

   // \033[32mТЕКСТ\033[0m - для цвета
   std::cout << "Button::operator bool() " <<  (good_test ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << std::endl;
}

