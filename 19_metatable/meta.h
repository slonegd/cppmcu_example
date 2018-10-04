#pragma once

#include <type_traits>
#include <utility>     // std::index_sequence
#include <iterator>    // std::size

// добавим namespace чтобы явно отобразить в кодe откуда берутся конструкции
namespace meta {

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



// часто необходимо сформировать массив данных на этапе компиляции
// примеры: таблица CRC, значение сопротивления от температуры,
// коды символов на алфавитный идикатор

/// генерирация массива данных (хелперы)
/// пример генерирующей функции - расчёт числа фибоначи (сумма 2-ух прердыдущих чисел)
/// constexpr обязателен, т.к. расчёт ведётся на этапе компиляции
/// тут используется рекурсия - вызов функции внутри этой же функции
/// i - индекс элемента массива, который генерируем
constexpr size_t fibo (size_t i) { return (i > 1) ? fibo(i-2) + fibo(i-1) : 1; }

// объявим шаблонную структуру, которая будет хранить нашу таблицу с данными
// структура нужна для того, чтобы можно было параметризовать таблицу
// Т - тип, который хранится в таблице
// f - указатель на генерирующую функцию. выводим тип с помощью decltype,
// а то у функций типы выглядят страшно
// index_sequence - специальный класс, который помогает сделать
// последовательность индексов в виде вариадика,
// подробнее тут https://en.cppreference.com/w/cpp/utility/integer_sequence
// яснее зачем будет далее
template<class T, decltype(&fibo) f, class index_sequence>
struct generate_impl;

// делаем частичную специализацию, где явно зададим index_sequence
// для std::index_sequence в качестве параметров шаблона
// принимаются числа из вариадика
template<class T, decltype(&fibo) f, size_t ... i>
struct generate_impl<T, f, std::index_sequence<i...>>
{
   static constexpr T table[] {f(i)...};
};

// пишем using, для того, чтобы конструкцией было легче пользоваться
// скрываем всё, что касается index_sequence
// std::make_index_sequence делает std::index_sequence с нужным количеством индексов
/// генерировать массив функцией f
template<decltype(&fibo) f, size_t size, class T = size_t>
using Generate = generate_impl<T, f, std::make_index_sequence<size>>;

// необходимо не забывать, что параметр шаблона адрес функции, поэтому &
/// пример использования + тест
using Fibo_example = Generate<&fibo, 6>;
static_assert (Fibo_example::table[0] == 1);
static_assert (Fibo_example::table[1] == 1);
static_assert (Fibo_example::table[2] == 2);
static_assert (Fibo_example::table[3] == 3);
static_assert (Fibo_example::table[4] == 5);
static_assert (Fibo_example::table[5] == 8);
static_assert (std::size(Fibo_example::table) == 6);






// аналогично делаем Transform, который на основе таблицы Source
// генерирует новую. Source - уже готовая ранее таблица
// можно заметить, что функция теперь принимает 2 параметра: индекс и значение
// результирующая таблица обязательно того-же размера, что и исходная
// полезность этой структуры сомнительно, так как эти действия можно
// сделать и в генерирующей функции
// ТУДУ попробовать сделать преобразование из меньшего массива данных в больший
/// изменения массива данных (хелперы)
/// пример изменяющей функции
constexpr size_t x2 (size_t i, size_t v) { return v * 2; }

template<class T, class Source, decltype(&x2) f, class index_sequence>
struct transform_impl;

template<class T, class Source, decltype(&x2) f, size_t ... i>
struct transform_impl<T, Source, f, std::index_sequence<i...>>
{
   static constexpr T table[] {f(i,Source::table[i])...};
};

/// изменить массив Source::table функцией f
template<class Source, decltype(&x2) f, class T = size_t>
using Transform = transform_impl<T, Source, f, std::make_index_sequence<std::size(Source::table)>>;

/// пример использования и тесты
using Fibox2_example = Transform<Fibo_example, &x2>;
static_assert (Fibox2_example::table[0] == 2);
static_assert (Fibox2_example::table[1] == 2);
static_assert (Fibox2_example::table[2] == 4);
static_assert (Fibox2_example::table[3] == 6);
static_assert (Fibox2_example::table[4] == 10);
static_assert (Fibox2_example::table[5] == 16);
static_assert (std::size(Fibox2_example::table) == 6);


} // namespace meta {