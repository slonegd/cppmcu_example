#include "timers.h"
#include <iostream>

// TDD - принцип работы через тестирование
// сначала пишутся тесты на требуемый функционал
// тесты - некое ТЗ, которое необходимо выполнить
// в качестве примера далее напишем тесты на класс кнопки
// Кнопка должна параметризироваться Pin-ом
// В качестве публичного метода необходимо выполнить лишь метод
// приведения к bool, который должен возвращать
// true, если Pin нажат не менее 10 мс (от дребезга)
// true должно возвращаться лишь один раз при одном нажатии

// напишем прототип класса, который удолетворяет этим требованиям
// поскольку Pin у нас чисто статический класс, объекты которого не создаются
// его необходимо передавать в качестве параметра шаблона
template<class Pin>
class Button
{
public:
   operator bool() { return false; } // пока без реализации
};

// нужен мок класс пина, чтобы подсунуть тестируемому классу
struct MockPin
{
   // будем менять состояние Pinа с помощтю переменной
   static bool pressed;
   // по логике классу Button необходимы только эти методы класса Pin
   static bool isSet()   { return pressed;     }
   static bool isClear() { return not pressed; }
};
bool MockPin::pressed {false};

int main()
{
   Button<MockPin> button;
   // далее все тестовые случаи будем логически перемножать с этой переменной, поэтому
   // она инициализируется true
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

