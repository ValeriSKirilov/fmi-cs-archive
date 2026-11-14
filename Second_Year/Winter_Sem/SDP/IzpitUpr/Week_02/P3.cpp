//
// Created by Lenovo on 24.1.2026 г..
//

#include <iostream>
#include <string>
#include <stack>

int findLongestValidSubexprLength(const std::string& expr)
{
    std::stack<int> st;
    st.push(-1);
    int result = 0;

    for (int i = 0; i < expr.size(); ++i)
    {
        if (expr[i] == '(')
            st.push(i);
        else
        {
            st.pop();

            if (st.empty())
                st.push(i);
            else
                result = std::max(i - st.top(), result);
        }
    }

    return result;
}

int main()
{
    std::string expr;
    std::getline(std::cin, expr);

    std::cout << findLongestValidSubexprLength(expr) << std::endl;
    return 0;
}
