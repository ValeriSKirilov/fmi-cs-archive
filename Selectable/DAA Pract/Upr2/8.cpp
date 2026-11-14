#include <bits/stdc++.h>

using namespace std;

#define MAX 2000000

int n;
long long a[MAX], b[MAX];

void merge(int l1, int r1, int l2, int r2, long long &count) {
  int idx1 = l1, idx2 = l2, pos = 0;

  while (idx1 <= r1 && idx2 <= r2) {
    if (a[idx1] <= a[idx2]) {
      b[pos] = a[idx1];
      pos++;
      idx1++;
    } else {
      b[pos] = a[idx2];
      pos++;
      idx2++;
      count += (r1 - idx1 + 1);
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

  for (int i = l1; i <= r2; i++) {
    a[i] = b[i - l1];
  }

  // for (int i = 0; i < n; i++)
  //     cout << b[i] << ' ';
  // cout << endl;

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

  cin >> n;

  for (int i = 0; i < n; ++i) cin >> a[i];

  long long count = 0;

  mergeSort(0, n - 1, count);

  // for (int i = 0; i < n; i++)
  //     cout << a[i] << ' ';
  // cout << endl;

  cout << count << endl;

  return 0;
}