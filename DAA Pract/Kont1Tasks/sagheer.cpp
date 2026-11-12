#include <bits/stdc++.h>

using namespace std;

#define MAX 100005

long long n, S;
vector<long long> a(MAX);
long long spent = 0, tempSum = 0;

bool check(int x) {
  vector<long long> temp(MAX);

  for (int i = 0; i < n; i++) temp[i] = a[i] + (long long)(i + 1) * x;

  sort(temp.begin(), temp.begin() + n);

  tempSum = 0;

  for (int i = 0; i < x; i++) tempSum += temp[i];

  return tempSum <= S;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> S;

  for (int i = 0; i < n; i++) cin >> a[i];

  int l = 0, r = n, ans = 0;

  while (l <= r) {
    int mid = (l + r) / 2;

    if (check(mid)) {
      ans = mid;
      spent = tempSum;
      l = mid + 1;
    } else
      r = mid - 1;
  }

  cout << ans << ' ' << spent << '\n';

  return 0;
}