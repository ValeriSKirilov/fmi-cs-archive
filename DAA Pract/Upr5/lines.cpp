#include <bits/stdc++.h>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int n;
  cin >> n;

  vector<pair<long long, long long>> xCoords;
  vector<pair<long long, long long>> yCoords;

  xCoords.reserve(2 * n);
  yCoords.reserve(2 * n);

  for (int i = 0; i < n; ++i) {
    long long x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;

    long long minX = min(x1, x2);
    long long maxX = max(x1, x2);

    xCoords.push_back({minX, -1});
    xCoords.push_back({maxX, 1});

    long long minY = min(y1, y2);
    long long maxY = max(y1, y2);

    yCoords.push_back({minY, -1});
    yCoords.push_back({maxY, 1});
  }

  sort(xCoords.begin(), xCoords.end());
  sort(yCoords.begin(), yCoords.end());

  int currentXOverlaps = 0;
  int maxXInter = 0;

  for (const pair<long long, long long> &x : xCoords) {
    currentXOverlaps -= x.second;

    maxXInter = max(currentXOverlaps, maxXInter);
  }

  int currentYOverlaps = 0;
  int maxYInter = 0;

  for (const pair<long long, long long> &y : yCoords) {
    currentYOverlaps -= y.second;

    maxYInter = max(currentYOverlaps, maxYInter);
  }

  cout << max(maxXInter, maxYInter);
}