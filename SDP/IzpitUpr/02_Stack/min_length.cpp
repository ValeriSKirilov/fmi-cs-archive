//
// Created by Lenovo on 28.1.2026 г..
//

#include <stack>
#include <iostream>

unsigned int minLength(const std::string& s)
{
    std::stack<char> stack;

    for (const char c : s)
    {
        if (!stack.empty())
        {
            if (stack.top() == 'a' && (c == 'b' || c == 'B'))
                stack.pop();
            else if (stack.top() == 'c' && (c == 'd' || c == 'D'))
                stack.pop();
            else
                stack.push(c);
        }
        else
            stack.push(c);
    }

    return stack.size();
}

int main()
{
    std::cout << minLength("aabacd") << std::endl;
    std::cout << minLength("aabbcd") << std::endl;

    return 0;
}
