#include <bits/stdc++.h>

using namespace std;

#define MAX 1100

int dp[MAX][MAX], a[MAX][MAX];

int main()
{
    int n, m;
    cin >> m >> n;

    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            cin >> a[i][j];

    for (int i = 1; i <= m; i++)
        dp[i][1] = a[i][1];

    for (int i = 2; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            dp[j][i] = min({j - 1 >= 1 ? dp[j - 1][i - 1] : INT_MAX,
                            dp[j][i - 1],
                            j + 1 <= m ? dp[j + 1][i - 1] : INT_MAX}) +
                       a[j][i];
        }
    }

    int ans = INT_MAX;
    for (int i = 1; i <= m; i++)
        ans = min(ans, dp[i][n]);

    cout << ans << '\n';

    return 0;
}