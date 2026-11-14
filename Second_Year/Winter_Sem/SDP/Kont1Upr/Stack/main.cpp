//
// Created by Lenovo on 13.11.2025 г..
//

#include "resizable_stack.hpp"
#include "linked_stack.h"

int main()
{
    RStack<int> stack;
    stack.push(2);
    stack.push(3);
    stack.push(4);

    LinkedStack<int> stack2;

    std::cout << stack.top() << std::endl;
    stack.pop();
    std::cout << stack.top() << std::endl;

    std::cout << "---------------------" << std::endl;

    stack2.push(1);
    stack2.push(2);
    stack2.push(3);

    std::cout << stack2.top() << std::endl;
    stack2.pop();
    std::cout << stack2.top() << std::endl;
    return 0;
}
