//
// Created by Lenovo on 17.10.2025 г..
//
#include "LinkedStack.hpp"
#include <iostream>
#include <string>

bool isOpening(const char c) {
    return c == '(' || c == '{' || c == '[' || c == '<';
}

bool isClosing(const char c) {
    return c == ')' || c == '}' || c == ']' || c == '>';
}

bool match(const char open, const char close) {
    return open == '(' && close == ')'
           || open == '{' && close == '}'
           || open == '[' && close == ']'
           || open == '<' && close == '>';
}

bool balanced(std::string const &s) {
    LinkedStack<char> stack;

    for (char c: s) {
        if (isOpening(c)) {
            stack.push(c);
        } else if (isClosing(c)) {
            if (stack.empty())
                return false;
            if (match(stack.peek(), c))
                stack.pop();
            else
                return false;
        }
    }

    return stack.empty();
}

int main() {
    std::cout << std::boolalpha << balanced("[{}]") << std::endl;
    return 0;
}
