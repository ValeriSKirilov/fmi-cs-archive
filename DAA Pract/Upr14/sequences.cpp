#include <bits/stdc++.h>

using namespace std;

#define MAX 55

// dp[length][sum]
long long dp[MAX][MAX];

int main() {
  int p, n, s;
  cin >> p >> n >> s;

  for (int i = 0; i < p; i++) {
    if (i < s) dp[1][i] = 1;
  }

  for (int i = 2; i <= n; i++) {
    for (int j = 0; j < s; j++) {
      for (int k = 0; k < p; k++)
        if (j - k >= 0) dp[i][j] += dp[i - 1][j - k];
    }
  }

  long long count = 0;
  for (int i = 0; i < s; i++) {
    count += dp[n][i];
  }

  cout << count << '\n';

  return 0;
}