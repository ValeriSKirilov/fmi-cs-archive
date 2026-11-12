#include <bits/stdc++.h>

using namespace std;

#define MAX 22000
#define PA pair<int, int>

int n, m, k, dist[MAX], a, b, c, d;

vector<PA> gr[MAX];
bool used[MAX];

void dijkstra(int node) {
  priority_queue<PA, vector<PA>, greater<PA>> pq;
  fill(dist, dist + n + 100, LLONG_MAX / 100);

  dist[node] = 0;
  pq.push({dist[node], node});

  while (!pq.empty()) {
    node = pq.top().second;
    pq.pop();

    if (used[node]) continue;
    used[node] = true;

    for (auto [u, w] : gr[node]) {
      if (dist[u] > dist[node] + w) {
        dist[u] = dist[node] + w;
        pq.push({dist[u], u});
      }
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);

  cin >> n >> m >> k;

  for (int i = 0; i < m; i++) {
    cin >> a >> b >> c >> d;

    gr[a].push_back({b, d - c});
    gr[b].push_back({a, d - c});
  }

  dijkstra(1);

  cout << (k - dist[n]);

  return 0;
}