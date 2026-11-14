//
// Created by Lenovo on 28.1.2026 г..
//

#include <iostream>
#include <stack>
#include <list>
#include <vector>
#include <algorithm>

struct Node
{
    int data;
    Node* next;

    Node(const int val, Node* next = nullptr) : data(val), next(next)
    {
    }
};

void print(Node* head)
{
    while (head)
    {
        std::cout << head->data << ' ';
        head = head->next;
    }
    std::cout << "nullptr\n";
}

void reverseList(Node*& head)
{
    Node* prev = nullptr;
    Node* curr = head;
    Node* next = nullptr;

    while (curr)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    head = prev;
}

bool isPalindrome(Node* head)
{
    if (!head || !head->next)
        return true;

    Node* slow = head;
    Node* fast = head;

    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    reverseList(slow);

    const Node* firstHalf = head;
    const Node* secondHalf = slow;

    while (secondHalf)
    {
        if (firstHalf->data != secondHalf->data)
            return false;
        firstHalf = firstHalf->next;
        secondHalf = secondHalf->next;
    }

    return true;
}

void removeConsecutive(Node* head)
{
    Node* curr = head;
    while (curr && curr->next)
    {
        if (curr->data == curr->next->data)
        {
            const Node* toDelete = curr->next;
            curr->next = curr->next->next;

            delete toDelete;
            toDelete = nullptr;
        }
        else
            curr = curr->next;
    }
}

Node* mergeLists(Node* l1, Node* l2)
{
    if (!l1) return l2;
    if (!l2) return l1;

    Node* head = nullptr;

    if (l1->data <= l2->data)
    {
        head = l1;
        l1 = l1->next;
    }
    else
    {
        head = l2;
        l2 = l2->next;
    }

    Node* curr = head;
    while (l1 && l2)
    {
        if (l1->data <= l2->data)
        {
            curr->next = l1;
            l1 = l1->next;
        }
        else
        {
            curr->next = l2;
            l2 = l2->next;
        }
        curr = curr->next;
    }

    if (l1) curr->next = l1;
    if (l2) curr->next = l2;

    return head;
}

Node* getMiddle(Node* head)
{
    Node* slow = head;
    Node* fast = head->next;

    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

Node* mergeSort(Node* head)
{
    if (!head || !head->next) return head;

    Node* middle = getMiddle(head);
    Node* nextToMiddle = middle->next;
    middle->next = nullptr;

    Node* left = mergeSort(head);
    Node* right = mergeSort(nextToMiddle);

    return mergeLists(left, right);
}

void splitAndReverse(Node*& head)
{
    if (!head || !head->next) return;

    Node* middle = getMiddle(head);
    Node* secondPartHead = middle->next;
    middle->next = nullptr;


    reverseList(secondPartHead);

    // print(secondPartHead);

    Node* curr = secondPartHead;
    while (curr->next)
        curr = curr->next;
    curr->next = head;

    head = secondPartHead;
}

void reorderList(Node* head)
{
    if (!head || !head->next) return;

    Node* middle = getMiddle(head);
    Node* secondPart = middle->next;
    middle->next = nullptr;

    reverseList(secondPart);

    Node* first = head;
    Node* second = secondPart;

    print(first);
    print(second);

    while (second)
    {
        Node* temp1 = first->next;
        Node* temp2 = second->next;

        first->next = second;
        second->next = temp1;

        first = temp1;
        second = temp2;
    }
}

void task08(std::list<std::stack<int>>& listOfStacks)
{
    std::vector<int> collectedElements;

    auto it = listOfStacks.begin();
    int index = 1;

    while (it != listOfStacks.end())
    {
        if (index % 2 == 0)
        {
            while (!it->empty())
            {
                collectedElements.push_back(it->top());
                it->pop();
            }

            it = listOfStacks.erase(it);
        }
        else
        {
            ++it;
        }
        index++;
    }

    std::sort(collectedElements.begin(), collectedElements.end(), std::less<int>());

    std::stack<int> newStack;
    for (const int val : collectedElements)
    {
        newStack.push(val);
    }

    listOfStacks.push_front(newStack);
}

int main()
{
    Node* l1 = new Node(5, nullptr);
    Node* l2 = new Node(4, l1);
    Node* l3 = new Node(3, l2);
    Node* l4 = new Node(2, l3);
    Node* l5 = new Node(1, l4);

    print(l5);

    // reverseList(l5);
    // print(l5);

    // std::cout << std::boolalpha << isPalindrome(l5);

    // removeConsecutive(l5);
    // print(l5);

    // splitAndReverse(l5);
    // print(l5);

    // reorderList(l5);
    // print(l5);


    delete l1;
    delete l2;
    delete l3;
    delete l4;
    delete l5;

    return 0;
}
