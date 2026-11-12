//
// Created by Lenovo on 14.11.2025 г..
//

#include <iostream>

template <typename T>
struct Node
{
    T value;
    Node* next;

    Node(const T& val = T{}, Node* next = nullptr) : value(val), next(next)
    {
    }
};

template <typename T>
void deallocate(Node<T>* head)
{
    Node<T>* it = head;

    while (it)
    {
        Node<T>* to_delete = it;
        it = it->next;
        delete to_delete;
    }
}

template <typename T>
void print(Node<T>* head)
{
    while (head)
    {
        std::cout << head->value << " ";
        head = head->next;
    }
    std::cout << "nullptr\n";
}

template <typename T>
unsigned int getOccurencesNumber(Node<T>* head, const T& target)
{
    Node<T>* it = head;
    int count = 0;

    while (it)
    {
        if (it->value == target) count++;

        it = it->next;
    }

    return count;
}

template <typename T>
void reverse(Node<T>*& head)
{
    if (!head || !head->next) return;

    Node<T>* prev = nullptr;

    while (head->next)
    {
        Node<int>* next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }

    head->next = prev;
}

template <typename T>
void appendMirroredCopy(Node<T>* head)
{
    if (!head) return;

    Node<T>* mirrored_head = nullptr;

    while (head->next)
    {
        mirrored_head = new Node<T>(head->value, mirrored_head);
        head = head->next;
    }
    head->next = new Node<T>(head->value, mirrored_head);
}

template <typename T>
Node<T>* getMiddle(Node<T>* head)
{
    if (!head || !head->next)
    {
        // throw std::runtime_error("List is empty!");
        return head;
    }

    Node<T> *fast = head, *slow = head;
    while (fast->next && fast->next->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

int main()
{
    // const int x = 5, y = 6, z = 7;
    // Node<int>* head = new Node<int>(x);
    // Node<int>* it = head;
    // for (int i = 0; i < 4; ++i)
    // {
    //     it->next = new Node<int>(i);
    //     it = it->next;
    // }

    auto* head = new Node(1, new Node(2, new Node(3)));

    // print<int>(head);
    // appendMirroredCopy(head);

    // reverse(head);
    print<int>(head);

    Node<int>* middle = getMiddle(head);

    std::cout << middle->value << std::endl;

    print<int>(middle);

    deallocate<int>(head);

    return 0;
}
