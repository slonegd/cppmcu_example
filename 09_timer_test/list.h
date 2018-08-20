#pragma once


template<class T> struct Listable
{
   T* prev {nullptr};
   T* next {nullptr};
};


// эта часть для прохода в автоцикле
// чтобы это сработало for (auto& element : *this) {}
// согласно этому https://en.cppreference.com/w/cpp/language/range-for
// необходимо создать методы begin и end, которые возвращают что-то,
// что можно разименовать и получить объект, на который он указывает,
// сравнить на неравенство, инкрементировать: итератор.
// Отличие от простого указателя в том, что переопределена адресная
// арифметика в соотвествии с логикой контейнера
// создаём класс итератора (реализацию методов оставим на потом)
template<class T> class ListIterator
{
   T* p {nullptr};              // хранит только указатель на объект
public:
   ListIterator (T*);           // конструктор итератора из указателя
   ListIterator() = default;    // поскольку написали свой конструктор, необходимо указать на создание дефолтного

   T&            operator*  () const;
   bool          operator!= (const ListIterator&) const;
   ListIterator& operator++ (); // только этот оператор специфичен для листа, остальные как у обычного указателя
};


// реализацию методов вынесем за класс, чтобы тут был виден сразу весь функционал
template<class T> class List
{
   T* first {nullptr};
   T* last  {nullptr};
public:
   void push_back  (T*);
   void push_front (T*);
   void remove     (T*);
   // необходимые методы для работы автоцикла
   ListIterator<T> begin(); // возвращает итератор на первый элемент
   ListIterator<T> end();   // возвращает итератор на элемент, стоящий за последним (всегда nullptr)
};





// реализация методов
// необходимо указывать к какому классу реализовывается метод,
// а поскольку класс шаблонный, то и тут надо использовать шаблон
template<class T>
void List<T>::push_back (T* p)
{
   p->prev = last;
   if (last)
      last->next = p;
   last = p;
   if (not first)
      first = p;
}


template<class T>
void List<T>::push_front (T* p)
{
   // не реализован
}


template<class T>
void List<T>::remove (T* p)
{
   if (p->prev and p->next) {
      p->prev->next = p->next;
      p->next->prev = p->prev;
   } else if (p->next) {
      first = p->next->prev;
   } else if (p->prev) {
      last = p->prev->next;
   }
   pobj->prev = nullptr;
   pobj->next = nullptr;
}


template<class T>
ListIterator<T> List<T>::begin()
{
   return ListIterator<T> {first};
}


template<class T>
ListIterator<T> List<T>::end()
{
   return ListIterator<T>();
}






/// Iterator
template<class T>
ListIterator<T>::ListIterator (T* other) : p {other} {}

template<class T>
T& ListIterator<T>::operator* () const 
{
   return *p;
}

template<class T>
bool ListIterator<T>::operator!= (const ListIterator<T>& other) const
{
   return p != other.p;
}

template<class T>
ListIterator<T>& ListIterator<T>::operator++ ()
{
   p = p->next;
   return *this;
}
