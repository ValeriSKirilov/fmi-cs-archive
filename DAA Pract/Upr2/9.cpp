#include <bits/stdc++.h>

using namespace std;

#define MAX 2000000

int n, k;
long long a[MAX], b[MAX];

void merge(int l1, int r1, int l2, int r2, long long &count) {
  int idx1 = l1, idx2 = l2, pos = 0;
  long long local_inv = 0;

  while (idx1 <= r1 && idx2 <= r2) {
    if (a[idx1] <= a[idx2]) {
      b[pos] = a[idx1];
      pos++;
      idx1++;
    } else {
      b[pos] = a[idx2];
      pos++;
      idx2++;
      local_inv += (r1 - idx1 + 1);
    }
  }

  while (idx1 <= r1) {
    b[pos] = a[idx1];
    pos++;
    idx1++;
  }

  while (idx2 <= r2) {
    b[pos] = a[idx2];
    pos++;
    idx2++;
  }

  long long len1 = r1 - l1 + 1;
  long long len2 = r2 - l2 + 1;
  long long total_pairs = len1 * len2;

  count += min(local_inv, total_pairs - local_inv);

  for (int i = l1; i <= r2; i++) {
    a[i] = b[i - l1];
  }

  return;
}

void mergeSort(int l, int r, long long &count) {
  if (l == r) {
    return;
  }

  int mid = (l + r) / 2;

  mergeSort(l, mid, count);
  mergeSort(mid + 1, r, count);

  merge(l, mid, mid + 1, r, count);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> k;

  n = pow(2, k);

  for (int i = 0; i < n; ++i) cin >> a[i];

  long long count = 0;

  mergeSort(0, n - 1, count);

  cout << count << endl;

  return 0;
}