#include <bits/stdc++.h>

using namespace std;

#define MAX 300

// dp[max_num][sum]
long long dp[MAX][MAX];

int main()
{
    int n, m;
    cin >> n >> m;

    for (int i = 0; i <= n; i++)
    {
        dp[1][i] = 1;
    }

    for (int i = 2; i <= m; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            dp[i][j] = (j - i >= 0 ? dp[i][j - i] : 0) + dp[i - 1][j];
        }
    }

    cout << dp[m][n] << '\n';

    return 0;
}