#include <bits/stdc++.h>

using namespace std;
#define MAX 3300

int dp[MAX][MAX];
int matrix[MAX][MAX];

int main() {
  int n, m;
  cin >> n >> m;

  string s;
  for (int i = 1; i <= n; i++) {
    cin >> s;
    for (int j = 1; j <= m; j++) {
      matrix[i][j] = s[j - 1] - '0';
    }
  }

  for (int i = 1; i <= n; i++) dp[i][1] = matrix[i][1];

  for (int i = 1; i <= m; i++) dp[1][i] = matrix[1][i];

  for (int i = 2; i <= n; i++) {
    for (int j = 2; j <= m; j++) {
      if (matrix[i][j] == 1) {
        int offset = 1;
        while (i + offset <= n && j + offset <= m && i - offset >= 1 &&
               j - offset >= 1 && matrix[i + offset][j + offset] == 1 &&
               matrix[i - offset][j - offset] == 1 &&
               matrix[i - offset][j + offset] == 1 &&
               matrix[i + offset][j - offset] == 1) {
          offset++;
        }
        dp[i][j] = 2 * (offset - 1) + 1;
      }
    }
  }

  int ans = INT_MIN;
  for (int i = 1; i <= n; i++)
    for (int j = 1; j <= m; j++) ans = max(ans, dp[i][j]);

  cout << ans << '\n';
  return 0;
}