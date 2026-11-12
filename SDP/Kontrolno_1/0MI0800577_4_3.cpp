#include <iostream>

template <typename T>
struct linkedElem
{
    T data;
    linkedElem<T> *next;

    linkedElem(const T &d = T{}, linkedElem<T> *n = nullptr) : data(d), next(n) {}
};

template <typename T>
void print(linkedElem<T> *head)
{
    linkedElem<T> *it = head;

    while (it)
    {
        std::cout << it->data << " ";
        it = it->next;
    }
    std::cout << "nullptr\n";
}

template <typename T>
bool div_by_3_is_1(T n)
{
    return n % 3 == 1;
}

template <typename T>
bool div_by_3_is_2(T n)
{
    return n % 3 == 2;
}

// Цялата логика трябва да работи със следващия елемент след head
template <typename T>
void reorder(linkedElem<T> *&head)
{
    linkedElem<T> *end_hold = head;

    while (end_hold->next)
    {
        end_hold = end_hold->next;
    }

    linkedElem<T> *end_hold_head = end_hold;

    linkedElem<T> *head_holder = head;

    while (head->next)
    {

        if (!div_by_3_is_1(head->data) && !div_by_3_is_2(head->data))
        {
            linkedElem<T> *next = head->next;

            delete head;
            head = nullptr;

            head = next;
        }

        if (div_by_3_is_2(head->data))
        {
            linkedElem<T> *to_del = head;

            head = to_del->next;

            end_hold->next = to_del;

            delete to_del;
            to_del = nullptr;
        }
    }

    head->next = end_hold_head;
    head = head_holder;
}

int main()
{
    linkedElem<int> *l1 = new linkedElem<int>(0,
                                              new linkedElem<int>(1,
                                                                  new linkedElem<int>(2,
                                                                                      new linkedElem<int>(3,
                                                                                                          new linkedElem<int>(4,
                                                                                                                              new linkedElem<int>(5,
                                                                                                                                                  new linkedElem<int>(6,
                                                                                                                                                                      new linkedElem<int>(7, nullptr))))))));

    linkedElem<int> *test = l1;

    print(l1);

    // while (test->next)
    // {
    //     test = test->next;
    // }

    // print(test);

    reorder(l1);

    print(l1);

    return 0;
}