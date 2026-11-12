#include <bits/stdc++.h>

using namespace std;

#define MAX 100005

int n, k;

int dist[MAX];

bool check(long long x)
{
    int used_tvs = 0;
    int i = 0;

    while (i < n)
    {
        used_tvs++;

        int j = i;
        long long sum_left = 0;
        while (j + 1 < n && sum_left + dist[j] <= x)
        {
            sum_left += dist[j];
            j++;
        }

        int k = j;
        long long sum_right = 0;
        while (k + 1 < n && sum_right + dist[k] <= x)
        {
            sum_right += dist[k];
            k++;
        }

        i = k + 1;
    }

    return used_tvs <= k;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> k;

    for (int i = 0; i < n - 1; i++)
        cin >> dist[i];

    long long l = 1, r = 1e15, ans;

    while (l <= r)
    {
        long long mid = (l + r) / 2;

        if (check(mid))
        {
            ans = mid;
            r = mid - 1;
        }
        else
            l = mid + 1;
    }

    cout << ans << '\n';

    return 0;
}