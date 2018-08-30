#pragma once

#include <type_traits>

template<class First, class ... Rest>
struct first_ { using type = First; };

using test1 = first_<int, double, bool>::type;
static_assert ( std::is_same<test1, int>::value );

template<class ... Ts>
using first_t = typename first_<Ts...>::type;

using test2 = first_t<bool, double, int>;
static_assert ( std::is_same<test2, bool>::value );



template<class T, class ... Ts>
struct is_inlist;

template<class T, class First, class ... Rest>
struct is_inlist<T,First,Rest...>
{
   static const bool value =
         std::is_same<T,First>::value
      or is_inlist<T,Rest...>::value;
};

template<class T, class Last>
struct is_inlist<T,Last>
{
   static const bool value = std::is_same<T,Last>::value;
};

template<class T, class ... Ts>
constexpr bool is_inlist_v = is_inlist<T,Ts...>::value;

auto constexpr test3 = is_inlist_v<test1, bool, double, int>;
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
