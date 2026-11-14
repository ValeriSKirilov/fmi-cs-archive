//
// Created by Lenovo on 28.1.2026 г..
//

#include <stack>
#include <iostream>

bool isOpening(const char c)
{
    return c == '(' || c == '{' || c == '[' || c == '<';
}

bool isClosing(const char c)
{
    return c == ')' || c == '}' || c == ']' || c == '>';
}

bool match(const char open, const char close)
{
    return open == '(' && close == ')'
        || open == '{' && close == '}'
        || open == '[' && close == ']'
        || open == '<' && close == '>';
}

bool balanced(const std::string& expr)
{
    std::stack<char> s;

    for (const char c : expr)
    {
        if (isOpening(c))
            s.push(c);
        else if (isClosing(c))
        {
            if (s.empty())
                return false;

            if (match(s.top(), c))
                s.pop();
            else
                return false;
        }
    }

    return s.empty();
}

int main()
{
    std::cout << std::boolalpha;

    std::cout << balanced("u{ffft}r") << std::endl; // true
    std::cout << balanced("{t(s)}") << std::endl; // true
    std::cout << balanced("{}()") << std::endl; // true
    std::cout << balanced("{}}") << std::endl; // false
    std::cout << balanced("{") << std::endl; // false
    std::cout << balanced("{(aab}a)t") << std::endl; // false
    std::cout << balanced("{e&]g}") << std::endl; // false

    return 0;
}
