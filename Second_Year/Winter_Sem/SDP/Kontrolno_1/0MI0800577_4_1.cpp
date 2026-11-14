#include <iostream>
#include <queue>
#include <list>
#include <stack>
#include <string>

int charToNum(char s)
{
    // if (s[0] == '-')
    //     return (-1) * stringToNum(s.substr(1));

    return s - '0';
}

bool is_operator(char op)
{
    return op == '+' ||
        op == '-' ||
        op == '*' ||
        op == '/';
}

int evaluate(char op, int n, int m)
{
    if (op == '+')
        return n + m;
    if (op == '-')
        return n - m;
    if (op == '*')
        return n * m;
    if (op == '/')
        return n / m;

    return -1;
}

int queueValue(std::queue<char> q)
{
    std::stack<char> operands;
    // std::stack<char> operators;

    if (q.size() < 1)
        return -1;

    if (is_operator(q.front()))
        return -1;

    operands.push(charToNum(q.front()));
    q.pop();

    if (q.empty())
    {
        int n = operands.top();
        operands.pop();

        return n;
    }

    if (is_operator(q.front()))
        return -1;

    operands.push(charToNum(q.front()));
    q.pop();

    while (!q.empty())
    {
        if (is_operator(q.front()))
        {
            int n = operands.top();
            operands.pop();
            int m = operands.top();
            operands.pop();
            operands.push(evaluate(q.front(), n, m));
        }
        else
        {
            operands.push(charToNum(q.front()));
        }

        q.pop();
    }

    // while (!q.empty())
    // {
    //     if (is_operator(q.front()))
    //         operators.push(q.front());
    //     else
    //         operands.push(charToNum(q.front()));

    //     q.pop();
    // }

    // if (operands.size() >= 2 && operators.empty())
    //     return -1;

    // while (!operators.empty())
    // {
    //     int n = operands.top();
    //     std::cout << n << std::endl;
    //     operands.pop();
    //     int m = operands.top();
    //     std::cout << m << std::endl;
    //     operands.pop();

    //     std::cout << evaluate(operators.top(), n, m) << std::endl;
    //     operands.push(evaluate(operators.top(), n, m));
    //     operators.pop();
    // }

    return operands.top();
}

bool evalList(std::list<std::queue<char>> lst)
{
    bool is_sym = true;
    while (lst.front() != lst.back() && is_sym)
    {
        if (queueValue(lst.front()) != queueValue(lst.back()))
            is_sym = false;

        lst.pop_front();
        lst.pop_back();
    }

    return is_sym;
}

int main()
{
    std::queue<char> q1;
    q1.push('4');
    q1.push('5');
    q1.push('+');

    std::cout << queueValue(q1) << std::endl;

    std::queue<char> q2;
    q2.push('1');
    q2.push('6');
    q2.push('+');
    q2.push('7');
    q2.push('*');

    std::cout << queueValue(q2) << std::endl;

    std::queue<char> q3;
    q3.push('8');

    std::cout << queueValue(q3) << std::endl;

    std::queue<char> q4;
    q4.push('1');
    q4.push('6');
    q4.push('+');
    q4.push('7');
    q4.push('*');

    std::cout << queueValue(q4) << std::endl;

    std::queue<char> q5;
    q5.push('4');
    q5.push('5');
    q5.push('+');

    std::cout << queueValue(q5) << std::endl;

    std::list<std::queue<char>> lst;
    lst.push_front(q1);
    lst.push_front(q2);
    lst.push_front(q3);
    lst.push_front(q4);
    lst.push_front(q5);

    std::cout << std::boolalpha << evalList(lst) << std::endl;

    return 0;
}
