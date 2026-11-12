#include <bits/stdc++.h>

using namespace std;

#define MAX 22000

int dp[MAX], a[MAX];

int main()
{
    int x, n = 1;
    while (cin >> x)
    {
        a[n] = x;
        n++;
    }

    fill(dp, dp + n + 5, 0);

    for (int i = 1; i <= n; i++)
        dp[a[i]] = dp[a[i] - 1] + 1;

    int ans = 0;
    for (int i = 0; i < MAX; i++)
        ans = max(ans, dp[i]);

    cout << ans << '\n';

    return 0;
}