#include <bits/stdc++.h>

using namespace std;

#define MAX 100005

unsigned long long n, m;

vector<pair<long long, long>> intervals;

long long int_end = 0;

bool check(long long x) {
  long long coins_left = n - 1;
  long long last_placed = intervals[0].first, next_coin = last_placed + x;

  for (int i = 0; i < m; i++) {
    if (coins_left == 0) return true;

    if (last_placed + x > int_end) return false;

    if (next_coin < intervals[i].first) next_coin = intervals[i].first;

    while (next_coin <= intervals[i].second) {
      last_placed = next_coin;
      next_coin = last_placed + x;
      coins_left--;
    }
  }

  return coins_left <= 0;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> m;

  for (int i = 0; i < m; i++) {
    long long l, r;
    cin >> l >> r;

    intervals.push_back({l, r});
    int_end = max(int_end, r);
  }

  sort(intervals.begin(), intervals.begin() + m);

  long long l = 1, r = 1e18;
  long long ans;

  while (l <= r) {
    long long mid = (r + l) / 2;

    if (check(mid)) {
      ans = mid;
      l = mid + 1;
    } else
      r = mid - 1;
  }

  cout << ans << '\n';

  return 0;
}