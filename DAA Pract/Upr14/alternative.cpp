#include <bits/stdc++.h>

using namespace std;

#define MAX 55

long long dp[MAX][MAX];

int main()
{
    int k, m, n;

    cin >> k >> m >> n;

    for (int i = 1; i < m; i++)
    {
        dp[1][i] = 1;
    }

    for (int i = 2; i <= n; i++)
    {
        for (int j = 1; j < m; j++)
        {
            dp[i][j] = (j > k ? dp[i - 1][j - k] : 0) +
                       (j + k < m ? dp[i - 1][j + k] : 0);
        }
    }

    long long count = 0;
    for (int i = 0; i < m; i++)
    {
        count += dp[n][i];
    }

    cout << count << '\n';

    return 0;
}