/**
 * 05 паттерн наблюдатель
 * данный патерн идеален 
 * когда один источник некого события, при его наступлении
 * оповещает все объекты, которые на событие подписаны
 * В контексте CyclicTask метод tick - реакция на событие
 * конкретного объекта. Генерирует же событие прерывание.
 */

#include <iostream>
#include <list>

// интерфейс подписчика, именно этот переопределённый метод подписчика
// вызывается, когда в издателе произойдёт событие
struct Isubscriber {
   virtual void update() = 0;
};

// базовый класс издателя. Объект этого класса генерирует событие
// любой объект класса, который наследуется от базового
// при наступлении события, вызывает свой метод update, который
// в свою очередь вызовет метод update у всех подписчиков из списка
struct Publisher {
   // издатель должен где-то хранить список всех подписчиков
   std::list<Isubscriber*> subs;
   // метод подписки на события (принимает ссылку на подписчика,
   // поэтому необходимо применять оператор взятия адреса, чтобы получить указатель)
   void subscribe(Isubscriber& s) { subs.push_back(&s); }
   // метод отписки от события
   void unsubscribe(Isubscriber& s) { subs.remove(&s); }
   // проход по всем подписчикам с вызовом update
   void update() { for (auto s : subs) s->update(); }
};


// далее пример использования этих двух классов
// для простоты сделаем в качестве события ввод строки в консоли
// издатель должен наследоваться от базового Publisher
// наследование работает так, что в новом классе
// уже будут все поля и методы базового класса
// то есть, тут уже есть список подписчиков и методы
// subscribe, unsubscribe, update
struct Controller : Publisher
{
   // добавляю метод получения строки
   void run()
   {
      std::string line;
      while (std::getline(std::cin, line)) {
         update();        // оповещение о событии
         if (line == "q") // случай для прекращения выполнения метода
            break;
      }
   }
};

// далее создаём трёх подписчиков, которые отличаються друг от друга
// тривиально, только одной цифрой в названии типа
// нужно понимать, что здесь при описании метода update
// разные классы могут делать абсолютно разные вещи
struct Subs1 : Isubscriber
{
   // контруктор подписчика принимает в качестве параметра, к какому
   // издателю подписаться. Издатель в методе subscribe в качестве
   // параметра принимает ссылку, поэтому необходимо указатель
   // на самого себя this разименовать 
   Subs1 (Publisher& publisher) { publisher.subscribe (*this); }
   // переопределяем виртуальный метод интерфейса
   // __PRETTY_FUNCTION__ просто выводит в консоль имя функции (удобно для изучения)
   void update() override { std::cout << __PRETTY_FUNCTION__ << std::endl; }
};

struct Subs2 : Isubscriber
{
   Subs2 (Publisher& publisher) { publisher.subscribe (*this); }
   void update() override { std::cout << __PRETTY_FUNCTION__ << std::endl; }
};

struct Subs3 : Isubscriber
{
   Subs3 (Publisher& publisher) { publisher.subscribe (*this); }
   void update() override { std::cout << __PRETTY_FUNCTION__ << std::endl; }
};



// как наблюдатель работает в самом коде
int main()
{
   // создание контроллера, который является издателем
   Controller controller;
   // создание трёх разных подписчиков с автоматической подпиской в конструкторе
   Subs1 subs1 {controller};
   Subs2 subs2 {controller};
   Subs3 subs3 {controller};
   // запуск контроллера
   // можно вводить любые строки, кроме q
   // при этом в консоль должно вывестись
   // virtual void Subs1::update()
   // virtual void Subs2::update()
   // virtual void Subs3::update()
   controller.run();
   // чтобы перейти сюда надо в консоли набрать q
   // отписываем одного из подписчиков
   controller.unsubscribe (subs2);
   // здесь при вводе любой строки, кроме q
   // в консоль должно вывестись
   // virtual void Subs1::update()
   // virtual void Subs3::update()
   controller.run();
   // чтобы выйти из программы набрать q
}
