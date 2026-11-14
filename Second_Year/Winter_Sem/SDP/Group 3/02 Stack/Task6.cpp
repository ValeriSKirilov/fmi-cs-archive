//
// Created by Lenovo on 17.10.2025 г..
//

#include "LinkedStack.hpp"
#include <string>
#include <iostream>

#include "bits/valarray_after.h"

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

double eval(const double lhs, const double rhs, const char op) {
    switch (op) {
        case '+': return lhs + rhs;
        case '-': return lhs - rhs;
        case '*': return lhs * rhs;
        case '/': return lhs / rhs;
        case '^': return std::pow(lhs, rhs);
        default:
            throw std::runtime_error("Invalid operator");
    }
}

std::string toRPN(std::string const &s) {
    LinkedStack<double> nums;

    for (const char c: s) {
        if (std::isdigit(c)) {
            nums.push(c - '0');
        } else if (isOperator(c)) {
            try {
                const double rhs = nums.pop();
                const double lhs = nums.pop();

                nums.push(eval(lhs, rhs, c));
            } catch (...) {
                throw std::runtime_error("Not enough numbers");
            }
        } else {
            throw std::runtime_error("Invalid symbol");
        }

        if (nums.empty())
            throw std::runtime_error("Invalid expression");

        double result = nums.pop();

        if (!nums.empty())
            throw std::runtime_error("Too many numbers");

        return result;
    }
}

int main() {
    std::cout << evalRPN("1") << std::endl;

    return 0;
}
