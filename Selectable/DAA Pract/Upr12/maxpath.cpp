#include <bits/stdc++.h>

using namespace std;

#define MAX 1000

vector<pair<int, int>> gr[MAX];
int dp[MAX], in[MAX];
vector<int> topo;

int main() {
  int n, m;
  cin >> n >> m;

  int u, v, w;
  for (int i = 1; i <= n; i++) {
    cin >> u >> v >> w;

    gr[u].push_back({v, w});
    in[v]++;
  }

  int s, t;
  cin >> s >> t;
  dp[s] = 0;
  queue<int> q;

  for (int i = 1; i <= n; i++) {
    if (in[i] == 0) q.push(i);
    dp[i] = INT_MIN;
  }

  while (!q.empty()) {
    int v = q.front();
    topo.push_back(v);
    q.pop();

    for (auto [u, w] : gr[v]) {
      in[u]--;
      if (in[u] == 0) q.push(u);
    }
  }

  for (auto v : topo) {
    for (auto [u, w] : gr[v]) {
      dp[u] = max(dp[u], dp[v] + w);
    }
  }

  cout << dp[t] << '\n';
}