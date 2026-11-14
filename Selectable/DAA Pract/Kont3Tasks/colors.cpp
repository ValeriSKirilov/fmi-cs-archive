#include <bits/stdc++.h>

using namespace std;
#define MAX 3300
#define MOD 1000000007

long long dp[MAX][MAX];
int c[MAX];

int main() {
  int n, k;
  cin >> n >> k;

  for (int i = 1; i <= n; i++) {
    cin >> c[i];
  }

  if (c[1] == 0)
    fill(dp[1], dp[1] + k + 5, 1);
  else
    dp[1][c[1]] = 1;

  for (int i = 2; i <= n; i++) {
    long long currSum = 0;
    for (int j = 1; j <= k; j++) currSum = (currSum + dp[i - 1][j]) % MOD;

    if (c[i] == 0)
      for (int j = 1; j <= k; j++)
        dp[i][j] = (currSum - dp[i - 1][j] + MOD) % MOD;
    else
      dp[i][c[i]] = (currSum - dp[i - 1][c[i]] + MOD) % MOD;
  }

  long long ans = 0;
  for (int i = 1; i <= k; i++) ans = (ans + dp[n][i]) % MOD;

  cout << ans << '\n';

  return 0;
}