//
// Created by Lenovo on 19.2.2026 г..
//

#include <iostream>

using namespace std;

#define MAX 1100

int n, a[MAX];

void bubble_sort()
{
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - 1; ++j)
        {
            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);
        }

        for (int j = 0; j < n; ++j)
            cout << a[j] << " ";

        cout << endl;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> n;
    for (int i = 0; i < n; ++i)
        cin >> a[i];

    bubble_sort();

    return 0;
}
