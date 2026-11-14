#include <bits/stdc++.h>

using namespace std;

int t;

vector<int> candies;

void logic() {
  int n;
  cin >> n;

  for (int i = 0; i < n; i++) {
    int w;
    cin >> w;
    candies.push_back(w);
  }

  int ans = 0, l = 0, r = n - 1;
  long long currSumA = candies[l], currSumB = candies[r];

  while (l < r) {
    if (currSumA < currSumB) {
      l++;
      currSumA += candies[l];
    } else if (currSumA > currSumB) {
      r--;
      currSumB += candies[r];
    } else {
      ans = max(ans, (l + 1) + (n - r));
      l++;
      currSumA += candies[l];
    }
  }

  cout << ans << '\n';

  candies.clear();
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> t;

  for (int i = 1; i <= t; i++) logic();

  return 0;
}