#include <bits/stdc++.h>

using namespace std;

#define MAX 10005

int n;

vector<int> a(MAX);

long long ans = 0;

void helper(int start, int target) {
  int l = start + 1, r = n - 1;

  while (l < r) {
    if (a[l] + a[r] < target)
      l++;
    else if (a[l] + a[r] > target)
      r--;
    else {
      if (a[l] == a[r]) {
        int N = r - l + 1;
        ans += N * (N - 1) / 2;
        break;
      } else {
        int count_l = 1, count_r = 1, idx_l = l + 1, idx_r = r - 1;

        while (a[idx_l] == a[l]) {
          count_l++;
          idx_l++;
        }

        while (a[idx_r] == a[r]) {
          count_r++;
          idx_r--;
        }

        l += count_l;
        r -= count_r;

        ans += count_l * count_r;
      }
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n;

  for (int i = 0; i < n; i++) cin >> a[i];

  sort(a.begin(), a.begin() + n);

  for (int i = 0; i < n; i++) {
    helper(i, -a[i]);
  }

  cout << ans << '\n';

  return 0;
}