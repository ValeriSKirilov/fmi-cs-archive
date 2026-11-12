//
// Created by Lenovo on 19.2.2026 г..
//

#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int n, m, k;

    cin >> n >> m >> k;

    long long matrix1[n][m];
    long long matrix2[m][k];

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            cin >> matrix1[i][j];
        }
    }

    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < k; ++j)
        {
            cin >> matrix2[i][j];
        }
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < k; j++)
        {
            long long currEl = 0;

            for (int h = 0; h < m; ++h)
                currEl += matrix1[i][h] * matrix2[h][j];

            cout << currEl << " ";
        }
        cout << '\n';
    }

    return 0;
}
