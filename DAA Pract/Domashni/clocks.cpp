#include <bits/stdc++.h>

using namespace std;

#define MAX 200005

int n, q;
long long m;

vector<long long> a, b, t, ans;
vector<long long> int_starts, int_ends;

bool check_started(long long start, long long curr)
{
    return start <= curr;
}

bool check_ended(long long end, long long curr)
{
    return end < curr;
}

long long findAns(long long curr)
{
    if (int_starts.empty() || int_ends.empty())
        return 0;

    long long l1 = 0, r1 = int_starts.size() - 1, started = 0;
    while (l1 <= r1)
    {
        long long mid = l1 + (r1 - l1) / 2;

        if (check_started(int_starts[mid], curr))
        {
            started = mid + 1;
            l1 = mid + 1;
        }
        else
            r1 = mid - 1;
    }

    long long l2 = 0, r2 = int_ends.size() - 1, ended = 0;
    while (l2 <= r2)
    {
        long long mid = l2 + (r2 - l2) / 2;

        if (check_ended(int_ends[mid], curr))
        {
            ended = mid + 1;
            l2 = mid + 1;
        }
        else
            r2 = mid - 1;
    }

    return started - ended;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> m;

    for (int i = 0; i < n; i++)
    {
        long long ai, bi;
        cin >> ai >> bi;

        a.push_back(ai);
        b.push_back(bi);

        long long leftover = ai % m;
        if (leftover > 0)
            if (bi + leftover - 1 < m)
            {
                int_starts.push_back(bi);
                int_ends.push_back(bi + leftover - 1);
            }
            else
            {
                int_starts.push_back(bi);
                int_ends.push_back(m - 1);
                
                int_starts.push_back(0);
                int_ends.push_back((b[i] + leftover - 1) % m);
            }
    }

    sort(int_starts.begin(), int_starts.end());
    sort(int_ends.begin(), int_ends.end());

    cin >> q;

    long long fullDays = 0;
    for (int i = 0; i < n; i++)
        fullDays += a[i] / m;

    for (int i = 0; i < q; i++)
    {
        long long ti;
        cin >> ti;

        ans.push_back(fullDays + findAns(ti));
    }

    for (int i = 0; i < q; i++)
        cout << ans[i] << '\n';

    return 0;
}