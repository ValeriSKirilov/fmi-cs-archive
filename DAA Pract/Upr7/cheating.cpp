#include <bits/stdc++.h>

using namespace std;

#define MAX 2000005

long long n, a[MAX], ans, l[MAX], r[MAX];

stack<long long> s;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n;
    for (int i = 1; i <= n; i++)
        cin >> a[i];

    for (int i = 1; i <= n; i++)
    {
        while (!s.empty() && a[s.top()] < a[i])
            s.pop();

        if (s.empty())
            l[i] = 0;
        else
            l[i] = s.top();

        s.push(i);
    }

    while (!s.empty())
        s.pop();

    for (int i = n; i >= 1; i--)
    {
        while (!s.empty() && a[s.top()] <= a[i])
            s.pop();

        if (s.empty())
            r[i] = n + 1;
        else
            r[i] = s.top();

        s.push(i);
    }

    for (int i = 1; i <= n; i++)
        ans += (i - l[i] - 1) + (r[i] - i - 1);

    cout << ans << endl;

    return 0;
}