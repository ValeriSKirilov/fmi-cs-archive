//
// Created by Lenovo on 28.1.2026 г..
//

#include <iostream>
#include <stack>
#include <cmath>

bool isOperation(const char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

double applyOperation(const char op, const double lhs, const double rhs)
{
    switch (op)
    {
    case '+': return lhs + rhs;
    case '-': return lhs - rhs;
    case '*': return lhs * rhs;
    case '/': return lhs / rhs;
    case '^': return pow(lhs, rhs);
    default:
        throw std::runtime_error("invalid operation symbol");
    }
}

double evalRPN(const std::string& expr)
{
    std::stack<double> nums;

    for (const char c : expr)
    {
        if (std::isdigit(c))
            nums.push(c - '0');
        else if (isOperation(c))
        {
            if (nums.size() < 2)
                throw std::runtime_error("invalid expression");

            const double n = nums.top();
            nums.pop();
            const double m = nums.top();
            nums.pop();

            nums.push(applyOperation(c, n, m));
        }
        else
            throw std::runtime_error("invalid symbol");
    }

    if (nums.empty())
        throw std::runtime_error("invalid expression");

    if (nums.size() > 1)
        throw std::runtime_error("too many numbers in expression");

    return nums.top();
}

int priority(const char op)
{
    switch (op)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    default:
        throw std::runtime_error("unrecognised operation");
    }
}

std::string toRPN(const std::string& expr)
{
    std::stack<char> ops;

    std::string result;
    for (const char c : expr)
    {
        if (std::isdigit(c))
            result += c;
        else if (c == '(')
            ops.push(c);
        else if (c == ')')
        {
            while (!ops.empty() && ops.top() != '(')
            {
                result += ops.top();
                ops.pop();
            }
            if (ops.empty())
                throw std::runtime_error("no opening parenthesis found");

            ops.pop();
        }
        else if (isOperation(c))
        {
            int cp = priority(c);

            while (!ops.empty() && ops.top() != '(' && cp <= priority(ops.top()))
            {
                result += ops.top();
                ops.pop();
            }
            ops.push(c);
        }
        else
            throw std::runtime_error("invalid symbol");
    }

    while (!ops.empty())
    {
        if (ops.top() == '(')
            throw std::runtime_error("parenthesis was never closed");
        result += ops.top();
        ops.pop();
    }

    return result;
}

int main()
{
    try
    {
        const std::string rpn = toRPN("2+2*4/8+3^2");
        const double result = evalRPN(rpn);
        std::cout << rpn << std::endl;
        std::cout << result << std::endl;
    }
    catch (std::exception const& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
