#include <bits/stdc++.h>

using namespace std;

#define MAX 110000
#define PA pair<long long, long long>

struct Edge {
  long long cost;
  int u, v;

  bool operator<(const Edge &other) const { return cost < other.cost; }
};

vector<Edge> edges;

int n, x, y, d[MAX], par[MAX], sz[MAX];

vector<PA> bug;
vector<PA> gr[MAX];

int find(int a) {
  if (par[a] == a) return a;

  return par[a] = find(par[a]);
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
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);

  cin >> n;

  for (int i = 0; i < n; i++) {
    cin >> x >> y;
    bug.push_back({x, y});
  }

  for (int i = 0; i < n; i++) par[i] = i;

  sort(bug.begin(), bug.end());

  int last_seen[11];
  for (int i = 0; i < 11; i++) {
    last_seen[i] = -1;
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 11; j++) {
      if (last_seen[j] != -1) {
        long long cost = 1LL * (bug[i].first - bug[last_seen[j]].first) *
                             (bug[i].first - bug[last_seen[j]].first) +
                         1LL * (bug[i].second - bug[last_seen[j]].second) *
                             (bug[i].second - bug[last_seen[j]].second);
        edges.push_back({cost, i, last_seen[j]});
      }
      if (bug[i].second == j) last_seen[j] = i;
    }
  }

  sort(edges.begin(), edges.end());

  long long total_cost = 0;

  for (int i = 0; i < edges.size(); i++) {
    x = edges[i].u;
    y = edges[i].v;

    if (find(x) != find(y)) {
      total_cost += edges[i].cost;
      myUnion(x, y);
    }
  }

  cout << total_cost << '\n';
}