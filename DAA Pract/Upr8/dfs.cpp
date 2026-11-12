#include <bits/stdc++.h>

using namespace std;

int n, m;
const int maxn = 105;
vector<int> g[maxn];
bool visited[maxn];

void dfs(int v) {
  visited[v] = true;

  cout << v << ' ';

  for (auto u : g[v]) {
    if (!visited[u]) dfs(u);
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> m;

  int u, v;
  for (int i = 1; i <= m; i++) {
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }

  for (int i = 0; i < n; i++) sort(g[i].begin(), g[i].end());

  dfs(0);
  cout << endl;

  return 0;
}