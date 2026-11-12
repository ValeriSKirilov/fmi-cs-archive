#include <bits/stdc++.h>

using namespace std;

#define MAX 100005

int n, p;

vector<pair<long long, long long>> factories(MAX);

bool check(long long x) {
  long long details_produced = 0;

  for (int i = 0; i < n; i++) {
    long long usable_days = max((long long)0, x - factories[i].second);

    details_produced += floor(usable_days / factories[i].first);
  }

  return details_produced >= p;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> p;

  for (int i = 0; i < n; i++) cin >> factories[i].first >> factories[i].second;

  long long l = 1, r = 1e15;
  long long ans;

  while (l <= r) {
    long long mid = (l + r) / 2;

    if (check(mid)) {
      ans = mid;
      r = mid - 1;
    } else
      l = mid + 1;
  }

  cout << ans << '\n';
}