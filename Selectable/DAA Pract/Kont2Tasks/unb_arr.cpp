#include <bits/stdc++.h>

using namespace std;

#define MAX 110000

int t, n, q, par[MAX], d[MAX], val[MAX], pos[MAX];

int find(int node) {
  if (par[node] == node) return node;
  return par[node] = find(par[node]);
}

void solve(int idx) {
  cin >> n >> q;
  fill(pos, pos + MAX, 0);

  for (int i = 1; i <= n; i++) {
    int a;
    cin >> a;
    if (pos[a] == 0) {
      pos[a] = i;
      val[i] = a;
      par[i] = i;
    } else {
      par[i] = pos[a];
    }
  }

  cout << "Case " << idx << ':' << '\n';

  for (int i = 0; i < q; i++) {
    int type, x, y, query_idx;
    cin >> type;

    if (type == 1) {
      cin >> x >> y;
      if (x == y || pos[x] == 0) continue;

      if (pos[y] == 0) {
        pos[y] = pos[x];
        val[pos[y]] = y;
        pos[x] = 0;
      } else {
        par[pos[x]] = pos[y];
        pos[x] = 0;
      }
    } else if (type == 2) {
      cin >> query_idx;
      cout << val[find(query_idx)] << '\n';
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  cin >> t;
  for (int i = 1; i <= t; i++) solve(i);

  return 0;
}