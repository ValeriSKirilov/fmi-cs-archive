#include <bits/stdc++.h>

using namespace std;

#define MAX 200005

int n;

vector<long long> x(MAX);

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n;

    for (int i = 0; i < n; i++)
        cin >> x[i];

    sort(x.begin(), x.begin() + n);

    int l = 0;
    long long minEl1 = 0, minEl2 = 1;

    int ans = 2;

    for (int r = 2; r < n; r++)
    {
        while (r - l >= 2 && x[l] + x[l + 1] <= x[r])
        {
            l++;
        }

        ans = max(r - l + 1, ans);
    }

    cout << ans << '\n';

    return 0;
}