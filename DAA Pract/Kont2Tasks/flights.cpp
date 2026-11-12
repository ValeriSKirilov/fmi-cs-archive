#include <bits/stdc++.h>

using namespace std;
#define MAX 22000
#define PA pair<int, int>

int n, m, a, b, c, dist[MAX];

vector<PA> gr[MAX];
bool visited[MAX];

void dijkstra(int node)
{
    priority_queue<PA> pq;
    fill(dist, dist + n + 10, 0);

    dist[node] = INT_MAX;
    pq.push({dist[node], node});

    while (!pq.empty())
    {
        node = pq.top().second;
        pq.pop();

        if (visited[node])
            continue;
        visited[node] = true;

        for (auto [u, w] : gr[node])
        {
            if (dist[u] < min(dist[node], w))
            {
                dist[u] = min(dist[node], w);
                pq.push({dist[u], u});
            }
        }
    }
}

int main()
{
    cin >> n >> m;

    for (int i = 0; i < m; i++)
    {
        cin >> a >> b >> c;

        gr[a].push_back({b, c});
        gr[b].push_back({a, c});
    }

    dijkstra(0);

    for (int i = 1; i < n; i++)
        cout << dist[i] << ' ';
    cout << '\n';
}