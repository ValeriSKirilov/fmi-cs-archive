#include <bits/stdc++.h>

using namespace std;

#define MAX 2000000

long long n;
int k;
int a[MAX];

int partition(int l, int r, int p)
{
    int uk1 = l, uk2 = r;
    int x = a[p];
    swap(a[p], a[r]);

    while (uk1 < uk2)
    {
        if (a[uk1] >= x && a[uk2] < x)
            swap(a[uk1], a[uk2]);

        if (a[uk1] < x)
            uk1++;

        if (a[uk2] >= x)
            uk2--;
    }

    if (a[uk1] < x)
        uk1++;

    swap(a[uk1], a[r]);

    return uk1;
}

long long quickSort(int l, int r)
{
    if (r <= l)
        return -1;

    int p = (l + r) / 2;
    int id = partition(l, r, p);

    if (id == n - k)
        return a[id];
    else if (k < id)
        return quickSort(l, id - 1);
    else
        return quickSort(id + 1, r);

    return -1;
}

int main()
{
    cin >> n >> k;

    for (int i = 0; i < n; ++i)
        cin >> a[i];

    int idx = quickSort(0, n - 1);

    cout << idx << endl;
}