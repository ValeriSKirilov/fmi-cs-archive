//
// Created by Lenovo on 24.1.2026 г..
//

#include <iostream>
#include <stack>

bool isOpening(const char c)
{
    return c == '(' || c == '[' || c == '{';
}

bool isClosing(const char c)
{
    return c == ')' || c == ']' || c == '}';
}

bool isRight(const char opened, const char closed)
{
    return (opened == '(' && closed == ')')
        || (opened == '[' && closed == ']')
        || (opened == '{' && closed == '}');
}

bool is_valid(const std::string& expr)
{
    std::stack<char> s;

    for (const char& ch : expr)
    {
        if (isClosing(ch))
        {
            if (s.empty())
                return false;

            char last_opened = s.top();
            s.pop();

            if (!isRight(last_opened, ch))
                return false;
        }
        else if (isOpening(ch))
            s.push(ch);
        else
            return false;
    }

    return s.empty();
}

int main()
{
    std::string expr;
    std::getline(std::cin, expr);

    std::cout << std::boolalpha << is_valid(expr) << std::endl;

    return 0;
}
