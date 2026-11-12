#include <iostream>
#include "task2_iterator.hpp"

template <typename T>
void LinkedList<T>::InserAfterNInterval(I it, int n, T x)
{
    LinkedListElement<T> *to_add = new LinkedListElement(x, nullptr);

    I curr = front;

    if (front == back == nullptr)
    {
        front = new I(x, nullptr);
    }

    while (front != back)
    {
        if (curr != it)
        {
            ++curr;
        }
        else
        {
            I next = it->next;
            it->next = new I(x, next);
            it += 2;
        }
    }
}
