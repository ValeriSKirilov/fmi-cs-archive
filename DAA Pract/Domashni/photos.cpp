#include <bits/stdc++.h>

using namespace std;
#define MAX 2100

int n, m, dist[MAX][MAX];

pair<int, int> pesho, cat;

queue<pair<int, int>> pluses;
bool visited[MAX][MAX];

int mx[4] = {-1, 0, 0, 1}, my[4] = {0, -1, 1, 0};

void plusesBFS() {
  while (!pluses.empty()) {
    int x = pluses.front().first;
    int y = pluses.front().second;
    int curr = dist[x][y];

    pluses.pop();

    for (int i = 0; i < 4; i++) {
      if (x + mx[i] < 0 || x + mx[i] >= n || y + my[i] < 0 || y + my[i] >= m)
        continue;

      if (dist[x + mx[i]][y + my[i]] > curr + 1) {
        dist[x + mx[i]][y + my[i]] = curr + 1;
        pluses.push({x + mx[i], y + my[i]});
      }
    }
  }
}

bool peshoBFS(int x, int y, int r) {
  if (dist[x][y] < r) return false;

  queue<pair<int, int>> q;
  q.push({x, y});

  while (!q.empty()) {
    x = q.front().first;
    y = q.front().second;

    q.pop();

    // if (visited[x][y])
    //     continue;
    visited[x][y] = true;

    if (x == cat.first && y == cat.second) {
      return true;
    }

    for (int i = 0; i < 4; i++) {
      if (x + mx[i] < 0 || x + mx[i] >= n || y + my[i] < 0 || y + my[i] >= m)
        continue;

      if (!visited[x + mx[i]][y + my[i]] && dist[x + mx[i]][y + my[i]] >= r) {
        visited[x + mx[i]][y + my[i]] = true;
        q.push({x + mx[i], y + my[i]});
      }
    }
  }

  return false;
}

bool check(int r) {
  for (int i = 0; i < n; i++) fill(visited[i], visited[i] + m + 100, false);

  return peshoBFS(pesho.first, pesho.second, r);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> m;

  for (int i = 0; i < n; i++) fill(dist[i], dist[i] + m + 100, INT_MAX);

  char c;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cin >> c;

      if (c == '+') {
        pluses.push({i, j});
        dist[i][j] = 0;
      }

      if (c == 'P') pesho = {i, j};

      if (c == 'C') cat = {i, j};
    }
  }

  plusesBFS();

  int l = 0, r = 1e6, ans = 0;
  while (l <= r) {
    int mid = (l + r) / 2;

    if (check(mid)) {
      ans = mid;
      l = mid + 1;
    } else
      r = mid - 1;
  }

  cout << ans << '\n';
}