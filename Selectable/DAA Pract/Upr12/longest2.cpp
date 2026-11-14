#include <bits/stdc++.h>

using namespace std;

#define MAX 110000

int n, a[MAX], dp[MAX], par[MAX], idx[MAX];

void LIS() {
  int ans = 0;
  fill(dp, dp + n + 2, INT_MAX);
  for (int i = 1; i <= n; i++) {
    int l = 1, r = n, opt, mid;
    while (l <= r) {
      mid = (l + r) / 2;
      if (a[i] <= dp[mid]) {
        r = mid - 1;
        opt = mid;
      } else {
        l = mid + 1;
      }
    }
    dp[opt] = a[i];
    // idx[opt] = i;
    // par[i] = idx[opt - 1];
    ans = max(ans, opt);
  }

  vector<int> seq;
  int best = idx[ans];
  while (best != 0) {
    seq.push_back(best);
    best = par[best];
  }

  cout << ans << '\n';

  reverse(seq.begin(), seq.end());

  for (auto x : seq) cout << x << ' ';
  cout << '\n';
  return;
}

int main() {
  cin >> n;

  for (int i = 1; i <= n; i++) cin >> a[i];

  LIS();

  return 0;
}