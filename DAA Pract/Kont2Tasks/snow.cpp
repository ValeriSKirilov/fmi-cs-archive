#include <bits/stdc++.h>

using namespace std;

#define MAX 110000
#define PA pair<double, int>

int n, m, a, b;
double c, dist[MAX];

vector<pair<int, double>> gr[MAX];
bool used[MAX];
vector<int> ans;
int parent[MAX];

void dijkstra(int node) {
  priority_queue<PA> pq;
  fill(dist, dist + n + 10, 0.0);

  dist[node] = 1.0;
  pq.push({dist[node], node});

  while (!pq.empty()) {
    node = pq.top().second;
    pq.pop();

    if (used[node]) continue;
    used[node] = true;

    for (auto [u, w] : gr[node]) {
      if (dist[u] < dist[node] * w) {
        dist[u] = dist[node] * w;
        pq.push({dist[u], u});
        parent[u] = node;
      }
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);

  cin >> n >> m;

  for (int i = 0; i < m; i++) {
    cin >> a >> b >> c;

    gr[a].push_back({b, (double)c / 1000});
    gr[b].push_back({a, (double)c / 1000});
  }

  cin >> a >> b;
  parent[a] = 0;

  dijkstra(a);

  int x = b;
  while (x != 0) {
    ans.push_back(x);
    x = parent[x];
  }
  reverse(ans.begin(), ans.end());

  for (auto node : ans) cout << node << ' ';
  cout << '\n';

  return 0;
}