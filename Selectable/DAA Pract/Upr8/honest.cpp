#include <bits/stdc++.h>

using namespace std;

#define MAX 10005

int n, m, k;
vector<pair<int, int>> g[MAX];
bool visited[MAX];

int cnt[2];

void dfs(int v, bool is_liar) {
  visited[v] = true;
  cnt[is_liar]++;

  for (auto u : g[v]) {
    if (!visited[u.first]) {
      dfs(u.first, (u.second != is_liar));
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> m;

  int u, v;
  char w;
  for (int i = 1; i <= m; i++) {
    cin >> u >> w >> v;
    g[u].push_back({v, (w == 'l')});
    g[v].push_back({u, (w == 'l')});
  }

  int ans = 0;
  for (int i = 1; i <= n; i++) {
    if (!visited[i]) {
      cnt[0] = cnt[1] = 0;
      dfs(i, 0);
      ans += max(cnt[0], cnt[1]);
    }
  }

  cout << ans << endl;

  return 0;
}