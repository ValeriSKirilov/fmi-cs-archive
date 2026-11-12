#include <bits/stdc++.h>

using namespace std;

#define MAX 1100
#define MAX2 2000200
#define OFFSET 1000000

// dp[curr][sum]
bool dp[MAX2];
long long a[MAX];

int main()
{
    int n;
    cin >> n;

    for (int i = 1; i <= n; i++)
        cin >> a[i];

    for (int i = 1; i <= n; i++)
    {
        bool next_dp[MAX2] = {false};
        next_dp[a[i] + OFFSET] = true;
        for (int j = 0; j < MAX2; j++)
        {
            if (dp[j])
                next_dp[j + a[i]] = true;
        }

        for (int j = 0; j < MAX2; j++)
            if (next_dp[j])
                dp[j] = true;
    }

    long long count = 0;
    for (int i = 0; i < MAX2; i++)
    {
        count += dp[i];
    }

    cout << count << '\n';

    return 0;
}