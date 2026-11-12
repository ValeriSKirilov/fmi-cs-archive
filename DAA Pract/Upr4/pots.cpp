#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int n, m;

    cin >> n >> m;

    vector<int> arr1(n);
    vector<int> arr2(m);

    for (int i = 0; i < n; ++i)
        cin >> arr1[i];

    for (int i = 0; i < m; ++i)
        cin >> arr2[i];

    sort(arr1.begin(), arr1.end());
    sort(arr2.begin(), arr2.end());

    int count = 0;
    int idx1 = 0, idx2 = 0;

    while(idx1 < n && idx2 < m)
    {
        if (arr2[idx2] >= arr1[idx1])
        {
            count++;
            idx1++;
            idx2++;
        }
        else
        {
            idx2++;
        }
    }

    cout << count;
}