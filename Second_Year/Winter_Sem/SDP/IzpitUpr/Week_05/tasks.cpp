//
// Created by Lenovo on 25.1.2026 г..
//

#include <iostream>
#include <list>
#include <stack>

constexpr char True = 't';
constexpr char False = '0';

struct Node
{
    int data;
    Node* next;

    Node(const int data, Node* next = nullptr) : data(data), next(next)
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
    std::cout << std::endl;
}

int getNum(const std::list<int>& l)
{
    int result = 0;

    for (std::list<int>::const_iterator it = l.cbegin(); it != l.cend(); ++it)
    {
        result *= 10;
        result += *it;
    }

    return result;
}

int getReversedNum(const std::list<int>& l)
{
    int result = 0;

    for (std::list<int>::const_reverse_iterator it = l.crbegin(); it != l.crend(); ++it)
    {
        result *= 10;
        result += *it;
    }

    return result;
}

int findListsSum(std::list<std::list<int>>& lists)
{
    int result = 0;

    for (const std::list<int>& l : lists)
    {
        result += getNum(l);
        result += getReversedNum(l);
    }

    return result;
}

Node* findMiddle(Node* head)
{
    if (!head || !head->next)
        return head;

    Node* slow = head;
    Node* fast = head->next;

    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
}

Node* shuffle(Node* head)
{
    Node* middle = findMiddle(head);

    Node* iter = middle->next;
    middle->next = nullptr;

    while (iter)
    {
        Node* next = iter->next;
        iter->next = head;
        head = iter;
        iter = next;
    }

    return head;
}

Node* reverseStack(Node* head)
{
    if (!head)
        return head;

    std::stack<Node*> s;

    while (head)
    {
        s.push(head);
        head = head->next;
    }

    head = s.top();
    s.pop();
    Node* iter = head;

    while (!s.empty())
    {
        Node* curr = s.top();
        iter->next = curr;
        iter = iter->next;
        s.pop();
    }

    return head;
}

Node* shuffle2(Node* head)
{
    Node* middle = findMiddle(head);

    Node* iter = middle->next;
    middle->next = nullptr;

    Node* reversed = reverseStack(iter);
    iter->next = head;

    return reversed;
}

bool letter(const char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool evalOp(const char op, bool hasTrue, bool hasFalse)
{
    if (op == '!')
        return !hasTrue;

    if (op == '&')
        return !hasFalse;

    if (op == '|')
        return hasTrue;

    throw "Invalid expression";
}

bool evalExpr(const std::string& expr)
{
    std::stack<char> s;

    for (char c : expr)
    {
        if (c == ',' || c == '\'')
            continue;

        if (c != ')')
            s.push(c);
        else
        {
            bool hasTrue = false;
            bool hasFalse = false;
            char top = s.top();

            while (top != '(')
            {
                if (letter(top))
                    hasTrue = true;
                else
                    hasFalse = true;

                s.pop();
                top = s.top();
            }

            s.pop();
            const char op = s.top();
            s.pop();

            if (evalOp(op, hasTrue, hasFalse))
                s.push(True);
            else
                s.push(False);
        }
    }

    if (s.size() != 1)
        throw "Invalid expression";

    return letter(s.top());
}

struct DCLE
{
    int data;
    DCLE* next;
    DCLE* prev;
};

bool shouldDelete(const DCLE* head)
{
    if (!head || !head->next || !head->prev)
        return false;

    return head->data % head->next->data == 0 && head->data % head->prev->data == 0;
}

DCLE* deleteNode(DCLE* toDelete)
{
    if (!toDelete)
        return nullptr;

    if (toDelete == toDelete->next)
    {
        delete toDelete;

        return nullptr;
    }

    toDelete->prev->next = toDelete->next;
    toDelete->next->prev = toDelete->prev;

    DCLE* next = toDelete->next;
    delete toDelete;

    return next;
}

std::list<DCLE*> getToDelete(DCLE* head)
{
    std::list<DCLE*> result;

    if (!head)
        return result;

    if (shouldDelete(head))
        result.push_back(head);

    DCLE* iter = head->next;

    while (iter)
    {
        if (shouldDelete(iter))
            result.push_back(iter);

        iter = iter->next;
    }

    return result;
}

DCLE* task4(DCLE* head)
{
    std::list<DCLE*> toDelete = getToDelete(head);

    while (!toDelete.empty())
    {
        for (DCLE* el : toDelete)
        {
            if (el == head)
                head = deleteNode(el);
            else
                deleteNode(el);
        }

        toDelete = getToDelete(head);
    }

    return head;
}

int main()
{
    // std::list<int> l;
    // l.push_back(1);
    // l.push_back(2);
    // l.push_back(3);
    // std::list<std::list<int>> ll;
    // ll.push_back(l);
    // ll.push_back(l);
    // std::cout << findListsSum(ll);

    Node* l1 = new Node(1, nullptr);
    Node* l2 = new Node(2, l1);
    Node* l3 = new Node(3, l2);
    Node* l4 = new Node(4, l3);
    Node* l5 = new Node(5, l4);

    Node* newHead = shuffle(l5);
    print(newHead);

    delete l1;
    delete l2;
    delete l3;
    delete l4;
    delete l5;

    return 0;
}
