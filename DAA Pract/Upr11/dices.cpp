#include <bits/stdc++.h>

using namespace std;

#define MAXN 10005
#define MOD 1000000007

long long dp[MAXN][MAXN];

// Вариант 1
long long calc(int s, int k) {
  dp[0][0] = 1;
  for (int i = 1; i <= k; i++) {
    for (int j = 1; j <= s; j++) {
      for (int l = 1; l <= min(6, j); l++) {
        dp[j][i] += dp[j - 1][i - l];
        dp[j][i] %= MOD;
      }
    }
  }

  return dp[s][k];
}

// Вариант 2
// long long rec(int s, int k)
// {
//     if (s == 0 && k == 0)
//         return dp[s][k] = 1;

//     if (dp[s][k] != -1)
//         return dp[s][k];

//     dp[s][k] = 0;
//     for (int i = 2; i <= min(6, s); i++)
//     {
//         dp[s][k] += rec(s - i, k - 1);
//         dp[s][k] %= MOD;
//     }

//     return dp[s][k];
// }

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int s, k;
  cin >> k >> s;

  // for (int i = 0; i <= s; i++)
  // {
  //     for (int j = 0; j <= k; j++)
  //     {
  //         dp[i][j] = -1;
  //     }
  // }

  cout << calc(s, k) << '\n';

  return 0;
}