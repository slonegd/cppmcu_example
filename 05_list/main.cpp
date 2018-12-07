/**
 * 05 двунаправлленый список
 **/

#include <iostream>


// в прошлом примере с паттерном подписчик для хранения использовался
// стандартный контейнер std::list
// он использует для своей работы динамическое выделение памяти с помощью оператора new
// в микромире этот оператор необходимо использовать с большой осторожностью
// во избежание дефрагментации, у нас мало памяти
// вернее new то вызывать можно, а вот с delete надо быть аккуратным
// можно самому определить аллокатор, который не вызывает delete, но это довольно сложная тема,
// поэтому стандартные контейнеры нам пока не доступны, придется писать свой.

// В stl двунаправленный список представляет из себя некоторе количество
// структур (Node), разбросанных беспорядочно в памяти.
// Чтобы была возможность найти все элементы, хранящиеся в контейнере,
// каждая Node хранит в себе указатель на предыдущую Node и на следующую

template<class T>
struct Node {
   T value;
   Node* prev;
   Node* next;
};

// Таким образом, если начать с первой Node, и пройтись по указателям next,
// пока next не будет nullptr, можно пройтись по всем Node в прямом порядке.
// Думаю понятно, как это сделать в обратном порядке. Только нужна структура,
// которая будет хранить указатель на первую Node и на последнюю

template<class T>
struct List_ {
   Node<T>* first;
   Node<T>* last;
};

// для того чтобы удалить Node из списка достаточно изменить указатели 
// prev->next = next; next->prev = prev;
// для вставки новой Node в конец списка
// prev = List::last; List::last = this;
// аналогично для вставки в начало
// Думаю теперь понятно, почему список называется двунаправленным.

// плюсы контейнера:
// + быстрая вставка
// + быстрое удаление
// + возможность последовательно пройтись по всем значениям
// минусы контейнера
// - доступ к произвольному элементу относительно долгая операция (кроме первого и последнего)


// Далее рассмотрим, как реализовать контейнер без динамического выделения памяти
// Память для хранения элементов уже будет выделена конструктором класса, который мы хотим хранить в списке
// Напишем базовый класс. При наследовании от него, новый класс будет хранить в себе
// указатели на предыдущий и следующий, как в Node. Само значение хранить не надо,
// так как объект класса и есть значение
template<class T>
struct Listable
{
   T* prev {nullptr};
   T* next {nullptr};
};

// класс самого листа
template<class T>
struct List
{
   // хранит значения на первый и последний, при создании инициализируется нулями, так как элементов ещё нет
   T* first {nullptr};
   T* last  {nullptr};

   // должен содержать метод добавления в конец списка
   void push_back (T* pobj)
   {
      pobj->prev = last;
      if (last)
         last->next = pobj;
      last = pobj;
      if (not first)                      // при добавлении первого элемента
         first = pobj;
   }
   // добавление в начало списка
   void push_front (T* pobj) {}           // реализацию не указываю
   // удаление из списка
   void remove (T* pobj)
   {
      if (pobj->prev and pobj->next) {    // если в серединке
         pobj->prev->next = pobj->next;
         pobj->next->prev = pobj->prev;
      } else if (pobj->next) {            // если первый
         first = pobj->next;
         first->prev = nullptr;
         if (not first) last = nullptr;
      } else if (pobj->prev) {            // если последний
         last = pobj->prev;
         last->next = nullptr;
         if (not last) first = nullptr;
      }
      // обнуление всех указателей, мы не в списке
      pobj->prev = nullptr;
      pobj->next = nullptr;
   }
};


// создадим тривиальную структуру
// наследуем её от базового Listable, чтобы хранить указатели на предыдущий и следующий 
struct A : Listable<A>
{
   // любые, необходимые нам поля и методы, самому листу о них ничего знать не надо
   // тут для удобства отслеживания будем хранить номер созданного объекта
   int n {0};
   // конструктор принимает в качестве аргумента, лист к которому надо добавиться
   A (List<A>& list) { list.push_back (this); }
   // метод, возвращающий адрес, для удобства отслеживания далее
   A* adr() { return this; }
};




