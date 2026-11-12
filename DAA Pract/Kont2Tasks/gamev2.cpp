#include <bits/stdc++.h>

using namespace std;

#define MAX 22000
#define PA pair<long long, long long>

long long n, m, k, a, b, c, d, dist[MAX];

vector<PA> gr[MAX];
bool visited[MAX];

void dijkstra(int node)
{
    priority_queue<PA, vector<PA>, greater<PA>> pq;
    fill(dist, dist + n + 10, INT_MAX);

    dist[node] = 0;
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
    cin >> n >> m >> k;

    for (int i = 0; i < m; i++)
    {
        cin >> a >> b >> c >> d;

        gr[a].push_back({b, d - c});
        gr[b].push_back({a, d - c});
    }

    dijkstra(1);

    cout << k - dist[n] << '\n';

    return 0;
}