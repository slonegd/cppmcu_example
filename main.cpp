#include <iostream>
constexpr auto sum (auto a, auto b)
{
   return a + b;
}
static_assert ( sum(1,2) == 3 );
static_assert ( sum(1,2.1) == 3.1 );


// Дано трехзначное число. Найдите произведение их цифр.
constexpr auto mul (int a)
{
    // std::cout << "Enter a three-digit number" << std::endl;
    // std::cin >> a;

    auto a1 = a/100;
    if (a1 == 0)
        a1 = 1;
    else if (a1 > 10) {
        while (a1 > 10){ 
            a=a/10;
            a1 = a/100;
        }
    }
    auto a2 = (a/10)%10;
    auto a3 = a%10;

    return a1*a2*a3;

    // std::cout << "Multiplying the digits of a numder is " << mul << std:: endl;

}

static_assert ( mul(123)  == 6 );
static_assert ( mul(321)  == 6 );
static_assert ( mul(1322) == 6 );
static_assert ( mul(21)   == 2 );
static_assert ( mul(123456) == 6 );

// Дано пятизначное число. Найдите разность двух чисел.
// Первое число равно сумме цифр исходного числа, стоящих на четных местах.
// Второе число равно сумме цифр, стоящих на нечетных местах.
constexpr auto delta (int a)
{
   auto sumOdd = 0;
   auto sumEven = 0;

   while (a>0) {
       auto odd = a%10;
       sumOdd +=odd;
       a /= 10;
       auto even = a%10; 
       sumEven += even;
       a /= 10;
    }

    return sumEven - sumOdd;

}

static_assert ( delta(12345)  == -3 );
static_assert ( delta(14253)  == 3  );
static_assert ( delta(145253)  == 2 );


// Дано трехзначное число. Выведите на экран новое число,
// полученное из исходного путем перестановки цифр в обратном порядке.
constexpr auto rev (int a)
{
   auto b = 0;
   while (a>0) {
       b = 10*b +a%10;
       a = a/10;
   }
    return b;
}

static_assert ( rev(123) == 321 );
static_assert ( rev(321) == 123  );
static_assert ( rev(4321) == 1234  );


// Дано трехзначное число. Замените среднюю цифру на ноль.
constexpr auto nul (int a)
{
   
   return a/100*100 + a%10;
    
}

static_assert ( nul(123) == 103 );
static_assert ( nul(321) == 301  );


// Дано шестизначное число. Поменяйте местами первую и последнюю цифры.
constexpr auto rev2 (int a)
{
    
}

static_assert ( rev2(123456) == 623451 );
static_assert ( rev2(654321) == 154326 );

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