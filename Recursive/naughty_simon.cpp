#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

#define Used 1
#define Not_used 0

typedef struct Location
{
    int row;
    int column;
} location;

int column_move[] = {0, 1};
int row_move[] = {1, 0};

void make_matris(vector<vector<int>> &matris)
{

    // half of martis is 
    for (int i = 0; i < matris.size(); i++)
    {
        for (int j = matris.size(); j > i; j--)
        {
            matris[i][j] = Used;
        }
    }

    matris[0][0] = Used;
}

void print_matris(vector<vector<int>> &matris, int i = 0)
{
    if (i >= matris.size())
    {
        return;
    }

    for (int j = 0; j < matris.size(); j++)
    {
        cout << matris[i][j] << ' ';
    }
    cout << endl;
    print_matris(matris, i + 1);
}

void make_move(vector<vector<int>> &matris, location &current, int mode)
{
    matris[current.row + row_move[mode]][current.column + column_move[mode]] = Used;
}

bool check_move(vector<vector<int>> &matris, location before, int column, int row)
{
    if (column < 0 || matris.size() - 1 < column)
    {
        return false;
    }
    if (row < 0 || matris.size() - 1 < row)
    {
        return false;
    }

    if (matris[row][column] == Used)
    {
        return false;
    }
    if (matris[row][column] == matris[before.row][before.column])
    {
        return false;
    }

    return true;
}

bool check_solution(vector<vector<int>> &matris, location current)
{
    if (current.column == (matris.size() - 1) && current.row == (matris.size() - 1))
    {
        return true;
    }

    return false;
}

bool solve(vector<vector<int>> &matris, location current, location before, int &counter)
{
    // print_matris(matris);
    // cout << "----------\n";

    if (check_solution(matris, current))
    {
        // count the solutions
        counter++;

        matris[current.row][current.column] = Not_used;
        return false;
    }

    location temp_current;
    for (int i = 0; i < 2; i++)
    {
        if (check_move(matris, before, current.column + column_move[i], current.row + row_move[i]))
        {
            make_move(matris, current, i);
            temp_current.row = current.row + row_move[i];
            temp_current.column = current.column + column_move[i];

            if (solve(matris, temp_current, current, counter))
            {
                return true;
            }
        }
    }

    matris[current.row][current.column] = Not_used;
    return false;
}

int main()
{
    location current = {0, 0}, before = {0, 0};

    int n, counter = 0;
    cin >> n;

    vector<vector<int>> matris(n + 1, vector<int>(n + 1, Not_used));

    make_matris(matris);

    solve(matris, current, before, counter);\
    cout << counter << endl;
}