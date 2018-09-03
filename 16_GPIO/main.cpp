/**
 * 16 абстракция над GPIO
 **/

#include "defines.h"
#include "stm32f0xx.h"
#include "pin.h"
#include "timers.h"


// уберём метод setAsOut в классе пина, сделаем его более универсальным
// чтобы можно было задать любую конфигурацию (см строки 23-25)
// в init также будем включать тактирование
// 25 сделана только для примера
// см. GPIO.h pin.h (в этом порядке)

// без 25 строки 1048 -> 1080 байт -0s (2 раза включаем тактирование порта внутри init)
// с 25 строкой 1104 -0s
int main(void)
{
   using BlueLed  = PC8;
   using GreenLed = PC9;
   BlueLed ::init<PinMode::Output>();
   GreenLed::init<PinMode::Output>();
   PA0     ::init<PinMode::Input>();

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
















