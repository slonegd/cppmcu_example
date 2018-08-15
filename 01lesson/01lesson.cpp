/**
 * 01lesson работа с портами ввода/вывода
 **/
#define STM32F030x6

#include "stm32f0xx.h"


/// взято отсюда
/// https://habr.com/post/406839/
/// недостатки:
/// 1. магические числа
/// 2. только по комментариям можно понять, что делает код
/// 3. не работает при отличной от -O0 оптимизации
namespace iter1 {

void process()
{
   /* Включаем тактирование на порту GPIO */
   RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
   
   /* Настраиваем режим работы портов PC8 и PC9 в Output*/
   GPIOC ->MODER = 0x50000;
   
   /* Настраиваем Output type в режим Push-Pull */
   GPIOC->OTYPER = 0;
   
   /* Настраиваем скорость работы порта в Low */
   GPIOC->OSPEEDR = 0;
   
   while(1)
   {
      /* Зажигаем светодиод PC8, гасим PC9 */
      GPIOC->ODR = 0x100;
      for (int i=0; i<500000; i++){}	// Искусственная задержка
               
      /* Зажигаем светодиод PC9, гасим PC8 */
      GPIOC->ODR = 0x200;
      for (int i=0; i<500000; i++){}	// Искусственная задержка
   }		
}

} // namespace iter1 {


///////////////////////////////////////////////////////////////////////////////
//
// c-style
//
///////////////////////////////////////////////////////////////////////////////

/// избавляемся от магических чисел средствами CMSIS
/// заставляем работать при любой оптимизации (volatile)
/// запись/сброс пина через регистр атомарного доступа (GPIOC->BSRR)
namespace iter2 {

void process()
{
   /* Включаем тактирование на порту GPIO */
   RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
   
   /* Настраиваем режим работы портов PC8 и PC9 в Output*/
   GPIOC->MODER = GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0; 
   

   while(1)
   {
      /* Зажигаем светодиод PC8, гасим PC9 */
      GPIOC->BSRR = 1 << 8 | 1 << (9 + 16);
      for (volatile int i=0; i<500000; i++){}	// Искусственная задержка
               
      /* Зажигаем светодиод PC9, гасим PC8 */
      GPIOC->BSRR = 1 << 9 | 1 << (8 + 16);
      for (volatile int i=0; i<500000; i++){}	// Искусственная задержка
   }		
}

} // namespace iter2 {


/// избавляемся от комменатриев путём перехода к функциям с говорящими названиями
namespace iter3 {

void pcClockEnable()     { RCC->AHBENR |= RCC_AHBENR_GPIOCEN; }
void pc8pc9SetAsOutput() { GPIOC->MODER = GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0; }
void pc8Set_pc9clear()   { GPIOC->BSRR = 1 << 8 | 1 << (9 + 16); }
void pc9Set_pc8clear()   { GPIOC->BSRR = 1 << 9 | 1 << (8 + 16); }

void process()
{
   pcClockEnable();
   pc8pc9SetAsOutput(); 

   while(1)
   {
      pc8Set_pc9clear();
      for (volatile int i=0; i<500000; i++){}	// Искусственная задержка
               
      pc9Set_pc8clear();
      for (volatile int i=0; i<500000; i++){}	// Искусственная задержка
   }		
}

} // namespace iter3 {


/// делаем универсальные функции, подходящие для любого пина
namespace iter4 {

void portClockEnable (GPIO_TypeDef* port)
{
   // по порту определяем маску разрешения тактирования
   const auto mask =
      port == GPIOA ? RCC_AHBENR_GPIOAEN :
      port == GPIOB ? RCC_AHBENR_GPIOBEN :
      port == GPIOC ? RCC_AHBENR_GPIOCEN :
      port == GPIOD ? RCC_AHBENR_GPIODEN :
      port == GPIOF ? RCC_AHBENR_GPIOFEN : 0;
   RCC->AHBENR |= mask;
}
void pinSetAsOut (GPIO_TypeDef* port, int n) { port->MODER |= 1 << (2*n); }
void pinSet      (GPIO_TypeDef* port, int n) { port->BSRR = 1 << n; }
void pinClear    (GPIO_TypeDef* port, int n) { port->BSRR = 1 << (16 + n); }


void process()
{
   portClockEnable(GPIOC);
   pinSetAsOut (GPIOC, 8);
   pinSetAsOut (GPIOC, 9);

   while(1)
   {
      pinSet   (GPIOC, 8);
      pinClear (GPIOC, 9);
      for (volatile int i=0; i<500000; i++){}	// Искусственная задержка
               
      pinClear (GPIOC, 8);
      pinSet   (GPIOC, 9);
      for (volatile int i=0; i<500000; i++){}	// Искусственная задержка
   }		
}

} // namespace iter4 {


