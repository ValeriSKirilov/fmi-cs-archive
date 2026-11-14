#include <bits/stdc++.h>

using namespace std;
#define MAX 110

int num[MAX], dp[MAX], used[MAX];

int main() {
  int n, a, b;
  cin >> n >> a >> b;

  for (int i = 1; i <= n; i++) cin >> num[i];

  dp[0] = 1;

  for (int i = 1; i <= n; i++) {
    for (int j = b; j >= num[i]; j--) {
      if (dp[j - num[i]] == 1) dp[j] = 1;
    }
  }

  int count = 0;
  for (int i = a; i <= b; i++) count += dp[i];

  cout << count << '\n';

  return 0;
}