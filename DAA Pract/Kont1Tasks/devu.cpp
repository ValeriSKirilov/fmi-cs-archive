#include <bits/stdc++.h>

using namespace std;

#define MAX 100005

int n, m;

vector<long long> a(MAX), b(MAX);

long long cost(long long x)
{
    long long total = 0;

    for (int i = 0; i < n; i++)
        if (a[i] < x)
            total += x - a[i];

    for (int i = 0; i < m; i++)
        if (b[i] > x)
            total += b[i] - x;

    return total;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> m;

    for (int i = 0; i < n; i++)
        cin >> a[i];

    for (int i = 0; i < m; i++)
        cin >> b[i];

    long long l = 0, r = 1e9;

    while (r - l > 2)
    {
        long long m1 = l + (r - l) / 3;
        long long m2 = r - (r - l) / 3;

        if (cost(m1) < cost(m2))
            r = m2;
        else
            l = m1;
    }

    long long ans = cost(l);
    for (long long i = l + 1; i <= r; i++)
        ans = min(ans, cost(i));

    cout << ans << '\n';

    return 0;
}