/// очевидно, что параметры универсальных функций одинаковые
/// выделяем под это стуктуру (принцип DRY)
/// в коде появилась возможность задать функцию вывода именем экземпляра стуктуры
namespace iter5 {

struct Pin {
   GPIO_TypeDef* const port;
   const int n;
};

void portClockEnable (Pin* pin)
{
   // по порту определяем маску разрешения тактирования
   const auto mask =
      pin->port == GPIOA ? RCC_AHBENR_GPIOAEN :
      pin->port == GPIOB ? RCC_AHBENR_GPIOBEN :
      pin->port == GPIOC ? RCC_AHBENR_GPIOCEN :
      pin->port == GPIOD ? RCC_AHBENR_GPIODEN :
      pin->port == GPIOF ? RCC_AHBENR_GPIOFEN : 0;
   RCC->AHBENR |= mask;
}
void pinSetAsOut (Pin* pin) { pin->port->MODER |= 1 << (2*pin->n); }
void pinSet      (Pin* pin) { pin->port->BSRR = 1 << pin->n; }
void pinClear    (Pin* pin) { pin->port->BSRR = 1 << (16 + pin->n); }


void process()
{
   // иницализация полей стуктуры по аналогии с массивом в {} скобках 
   Pin blueLed  {GPIOC, 8};
   Pin greenLed {GPIOC, 9};
   portClockEnable(&blueLed);
   pinSetAsOut (&blueLed);
   pinSetAsOut (&greenLed);

   while(1)
   {
      pinSet   (&blueLed);
      pinClear (&greenLed);
      for (volatile int i=0; i<500000; i++){}	// Искусственная задержка
               
      pinClear (&blueLed);
      pinSet   (&greenLed);
      for (volatile int i=0; i<500000; i++){}	// Искусственная задержка
   }		
}

} // namespace iter5 {



///////////////////////////////////////////////////////////////////////////////
//
// c++-style
//
///////////////////////////////////////////////////////////////////////////////
/// переводим функции в методы, отсюда сможем
/// 1. Названия методов можно сократить (DRY)
/// 2. Избавляемся от & в вызове функции
/// 3. можно использовать decltype и не запоминать CMSIS-ные названия типов
namespace iter6 {

struct Pin {
   const decltype(GPIOA) port;
   const int n;
   void clockEnable()
   {
      // по порту определяем маску разрешения тактирования
      const auto mask =
         port == GPIOA ? RCC_AHBENR_GPIOAEN :
         port == GPIOB ? RCC_AHBENR_GPIOBEN :
         port == GPIOC ? RCC_AHBENR_GPIOCEN :
         port == GPIOD ? RCC_AHBENR_GPIODEN :
         port == GPIOF ? RCC_AHBENR_GPIOFEN : 0;
      RCC->AHBENR |= mask;
   }
   void setAsOut() { port->MODER |= 1 << (2*n); }
   void set()      { port->BSRR = 1 << n; }
   void clear()    { port->BSRR = 1 << (16 + n); }

};


void process()
{
   Pin blueLed  {GPIOC, 8};
   Pin greenLed {GPIOC, 9};
   blueLed.clockEnable();
   blueLed.setAsOut();
   greenLed.setAsOut();

   while(1)
   {
      blueLed.set();
      greenLed.clear();
      for (volatile int i=0; i<500000; i++){}	// Искусственная задержка
               
      blueLed.clear();
      greenLed.set();
      for (volatile int i=0; i<500000; i++){}	// Искусственная задержка
   }		
}

} // namespace iter6 {



