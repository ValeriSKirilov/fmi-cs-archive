#include <bits/stdc++.h>

using namespace std;

#define MAX 110000

long long n, m, c[MAX], d[MAX], sz[MAX], par[MAX], x, y;

vector<pair<long long, int>> hills;
vector<pair<long long, pair<int, int>>> edges;

long long find(long long node) {
  if (par[node] == 0) return node;

  return par[node] = find(par[node]);
}

void myUnion(int a, int b) {
  a = find(a);
  b = find(b);

  if (a == b) return;

  if (d[a] > d[b]) {
    par[b] = a;
    sz[a] += sz[b];
  } else {
    par[a] = b;
    sz[b] += sz[a];

    if (d[a] == d[b]) d[b]++;
  }
}

int main() {
  cin >> n >> m;

  for (int i = 1; i <= n; i++) {
    cin >> x;
    c[i] = x;
    hills.push_back({c[i], i});
  }

  sort(hills.begin(), hills.end());

  for (int i = 1; i <= n; i++) {
    par[i] = 0;
    sz[i] = 1;
  }

  for (int i = 0; i < m; i++) {
    cin >> x >> y;
    if (find(x) != find(y)) myUnion(x, y);
  }

  for (int i = 0; i < n - 1; i++) {
    edges.push_back({hills[i + 1].first - hills[i].first,
                     {hills[i].second, hills[i + 1].second}});
  }

  sort(edges.begin(), edges.end());

  long long cost = 0;

  for (int i = 0; i < edges.size(); i++) {
    int u = edges[i].second.first;
    int v = edges[i].second.second;

    if (find(u) != find(v)) {
      cost += edges[i].first;
      myUnion(u, v);
    }
  }

  cout << cost << '\n';

  return 0;
}