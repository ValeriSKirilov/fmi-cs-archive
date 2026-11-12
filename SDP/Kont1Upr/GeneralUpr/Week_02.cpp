//
// Created by Lenovo on 15.11.2025 г..
//

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <cassert>
#include <queue>

// P1
std::string removeDuplicates(const std::string& s)
{
    std::stack<char> st;

    for (int i = s.size() - 1; i >= 0; i--)
    {
        if (st.empty() || st.top() != s[i])
            st.push(s[i]);
        else if (!st.empty() || st.top() == s[i])
            st.pop();
    }

    std::string result;
    while (!st.empty())
    {
        result += st.top();
        st.pop();
    }

    return result;
}

// P2
int findNextChar(const std::string& s, const int from)
{
    int i = from;
    while (i < s.size() && s[i] != '/')
        ++i;

    return i;
}

std::string simplify(const std::string& path)
{
    std::stack<std::string> st;
    int start = 0, end = -1;

    while (start < path.size())
    {
        end = findNextChar(path, start);
        std::string token = path.substr(start, end - start);

        if (token == "..")
        {
            if (!st.empty())
                st.pop();
        }
        else if (token != "" && token != ".")
            st.push(token);

        start = end + 1;
    }

    if (st.empty()) return "/";

    std::string result;
    while (!st.empty())
    {
        result = "/" + st.top() + result;
        st.pop();
    }

    return result;
}

// P3
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
                result = std::max(result, i - st.top());
        }
    }

    return result;
}

// P4
int stringToNumber(const std::string& s)
{
    if (s[0] == '-')
    {
        return (-1) * stringToNumber(std::string(s.substr(1)));
    }

    int result = 0;
    for (const char i : s)
        result = result * 10 + (i - '0');

    return result;
}

bool is_operator(const std::string& s)
{
    return s == "+" ||
        s == "-" ||
        s == "*" ||
        s == "/";
}

int getResult(const std::string& op, const int n, const int m)
{
    if (op == "+")
        return n + m;
    if (op == "-")
        return n - m;
    if (op == "*")
        return n * m;
    if (op == "/")
        return n / m;

    return -1;
}

int evalRPN(const std::vector<std::string>& tokens)
{
    std::stack<int> st;

    for (const std::string& s : tokens)
    {
        if (!is_operator(s))
        {
            st.push(stringToNumber(s));
            continue;
        }

        const int n = st.top();
        st.pop();
        const int m = st.top();
        st.pop();

        st.push(getResult(s, n, m));
    }

    assert(st.size() == 1);
    return st.top();
}

// P6
void reverseFirstK(std::queue<int>& q, int k)
{
    std::stack<int> st;

    for (int i = 0; i < k; ++i)
    {
        st.push(q.front());
        q.pop();
    }

    for (int i = 0; i < k; ++i)
    {
        q.push(st.top());
        st.pop();
    }

    int last_count = q.size() - k;
    for (int i = 1; i <= last_count; ++i)
    {
        q.push(q.front());
        q.pop();
    }
}

int main()
{
    // P1
    // std::string s;
    // std::getline(std::cin, s);
    //
    // std::cout << removeDuplicates(s) << std::endl;

    // P2
    // std::string path;
    // std::getline(std::cin, path);
    //
    // std::cout << simplify(path) << std::endl;

    // P3
    // std::string expr;
    // std::getline(std::cin, expr);
    //
    // std::cout << findLongestValidSubexprLength(expr) << std::endl;

    // P4
    // int n;
    // std::cin >> n;
    // std::cin.ignore();
    //
    // std::vector<std::string> tokens(n);
    // for (int i = 0; i < n; ++i)
    //     std::getline(std::cin, tokens[i]);
    //
    // std::cout << evalRPN(tokens) << std::endl;

    // P6
    int n, k;
    std::cin >> n >> k;

    std::queue<int> q;
    int temp;
    for (int i = 1; i <= n; ++i)
    {
        std::cin >> temp;
        q.push(temp);
    }

    reverseFirstK(q, k);
    while (!q.empty())
    {
        std::cout << q.front() << ' ';
        q.pop();
    }
    std::cout << std::endl;

    return 0;
}
