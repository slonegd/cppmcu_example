/**
 * 18 выравнивание битовых полей
 **/

#include <stdint.h>


// упаковаывание битовых полей штука непредсказуемая
// согласно ответу тут 
// https://ru.stackoverflow.com/questions/885645/глюк-с-битовыми-полями-и-перечислениями-arm-none-eabi-gcc
// стандартом это никак не определено
// поэтому могут возникнуть неожидонности как со структурой test1
// которую компилятор не упаковал в одну ячейку 32битной памяти 
struct {
   enum Num { _0 = 0b000,_1,_2,_3,_4,_5,_6,_7 };
   uint32_t pole1 : 6;
   Num      pole2 : 3;
   uint32_t pole3 : 23;
} test1;


// для решения этой проблемы есть специальный аттрибут gcc
// __attribute__((packed)), который
// заставляют упаковывать биты так, как указано
// При этом будет выведено сообщение компилятора
// note: offset of packed bit-field '<unnamed struct>::pole2' has changed in GCC 4.4
// для того, чтобы этих надписей не было, есть флаг компиляции
// -Wno-packed-bitfield-compat
// структура test2 ведёт себя предсказуемо
struct {
   enum Num { _0 = 0b000,_1,_2,_3,_4,_5,_6,_7 };
   uint32_t pole1 : 6;
   Num      pole2 : 3;
   uint32_t pole3 : 23;
} __attribute__((packed)) test2;

int main()
{
   volatile auto t1 = sizeof(test1); // в ассембеле видно, что 8 байт
   volatile auto t2 = sizeof(test2); // в ассембеле видно, что 4 байт + note, что выравнивание изменено
}
