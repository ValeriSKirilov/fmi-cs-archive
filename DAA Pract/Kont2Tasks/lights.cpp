#include <bits/stdc++.h>

using namespace std;

struct Edge {
  long long to, s, g, r;
};

#define MAX 110000
#define PA pair<long long, long long>

long long n, m, S, a, b, s, g, r, dist[MAX];

vector<Edge> gr[MAX];
bool visited[MAX];

void dijkstra(int node) {
  priority_queue<PA, vector<PA>, greater<PA>> pq;
  fill(dist, dist + n + 10, LLONG_MAX);

  dist[node] = S;
  pq.push({dist[node], node});

  while (!pq.empty()) {
    node = pq.top().second;
    pq.pop();

    if (visited[node]) continue;
    visited[node] = true;

    for (auto [u, s, g, r] : gr[node]) {
      long long temp = dist[node] % (g + r);
      if (dist[u] > dist[node] + s + (temp >= g ? g + r - temp : 0)) {
        dist[u] = dist[node] + s + (temp >= g ? g + r - temp : 0);
        pq.push({dist[u], u});
      }
    }
  }
}

int main() {
  cin >> n >> m >> S;

  for (int i = 0; i < m; i++) {
    cin >> a >> b >> s >> g >> r;

    gr[a].push_back({b, s, g, r});
    gr[b].push_back({a, s, g, r});
  }

  dijkstra(1);

  for (int i = 2; i <= n; i++) cout << dist[i] << ' ';
  cout << '\n';

  return 0;
}