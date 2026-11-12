#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    
    int n, m, p;
    
    cin >> n >> m >> p;
    
    vector<int> arr(n);
    
    for (int i = 0; i < n; ++i)
        cin >> arr[i];

    long long sum = 0;

    sort(arr.begin(), arr.end());

    for (int i = 0; i < m; ++i)
        sum -= arr[i];

    for (int i = m; i < n; ++i)
        sum += arr[i];

    cout << sum;
}