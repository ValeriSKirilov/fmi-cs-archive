#include <bits/stdc++.h>

using namespace std;

#define MAX 1005

int n, m, k;
vector<pair<int, int>> g[MAX];
bool visited[MAX];

void dfs(int v) {
  visited[v] = true;

  for (auto u : g[v]) {
    if (!visited[u.first] && u.second >= k) {
      dfs(u.first);
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> m;

  int u, v, w;
  for (int i = 1; i <= m; i++) {
    cin >> u >> v >> w;
    g[u].push_back({v, w});
    g[v].push_back({u, w});
  }

  cin >> k;

  int count = 0;
  for (int i = 1; i <= n; i++) {
    if (!visited[i]) {
      count++;
      dfs(i);
    }
  }

  cout << count - 1 << endl;

  return 0;
}