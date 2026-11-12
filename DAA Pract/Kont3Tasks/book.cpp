#include <bits/stdc++.h>

using namespace std;
#define MAX 1100

double dp[MAX][MAX], price[MAX];
int pages[MAX];

int main() {
  int n, m;
  cin >> n >> m;

  for (int i = 1; i <= n; i++) cin >> pages[i] >> price[i];

  for (int i = 1; i <= n; i++) {
    for (int j = 0; j <= m; j++) {
      if (pages[i] > j)
        dp[i][j] = dp[i - 1][j];
      else
        dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - pages[i]] + price[i]);
    }
  }

  cout << fixed << setprecision(2) << dp[n][m] << '\n';

  return 0;
}
