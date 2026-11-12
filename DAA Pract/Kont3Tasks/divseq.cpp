#include <bits/stdc++.h>

using namespace std;
#define MAX 1100

long long dp[MAX][MAX];
int a[MAX];

int main()
{
    int n, k;
    cin >> n >> k;

    for (int i = 1; i <= n; i++)
        cin >> a[i];

    fill(dp[0], dp[0] + k + 10, -1);
    dp[0][0] = 0;

    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            int prev_j = (j - (a[i] % k) + k) % k;
            dp[i][j] = max(dp[i - 1][j], dp[i - 1][prev_j] != -1 ? dp[i - 1][prev_j] + 1 : LLONG_MIN);
        }
    }c

    cout << dp[n][0] << '\n';
}