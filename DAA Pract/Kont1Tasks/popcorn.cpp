#include <bits/stdc++.h>

using namespace std;

#define MAX 100005

int n, k, s;
vector<int> popcorn(MAX);

bool works(long long size)
{
    long long max = size * s;
    int needed = 1;
    long long currSum = 0;

    for (int i = 0; i < n; i++)
    {
        if (popcorn[i] > max)
            return false;

        if (currSum + popcorn[i] > max)
        {
            needed++;
            currSum = popcorn[i];
        }
        else
            currSum += popcorn[i];
    }

    return needed <= k;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> k >> s;

    for (int i = 0; i < n; i++)
        cin >> popcorn[i];

    long long l = 1, r = 1e10;
    long long ans;

    while (l <= r)
    {
        long long mid = (r + l) / 2;

        if (works(mid))
        {
            ans = mid;
            r = mid - 1;
        }
        else
            l = mid + 1;
    }

    cout << ans << endl;

    return 0;
}