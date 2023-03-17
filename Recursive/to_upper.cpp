#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

void to_upper(string &line, int from = 0)
{
    if (from == line.size() + 1)
    {
        return;
    }

    line[from] = toupper(line[from]);

    return to_upper(line, from + 1); // returning it useless btw (because we are using address)
}

int main()
{
    string line;

    while (getline(cin, line))
    {
        to_upper(line);
        cout << line << endl;
    }
}