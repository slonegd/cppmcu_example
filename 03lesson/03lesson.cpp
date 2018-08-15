/**
 * 03lesson - динамический полиморфизм
 */

#include <iostream>
#include <vector>
#include <string>


// динамический полиморфизм основан на использовании интерфейсов
// классов с чисто виртуальными функциями
struct Ispeakable {
    // чисто виртуальная функция, не имеет реализации
    // все классы, наследуемые от этого интерфейса
    // обязаны дать реализацию этого метода
    virtual void speak() = 0;
};

struct Dog : Ispeakable {
    // реализация наследуемого метода для этого класса
    // если не писать override, то ошибки не будет, но стоит явно указывать свои намерения
    // в будущем это поможет в исправлении ошибок
    void speak() override { std::cout << "Собака говорит: Гав" << std::endl; }
    // другие поля и методы класса, тут не указываю
};

struct Cat : Ispeakable {
    void speak() override { std::cout << "Кошка говорит: Мяу" << std::endl; }
};

struct Bird : Ispeakable {
    void speak() override { std::cout << "Птичка говорит: Чик-чирик" << std::endl; }
};




// пример полиморфизма
// после запуска можно вводить несколько команд
// cat dog bird - добавит в массив вновь созданные
// out - выведет у всех из массива метод speak
// q - для завершения
// при добавлении нескольких животных и введения команды out
// можно увидеть, как вызываются методы в том порядке, в котором животные создавались
int main()
{
    // массив указателей на объекты, реализующие интерфейс Ispeakable
    std::vector<Ispeakable*> animals;
    // с консоли набираем команды построчно
    std::string command;
    while (std::getline (std::cin, command)) {
        if (command == "dog") {
            // оператор new возвращает указатель на созданный объект (Dog*)
            // массив хранит указатели (Ispeakable*), но поскольку он является
            // базовым для Dog, то происходит неявное преобразование
            animals.push_back (new Dog);
        } else if (command == "cat") {
            animals.push_back (new Cat);
        } else if (command == "bird") {
            animals.push_back (new Bird);
        } else if (command == "out") {
            // animals хранит указатели на Ispeakable
            // потому он "знает" лишь о методе speak
            // другие методы, с помощью этого указателя вызвать нельзя
            // происходит сокррытие других методов классов наследников (инкапсуляция)
            // но какаой метод вызвать?
            // вот тут и проявляется полиморфизм
            // вызовется метод нужного наследника
            // на самом деле программа идёт в таблицу виртуальных функций
            // где каждому указателю соотвествует указатель на нужную функцию
            for (auto a : animals)
                a->speak();
        } else if (command == "q") {
            // для завершения работы ввести q
            break;
        } else {
            std::cout << "неверная команда" << std::endl;
        }
    }
}