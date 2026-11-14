#include <bits/stdc++.h>

using namespace std;

#define MAX 220000

int n, m, q, a, b;

vector<int> gr[MAX];
vector<pair<int, int>> holder1, holder2;
int color[MAX];

bool BFS(int node) {
  queue<int> q;
  q.push(node);
  color[node] = 1;

  while (!q.empty()) {
    node = q.front();
    q.pop();

    for (auto x : gr[node]) {
      if (color[x] == 0) {
        color[x] = 3 - color[node];
        q.push(x);
      } else if (color[x] == color[node])
        return false;
    }
  }

  return true;
}

bool solve(int days) {
  for (int i = 0; i < m; i++) {
    gr[holder1[i].first].push_back(holder1[i].second);
    gr[holder1[i].second].push_back(holder1[i].first);
  }

  for (int i = 0; i < days; i++) {
    gr[holder2[i].first].push_back(holder2[i].second);
    gr[holder2[i].second].push_back(holder2[i].first);
  }

  bool isBipar = true;

  for (int i = 1; i <= n; i++) {
    if (color[i] == 0 && !BFS(i)) return false;
  }

  return isBipar;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);

  cin >> n >> m >> q;

  for (int i = 0; i < m; i++) {
    cin >> a >> b;
    holder1.push_back({a, b});
  }

  for (int i = 0; i < q; i++) {
    cin >> a >> b;
    holder2.push_back({a, b});
  }

  if (!solve(0)) {
    cout << "-1" << '\n';
    return 0;
  }

  int left = 0, right = q, ans = -2;
  while (left <= right) {
    for (int i = 1; i <= n; i++) gr[i].clear();

    fill(color, color + n + 10, 0);
    int mid = (left + right) / 2;

    if (solve(mid))
      left = mid + 1;
    else {
      ans = mid;
      right = mid - 1;
    }
  }

  cout << ans << '\n';
  return 0;
}