#pragma once

#include <iterator>

template<class T>
struct Listable
{
   T* prev {nullptr};
   T* next {nullptr};
};


template<class T>
struct List
{

   T* first {nullptr};
   T* last  {nullptr};

   void push_back (T* pobj)
   {
      pobj->prev = last;
      if (last)
         last->next = pobj;
      last = pobj;
      if (not first)
         first = pobj;
   }

   void push_front (T* pobj) {}

   void remove (T* pobj)
   {
      if (pobj->prev and pobj->next) {
         pobj->prev->next = pobj->next;
         pobj->next->prev = pobj->prev;
      } else if (pobj->next) {
         first = pobj->next->prev;
      } else if (pobj->prev) {
         last = pobj->prev->next;
      }
   }

   // эта часть для прохода в автоцикле, сделал на будущее
   // struct iterator
   // {
   //    T* p;
   //    iterator()          : p {nullptr} {}
   //    iterator (T* other) : p {other}   {}

   //    T&        operator*  () const { return *p; }
   //    bool      operator!= (const iterator &other) const { return p != other.p; }
   //    iterator& operator++ () { p = p->next; return *this; }
   // };

   // iterator begin() noexcept { return iterator {first}; }
   // iterator end()   noexcept { return nullptr; }

};
