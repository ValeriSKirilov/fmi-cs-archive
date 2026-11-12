#include <bits/stdc++.h>

using namespace std;

#define MAX 11000
#define MAXN 1100

int n, m, a, b, c, d[MAXN], visited = 0;

vector<int> gr[MAXN], sorted;
vector<pair<int, pair<int, int>>> edges;

bool topo()
{
    priority_queue<int, vector<int>, greater<int>> q;

    for (int i = 1; i <= n; i++)
        if (d[i] == 0)
            q.push(i);

    while (!q.empty())
    {
        int node = q.top();
        q.pop();

        sorted.push_back(node);
        visited++;

        for (auto x : gr[node])
        {
            d[x]--;
            if (d[x] == 0)
                q.push(x);
        }
    }

    return visited == n;
}

bool check(int rad)
{
    visited = 0;
    fill(d, d + n + 10, 0);
    sorted.clear();

    for (int i = 0; i <= n; i++)
        gr[i].clear();

    int i = 0;
    while (i < edges.size() && edges[i].first <= rad)
    {
        gr[edges[i].second.first].push_back(edges[i].second.second);
        d[edges[i].second.second]++;

        i++;
    }

    return topo();
}

int main()
{
    cin >> n >> m;

    for (int i = 0; i < m; i++)
    {
        cin >> a >> b >> c;

        edges.push_back({c, {b, a}});
    }

    sort(edges.begin(), edges.end());

    if (check(100005))
    {
        cout << "-1\n";

        for (auto x : sorted)
            cout << x << ' ';

        return 0;
    }

    int l = 1, r = 100005, ans = -1;
    while (l <= r)
    {
        int mid = (l + r) / 2;

        if (check(mid))
        {
            ans = mid;
            l = mid + 1;
        }
        else
            r = mid - 1;
    }

    check(ans);

    cout << ans << '\n';

    for (auto x : sorted)
        cout << x << ' ';

    return 0;
}