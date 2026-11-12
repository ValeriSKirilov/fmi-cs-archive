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

    int nums[n];

    for (int i = 0; i < n; ++i)
    {
        cin >> nums[i];
    }

    for (int i = 0; i < n; ++i)
    {
        int ones = 0;

        while (nums[i] != 0)
        {
            if (nums[i] % 2 == 1)
                ones++;

            nums[i] /= 2;
        }

        std::cout << ones << '\n';
    }

    return 0;
}
