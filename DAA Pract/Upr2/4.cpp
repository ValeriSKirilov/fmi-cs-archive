#include <bits/stdc++.h>

using namespace std;

int a[3000000];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int n;
  cin >> n;

  for (int i = 0; i < n; ++i) a[i] = 0;

  int lastIdx = 0;

  for (int i = 0; i < n; ++i) {
    int num;
    cin >> num;

    if (num > lastIdx) lastIdx = num;

    a[num]++;
  }

  for (int i = 0; i <= lastIdx; ++i) {
    if (a[i] > 0)
      for (int j = 0; j < a[i]; ++j) cout << i << ' ';
  }

  cout << endl;
}