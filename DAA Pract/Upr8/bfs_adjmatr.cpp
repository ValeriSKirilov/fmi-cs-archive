#include <bits/stdc++.h>

using namespace std;

int n, m;
const int maxn = 105;
bool a[maxn][maxn];
bool visited[maxn];

void bfs(int start) {
  queue<int> q;
  q.push(start);
  visited[start] = true;

  while (!q.empty()) {
    cout << q.front() << ' ';

    for (int i = 0; i < n; i++) {
      if (a[q.front()][i] && !visited[i]) {
        visited[i] = true;
        q.push(i);
      }
    }

    q.pop();
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
    a[u][v] = 1;
    a[v][u] = 1;
  }

  bfs(0);
  cout << endl;

  return 0;
}