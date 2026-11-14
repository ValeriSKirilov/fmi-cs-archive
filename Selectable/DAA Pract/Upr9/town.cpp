#include <bits/stdc++.h>

using namespace std;

#define MAX 220000

int n, a, b, u, v, dist[MAX];
vector<pair<int, int>> gr[MAX];
bool used[MAX];

void BFS01(int node) {
  deque<pair<int, int>> q;

  fill(dist, dist + n + 10, INT_MAX / 100);
  dist[node] = 0;
  q.push_front({dist[node], node});

  while (!q.empty()) {
    node = q.front().second;
    q.pop_front();

    if (used[node]) continue;
    used[node] = true;

    for (auto [u, w] : gr[node]) {
      if (dist[u] > dist[node] + w) {
        dist[u] = dist[node] + w;

        if (dist[u] == dist[node] + 1)
          q.push_back({dist[u], u});
        else if (dist[u] == dist[node])
          q.push_front({dist[u], u});
      }
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> a >> b;

  while (cin >> u >> v) {
    gr[u].push_back({v, 0});
    gr[v].push_back({u, 1});
  }

  BFS01(a);

  if (dist[b] == INT_MAX / 100)
    cout << 'X' << '\n';
  else
    cout << dist[b] << '\n';

  return 0;
}