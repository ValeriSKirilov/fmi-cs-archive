#include <bits/stdc++.h>

using namespace std;

#define MAX 2000000

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

void quickSort(int l, int r)
{
    if (r <= l)
        return;

    int p = l;
    int id = partition(l, r, p);

    quickSort(l, id - 1);
    quickSort(id + 1, r);
}

int main()
{
    long long n;
    cin >> n;

    for (int i = 0; i < n; ++i)
        cin >> a[i];

    quickSort(0, n - 1);

    for (int i = 0; i < n; ++i)
        cout << a[i] << ' ';
    cout << endl;
}