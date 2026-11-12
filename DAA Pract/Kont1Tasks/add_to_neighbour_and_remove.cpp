#include <bits/stdc++.h>

using namespace std;

#define MAX 3005

int t;
vector<long long> answers(MAX);

long long logic() {
  int n;
  cin >> n;

  vector<long long> a(MAX);

  for (int i = 0; i < n; i++) cin >> a[i];

  long long S = 0;
  for (int i = 0; i < n; i++) S += a[i];

  for (int k = n; k >= 1; k--) {
    if (S % k != 0) continue;

    long long target_sum = S / k;
    long long curr_sum = 0;
    bool possible = true;

    for (int i = 0; i < n; i++) {
      curr_sum += a[i];

      if (curr_sum == target_sum)
        curr_sum = 0;
      else if (curr_sum > target_sum) {
        possible = false;
        break;
      }
    }

    if (possible) {
      return n - k;
    }
  }

  return 0;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> t;

  for (int i = 0; i < t; i++) answers[i] = logic();

  for (int i = 0; i < t; i++) cout << answers[i] << '\n';

  return 0;
}