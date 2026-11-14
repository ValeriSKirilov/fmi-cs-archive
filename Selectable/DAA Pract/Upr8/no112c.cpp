#include <bits/stdc++.h>

using namespace std;

#define MAX 1005

int n, m, q;
vector<int> g[MAX];
bool visited[MAX];
int comp[MAX];
int curr;

void dfs(int v) {
  visited[v] = true;
  comp[v] = curr;

  for (auto u : g[v]) {
    if (!visited[u]) {
      dfs(u);
    }
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

  for (int i = 1; i <= n; i++) {
    if (!visited[i]) {
      curr++;
      dfs(i);
    }
  }

  cin >> q;
  for (int i = 0; i < q; i++) {
    cin >> u >> v;
    cout << (comp[u] == comp[v]);
  }

  cout << '\n';

  return 0;
}