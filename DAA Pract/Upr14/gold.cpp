#include <bits/stdc++.h>

using namespace std;

#define MAX 55
#define TOTAL 51000

int dp[TOTAL], a[MAX], sum = 0;

int main()
{
    int n;
    cin >> n;

    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
        sum += a[i];
    }

    fill(dp, dp + TOTAL, -1);

    dp[0] = 0;

    for (int i = 0; i < n; i++)
    {
        int next_dp[TOTAL];
        fill(next_dp, next_dp + TOTAL, -1);

        next_dp[0] = 0;
        for (int j = 0; j < TOTAL; j++)
        {
            if (dp[j] > -1)
            {
                next_dp[j] = max(next_dp[j], dp[j]);
                next_dp[j + a[i]] = max(next_dp[j + a[i]], dp[j] + a[i]);
                next_dp[abs(j - a[i])] = max(next_dp[abs(j - a[i])], dp[j] + a[i]);
            }
        }

        for (int j = 0; j < TOTAL; j++)
            if (next_dp[j] > -1)
                dp[j] = next_dp[j];
    }

    cout << sum dp[0] << '\n';

    return 0;
}