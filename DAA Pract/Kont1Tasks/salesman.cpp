#include <bits/stdc++.h>

using namespace std;

struct Stock
{
    int idx;
    double a, b;

    bool operator<(const Stock &other) const
    {
        if (b != other.b)
            return b > other.b;

        return a > other.a;
    }
};

#define MAX 100

double m;
int n;

vector<Stock> spices;
vector<double> amounts(MAX, 0);

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> m >> n;

    for (int i = 0; i < n; i++)
    {
        double a, b;
        cin >> a >> b;

        spices.push_back({i, a, b});
    }

    sort(spices.begin(), spices.end());

    double sum = 0;
    int idx = 0;

    while (m > 0 && idx < n)
    {
        double sub = min(m, spices[idx].a);

        amounts[spices[idx].idx] += sub;

        sum += sub * spices[idx].b;
        m -= sub;

        idx++;
    }

    cout << fixed << setprecision(2) << sum << '\n';

    for (int i = 0; i < n; i++)
        cout << fixed << setprecision(3) << amounts[i] << '\n';

    return 0;
}
