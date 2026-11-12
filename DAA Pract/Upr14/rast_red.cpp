#include <bits/stdc++.h>

using namespace std;

#define MAX 55

long long dp[MAX][MAX];

int main()
{
    int n, k;

    cin >> k >> n;

    for (int i = 0; i <= k; i++)
    {
        dp[i][0] = 1;
    }

    for (int i = 1; i <= k; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            dp[i][j] = dp[i - 1][j] + dp[i - 1][j - 1];
        }
    }

    cout << dp[k][n];

    return 0;
}