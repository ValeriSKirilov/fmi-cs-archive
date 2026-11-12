#include <bits/stdc++.h>

using namespace std;

#define MAX 1000
#define PA pair<int, pair<int, int>>

int n, m, dist[MAX][MAX];
char c;

string grid[MAX];
bool visited[MAX][MAX];

int mx[4] = {-1, 0, 0, 1}, my[4] = {0, -1, 1, 0};

void dijkstra(int x, int y) {
  priority_queue<PA, vector<PA>, greater<PA>> pq;
  for (int i = 0; i < n; i++) fill(dist[i], dist[i] + m + 10, INT_MAX);

  dist[x][y] = 0;
  pq.push({dist[x][y], {x, y}});

  while (!pq.empty()) {
    x = pq.top().second.first;
    y = pq.top().second.second;

    pq.pop();

    if (visited[x][y]) continue;
    visited[x][y] = true;

    for (int i = 0; i < 4; i++) {
      int next_r = x + mx[i];
      int next_c = y + my[i];
      if (next_r >= 0 && next_r < n && next_c >= 0 && next_c < m) {
        if (grid[next_r][next_c] == '*') continue;
        if (dist[next_r][next_c] >
            dist[x][y] + (grid[next_r][next_c] - 'A' + 1)) {
          dist[next_r][next_c] = dist[x][y] + (grid[next_r][next_c] - 'A' + 1);
          pq.push({dist[next_r][next_c], {next_r, next_c}});
        }
      }
    }
  }
}

int main() {
  cin >> n >> m;

  for (int i = 0; i < n; i++) {
    cin >> grid[i];
  }

  int start_x, start_y;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (grid[i][j] == '#') {
        start_x = i;
        start_y = j;
        break;
      }
    }
  }

  dijkstra(start_x, start_y);

  int ans = INT_MAX;

  for (int i = 0; i < m; i++) ans = min({ans, dist[0][i], dist[n - 1][i]});

  for (int i = 0; i < n; i++) ans = min({ans, dist[i][0], dist[i][m - 1]});

  cout << ans << '\n';

  return 0;
}