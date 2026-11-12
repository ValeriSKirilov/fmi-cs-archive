#include <bits/stdc++.h>

using namespace std;

#define MAX 60005

int n;
vector<double> starts(MAX), speeds(MAX);

bool check(double time)
{
    double max_left = -1e18;
    double min_right = 1e18;

    for (int i = 0; i < n; i++)
    {
        double current_left = starts[i] - time * speeds[i];
        double current_right = starts[i] + time * speeds[i];

        max_left = max(max_left, current_left);
        min_right = min(min_right, current_right);
    }

    return max_left <= min_right;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n;

    for (int i = 0; i < n; i++)
        cin >> starts[i];

    for (int i = 0; i < n; i++)
        cin >> speeds[i];

    double l = 0.0, r = 1e9;

    for (int i = 0; i < 100; i++)
    {
        double mid = (l + r) / 2.0;

        if (check(mid))
            r = mid;
        else
            l = mid;
    }

    cout << fixed << setprecision(12) << r << '\n';

    return 0;
}