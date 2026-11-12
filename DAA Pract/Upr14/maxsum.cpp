#include <bits/stdc++.h>

using namespace std;

#define MAXN 110000
#define MAXK 1100

long long dp_left[MAXN], dp_right[MAXN];
int a[MAXN], b[MAXK];

int main()
{
    int n, k;
    cin >> n >> k;

    for (int i = 1; i <= n; i++)
        cin >> a[i];

    for (int i = 0; i < k; i++)
        cin >> b[i];

    dp_left[1] = a[1];

    for (int i = 2; i <= n; i++)
    {
        dp_left[i] = LLONG_MIN;
        for (int j = 0; j < k; j++)
        {
            if (i - b[j] > 0)
                if (dp_left[i - b[j]] != LLONG_MIN)
                    dp_left[i] = max(dp_left[i - b[j]] + a[i], dp_left[i]);
        }
    }

    dp_right[n] = a[n];

    for (int i = n - 1; i >= 1; i--)
    {
        dp_right[i] = LLONG_MIN;
        for (int j = 0; j < k; j++)
        {
            if (i + b[j] <= n)
                if (dp_right[i + b[j]] != LLONG_MIN)
                    dp_right[i] = max(dp_right[i + b[j]] + a[i], dp_right[i]);
        }
    }

    long long total_max = LLONG_MIN;
    long long best_left = LLONG_MIN;

    for (int i = 2; i <= n; i++)
    {
        best_left = max(best_left, dp_left[i - 1]);
        if (dp_right[i] != LLONG_MIN && best_left != LLONG_MIN)
        {
            total_max = max(total_max, best_left + dp_right[i]);
        }
    }

    cout << total_max << '\n';

    return 0;
}