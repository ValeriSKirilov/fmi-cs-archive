#include <bits/stdc++.h>

using namespace std;

int n;
long long k, minlen = LLONG_MAX;

vector<long long> L, R, L_prefix, R_prefix;

bool checkL(long long idx, long long boundary) { return L[idx] > boundary; }

bool checkR(long long idx, long long boundary, long long intLen) {
  return R[idx] < boundary + intLen - 1;
}

long long calc(long long y, long long x) {
  long long used = 0;

  long long Ll = 0, Lr = L.size() - 1, ans1 = L.size();
  while (Ll <= Lr) {
    long long mid = Ll + (Lr - Ll) / 2;

    if (checkL(mid, y)) {
      Lr = mid - 1;
      ans1 = mid;
    } else
      Ll = mid + 1;
  }

  long long countL = L.size() - ans1;
  long long beforeYCost = 0;
  if (countL > 0) {
    beforeYCost =
        L_prefix[n - 1] - (ans1 > 0 ? L_prefix[ans1 - 1] : 0) - (countL * y);
  }
  used += beforeYCost;

  long long Rl = 0, Rr = R.size() - 1, ans2 = -1;
  while (Rl <= Rr) {
    long long mid = Rl + (Rr - Rl) / 2;

    if (checkR(mid, y, x)) {
      Rl = mid + 1;
      ans2 = mid;
    } else
      Rr = mid - 1;
  }

  long long countR = ans2 + 1;
  long long afterYCost = 0;
  if (countR > 0) {
    afterYCost = (countR * y) - R_prefix[countR - 1] + countR * x - countR;
  }
  used += afterYCost;

  return used;
}

bool check(long long x) {
  long long left = 0, right = 1e9;
  while (right - left > 2) {
    long long m1 = left + (right - left) / 3;
    long long m2 = right - (right - left) / 3;

    if (calc(m1, x) < calc(m2, x))
      right = m2;
    else
      left = m1;
  }

  long long min_cost = LLONG_MAX;
  for (long long i = left; i <= right; i++) {
    min_cost = min(min_cost, calc(i, x));
  }

  return min_cost <= k;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> k;

  for (int i = 0; i < n; i++) {
    long long a, b;
    cin >> a >> b;

    L.push_back(a);
    R.push_back(b);

    minlen = min(minlen, b - a + 1);
  }

  sort(L.begin(), L.end());
  sort(R.begin(), R.end());

  L_prefix.push_back(L[0]);
  R_prefix.push_back(R[0]);

  for (int i = 1; i < L.size(); i++) {
    L_prefix.push_back(L_prefix[i - 1] + L[i]);
    R_prefix.push_back(R_prefix[i - 1] + R[i]);
  }

  long long low = 1, high = minlen, ans = 0;
  while (low <= high) {
    long long mid = low + (high - low) / 2;

    if (check(mid)) {
      ans = mid;
      low = mid + 1;
    } else
      high = mid - 1;
  }

  cout << ans << '\n';

  return 0;
}