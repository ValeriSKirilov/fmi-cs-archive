#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    long long A, K, B, M, X;

    cin >> A >> K >> B >> M >> X;

    long long low = 1;
    long long high = 2000000000000000000LL;
    long long ans = 0;

    while (low <= high)
    {
        long long mid = low + (high - low) / 2;

        long long tilesA = (mid - (mid / K));
        long long tilesB = (mid - (mid / M));

        __int128_t totalLines = (__int128_t)tilesA * A + (__int128_t)tilesB * B;

        if (totalLines >= X)
        {
            ans = mid;
            high = mid - 1;
        }
        else
            low = mid + 1;
    }

    cout << ans << endl;
}