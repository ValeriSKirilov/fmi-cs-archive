#include <bits/stdc++.h>

using namespace std;

#define MAX 90

long long dp[MAX];

int main() {
  int n, k;
  cin >> n >> k;

  for (int i = 2; i <= k; i++) dp[i] = 1;

  for (int i = 2; i <= n; i++) {
    if (dp[i] > 0) {
      for (int j = 2; j <= k; j++) {
        if (i + j <= n) dp[i + j] += dp[i];

        if (i * j <= n) dp[i * j] += dp[i];
      }
    }
  }

  cout << dp[n] << '\n';

  return 0;
}