#include <bits/stdc++.h>

using namespace std;

#define MAX 200005

long long n, k;
vector<long long> a(MAX), b(MAX);

long long countPairs(long long target) {
  long long count = 0;
  int j = n - 1;

  for (int i = 0; i < n; i++) {
    while (j >= 0 && a[i] + b[j] > target) j--;

    if (j >= 0) count += j + 1;
  }

  return count;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> k;

  for (int i = 0; i < n; i++) cin >> a[i];

  for (int i = 0; i < n; i++) cin >> b[i];

  sort(a.begin(), a.begin() + n);
  sort(b.begin(), b.begin() + n);

  long long low = a[0] + b[0], high = a[n - 1] + b[n - 1], ans = high;

  while (low <= high) {
    long long mid = (low + high) / 2;

    if (countPairs(mid) >= k) {
      high = mid - 1;
      ans = mid;
    } else
      low = mid + 1;
  }

  cout << ans << endl;

  return 0;
}
