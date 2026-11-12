//
// Created by Lenovo on 19.2.2026 г..
//

#include <iostream>

using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int n;
    cin >> n;

    long long arr[n];

    for (int i = 0; i < n; ++i)
        cin >> arr[i];

    long long sum = 0;
    for (int i = 0; i < n; ++i)
    {
        long long currSum = 0;
        for (int j = i; j < n; ++j)
        {
            currSum += arr[j];
            if ((j - i + 1) % 2 != 0)
                sum += currSum;
        }
    }

    cout << sum << '\n';
}