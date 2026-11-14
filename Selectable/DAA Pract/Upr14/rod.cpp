#include <bits/stdc++.h>

using namespace std;

#define MAX 550

int dp[MAX][MAX];
int a[MAX], pref[MAX];

int main() {
  int n;
  cin >> n;

  for (int i = 0; i < n; i++) cin >> a[i];

  pref[0] = a[0];

  for (int i = 1; i < n; i++) {
    pref[i] = pref[i - 1] + a[i];
  }

  for (int i = 0; i < n; i++) dp[i][i + 1] = 0;

  for (int len = 2; len <= n; len++) {
    for (int i = 0; i <= n - len; i++) {
      int j = i + len;
      dp[i][j] = INT_MAX;

      for (int k = i + 1; k < j; k++) {
        dp[i][j] =
            min(dp[i][j], dp[i][k] + dp[k][j] + (pref[j - 1] - pref[i - 1]));
      }
    }
  }

  cout << dp[0][n] << '\n';

  return 0;
}