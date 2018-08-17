#include "stm32f0xx.h"
#include "pin.h"
#include "interrupt.h"

template <uint32_t adr, int n>
class Button {
    
    public:
    Button::clockEnable();

    static void systemTimerInit()
{
    constexpr uint32_t val = F_CPU/(1000)-1;
    SysTick->LOAD = val;                        // Загрузка значения
    SysTick->VAL = val;                         // Обнуляем таймеры и флаги 
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk  //processor clock
                  | SysTick_CTRL_TICKINT_Msk;   //разрешение прерывания
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   //запускает таймер
}
    void tick()  { time_count++; }

    enum 
}
