#include <bits/stdc++.h>

using namespace std;

struct Kid
{
    int id;
    int R;

    bool operator<(const Kid &other) const
    {
        return R > other.R;
    }
};

#define MAX 1000005

int n;

vector<vector<Kid>> starts_at(MAX);
int ans[MAX];

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n;

    for (int i = 0; i < n; i++)
    {
        int L, R;
        cin >> L >> R;

        starts_at[L].push_back({i, R});
    }

    priority_queue<Kid> pq;

    for (int i = 1; i <= n; i++)
    {
        for (Kid k : starts_at[i])
            pq.push(k);

        if (pq.empty())
        {
            cout << "NO\n";
            return 0;
        }

        Kid prio_kid = pq.top();

        if (prio_kid.R < i)
        {
            cout << "NO\n";
            return 0;
        }

        ans[prio_kid.id] = i;
        pq.pop();
    }

    cout << "YES\n";

    for (int i = 0; i < n; i++)
        cout << ans[i] << ' ';

    cout << '\n';

    return 0;
}