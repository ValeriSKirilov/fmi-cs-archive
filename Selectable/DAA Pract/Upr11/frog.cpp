#include <bits/stdc++.h>

using namespace std;

#define MAXN 110000

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int n;
  cin >> n;

  int h[MAXN];

  for (int i = 1; i <= n; i++) cin >> h[i];

  int dp[MAXN];

  dp[1] = 0;
  dp[2] = abs(h[2] - h[1]);

  for (int i = 3; i <= n; i++)
    dp[i] =
        min(dp[i - 1] + abs(h[i] - h[i - 1]), dp[i - 2] + abs(h[i] - h[i - 2]));

  cout << dp[n] << '\n';

  return 0;
}