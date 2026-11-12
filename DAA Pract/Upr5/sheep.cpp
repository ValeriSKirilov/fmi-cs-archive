#include <bits/stdc++.h>

using namespace std;

#define MAXN 1001
int n, k;
vector<int> sheep(MAXN);
bool used[MAXN];

bool works(long long size) {
  fill(used, used + n, false);
  int usedEl = 0;
  for (int i = 0; i < k; i++) {
    long long sum = 0;
    for (int j = 0; j < n; j++) {
      if (!used[j] && sum + sheep[j] <= size) {
        used[j] = true;
        sum += sheep[j];
        usedEl++;
      }
    }
  }

  return usedEl == n;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> k;

  for (int i = 0; i < n; i++) {
    cin >> sheep[i];
  }

  sort(sheep.begin(), sheep.end());
  reverse(sheep.begin(), sheep.end());

  long long l = 1, r = 1e8;
  long long ans;

  while (l <= r) {
    long long mid = (l + r) / 2;

    if (works(mid)) {
      ans = mid;
      r = mid - 1;
    } else
      l = mid + 1;
  }

  cout << ans << endl;
}