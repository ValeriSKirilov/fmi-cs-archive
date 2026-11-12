#include <bits/stdc++.h>

using namespace std;

#define MAX 110
#define MOD 1000000007

long long dp[MAX][MAX];
int a[MAX], pref[MAX];

int main() {
  string s;
  cin >> s;

  for (int i = 0; i < s.size(); i++) dp[i][i] = 1;

  for (int len = 1; len < s.size(); len++) {
    for (int i = 0; i < s.size() - len; i++) {
      int j = i + len;

      if (s[i] != s[j])
        dp[i][j] = (dp[i + 1][j] + dp[i][j - 1] - dp[i + 1][j - 1] + MOD) % MOD;
      else
        dp[i][j] = (dp[i + 1][j] + dp[i][j - 1] + 1) % MOD;
    }
  }

  cout << dp[0][s.size() - 1] << '\n';

  return 0;
}