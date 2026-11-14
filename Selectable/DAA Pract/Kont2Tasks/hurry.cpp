#include <bits/stdc++.h>

using namespace std;

#define MAX 220000
#define PA pair<long long, long long>

long long t, n, m, u, v, a, b, dist[MAX];

vector<PA> gr[MAX];
bool visited[MAX];
vector<long long> ans;

void dijkstra(int node) {
  priority_queue<PA, vector<PA>, greater<PA>> pq;
  fill(dist, dist + 2 * n + 10, LLONG_MAX);

  dist[node] = 0;
  pq.push({dist[node], node});

  while (!pq.empty()) {
    node = pq.top().second;
    pq.pop();

    if (visited[node]) continue;
    visited[node] = true;

    for (auto [u, w] : gr[node]) {
      if (dist[u] > dist[node] + w) {
        dist[u] = dist[node] + w;
        pq.push({dist[u], u});
      }
    }
  }
}

void solve() {
  for (int i = 1; i <= 2 * n; i++) {
    gr[i].clear();
  }

  fill(visited, visited + 2 * n + 20, false);

  cin >> n >> m;

  for (int i = 0; i < m; i++) {
    cin >> u >> v >> a >> b;

    gr[u].push_back({v, a});
    gr[v].push_back({u, a});

    gr[u + n].push_back({v + n, b});
    gr[v + n].push_back({u + n, b});
  }

  for (int i = 1; i <= n; i++) {
    gr[i].push_back({i + n, 0});
  }

  dijkstra(1);

  ans.push_back(dist[2 * n]);
}

int main() {
  cin >> t;

  for (int i = 0; i < t; i++) {
    solve();
  }

  for (int i = 0; i < t; i++) {
    cout << ans[i] << '\n';
  }

  return 0;
}