#include <bits/stdc++.h>

using namespace std;
#define MAX 300

long long dp[MAX][MAX];

int main()
{
    int n, m;
    cin >> n >> m;

    dp[0][0] = 1;

    for (int i = 0; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            dp[i][j] = dp[i][j - 1];

            if (i >= j)
                dp[i][j] += dp[i - j][j];
        }
    }

    cout << dp[n][m] << '\n';
}