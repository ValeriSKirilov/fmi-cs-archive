#include <bits/stdc++.h>

using namespace std;

long long a[300000];

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int n, r;
    cin >> n >> r;

    for (int i = 0; i < n; i++)
        cin >> a[i];

    int lFlag = 0, rFlag = 1;
    long long monuments = 0;
    while (lFlag != n)
    {
        if (a[rFlag] - a[lFlag] > r)
        {
            monuments += n - rFlag;
            lFlag++;
        }
        else if (rFlag != n)
            rFlag++;
        else
            lFlag++;
    }

    cout << monuments << endl;
}