int main ()
{
   List<A> list;
   A a1 {list};
   a1.n = 1;
   std::cout << "A a1 {list};" << std::endl;
   std::cout << list.first << ' ' << list.last << std::endl;
   std::cout << a1.adr() << ": " << a1.n << ' ' << a1.prev << ' ' << a1.next << std::endl;
   // пример вывода тут
   // A a1 {list};
   // 0x7ffce04510a0 0x7ffce04510a0
   // 0x7ffce04510a0: 1 0 0
   // list.first == list.last , потому что в списке всего 1 элемент
   // у самого элемента указатели на предыдущий и следующий нулевые, опять же потому что он 1
   A a2 {list};
   a2.n = 2;
   std::cout << "A a2 {list};" << std::endl;
   std::cout << list.first << ' ' << list.last << std::endl;
   std::cout << a1.adr() << ": "  << a1.n << ' ' << a1.prev << ' ' << a1.next << std::endl;
   std::cout << a2.adr() << ": "  << a2.n << ' ' << a2.prev << ' ' << a2.next << std::endl;
   // пример вывода тут
   // A a2 {list};
   // 0x7ffce04510a0 0x7ffce0451080
   // 0x7ffce04510a0: 1 0 0x7ffce0451080
   // 0x7ffce0451080: 2 0x7ffce04510a0 0
   // first и last уже отличается, первый указывает на первый, последний на второй
   // в первом элементе указатель на предыдущий нулевой, на следующий - адрес второго
   // во втором элементе указатель на предыдущий - адрес первого, на следующий нулевой
   A a3 {list};
   a3.n = 3;
   std::cout << "A a3 {list};" << std::endl;
   std::cout << list.first << ' ' << list.last << std::endl;
   std::cout << a1.adr() << ": "  << a1.n << ' ' << a1.prev << ' ' << a1.next << std::endl;
   std::cout << a2.adr() << ": "  << a2.n << ' ' << a2.prev << ' ' << a2.next << std::endl;
   std::cout << a3.adr() << ": "  << a3.n << ' ' << a3.prev << ' ' << a3.next << std::endl;
   // пример вывода тут
   // A a3 {list};
   // 0x7ffce04510a0 0x7ffce0451060
   // 0x7ffce04510a0: 1 0 0x7ffce0451080
   // 0x7ffce0451080: 2 0x7ffce04510a0 0x7ffce0451060
   // 0x7ffce0451060: 3 0x7ffce0451080 0
   // изменился указатель на последний - адрес третего
   // второй теперь указывает на третьего
   // а третий указывает только на второго
   list.remove (&a2);
   std::cout << "list.remove (&a2);" << std::endl;
   std::cout << list.first << ' ' << list.last << std::endl;
   std::cout << a1.adr() << ": "  << a1.n << ' ' << a1.prev << ' ' << a1.next << std::endl;
   std::cout << a3.adr() << ": "  << a3.n << ' ' << a3.prev << ' ' << a3.next << std::endl;
   // пример вывода тут
   // list.remove (&a2);
   // 0x7ffce04510a0 0x7ffce0451060
   // 0x7ffce04510a0: 1 0 0x7ffce0451060
   // 0x7ffce0451060: 3 0x7ffce04510a0 0
   // указатель на первый и последний не изменились, потому что удалялся из списка средний элемент
   // первый теперь указывает на третий
   // а третий на первый
   list.push_back (&a2);
   std::cout << "list.push_back (&a2);" << std::endl;
   std::cout << list.first << ' ' << list.last << std::endl;
   std::cout << a1.adr() << ": "  << a1.n << ' ' << a1.prev << ' ' << a1.next << std::endl;
   std::cout << a3.adr() << ": "  << a3.n << ' ' << a3.prev << ' ' << a3.next << std::endl;
   std::cout << a2.adr() << ": "  << a2.n << ' ' << a2.prev << ' ' << a2.next << std::endl;
   // пример вывода тут
   // list.push_back (&a2);
   // 0x7ffce04510a0 0x7ffce0451080
   // 0x7ffce04510a0: 1 0 0x7ffce0451060
   // 0x7ffce0451060: 3 0x7ffce04510a0 0x7ffce0451080
   // 0x7ffce0451080: 2 0x7ffce0451060 0
   // второй переместился в конец списка
}


// для лучшего понимания работы с указателями рассмотрим программу пошагово и что происходт в памяти
// для простоты считаем что никаких оптимизаций и инлайнинга не происходит
// int и указатели занимают 4 байта - одна ячейка
// начало программы, адрес стека условно 0х10 (адреса пишем в 16-ричной системе)
// List<A> list; // создали объект листа, два указатели, из которых этот объект состояит инициализировались nullptr
// объект лёг по адресу 0х10
// адрес : что лежит = чему равно
// 0х10 : list.first = 0x00
// 0х14 : list.last  = 0x00
// int i {5}; // создали i, инициализировали 5, лёг по адресу 0х18, занимает 1 ячейку
// 0х10 : list.first = 0x00
// 0х14 : list.last  = 0x00
// 0х18 : i          = 5
// А а1; // создали объект типа А, он наследник от Listable, потому имеет 3 поля данных, занимает 3 ячейки
// ...
// 0х18 : i          = 5
// 0х1С : a1.prev    = 0x00
// 0х1С : a1.last    = 0x00
// 0х1С : a1.n       = 0
// a1.n = 1;
// ...
// 0х1С : a1.last    = 0x00
// 0х1С : a1.n       = 1
// list.push_back (&a1)
// сразу переходим внутрь метода, где
// pobj = &a1 = 0x1C
// 0х10 : list.first = 0x00
// 0х14 : list.last  = 0x00
// 0х18 : i          = 5
// 0х1С : a1.prev    = 0x00
// 0х1С : a1.last    = 0x00
// 0х1С : a1.n       = 1
// 0х20 : pobj       = 0x1C

