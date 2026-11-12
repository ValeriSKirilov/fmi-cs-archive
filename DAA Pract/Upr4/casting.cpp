#include <bits/stdc++.h>

using namespace std;

const int maxn = 2e6 + 5;
int studios[maxn];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int n, m;
  cin >> n >> m;

  vector<pair<int, int>> singers(m);

  for (int i = 0; i < m; i++) cin >> singers[i].first >> singers[i].second;

  sort(singers.begin(), singers.end(),
       [](pair<int, int> p1, pair<int, int> p2) {
         if (p1.first != p2.first)
           return p1.first < p2.first;
         else
           return p1.second > p2.second;
       });

  for (int i = 0; i < m; i++) {
    int start = singers[i].first;
    int dur = singers[i].second;

    int best = 0;
    for (int j = 1; j < n; j++) {
      if (studios[best] > start && studios[j] < studios[best]) best = j;
    }
    if (studios[best] <= start)
      studios[best] = start + dur;
    else
      studios[best] += dur;
  }

  int ans = 0;

  for (int i = 1; i < n; i++) {
    if (studios[ans] <= studios[i]) ans = i;
  }

  cout << studios[ans] << " " << ans + 1;
}
