//
// Created by Lenovo on 14.11.2025 г..
//

#include  <iostream>
#include <functional>
#include <cassert>
#include <string>

template <typename T>
struct Node
{
    T value;
    Node* next;

    Node(const T& val = T(), Node* next = nullptr) :
        value(val), next(next)
    {
    }
};

template <typename T>
void deallocate(Node<T>*& head)
{
    while (head)
    {
        Node<T>* toDelete = head;
        head = head->next;
        delete toDelete;
    }
}

template <typename T>
void print(Node<T>* head)
{
    while (head)
    {
        std::cout << head->value << ' ';
        head = head->next;
    }
    std::cout << "nullptr\n";
}

template <typename T>
T foldl(Node<T>* head, const std::function<T(T, T)>& func, const T& neutral)
{
    T result = neutral;

    while (head)
    {
        result = func(result, head->value);
        head = head->next;
    }

    return result;
}

std::string append(const std::string& x, const std::string& y)
{
    return x + y;
}

Node<int>* create(int from, int to, Node<int>* & new_tail)
{
    const int step = (to - from > 0 ? 1 : -1);

    Node<int> *new_head = new Node<int>(from + step), *it = new_head;

    for (int i = from + 2 * step; i != to; i += step)
    {
        it->next = new Node<int>(i);
        it = it->next;
    }

    new_tail = it;

    return new_head;
}

void add_in_between(Node<int>* head)
{
    if (!head || !head->next) return;

    while (head->next)
    {
        int from = head->value, to = head->next->value;

        if (std::abs(from - to) > 1)
        {
            Node<int> *new_tail, *new_head = create(from, to, new_tail);

            new_tail->next = head->next;
            head->next = new_head;
            head = new_tail->next;
        }
        else head = head->next;
    }
}

void remove(Node<int>* head)
{
    if (!head || !head->next || !head->next->next) return;

    int prevprev = head->value, prev = head->next->value;

    Node<int>* prev_valid = head->next;

    while (prev_valid->next)
    {
        if (prev_valid->next->value != prevprev * prev)
        {
            Node<int>* holder = prev_valid->next;
            prev_valid->next = prev_valid->next->next;
            delete holder;

            continue;
        }

        prevprev = prev;
        prev = prev_valid->next->value;

        head = prev_valid;
        prev_valid = head->next;
    }
}

int main()
{
    // Node<std::string>* head = new Node<std::string>("a");
    // Node<std::string>* el1 = new Node<std::string>("b");
    // Node<std::string>* el2 = new Node<std::string>("c");
    // Node<std::string>* el3 = new Node<std::string>("d");
    // Node<std::string>* el4 = new Node<std::string>("e");
    //
    // head->next = el1;
    // el1->next = el2;
    // el2->next = el3;
    // el3->next = el4;
    //
    // print<std::string>(head);
    //
    // std::string result = foldl<std::string>(head, append, "");
    // std::cout << result << std::endl;
    //
    // print<std::string>(head);
    // deallocate<std::string>(head);
    // assert(head == nullptr);

    // Node<int>* head = new Node<int>(-1);
    // Node<int>* el1 = new Node<int>(-8);
    // Node<int>* el2 = new Node<int>(6);
    // Node<int>* el3 = new Node<int>(-2);
    //
    // head->next = el1;
    // el1->next = el2;
    // el2->next = el3;
    //
    // print<int>(head);
    // add_in_between(head);
    // print<int>(head);

    // Node<int>* head = new Node<int>(1);
    // Node<int>* el1 = new Node<int>(2);
    // Node<int>* el2 = new Node<int>(2);
    // Node<int>* el3 = new Node<int>(3);
    // Node<int>* el4 = new Node<int>(5);
    // Node<int>* el5 = new Node<int>(4);
    //
    // head->next = el1;
    // el1->next = el2;
    // el2->next = el3;
    // el3->next = el4;
    // el4->next = el5;
    //
    // print<int>(head);
    // remove(head);
    // print<int>(head);

    return 0;
}
