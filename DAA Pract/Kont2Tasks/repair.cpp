#include <bits/stdc++.h>

using namespace std;

#define MAX 1100

int n, m, prices[MAX], a, b, c, d[MAX], sz[MAX], par[MAX], total_cost;

vector<pair<int, pair<int, int>>> roads;
vector<pair<int, int>> used_edges;

int find(int node)
{
    if (par[node] == 0)
        return node;

    return par[node] = find(par[node]);
}

void myUnion(int u, int v)
{
    u = find(u);
    v = find(v);

    if (u == v)
        return;

    if (d[u] > d[v])
    {
        par[v] = u;
        sz[u] += sz[v];
    }
    else
    {
        par[u] = v;
        sz[v] += sz[u];

        if (d[u] == d[v])
            d[v]++;
    }
}

int main()
{
    cin >> n >> m;

    for (int i = 1; i <= n; i++)
        cin >> prices[i];

    for (int i = 0; i < m; i++)
    {
        cin >> a >> b >> c;

        roads.push_back({c % 2 != 0 ? prices[min(a, b)] * (c / 2 + 1) + prices[max(a, b)] * (c / 2) : prices[a] * (c / 2) + prices[b] * (c / 2), {min(a, b), max(a, b)}});
    }

    sort(roads.begin(), roads.end());

    for (int i = 0; i < m; i++)
    {
        int temp1 = roads[i].second.first;
        int temp2 = roads[i].second.second;
        if (find(temp1) != find(temp2))
        {
            total_cost += roads[i].first;
            used_edges.push_back({roads[i].second});
            myUnion(temp1, temp2);
        }
    }

    cout << total_cost << '\n';

    sort(used_edges.begin(), used_edges.end());

    for (int i = 0; i < used_edges.size(); i++)
        cout << used_edges[i].first << ' ' << used_edges[i].second << '\n';

    return 0;
}