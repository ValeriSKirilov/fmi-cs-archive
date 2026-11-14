//
// Created by Lenovo on 19.2.2026 г..
//

#include <iostream>

using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);

  long long n, a;

  cin >> n;

  long long sum = 0;

  for (int i = 0; i < n; ++i) {
    cin >> a;
    sum += a;
  }

  cout << sum << '\n';

  return 0;
}
