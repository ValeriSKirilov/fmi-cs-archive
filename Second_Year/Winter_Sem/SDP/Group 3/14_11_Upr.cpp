//
// Created by Lenovo on 14.11.2025 г..
//

#include <iostream>
#include <vector>

struct node
{
    int data;
    node* next;

    node(const int data, node* next = nullptr) : data(data), next(next)
    {
    }
};

node* fromVector(std::vector<int> const& v)
{
    if (v.empty()) return nullptr;

    node* begin = new node(v.front());
    node* list = begin;
    for (std::size_t i = 1; i < v.size(); i++)
    {
        list->next = new node(v[i]);
        list = list->next;
    }

    list->next = begin;

    return begin;
}

void print(node* l, std::ostream& os = std::cout)
{
    if (!l) return;

    node const* first = l;
    std::cout << l->data << " ";
    do
    {
        std::cout << l->data << " ";
        l = l->next;
    }
    while (first != l);
}

bool hasCycle(node* l)
{
    node *slow = l, *fast = l;

    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) return true;
    }

    return false;
}

node* find_last(node* l)
{
    node* iter = l;
    while (iter && iter->next != l)
    {
        iter = iter->next;
    }

    return iter;
}

void push_front(const int el, node*& front)
{
    if (!front)
    {
        front = new node(el);
        return;
    }

    node* last = find_last(front);

    front = new node(el, front);

    last->next = front;
}

void remove_next(node*& head)
{
    if (!head)
    {
        throw std::runtime_error("Empty list!");
        return;
    }

    if (node* next = head->next)
    {
        head->next = next->next;
        delete next;
        next = nullptr;
        return;
    }
}

int josephus(int n, const int k)
{
    if (k <= 0 || n <= 0) return -1;

    node* list = nullptr;
    while (n > 0)
    {
        push_front(n--, list);
    }

    node* prev = find_last(list);

    while (prev != prev->next)
    {
        for (int i = 0; i < k - 1; ++i) prev = prev->next;

        remove_next(prev);
    }

    int winner = prev->data;
    delete prev;

    return winner;
}

int main()
{
    // const std::vector<int> v{1, 2, 3, 4, 5};
    // node* l = fromVector(v);
    //
    // print(l);
    //
    // std::cout << hasCycle(l) << std::endl;
    //
    // delete l;

    // node* l3 = nullptr;
    // push_front(1, l3);
    // push_front(2, l3);
    // push_front(3, l3);

    // print(l3);

    std::cout << josephus(4, 1) << std::endl;

    return 0;
}
