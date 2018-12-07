#include "button.h"
#include <iostream>

struct MockPin
{
   static bool pressed;
   static bool clockEnable() { }
   static bool isSet()   { return pressed;     }
   static bool isClear() { return not pressed; }
};
bool MockPin::pressed {false};

// добавим вторую мок структуру
struct MockPin2
{
   static bool pressed;
   static bool clockEnable() { }
   static bool isSet()   { return pressed;     }
   static bool isClear() { return not pressed; }
};
bool MockPin2::pressed {false};

int main()
{
   Buttons<MockPin> button;
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


   std::cout << '\n' << "Тесты класса Buttons:" << std::endl;

   std::cout << "Buttons::operator bool() " <<  (good_test ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;

   // тесты для двух кнопок (метод isPush)
   good_test = true;
   Buttons<MockPin, MockPin2> buttons;
   good_test &= not buttons.isPush<MockPin>();
   MockPin::pressed = true;
   n = 10;
   while (n--) {
      good_test &= not buttons.isPush<MockPin>();
      good_test &= not buttons.isPush<MockPin2>();
      tickUpdater.notify();
   }
   // прошло 10 мс
   good_test &= not buttons.isPush<MockPin2>();
   // сработала только первая кнопка
   good_test &= buttons.isPush<MockPin>();
   good_test &= not buttons.isPush<MockPin>();
   MockPin ::pressed = false;
   tickUpdater.notify();
   good_test &= not buttons.isPush<MockPin>();
   good_test &= not buttons.isPush<MockPin2>();

   // тоже самое для второй
   MockPin2::pressed = true;
   n = 10;
   while (n--) {
      good_test &= not buttons.isPush<MockPin>();
      good_test &= not buttons.isPush<MockPin2>();
      tickUpdater.notify();
   }
   // прошло 10 мс
   good_test &= buttons.isPush<MockPin2>();
   good_test &= not buttons.isPush<MockPin>();
   good_test &= not buttons.isPush<MockPin2>();

   std::cout << "Buttons::isPush()        " <<  (good_test ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   std::cout << std::endl;
}

