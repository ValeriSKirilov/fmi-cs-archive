#include <bits/stdc++.h>

using namespace std;

long long a[1000000], b[1000000];

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int n, m;
    cin >> n >> m;

    for (int i = 0; i < n; ++i)
        cin >> a[i];

    for (int i = 0; i < m; ++i)
        cin >> b[i];

    int aIndex = 0, bIndex = 0;
    while (aIndex < n && bIndex < m)
    {
        if (a[aIndex] < b[bIndex])
        {
            cout << a[aIndex] << ' ';
            aIndex++;
        }
        else
        {
            cout << b[bIndex] << ' ';
            bIndex++;
        }
    }

    while (aIndex < n)
    {
        cout << a[aIndex] << ' ';
        aIndex++;
    }

    while (bIndex < m)
    {
        cout << b[bIndex] << ' ';
        bIndex++;
    }

    cout << endl;
}