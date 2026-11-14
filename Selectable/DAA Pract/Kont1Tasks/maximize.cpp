#include <bits/stdc++.h>

using namespace std;

#define MAX 500005

int q;
vector<int> S;
vector<double> results;
vector<long long> prefixSums;

bool check(long long x) {
  if (x == 0) return S[0] <= S.back();

  long long sum = prefixSums[x - 1] + S.back();

  double avg = (double)sum / (x + 1.0);

  return S[x] < avg;
}

double helper() {
  if (S.size() == 1) return 0;

  int l = 0, r = S.size() - 2;
  long long k = 0;

  while (l <= r) {
    long long mid = (l + r) / 2;

    if (check(mid)) {
      k = mid;
      l = mid + 1;
    } else
      r = mid - 1;
  }

  double ans = S.back() - (double)(prefixSums[k] + S.back()) / (k + 2.0);

  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> q;

  for (int i = 0; i < q; i++) {
    int type;
    cin >> type;

    if (type == 1) {
      int temp;
      cin >> temp;
      S.push_back(temp);
      if (!prefixSums.empty())
        prefixSums.push_back(prefixSums.back() + temp);
      else
        prefixSums.push_back(temp);
    } else if (type == 2)
      results.push_back(helper());
  }

  for (int i = 0; i < results.size(); i++)
    cout << fixed << setprecision(10) << results[i] << '\n';

  return 0;
}