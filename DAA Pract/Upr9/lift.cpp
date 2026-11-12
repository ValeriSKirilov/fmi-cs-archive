#include <bits/stdc++.h>

using namespace std;

#define MAX 1100000
#define PA pair<long long, long long>

long long n, u, d, i, j, l, k, a, dist[MAX];
vector<PA> gr[MAX];
bool used[MAX];

void dijkstra(long long node) {
  priority_queue<PA, vector<PA>, greater<PA>> pq;

  fill(dist, dist + 1100000, LLONG_MAX / 10);
  dist[node] = 0;
  pq.push({dist[node], node});

  while (!pq.empty()) {
    node = pq.top().second;
    pq.pop();

    if (used[node]) continue;
    used[node] = true;

    for (auto [adj, w] : gr[node]) {
      if (dist[adj] > dist[node] + w) {
        dist[adj] = dist[node] + w;
        pq.push({dist[adj], adj});
      }
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> u >> d >> i >> j >> l;

  vector<long long> floors;
  floors.push_back(1);
  floors.push_back(n);

  for (int p = 1; p <= l; p++) {
    long long dummy_node = 1000000 + p;

    cin >> k;
    for (int m = 0; m < k; m++) {
      cin >> a;
      gr[a].push_back({dummy_node, i});
      gr[dummy_node].push_back({a, j});

      floors.push_back(a);
    }
  }

  sort(floors.begin(), floors.end());
  floors.erase(unique(floors.begin(), floors.end()), floors.end());

  for (int i = 0; i < floors.size() - 1; i++) {
    gr[floors[i]].push_back({floors[i + 1], (floors[i + 1] - floors[i]) * u});
    gr[floors[i + 1]].push_back({floors[i], (floors[i + 1] - floors[i]) * d});
  }

  dijkstra(1);

  cout << dist[n] << '\n';

  return 0;
}