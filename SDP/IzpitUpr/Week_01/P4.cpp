//
// Created by Lenovo on 24.1.2026 г..
//

#include <iostream>
#include <stack>

template <typename T>
void print_stack(const std::stack<T>& s)
{
    std::stack<T> copy = s;
    while (!copy.empty())
    {
        std::cout << copy.top() << ' ';
        copy.pop();
    }
    std::cout << std::endl;
}

template <typename T>
void reverse_stack(std::stack<T>& s)
{
    std::stack<T> copy;
    while (!s.empty())
    {
        copy.push(s.top());
        s.pop();
    }

    s = copy;
}

int main()
{
    unsigned int n;
    std::cin >> n;

    std::stack<int> st;
    int temp;
    for (std::size_t it = 0; it < n; ++it)
    {
        std::cin >> temp;
        st.push(temp);
    }

    print_stack(st);
    reverse_stack(st);
    print_stack(st);

    return 0;
}
