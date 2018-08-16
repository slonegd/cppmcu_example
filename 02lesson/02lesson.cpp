/**
 * 02lesson программный таймер
 **/

#include "defines.h"
#include "pin.h"
#include "stm32f0xx.h"
#include <cstddef>

// необходима функция инициализации системного таймера для перывания каждую мс
// пишем на CMSIS регистрах (об абстракции над регистрами ещё не говорили)
void systemTimerInit()
{
    constexpr uint32_t val = F_CPU/(1000)-1;
    SysTick->LOAD = val;                        // Загрузка значения
    SysTick->VAL = val;                         // Обнуляем таймеры и флаги 
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk  //processor clock
                  | SysTick_CTRL_TICKINT_Msk;   //разрешение прерывания
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   //запускает таймер
}


/// эта итерация взята с 01lesson::iter8
/// структуру пина выделим в отдельный файл, чтоб она тут не мешала
/// так же отдельно выделим файл define.h, где укажем частоту и другие параметры
/// Очевидно, что проблеммной частью является задержка
/// 1. Она блокирует выполнение программы
/// 2. Она зависит от частоты контроллера
/// 3. Необходим комментарий для понимания
namespace iter01 {

void process()
{
   using BlueLed  = PC8;
   using GreenLed = PC9;
   BlueLed::clockEnable();
   BlueLed::setAsOut();
   GreenLed::setAsOut();

   while(1)
   {
      BlueLed::set();
      GreenLed::clear();
      for (volatile int i=0; i<500000; i++){}	// Искусственная задержка
               
      BlueLed::clear();
      GreenLed::set();
      for (volatile int i=0; i<500000; i++){}	// Искусственная задержка
   }		
}

} // namespace iter01 {



/// поскольку задача не сделать задержку, а реализовать циклическую задачу,
/// которая переключала бы светодиоды, то назовём класс CyclicTask
namespace iter02 {

// не предполагается изменения периодичности задачи во время выполнения,
// потому время устанавливаем в качестве параметра шаблона
template<std::size_t time>
class CyclicTask {
   // поле приватное, поскольку счёт не должен зависеть от внешней программы
   // сразу инициируем переменную нулём, считать будем до time
   std::size_t time_count {0};
public:
   // данная функция должна вызываться в прерывании системного таймера
   // потому её нельзя сделать приватной, что является нарушением инкапсуляции
   // - сокрытия реализации от пользовательского кода
   // решение есть (полиморфизм, наблюдатель), будет рассмотрено далее 
   void tick()  { time_count++; }
   bool event() { auto tmp = time_count == time; time_count %= time; return tmp; }
};

// создавать объект необходимо в глобальной зоне видимости, поскольку
// метод tick необходимо использовать в прерывании системного таймера
CyclicTask<100> cyclic_task;

void process()
{
   using BlueLed  = PC8;
   using GreenLed = PC9;
   BlueLed::clockEnable();
   BlueLed::setAsOut();
   GreenLed::setAsOut();
   
   BlueLed::set();
   GreenLed::clear();

   systemTimerInit();

   while(1)
   {
      // методы toggle добавил в структуру Pin
      if (cyclic_task.event()) {
         BlueLed::toggle();
         GreenLed::toggle();
      }

      // делать в этом цикле пока нечего, потому спим до прерывания
      __WFI();
   }		
}

} // namespace iter02 {


// класс CyclicTask имеет всего один интерфейсный метод 
// (доступный для пользовательского кода),
// который возвращает bool
// можно заставить работать объекты этого класса как логические переменные
// переопределив оператор приведения к bool
namespace iter03 {

template<std::size_t time>
class CyclicTask {
   std::size_t time_count {0};
public:
   void tick()  { time_count++; }
   // переопределение оператора, просто заменили название функции, остальное то же
   operator bool() { auto tmp = time_count == time; time_count %= time; return tmp; }
};

CyclicTask<100> cyclic_task;

void process()
{
   using BlueLed  = PC8;
   using GreenLed = PC9;
   BlueLed::clockEnable();
   BlueLed::setAsOut();
   GreenLed::setAsOut();
   
   BlueLed::set();
   GreenLed::clear();

   systemTimerInit();

   while(1)
   {
      // теперь cyclic_task можно использовать как bool переменную
      if (cyclic_task) {
         BlueLed::toggle();
         GreenLed::toggle();
      }

      // делать в этом цикле пока нечего, потому спим до прерывания
      __WFI();
   }		
}

} // namespace iter03 {

