#include <bits/stdc++.h>

using namespace std;

#define MAX 110000
#define PA pair<long long, long long>

long long n, m, a, b, c, dist[MAX], cost[MAX];
vector<PA> gr[MAX];

bool used[MAX];

priority_queue<PA, vector<PA>, greater<PA>> pq;

void dijkstra() {
  while (!pq.empty()) {
    int node = pq.top().second;
    int curr_cost = pq.top().first;
    pq.pop();

    if (curr_cost > cost[node]) continue;

    for (auto [u, w] : gr[node]) {
      if (cost[node] + cost[u] < cost[w]) {
        cost[w] = cost[node] + cost[u];
        pq.push({cost[w], w});
      }
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> m;

  for (int i = 1; i <= n; i++) {
    cin >> cost[i];
    pq.push({cost[i], i});
  }

  for (int i = 0; i < m; i++) {
    cin >> a >> b >> c;
    gr[b].push_back({c, a});
    gr[c].push_back({b, a});
  }

  dijkstra();

  cout << cost[1] << '\n';

  return 0;
}