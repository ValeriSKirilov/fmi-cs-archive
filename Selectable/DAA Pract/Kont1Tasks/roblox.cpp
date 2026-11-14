#include <bits/stdc++.h>

using namespace std;

#define MAX 10000005

long long n, k;

vector<int> a(MAX);

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> k;

  for (int i = 0; i < n; i++) cin >> a[i];

  unsigned long long s = 0, t = 0;

  long long total_intervals = 0;

  int l = 0;
  for (int r = 0; r < n; r++) {
    t = t + (s * a[r]);
    s += a[r];

    while (t >= k && l <= r) {
      total_intervals += n - r;

      s = s - a[l];
      t = t - (s * a[l]);

      l++;
    }
  }

  cout << total_intervals << '\n';

  return 0;
}