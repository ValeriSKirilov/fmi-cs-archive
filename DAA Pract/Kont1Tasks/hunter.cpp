#include <bits/stdc++.h>

using namespace std;

#define MAX 1000005

long long n, l, d, f, a, b;

long long traps[MAX];

int b_min[MAX + 10];
int b_max[MAX + 10];

bool check(long long x)
{
    if (l > (x + 1) * d)
        return false;

    int num_buckets = l / d + 1;

    for (int i = 0; i < num_buckets; i++)
    {
        b_min[i] = -1;
        b_max[i] = -1;
    }

    for (int i = 0; i < x; i++)
    {
        long long pos = traps[i];
        int idx = pos / d;

        if (b_min[idx] == -1 || pos < b_min[idx])
            b_min[idx] = pos;
        if (b_max[idx] == -1 || pos > b_max[idx])
            b_max[idx] = pos;
    }

    long long prev_max = 0;

    for (int i = 0; i < num_buckets; i++)
    {
        if (b_min[i] != -1)
        {
            if (b_min[i] - prev_max > d)
                return false;

            prev_max = b_max[i];
        }
    }

    if (l - prev_max > d)
        return false;

    return true;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> l >> d;
    cin >> f >> a >> b;

    traps[0] = f;

    for (int i = 1; i < n; i++)
        traps[i] = (traps[i - 1] * a + b) % (l + 1);

    long long left = 1, r = n;
    long long ans = -1;

    while (left <= r)
    {
        long long mid = (left + r) / 2;

        if (check(mid))
        {
            ans = mid;
            r = mid - 1;
        }
        else
            left = mid + 1;
    }

    cout << ans << '\n';

    return 0;
}