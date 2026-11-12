#include <bits/stdc++.h>

using namespace std;
#define MAX 3100

long long dp[MAX];

int main()
{
    int n;
    cin >> n;

    dp[1] = 1;
    dp[2] = 2;

    for (int i = 3; i <= n; i++)
    {
        if (i % 2 == 0)
        {
            dp[i] = dp[i - 2] + dp[i / 2];
        }
        else
            dp[i] = dp[i - 1];
    }

    cout << dp[n] << '\n';

    return 0;
}