#include <bits/stdc++.h>

using namespace std;

#define MAX 110000
#define PA pair<int, int>

int n, m, x, y, t, k, q, dist[MAX];

vector<PA> gr[MAX];

bool used[MAX];

void dejkstra(int node)
{
    priority_queue<PA, vector<PA>, greater<PA>> pq;
    fill(dist, dist + n + 10, INT_MAX / 10);

    dist[node] = 0;
    pq.push({dist[node], node});

    while (!pq.empty())
    {
        node = pq.top().second;
        pq.pop();

        if (used[node])
            continue;
        used[node] = true;

        for (auto [u, w] : gr[node])
        {
            if (dist[u] > dist[node] + w)
            {
                dist[u] = dist[node] + w;
                pq.push({dist[u], u});
            }
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> m;

    for (int i = 0; i < m; i++)
    {
        cin >> x >> y >> t;

        gr[x].push_back({y, t});
        gr[y].push_back({x, t});
    }

    cin >> k;

    for (int i = 0; i < k; i++)
    {
        cin >> x;
        gr[n].push_back({x, 0});
        gr[x].push_back({n, 0});
    }

    dejkstra(n);

    cin >> q;

    for (int i = 0; i < q; i++)
    {
        cin >> x;
        cout << dist[x] << '\n';
    }
}