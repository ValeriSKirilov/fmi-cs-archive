#include <bits/stdc++.h>

using namespace std;

#define MAX 5500

int a[MAX], dp[MAX];
int par[MAX];

int main()
{
    int n;
    cin >> n;

    for (int i = 1; i <= n; i++)
        cin >> a[i];

    dp[1] = 1;
    // par[1] = 1;
    for (int j = 2; j <= n; j++)
    {
        dp[j] = 1;
        // par[j] = j;
        for (int i = 1; i < j; i++)
        {
            if (a[i] < a[j])
            {
                dp[j] = max(dp[j], dp[i] + 1);
                // if (dp[j] < dp[i] + 1) // ALTERNATIVE FOR REBUILDING THE PATH
                // {
                //     dp[j] = dp[i] + 1;
                //     par[j] = i;
                // }}
            }
        }

        int ans = 0, best;
        for (int i = 1; i <= n; i++)
        {
            ans = max(ans, dp[i]);
            // if (ans < dp[i]) // ALTERNATIVE FOR REBUILDING THE PATH
            // {
            //     ans = dp[i];
            //     best = i;
            // }
        }

        vector<int> seq;
        while (best != 0)
        {
            seq.push_back(best);
            best = par[best];
        }

        cout << ans << '\n';
        reverse(seq.begin(), seq.end());

        for (auto x : seq)
            cout << x << ' ';
        cout << '\n';
    }