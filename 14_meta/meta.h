#pragma once

// некоторое количество метафункций описаны в стандартной библиотеке
// наиболее часто используемые из них это std::is_same
// и преобразования над типами, вроде std::remove_reference
#include <type_traits>

// метафункции это такие структуры, которые в своём составе имеют
// псевдоним типа type, если функция выводит тип
// или статическую константу value, если функция выводит какое-либо значение этапа компиляции
// имена type и value используются в стандартной библиотеке, а поскольку она часть языка
// то и мы будем придерживаться этой нотации

// метафункция: определяет первый тип вариадика
// параметры функции описываются в шаблонных параметров (вариадик)
// а результат тип - значит должен быть псевдоним type
// реализация (имя first занято, потому добавил _)
template<class First, class ... Rest>
struct first_ { using type = First; };

// пример использования и сразу тест
using test1 = first_<int, double, bool>::type;

// компилятор инстанцирует структуру first_ с введёными типами
// , где First = int, Rest = double, bool
// в Rest может не быть ни одного типа, вариадики это допускают
// внутри инстанцированной структуры есть псевдоним типа
// type = First, а First = int, значит и type = int
// и псевдоним test1 в результате эквивалентен int

// сразу тест, для проверки метафункции
// для определния правильности используется метафункция стандартной библиотеки
// std::is_same возвращает true если типы в параметрах шаблона одинаковы, false в ином случае
static_assert ( std::is_same<test1, int>::value );

// обёртка, которая принята в стандарте, для сокращения записи
// замена ::type на _t
template<class ... Ts>
using first_t = typename first_<Ts...>::type;

// пример использования и сразу тест
using test2 = first_t<bool, double, int>;
static_assert ( std::is_same<test2, bool>::value );


// вторая функция которая нам нужна немного сложнее
// она должна возвращать true, если тип T присутвует в списке Ts..., иначе false
// объявим структуру
template<class T, class ... Ts>
struct is_inlist;

// описание структуры зависит от параметров шаблона
// каждое инстанцирование это новая структура
// далее условно назовём это первое описание
template<class T, class First, class ... Rest>
struct is_inlist<T,First,Rest...>
{
   static const bool value =          // метафункция должна возвращать константу типа bool, значит нужно value
         std::is_same<T,First>::value
      or is_inlist<T,Rest...>::value; // инстанцируется новая структура с другими параметрами шаблона
};

// необходим граничный случай, на котором инстанцирование структур прекратится
// очевидно, что это случай когда останется только 2 типа в параметрах шаблона
// далее условно назовём это второе описание
template<class T, class Last>
struct is_inlist<T,Last>
{
   static const bool value = std::is_same<T,Last>::value;
};

// сразу напишем обёртку, принятую в стандарте для сокращения записи ::value => _v
template<class T, class ... Ts>
constexpr bool is_inlist_v = is_inlist<T,Ts...>::value;

// как это работает на примере
auto constexpr test3 = is_inlist_v<test1, bool, double, int>;
// test1 у нас int;
// таким образом истанцируется структура is_inlist<int, bool, double, int>
// , которая подходит под первое описание, где T=int, First=bool, Rest = double,int
// std::is_same вернёт false, поскольку T<>First (int<>bool)
// но это только первая часть логического выражения, после or инстанцируется новая структура
// is_inlist<int, double, int>, которая тоже подходит под первое описание
// , где T=int, First=double, Rest = int
// std::is_same вернёт false, поскольку T<>First (int<>double)
// после or инстанцируется новая структура
// is_inlist<int, int>, которая по числу параметров подходит уже под второе описание
// , где T=int, Last=int, std::is_same очевидно вернёт true,
// и новых структур не инстанцируется
// а поскольку у нас все значения складываются по or, то результат самой первой структуры
// is_inlist<int, bool, double, int>::value будет true
// если в списке не оказалось нужного типа, все значения всех инстанцированных структур
// будут false, а значит и самая первая будет false

// ну и тесты
static_assert (test3);
auto constexpr test4 = is_inlist_v<test2, float, double, int>;
static_assert (not test4);

// в качестве закрепления предлагаю самим написать метафункцию, которая
// возвращает первую позицию типа в списке, или -1, если в списке нет
// должна проходить следующие тесты
// static_assert (position_v <int ,bool,int   ,float> == 1 );
// static_assert (position_v <int ,bool,double,float> == -1);
// static_assert (position_v <int ,bool,int   ,int  > == 1 );
// static_assert (position_v <bool,bool,bool  ,int  > == 0 );
// static_assert (position_v <int ,bool,bool  ,int  > == 2 );