/// воспользуемся наличием в с++ конструкторов
/// и перенесём иницализацию пина в него
namespace iter7 {

struct Pin {
   const decltype(GPIOA) port;
   const int n;
   // очевидно, что тут необходимо передавать также параметры инициализации
   // к примеру: установить как выход, вход, аналоговый вход, альтернативная функция
   // но тут я привожу только минимально необходимый пример для решения задачи,
   // которая поставлена в первой итерации
   Pin (const decltype(GPIOA) port, const int n) : port(port), n(n)
   {
      clockEnable();
      setAsOut();
   }
   void clockEnable()
   {
      // по порту определяем маску разрешения тактирования
      const auto mask =
         port == GPIOA ? RCC_AHBENR_GPIOAEN :
         port == GPIOB ? RCC_AHBENR_GPIOBEN :
         port == GPIOC ? RCC_AHBENR_GPIOCEN :
         port == GPIOD ? RCC_AHBENR_GPIODEN :
         port == GPIOF ? RCC_AHBENR_GPIOFEN : 0;
      RCC->AHBENR |= mask;
   }
   void setAsOut() { port->MODER |= 1 << (2*n); }
   void set()      { port->BSRR = 1 << n; }
   void clear()    { port->BSRR = 1 << (16 + n); }

};


void process()
{
   // иницализация через конструктор возможна как в {} скобках,
   // так и в () скобках, придерживаемся варианта {} - универсальный
   Pin blueLed  {GPIOC, 8};
   Pin greenLed {GPIOC, 9};

   while(1)
   {
      blueLed.set();
      greenLed.clear();
      for (volatile int i=0; i<500000; i++){}	// Искусственная задержка
               
      blueLed.clear();
      greenLed.set();
      for (volatile int i=0; i<500000; i++){}	// Искусственная задержка
   }		
}

} // namespace iter7 {


/// поля структуры константны и известны на этапе компиляции
/// переносим их в шаблонные параметры
/// поскольку никаких полей данных структура не хранит,
/// то нет смысла создавать объекты, а все методы сделать static
/// так же появилась возможность использовать contexpr вместо const
/// НО, поскольку не создаём объекты, то инициализацию в
/// конструкторе уже не сделать
/// Можно развить идею в сторону создания объектов.
namespace iter8 {

template <uint32_t adr, int n>
struct Pin {
   static auto port() { return reinterpret_cast<decltype(GPIOA)>(adr); }
   static void clockEnable()
   {
      // по адресу порта определяем маску разрешения тактирования
      constexpr auto mask =
         adr == GPIOA_BASE ? RCC_AHBENR_GPIOAEN :
         adr == GPIOB_BASE ? RCC_AHBENR_GPIOBEN :
         adr == GPIOC_BASE ? RCC_AHBENR_GPIOCEN :
         adr == GPIOD_BASE ? RCC_AHBENR_GPIODEN :
         adr == GPIOF_BASE ? RCC_AHBENR_GPIOFEN : 0;
      RCC->AHBENR |= mask;
   }
   static void setAsOut() { port()->MODER |= 1 << (2*n); }
   static void set()      { port()->BSRR = 1 << n; }
   static void clear()    { port()->BSRR = 1 << (16 + n); }

};

// инстанцирование шаблонов, пишу не все, а только необходимые
using PC8 = Pin<GPIOC_BASE, 8>;
using PC9 = Pin<GPIOC_BASE, 9>;


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

} // namespace iter8 {




int main(void)
{
   // iter1::process(); // 496 байт - не рабочий вариант
   // iter2::process(); // 548 байт
   // iter3::process(); // 548 байт
   // iter4::process(); // 568 байт - больше код - плата за универсальность
                        // запись/сброс в порт происходят за 2 обращения к порту
                        // а не за одно, как в iter2
   // iter5::process(); // 568 байт
   // iter6::process(); // 568 байт
   iter7::process(); // 576 байт - тактирование включаем 2 раза у одного порта в конструкторе
   // iter8::process(); // 568 байт

   /// казалось бы, размер кода увеличился и мы заплатили за удобство скоростью,
   /// но вариант iter8 можно развивать дальше и с помощью метапрораммирования 
   /// расчитать на этапе компиляции маски включения тактирования,
   /// установки/сброса не у одного пина за раз, а сразу у нескольких,
   /// за одно обращение к регистру
   /// но данный вариант значительно сложнее в реализации, хотя результат того стоит
}
















