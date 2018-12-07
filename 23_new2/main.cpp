/**
 * 23 собственный оператор new
 **/

#include "defines.h"
#include "stm32f0xx.h"

// необходимо понимать, что в микромире работать с кучей надо особенно аккуратно
// постоянные выделения/освобождения памяти могут привести к дефрагментации
// от этого легко обезопаситься, если применять new к объектам
// , которые никогда не удаляются, а это практически все объекты
// , которые работают с переферией, инициализируются на старте
// и перестают работать только, когда пропадает питание

// Для собственного оператора нужна собственная куча
// самый простой способ, выделить память в самом начале в виде массива
// и выдавать указатель оператору, контролируя переполнение этого массива
// Способ посложнее, который не рассматриваю пока, прописать кучу в линкере
template<size_t heap_size>
class Heap
{
   uint8_t memory[heap_size];
   size_t free_index {0};
public:
   void* allocate (size_t size)
   {
      void* p = reinterpret_cast<void*>(memory + free_index);
      free_index += size;
      // если нам необходимо памяти больше, чем выделено, то тут будет HardFault
      // что определиться при первой же отладке
      return (free_index < heap_size) ? p : nullptr;
   }
};

// выделяем память под динамические объекты
constexpr size_t heap_size {256};
Heap<heap_size> heap {};

// собственный глобальный оператор new, который работает с нашей кучей
void* operator new (size_t size)
{
   return heap.allocate(size);
}




int main()
{
   // сделаю volatile, чтобы не соптимизировал
   auto i = new volatile int{};
   *i = 1;
   // volatile int i{};
   // i = 1;
}


// результат компиляции -0s
// text    data     bss     dec     hex filename
//  524      12    1824    2360     938 build/main.elf
// размер программы увеличился на 524-456=68 байт, это плата за выделение на куче

// выполнение на ассемблере программы с собственным оператором
// => 0x08000126 <main()+2>:	04 20	movs	r0, #4
//    0x08000128 <main()+4>:	ff f7 ee ff	bl	0x8000108 <operator new(unsigned int)>
//    0x0800012c <main()+8>:	00 23	movs	r3, #0
//    0x08000126 <main()+2>:	04 20	movs	r0, #4
// => 0x08000128 <main()+4>:	ff f7 ee ff	bl	0x8000108 <operator new(unsigned int)>
//    0x0800012c <main()+8>:	00 23	movs	r3, #0
//    0x08000108 <operator new(unsigned int)+0>:	05 4b	ldr	r3, [pc, #20]	; (0x8000120 <operator new(unsigned int)+24>)
//    0x0800010a <operator new(unsigned int)+2>:	1a 00	movs	r2, r3
//    0x0800010c <operator new(unsigned int)+4>:	fc 32	adds	r2, #252	; 0xfc
// => 0x0800010e <operator new(unsigned int)+6>:	51 68	ldr	r1, [r2, #4]
// => 0x08000110 <operator new(unsigned int)+8>:	40 18	adds	r0, r0, r1
// => 0x08000112 <operator new(unsigned int)+10>:	cb 18	adds	r3, r1, r3
// => 0x08000114 <operator new(unsigned int)+12>:	50 60	str	r0, [r2, #4]
//    0x08000116 <operator new(unsigned int)+14>:	ff 28	cmp	r0, #255	; 0xff
// => 0x08000118 <operator new(unsigned int)+16>:	00 d9	bls.n	0x800011c <operator new(unsigned int)+20>
//    0x0800011a <operator new(unsigned int)+18>:	00 23	movs	r3, #0
//    0x0800011c <operator new(unsigned int)+20>:	18 00	movs	r0, r3
// => 0x0800011e <operator new(unsigned int)+22>:	70 47	bx	lr
//    0x08000120 <operator new(unsigned int)+24>:	1c 00	movs	r4, r3
//    0x08000122 <operator new(unsigned int)+26>:	00 20	movs	r0, #0
//    0x0800012c in main () at main.cpp:50
//    0x08000126 <main()+2>:	04 20	movs	r0, #4
//    0x08000128 <main()+4>:	ff f7 ee ff	bl	0x8000108 <operator new(unsigned int)>
// => 0x0800012c <main()+8>:	00 23	movs	r3, #0
// => 0x0800012e <main()+10>:	01 22	movs	r2, #1
// => 0x08000130 <main()+12>:	03 60	str	r3, [r0, #0]
// => 0x08000132 <main()+14>:	02 60	str	r2, [r0, #0]
// => 0x08000134 <main()+16>:	18 00	movs	r0, r3
//    0x08000136 <main()+18>:	10 bd	pop	{r4, pc}
