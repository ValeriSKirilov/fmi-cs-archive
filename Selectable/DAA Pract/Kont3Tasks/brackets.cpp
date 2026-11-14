#include <bits/stdc++.h>

using namespace std;
#define MAX 2200
#define MOD 1001027

long long dp[MAX][MAX];
bool must[MAX];

int main() {
  int n, k;
  cin >> n >> k;

  int x;
  for (int i = 1; i <= k; i++) {
    cin >> x;
    must[x] = true;
  }

  dp[0][0] = 1;

  for (int i = 0; i < 2 * n; i++) {
    for (int j = 0; j <= n; j++) {
      if (must[i + 1]) {
        dp[i + 1][j + 1] += dp[i][j] % MOD;
      } else {
        if (j < n) dp[i + 1][j + 1] += dp[i][j] % MOD;

        if (j > 0) dp[i + 1][j - 1] += dp[i][j] % MOD;
      }
    }
  }

  cout << dp[2 * n][0] << '\n';

  return 0;
}