#include <bits/stdc++.h>

using namespace std;

long long a[10005];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int n;
  cin >> n;

  for (int i = 0; i < n; ++i) cin >> a[i];

  for (int i = 0; i < n - 1; ++i) {
    int currIdx = i + 1;
    for (int j = i; j >= 0; --j) {
      if (a[currIdx] < a[j]) {
        swap(a[currIdx], a[j]);
        currIdx = j;
      }
    }

    for (int j = 0; j < n; ++j) cout << a[j] << " ";
    cout << endl;
  }

  return 0;
}