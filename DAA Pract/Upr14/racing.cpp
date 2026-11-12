#include <bits/stdc++.h>

using namespace std;

#define MAX 1100

// dp[curr][sum]
long long dp[MAX];
long long a[MAX];

int main()
{
    int n;
    cin >> n;

    for (int i = 1; i <= n; i++)
        cin >> a[i];

    dp[0] = 0;
    dp[1] = a[1];
    dp[2] = min({dp[1] + a[2], dp[0] + a[2], a[2]});

    for (int i = 3; i <= n; i++)
    {
        dp[i] = min({dp[i - 1] + a[i], dp[i - 2] + a[i], dp[i - 3] + a[i]});
    }

    if (n == 1)
        cout << dp[1] << '\n';
    else if (n == 2)
        cout << min(dp[1], dp[2]);
    else
        cout << min({dp[n], dp[n - 1], dp[n - 2]}) << '\n';

    return 0;
}