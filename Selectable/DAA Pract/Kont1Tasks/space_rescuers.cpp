#include <bits/stdc++.h>

using namespace std;

#define MAX 105

struct Point {
  double x, y, z;

  double distanceTo(const Point &other) const {
    double dx = x - other.x;
    double dy = y - other.y;
    double dz = z - other.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
  }
};

int n;

vector<Point> planets(MAX);
Point best_point;
double min_overall_cost = 1e18;

double cost(Point current_guess) {
  double max_dist = 0;
  for (int i = 0; i < n; i++)
    max_dist = max(max_dist, current_guess.distanceTo(planets[i]));

  if (max_dist < min_overall_cost) {
    min_overall_cost = max_dist;
    best_point = current_guess;
  }

  return max_dist;
}

double search_z(double x, double y) {
  double l = -10000.0, r = 10000.0;

  for (int i = 0; i < 60; i++) {
    double m1 = l + (r - l) / 3.0;
    double m2 = r - (r - l) / 3.0;

    if (cost({x, y, m1}) < cost({x, y, m2}))
      r = m2;
    else
      l = m1;
  }

  return cost({x, y, l});
}

double search_y(double x) {
  double l = -10000.0, r = 10000.0;

  for (int i = 0; i < 60; i++) {
    double m1 = l + (r - l) / 3.0;
    double m2 = r - (r - l) / 3.0;

    if (search_z(x, m1) < search_z(x, m2))
      r = m2;
    else
      l = m1;
  }

  return search_z(x, l);
}

double search_x() {
  double l = -10000.0, r = 10000.0;

  for (int i = 0; i < 60; i++) {
    double m1 = l + (r - l) / 3.0;
    double m2 = r - (r - l) / 3.0;

    if (search_y(m1) < search_y(m2))
      r = m2;
    else
      l = m1;
  }

  return search_y(l);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n;

  for (int i = 0; i < n; i++) {
    double x, y, z;
    cin >> x >> y >> z;

    planets[i] = {x, y, z};
  }

  search_x();

  cout << fixed << setprecision(10) << best_point.x << ' ' << best_point.y
       << ' ' << best_point.z << '\n';

  return 0;
}