#include <bits/stdc++.h>

using namespace std;

#define MAX 110000

int n, m, a, b, ccomp, par[MAX], sz[MAX], d[MAX], maxcomp = 1;

int find(int a)
{
    if (par[a] == 0)
        return a;

    return par[a] = find(par[a]);
}

void myUnion(int a, int b)
{
    a = find(a);
    b = find(b);

    if (a == b)
    {
        return;
    }

    ccomp--;

    if (d[a] > d[b])
    {
        par[b] = a;
        sz[a] += sz[b];
        maxcomp = max(maxcomp, sz[a]);
    }
    else
    {
        par[a] = b;
        sz[b] += sz[a];
        maxcomp = max(maxcomp, sz[b]);
        if (d[a] == d[b])
        {
            d[b]++;
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> m;

    fill(sz + 1, sz + n + 1, 1);
    ccomp = n;

    for (int i = 0; i < m; i++)
    {
        cin >> a >> b;
        myUnion(a, b);

        cout << ccomp << " " << maxcomp << endl;
    }

    return 0;
}