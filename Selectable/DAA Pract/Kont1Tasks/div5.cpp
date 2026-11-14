#include <bits/stdc++.h>

using namespace std;

#define MAX 2000005

long long q;
vector<long long> l(MAX), r(MAX), ans(MAX);

int limit = 32000;
vector<bool> primes(limit + 1, true);

vector<long long> prime_powers;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> q;

  primes[0] = false;
  primes[1] = false;
  for (long long p = 2; p * p <= limit; p++) {
    if (primes[p])
      for (long long i = p * p; i <= limit; i += p) {
        primes[i] = false;
      }
  }

  for (long long i = 2; i <= limit; i++) {
    if (primes[i]) prime_powers.push_back(i * i * i * i);
  }

  for (int i = 0; i < q; i++) cin >> l[i];

  for (int i = 0; i < q; i++) cin >> r[i];

  for (int i = 0; i < q; i++) {
    auto start = lower_bound(prime_powers.begin(), prime_powers.end(), l[i]);
    auto end = upper_bound(prime_powers.begin(), prime_powers.end(), r[i]);

    ans[i] = distance(start, end);
  }

  for (int i = 0; i < q; i++) cout << ans[i] << ' ';

  cout << '\n';

  return 0;
}