#include <bits/stdc++.h>

using namespace std;

#define MAX 110000
#define PA pair<long long, long long>

long long n, m, dist[MAX], a, b, c, k, q;
vector<PA> gr[MAX];

bool used[MAX];

void dijkstra(int node) {
  priority_queue<PA, vector<PA>, greater<PA>> pq;
  fill(dist, dist + n + 10, LLONG_MAX);

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
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> m;

  for (int i = 0; i < m; i++) {
    cin >> a >> b >> c;

    gr[a].push_back({b, c});
    gr[b].push_back({a, c});
  }

  cin >> k;

  for (int i = 0; i < k; i++) {
    cin >> a;
    gr[n].push_back({a, 0});
    gr[a].push_back({n, 0});
  }

  dijkstra(n);

  cin >> q;

  for (int i = 0; i < q; i++) {
    cin >> a;
    cout << dist[a] << '\n';
  }

  return 0;
}