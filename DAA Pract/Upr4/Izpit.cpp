#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    long long n, F;
    cin >> n >> F;

    long double k = (1 - 2 * F + sqrtl((long double)(2 * F - 1) * (2 * F - 1) + 8.0L * n)) / 2.0L;

    cout << (long long)ceil(k);
}