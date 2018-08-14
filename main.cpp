
constexpr auto sum (auto a, auto b)
{
   return a + b;
}
static_assert ( sum(1,2) == 3 );
static_assert ( sum(1,2.1) == 3.1 );



// Дано трехзначное число. Найдите произведение их цифр.
// constexpr auto mul (int a)
// {
   
// }

// static_assert ( mul(123)  == 6 );
// static_assert ( mul(321)  == 6 );
// static_assert ( mul(1321) == 6 );
// static_assert ( mul(21)   == 2 );

// Дано пятизначное число. Найдите разность двух чисел.
// Первое число равно сумме цифр исходного числа, стоящих на четных местах.
// Второе число равно сумме цифр, стоящих на нечетных местах.
// constexpr auto delta (int a)
// {
   
// }

// static_assert ( delta(12345)  == -3 );
// static_assert ( delta(14253)  == 3  );


// Дано трехзначное число. Выведите на экран новое число,
// полученное из исходного путем перестановки цифр в обратном порядке.
// constexpr auto rev (int a)
// {
   
// }

// static_assert ( rev(123) == 321 );
// static_assert ( rev(321) == 123  );


// Дано трехзначное число. Замените среднюю цифру на ноль.
// constexpr auto nul (int a)
// {
   
// }

// static_assert ( nul(123) == 103 );
// static_assert ( nul(321) == 301  );


// Дано шестизначное число. Поменяйте местами первую и последнюю цифры.
// constexpr auto rev2 (int a)
// {
   
// }

// static_assert ( rev2(123456) == 623451 );
// static_assert ( rev2(654321) == 154326 );

// Дано пятизначное число. Цифры на четных позициях занулить.
// Например, из 12345 получается число 10305.
// constexpr auto nul2 (int a)
// {
   
// }

// static_assert ( nul2(12345) == 10305 );

// Даны два трехзначных числа. Найдите шестизначное число,
// образованное из двух данных чисел путем дописывания второго числа к первому справа.
// constexpr auto concat (int a, int b)
// {
   
// }

// static_assert ( concat(123,456) == 123456 );
// static_assert ( concat(234,561) == 234561 );

// Даны два числа. Выведите большее из них.
// constexpr auto max (auto a, auto b)
// {

// }

// static_assert ( max(123,456.1) == 456.1 );
// static_assert ( max(124,56.1)  == 124 );


// Даны три числа. Если первое число больше двух, то найти их сумму,
// иначе найти разность между вторым и третьим числами.
// constexpr auto if_ (auto a, auto b, auto c)
// {

// }

// static_assert ( if_(1,2,3) == -1 );
// static_assert ( if_(3,2,3) == 8  );

// Дано четырехзначное число. Переставить цифры так,
// чтобы получилось наибольшее из возможных число.
// constexpr auto max2 (int a)
// {

// }

// static_assert ( max2(1234) == 4321 );
// static_assert ( max2(2648) == 8642 );






int main()
{

}