// приведение к bool объектов класса CyclicTask как-то неверно ("если задача?")
// я привёл это только в качестве примера
// логичнее переопределить оператор функционального объекта
// т.е. создаётся задача и функция, выполняемая этой задачей
namespace iter04 {

template<std::size_t time>
class CyclicTask {
   std::size_t time_count {0};
public:
   void tick()  { time_count++; }
   // переопределение оператора (), принимаем функцию на выполнение
   void operator() (void(function)())
   { 
      if (time_count == time)
         function(); // выполняем функцию
      time_count %= time;
   }
};

CyclicTask<100> cyclic_task;
// алиасинг пришлось перенести в глобальную зону видимости, чтобы можно было
// объявить функцию
using BlueLed  = PC8;
using GreenLed = PC9;

void toggleLeds()
{
   BlueLed::toggle();
   GreenLed::toggle();	
}

void process()
{

   BlueLed::clockEnable();
   BlueLed::setAsOut();
   GreenLed::setAsOut();
   
   BlueLed::set();
   GreenLed::clear();

   systemTimerInit();

   while(1)
   {
      // теперь cyclic_task - функциональный объект, который принимает другую функцию
      cyclic_task (toggleLeds);

      // делать в этом цикле пока нечего, потому спим до прерывания
      __WFI();
   }		
}

} // namespace iter04 {


// нет необходимости объявлять функцию в глобальной зоне видимости
// для таких функций в с++ есть лямбды
// переписываем пример на лямбде
namespace iter05 {

template<std::size_t time>
class CyclicTask {
   std::size_t time_count {0};
public:
   void tick()  { time_count++; }
   void operator ()(void(function)())
   { 
      if (time_count == time)
         function(); // выполняем функцию
      time_count %= time;
   }
};

CyclicTask<100> cyclic_task;

void process()
{
   using BlueLed  = PC8;
   using GreenLed = PC9;
   BlueLed::clockEnable();
   BlueLed::setAsOut();
   GreenLed::setAsOut();
   
   BlueLed::set();
   GreenLed::clear();

   systemTimerInit();

   while(1)
   {
      // теперь cyclic_task - функциональный объект, который принимает безымянную
      // функцию - лямбду
      cyclic_task ([](){
         BlueLed::toggle();
         GreenLed::toggle();				
      });

      // делать в этом цикле пока нечего, потому спим до прерывания
      __WFI();
   }		
}

} // namespace iter05 {


// функцию можно передать в конструкторе
namespace iter06 {

template<std::size_t time>
class CyclicTask {
   std::size_t time_count {0};
   void(*function)(); // инициируем её в конструкторе
public:
   CyclicTask (void(function)()) : function(function) {}
   // теперь функцию можно вызывать автоматически прямо в tick
   void tick()
   {
      if (not (++time_count %= time))
         function(); // выполняем функцию
   }
};

using BlueLed  = PC8;
using GreenLed = PC9;
CyclicTask<100> cyclic_task ([](){
   BlueLed::toggle();
   GreenLed::toggle();			
});

void process()
{
   BlueLed::clockEnable();
   BlueLed::setAsOut();
   GreenLed::setAsOut();
   
   BlueLed::set();
   GreenLed::clear();

   systemTimerInit();

   while(1)
   {
      // ничего связанного с cyclic_task вызывать не надо
      // делать в этом цикле пока нечего, потому спим до прерывания
      __WFI();
   }		
}

} // namespace iter06 {





// само прерывание, прототип функции объявлен в startup файле
// для линковки с си-кодом используем extern "C"
// линковка с си нкобходима, так как эта функция объявлена
// в startup файле как си-функция
extern "C" void SysTick_Handler()
{
   // iter02::cyclic_task.tick();
   // iter03::cyclic_task.tick();
   // iter04::cyclic_task.tick();
   // iter05::cyclic_task.tick();
   iter06::cyclic_task.tick();
}


int main(void)
{
   // iter01::process(); // 568 байт
   // iter02::process(); // 916 байт
   // не смотря на то, что код стал больше, фактически программа
   // выполняется очень быстро (прерывание, инкремент, проверка на event, переключить светодиоды)
   // всё остальное время просто ждём следующего прерывания
   // при такой архитектуре можно легко добиться многозадачности
   // например светодиоды могут моргать с разной частотой
   // для этого создаётся 2 задачи, и в каждой переключается свой светодиод
   // iter03::process(); // 916 байт
   // iter04::process(); // 920 байт
   // iter05::process(); // 904 байт - не ссылаемся на глобальный объект-функцию, выигрываем в коде
   iter06::process(); // 928 байт - необходимость хранить указатель на функцию внутри класса

   // как писал выше у класса CyclicTask есть следующие недостатки
   // 1. недостаточная инкапусляция (метод tick открытый)
   // 2. необходимо следить, чтобы метод tick обязательно вызывался в прерывании
   // 3. так же необходимо не забыть инициировать системный таймер на прерывания
   // 4. необходимо создавать объекты только в глобальной зоне видимости
   //    из-за того, что tick надо вызывать в прерывании
   // данные недостатки решаются с помощью паттерна наблюдатель
   // но чтобы его понять, необходимо сначала изучить полиморфизм
   // и двунаправленный список

}
















