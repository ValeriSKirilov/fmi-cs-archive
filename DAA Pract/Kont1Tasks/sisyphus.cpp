#include <bits/stdc++.h>

using namespace std;

#define MAX 100005

long long n, m, k;

long long a[MAX];

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> m >> k;

    for (int i = 0; i < n; i++)
        cin >> a[i];

    vector<long long> D(n + 2, 0);

    for (int i = 0; i < m; i++)
    {
        int l, r;

        cin >> l >> r;

        D[l]++;
        D[r + 1] -= 1;
    }

    vector<long long> c(n + 2, 0);
    long long total = 0;

    for (int i = 1; i <= n; i++)
    {
        total += D[i];
        c[i] = total;
    }

    vector<pair<long long, long long>> pairs;

    for (int i = 0; i < n; i++)
    {
        pairs.push_back({c[i + 1], a[i]});
    }

    sort(pairs.begin(), pairs.end());
    reverse(pairs.begin(), pairs.end());

    int idx = 0;
    while (k > 0 && idx < n)
    {
        long long subtract = min(k, pairs[idx].second);
        pairs[idx].second -= subtract;
        k -= subtract;
        idx++;
    }

    unsigned long long sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += pairs[i].first * pairs[i].second;
    }

    cout << sum << '\n';

    return 0;
}