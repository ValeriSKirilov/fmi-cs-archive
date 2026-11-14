#include <bits/stdc++.h>

using namespace std;

#define MAX 100005

int n, k;

vector<long long> prices(MAX);

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> k;

  for (int i = 0; i < n; i++) cin >> prices[i];

  sort(prices.begin(), prices.end());
  reverse(prices.begin(), prices.end());

  long long sum = 0;

  for (int i = 0; i < n; i++) {
    if ((i + 1) % k == 0) continue;

    sum += prices[i];
  }

  cout << sum << '\n';
  return 0;
}