/**
 * 06lesson программный таймер, как подписчик
 **/

#include "defines.h"
#include "pin.h"
#include "stm32f0xx.h"
#include "list.h"
#include <cstddef>

// переписываем пример 02lesson используя паттерн подписчик
// всё, что касается двунаправленного списка перенёс в файд list.h
// классами из 04lesson воспользоваться не получится, так как нет std::list

namespace {
// эта функция по прежнему нужна
void systemTimerInit()
{
   constexpr uint32_t val = F_CPU/(1000)-1;
   SysTick->LOAD = val;                        // Загрузка значения
   SysTick->VAL  = val;                        // Обнуляем таймеры и флаги 
   SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk  // processor clock
                 | SysTick_CTRL_TICKINT_Msk;   // разрешение прерывания
   SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   // запускает таймер
}

// базовый класс подписчика
// отличается от подписчика из 04lesson тем, что сам будет хранить указатели
// на предыдущий и следующий объект, раелизующий этот базовый класс
struct TickSubscriber : Listable<TickSubscriber> {
   virtual void tick() = 0;
};

// Издатель
// наследуется от List<TickSubscriber>, чтобы запомнить
// указатели на первый и последний объекты
// а так же, чтоб была реализация методов push_back и remove
// наследование приватное, чтобы всё, что унаследованое тоже было приватное
struct TickUpdater : private List<TickSubscriber>
{
   void subscribe(TickSubscriber* p) { push_back(p); }
   void unsubscribe(TickSubscriber* p) { remove(p); }

   // в издателе 04lesson тут был автоцикл, чтобы его реализовать
   // надо знать об итераторах, оставим на потом, а функцию перепишем
   void tick() // { for (auto& subscriber : *this) subscriber.tick(); }
   {
      auto p = this->first;
      while (p) {
         p->tick();
         p = p->next;
      }
   }

   // в конструкторе вызываем инициализацию прерываний каждую мс
   TickUpdater() { systemTimerInit(); }

} tickUpdater; // ну и создаём глобальный объект, поскольку его метод tick будет вызываться в прерывании

extern "C" void SysTick_Handler()
{
   tickUpdater.tick();
}


// изменяем CyclicTask, наследуясь от TickSubscriber
// наследование приватное (для класса это по умолчанию)
template<std::size_t time>
class CyclicTask : TickSubscriber
{
private:
   std::size_t time_count {0};
   // метод теперь override и скрыт от пользовательского кода
   void tick() override { time_count++; }
public:
   // необходимо добавить конструктор с подпиской на издателя
   CyclicTask() { tickUpdater.subscribe(this); }
   void operator ()(void(function)())
   { 
      if (time_count == time)
         function(); // выполняем функцию
      time_count %= time;
   }
};

} // namespace
// всё что обёрнуто в namespace теперь можно убрать в отдельный h файл и лишь заинклюдить его


// переписываем программу. Нет необходимости думать о прерывании
// и можно создавать объекты CyclicTask где угодно
void process1()
{
   using BlueLed  = PC8;
   using GreenLed = PC9;
   BlueLed::clockEnable();
   BlueLed::setAsOut();
   GreenLed::setAsOut();
   
   BlueLed::set();
   GreenLed::clear();

   // нет необходимости теперь создавать объекты в глобальной зоне видимости
   CyclicTask<100> cyclic_task;

   while(1)
   {
      cyclic_task ([](){
         BlueLed::toggle();
         GreenLed::toggle();				
      });

      // делать в этом цикле пока нечего, потому спим до прерывания
      __WFI();
   }		
}

// покажем, как легко теперь изменить программу,
// чтобы было 2 задачи с разной периодичностью
void process2()
{
   using BlueLed  = PC8;
   using GreenLed = PC9;
   BlueLed::clockEnable();
   BlueLed::setAsOut();
   GreenLed::setAsOut();

   // вообще не задумываемся о прерываниях и подписках, всё само
   CyclicTask<200> _200ms;
   CyclicTask<300> _300ms;

   while(1)
   {
      _200ms ([](){ BlueLed::toggle();  });

      _300ms ([](){ GreenLed::toggle(); });

      // делать в этом цикле пока нечего, потому спим до прерывания
      __WFI();
   }		
}


int main(void)
{
   // process1(); // 1004 байт -0s
   process2(); // 1056 байт -0s
}
















