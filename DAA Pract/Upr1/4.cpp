//
// Created by Lenovo on 19.2.2026 г..
//

#include <cmath>
#include <iomanip>
#include <iostream>
#include <stack>

using namespace std;

int main()
{
    long long x;

    stack<double> st;

    while (cin >> x)
        st.push(sqrt(x));

    while (!st.empty())
    {
        cout << fixed << setprecision(6) << st.top() << '\n';
        st.pop();
    }

    return 0;
}
