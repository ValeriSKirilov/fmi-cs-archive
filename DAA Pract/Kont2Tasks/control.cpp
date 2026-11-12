#include <bits/stdc++.h>

using namespace std;

#define MAX 1100000

long long n, m, d[MAX], sz[MAX], par[MAX], x, y;

vector<long long> gr[MAX];

long long counter, total_pairs;

long long find(long long node) {
  if (par[node] == node) return node;

  return par[node] = find(par[node]);
}

void myUnion(long long a, long long b) {
  a = find(a);
  b = find(b);

  if (a == b) return;

  counter += 1LL * sz[a] * sz[b];

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

  cin >> n >> m;

  total_pairs = 1LL * n * (n - 1) / 2;

  for (int i = 0; i < n; i++) {
    par[i] = i;
    sz[i] = 1;
  }

  for (int i = 0; i < m; i++) {
    cin >> x >> y;
    if (find(x) != find(y)) myUnion(x, y);
  }

  cout << total_pairs - counter;

  return 0;
}