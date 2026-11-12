#include <bits/stdc++.h>

using namespace std;

#define MAX 3000000

int sumsIndex;
long long n, k;
long long a[MAX], sums[MAX];

void merge(int l1, int r1, int l2, int r2, int &idx)
{
    int idx1 = l1, idx2 = l2;

    int sum = 0;

    while (idx1 <= r1 && idx2 <= r2)
    {
        if (a[idx1] < a[idx2])
        {
            sum += a[idx1];
            idx1++;
        }
        else
        {
            idx2++;
        }
    }

    while (idx1 <= r1)
    {
        idx1++;
    }

    while (idx2 <= r2)
    {
        sum += a[idx2];
        idx2++;
    }

    sums[idx] = sum;
    idx++;

    return;
}

void mergeSort(int l, int r, int &idx)
{
    if (l == r)
        return;

    int mid = (l + r) / 2;

    mergeSort(l, mid, idx);
    mergeSort(mid + 1, r, idx);

    merge(l, mid, mid + 1, r, idx);
}

long long maxSum()
{
    long long maxEl = sums[0];

    for (int i = 1; i <= sumsIndex; i++)
    {
        if (sums[i] > k)
            break;

        maxEl = max(maxEl, sums[i]);
    }

    return maxEl;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cin >> n >> k;

    for (int i = 0; i < n; ++i)
        cin >> a[i];

    int idx = 0;

    mergeSort(0, n - 1, idx);

    sumsIndex = idx;

    cout << maxSum() << endl;

    return 0;
}