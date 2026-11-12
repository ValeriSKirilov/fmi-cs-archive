#include <bits/stdc++.h>

using namespace std;

#define MAX 100005

long long n, k;

vector<long long> atom(MAX);

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> k;

  for (int i = 0; i < n; i++) cin >> atom[i];

  sort(atom.begin(), atom.begin() + n);

  long long ans = 0;
  int l = 0;

  for (int r = 0; r < n; r++) {
    while (atom[r] - atom[l] > k) l++;

    long long currSize = r - l + 1;
    ans = max(ans, currSize);
  }

  cout << ans << '\n';

  return 0;
}