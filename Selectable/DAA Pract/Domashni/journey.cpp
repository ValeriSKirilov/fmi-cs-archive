#include <bits/stdc++.h>

using namespace std;
#define MAX 1110000
#define PA pair<pair<long long, int>, int>

int n, m, k, a, b, c, type[MAX];
long long price[MAX];
pair<long long, int> dist[MAX];

vector<pair<long long, pair<int, int>>> edges;
vector<pair<long long, pair<long long, int>>> gr[MAX];

bool visited[MAX];

void dijkstra(int node) {
  priority_queue<PA, vector<PA>, greater<PA>> pq;
  fill(dist, dist + n + k + 100, pair<long long, int>{LLONG_MAX, 0});

  dist[node] = {0, 0};
  pq.push({dist[node], node});

  while (!pq.empty()) {
    node = pq.top().second;
    pq.pop();

    if (visited[node]) continue;
    visited[node] = true;

    for (auto [u, s] : gr[node]) {
      int w = s.first;
      int t = s.second;

      pair<long long, int> newNode = {dist[node].first + w,
                                      dist[node].second + t};

      if (dist[u] > newNode) {
        dist[u] = newNode;
        pq.push({dist[u], u});
      }
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> m >> k;

  for (int i = 1; i <= k; i++) {
    cin >> type[i];
  }

  for (int i = 1; i <= k; i++) {
    cin >> price[i];
  }

  for (int i = 0; i < m; i++) {
    cin >> a >> b >> c;
    edges.push_back({c, {a, b}});
  }

  sort(edges.begin(), edges.end());

  for (int i = 0; i < m; i++) {
    int u = edges[i].second.first;
    int v = edges[i].second.second;

    gr[u].push_back(
        {edges[i].first + n, {price[edges[i].first], type[edges[i].first]}});
    gr[edges[i].first + n].push_back({u, {0, 0}});

    gr[v].push_back(
        {edges[i].first + n, {price[edges[i].first], type[edges[i].first]}});
    gr[edges[i].first + n].push_back({v, {0, 0}});
  }

  dijkstra(1);

  cout << dist[n].first << ' ' << dist[n].second << '\n';
}