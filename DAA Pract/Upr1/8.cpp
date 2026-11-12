//
// Created by Lenovo on 19.2.2026 г..
//

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n;
    cin >> n;

    vector<long long> arr;

    for (int i = 0; i < n; ++i)
    {
        long long currEl;
        cin >> currEl;
        arr.push_back(currEl);
    }

    sort(arr.begin(), arr.end());

    for (const int i : arr)
        cout << i << ' ';
}
