#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

#define MAX 0
#define MIN 1

int compare(int n1, int n2, int mode = MAX)
{
    switch (mode)
    {
    case MAX:
        if (n1 > n2)
            return n1;
        return n2;

        break;

    case MIN:
        if (n1 < n2)
            return n1;
        return n2;

        break;
    }
    return 10000;
}

void scale(vector<int> numbers, int digit_number, int from = 0, int to = 0, int compare_mode = MAX)
{
    int temp_compare;
    if (from + to >= digit_number)
    {
        return;
    }

    cout << numbers[from + to] << ' ';
    temp_compare = compare(numbers[from], numbers[from + to], compare_mode);

    scale(numbers, digit_number, from + to, temp_compare, (compare_mode + 1) % 2);
}

int main()
{
    int digit_number, temp;
    vector<int> numbers;

    cin >> digit_number;
    for (int i = 0; i < digit_number; i++)
    {
        cin >> temp;
        numbers.push_back(temp);
    }

    scale(numbers, digit_number);
    cout << endl;
}