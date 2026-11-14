#include <bits/stdc++.h>

using namespace std;

#define MAX 100005

long long n, k;

vector<long long> a(MAX);

int bit_counts[62] = {0};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> k;

  for (int i = 0; i < n; i++) cin >> a[i];

  int r = 0;
  long long currSum = 0, ans = 0;

  for (int l = 0; l < n; l++) {
    while (r < n && currSum < k) {
      for (int i = 0; i < 60; i++) {
        if ((a[r] >> i) & 1) bit_counts[i]++;
      }

      currSum |= a[r];
      r++;
    }

    if (currSum >= k) {
      ans += (n - r + 1);
    }

    currSum = 0;

    for (int i = 0; i < 60; i++) {
      if ((a[l] >> i) & 1) bit_counts[i]--;

      if (bit_counts[i] > 0) currSum |= (1LL << i);
    }
  }

  cout << ans << '\n';

  return 0;
}