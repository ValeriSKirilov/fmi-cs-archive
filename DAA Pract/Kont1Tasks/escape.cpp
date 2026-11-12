#include <bits/stdc++.h>

using namespace std;

struct Trip {
  long long times, amount;

  bool operator<(const Trip &other) const { return amount > other.amount; }
};

#define MAX 100005

unsigned long long n, k;

vector<pair<long long, long long>> a(MAX);

vector<Trip> trips;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> k;

  for (int i = 0; i < n; i++) cin >> a[i].first >> a[i].second;

  for (int i = 0; i < n; i++) {
    trips.push_back({a[i].first / a[i].second, a[i].second});
    trips.push_back({1, a[i].first % a[i].second});
  }

  sort(trips.begin(), trips.end());

  unsigned long long total = 0;
  long long idx = 0;

  while (k > 0 && idx < trips.size()) {
    total += min(k, (unsigned long long)trips[idx].times) * trips[idx].amount;
    k -= min(k, (unsigned long long)trips[idx].times);
    idx++;
  }

  cout << total << '\n';

  return 0;
}