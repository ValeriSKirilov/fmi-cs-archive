#include <bits/stdc++.h>

using namespace std;

#define MAX 22000

int k, a, b, d[MAX], uniq_cnt = 0;

vector<int> gr[MAX], ans;
bool used[MAX];

priority_queue<int, vector<int>, greater<int>> pq;

void topo() {
  for (int i = 1; i <= 20000; i++) {
    if (used[i] && d[i] == 0) pq.push(i);
  }

  while (!pq.empty()) {
    int node = pq.top();
    pq.pop();

    ans.push_back(node);
    for (auto adj : gr[node]) {
      d[adj]--;
      if (d[adj] == 0) pq.push(adj);
    }
  }

  // for (auto x : ans)
  //     cout << x << ' ';

  if (ans.size() == uniq_cnt) {
    cout << "1" << '\n';
  } else
    cout << '0' << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> k;

  for (int i = 0; i < k; i++) {
    cin >> a;
    vector<int> temp;

    for (int j = 0; j < a; j++) {
      cin >> b;

      if (!used[b]) {
        uniq_cnt++;
        used[b] = true;
      }

      temp.push_back(b);
    }

    for (int j = 0; j < a - 1; j++) {
      gr[temp[j]].push_back(temp[j + 1]);
      d[temp[j + 1]]++;
    }
  }

  topo();

  // cout << uniq_cnt << '\n';

  return 0;
}