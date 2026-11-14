#include <bits/stdc++.h>

using namespace std;

#define MAX 110000

int dp[MAX][2];
int a[MAX];

int main() {
  int n;
  cin >> n;

  for (int i = 1; i <= n; i++) cin >> a[i];

  dp[1][0] = a[1];
  dp[1][1] = a[1];

  for (int i = 2; i <= n; i++) {
    dp[i][0] = max(dp[i - 1][0], dp[i - 1][1]) + a[i];
    dp[i][1] = dp[i - 2][0] + a[i];
  }

  int ans = INT_MIN;
  for (int i = 1; i <= n; i++) ans = max({ans, dp[i][0], dp[i][1]});

  cout << ans << '\n';

  return 0;
}