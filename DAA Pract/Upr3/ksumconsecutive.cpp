#include <bits/stdc++.h>

using namespace std;

#define MAX 100005

long long n, k;

vector<long long> a(MAX);

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> k;

    for (int i = 0; i < n; i++)
        cin >> a[i];

    long long l = 0, ans = INT_MIN, temp_sum = 0;

    for (int r = 0; r < n; r++)
    {
        if (temp_sum + a[r] > k)
        {
            ans = max(ans, temp_sum);

            temp_sum += a[r];

            while (temp_sum > k)
            {
                temp_sum -= a[l];
                l++;
            }
        }
        else
            temp_sum += a[r];
    }

    if (temp_sum <= k)
        ans = max(ans, temp_sum);

    cout << ans << '\n';

    return 0;
}