#include <bits/stdc++.h>

using namespace std;

#define MAXN 100005
int n, k, s;
vector<int> popcorn(MAXN);

bool works(long long size)
{
    long long max_popcorns = size * s;
    int people_needed = 1;
    long long currSum = 0;

    for (int i = 0; i < n; i++)
    {
        if (popcorn[i] > max_popcorns)
            return false;

        if (currSum + popcorn[i] <= max_popcorns)
            currSum += popcorn[i];
        else
        {
            people_needed++;
            currSum = popcorn[i];
        }
    }

    return people_needed <= k;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> k >> s;

    for (int i = 0; i < n; i++)
    {
        cin >> popcorn[i];
    }

    long long l = 1, r = 1e10;
    long long ans;

    while (l <= r)
    {
        long long mid = (l + r) / 2;

        if (works(mid))
        {
            ans = mid;
            r = mid - 1;
        }
        else
            l = mid + 1;
    }

    cout << ans << endl;
}