// pobj->prev = last; // (0x1C->) ссылается на a1; a1.prev = list.last; а last = 0x00
// условие if (last) не выполняется
// last = pobj; // pobj = 0x1C
// if (not first) first = pobj; // pobj = 0x1C
// 0х10 : list.first = 0x1C
// 0х14 : list.last  = 0x1C
// 0х18 : i          = 5
// 0х1С : a1.prev    = 0x00
// 0х20 : a1.last    = 0x00
// 0х24 : a1.n       = 1
// 0х28 : pobj       = 0x1C

// выходим из метода pobj уничтожается
// 0х10 : list.first = 0x1C // указывает на единственный объект из списка
// 0х14 : list.last  = 0x1C // указывает на единственный объект из списка
// 0х18 : i          = 5
// 0х1С : a1.prev    = 0x00 // ни на что не указывает, он единственный объект из списка
// 0х20 : a1.last    = 0x00 // ни на что не указывает, он единственный объект из списка
// 0х24 : a1.n       = 1

// А а2; a2.n = 2; // создали объект типа А, имеет 3 поля данных, занимает 3 ячейки
// 0х24 : a1.n       = 1
// 0х28 : a2.prev    = 0x00
// 0х2С : a2.last    = 0x00
// 0х30 : a2.n       = 2

// list.push_back (&a2); // далее действия пронумерованы
// 0х10 : list.first = 0x1C
// 0х14 : list.last  = 0x1C (3) last = pobj       => 0x28
// 0х18 : i          = 5
// 0х1С : a1.prev    = 0x00
// 0х20 : a1.next    = 0x00 (2) last->next = pobj => 0x28
// 0х24 : a1.n       = 1
// 0х28 : a2.prev    = 0x00 (1) pobj->prev = last => 0x1C
// 0х2С : a2.next    = 0x00
// 0х30 : a2.n       = 2
// 0х34 : pobj       = 0x28

// выходим из метода
// 0х10 : list.first = 0x1C // указывает на a1
// 0х14 : list.last  = 0x28 // указывает на а2
// 0х18 : i          = 5
// 0х1С : a1.prev    = 0x00 // никуда не указывает (первый)
// 0х20 : a1.next    = 0x28 // указывает на а2
// 0х24 : a1.n       = 1
// 0х28 : a2.prev    = 0x1C // указывает на а1
// 0х2С : a2.next    = 0x00 // никуда не указывает (последний)
// 0х30 : a2.n       = 2

// А а3; a3.n = 3; list.push_back (&a3);
// 0х10 : list.first = 0x1C
// 0х14 : list.last  = 0x28 (3) last = pobj       => 0x34
// 0х18 : i          = 5
// 0х1С : a1.prev    = 0x00
// 0х20 : a1.next    = 0x28
// 0х24 : a1.n       = 1
// 0х28 : a2.prev    = 0x1C
// 0х2С : a2.next    = 0x00 (2) last->next = pobj => 0x34
// 0х30 : a2.n       = 2
// 0х34 : a3.prev    = 0x1C (1) pobj->prev = last => 0x28
// 0х38 : a3.next    = 0x00
// 0х3С : a3.n       = 3
// 0х40 : pobj       = 0x34

// выходим из метода
// 0х10 : list.first = 0x1C // указывает на а1
// 0х14 : list.last  = 0x34 // указывает на а3
// 0х18 : i          = 5
// 0х1С : a1.prev    = 0x00 // никуда не указывает (первый)
// 0х20 : a1.next    = 0x28 // указывает на а2
// 0х24 : a1.n       = 1
// 0х28 : a2.prev    = 0x1C // указывает на а1
// 0х2С : a2.next    = 0x34 // указывает на а3
// 0х30 : a2.n       = 2
// 0х34 : a3.prev    = 0x28 // указывает на а2
// 0х38 : a3.next    = 0x00 // никуда не указывает (последний)
// 0х3С : a3.n       = 3

// list.remove (&a2); // сразу внутрь метода
// 0х10 : list.first = 0x1C
// 0х14 : list.last  = 0x34
// 0х18 : i          = 5
// 0х1С : a1.prev    = 0x00
// 0х20 : a1.next    = 0x28 (1) pobj->prev->next = pobj->next => 0x34
// 0х24 : a1.n       = 1
// 0х28 : a2.prev    = 0x1C
// 0х2С : a2.next    = 0x34
// 0х30 : a2.n       = 2
// 0х34 : a3.prev    = 0x28 (2) pobj->next->prev = pobj->prev => 0x1C
// 0х38 : a3.next    = 0x00
// 0х3С : a3.n       = 3
// 0х40 : pobj       = 0x1С

      if (pobj->prev and pobj->next) {    // если в серединке
         pobj->prev->next = pobj->next;
         pobj->next->prev = pobj->prev;
      } else if (pobj->next) {            // если первый
         first = pobj->next;
         first->prev = nullptr;
         if (not first) last = nullptr;
      } else if (pobj->prev) {            // если последний
         last = pobj->prev;
         last->next = nullptr;
         if (not last) first = nullptr;
      }
      // обнуление всех указателей, мы не в списке
      pobj->prev = nullptr;
      pobj->next = nullptr;

