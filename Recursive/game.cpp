#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

#define LINE_SIZE 7

#define Right 0
#define Up 1
#define Left 2
#define Down 3

typedef struct Location
{
    int row;
    int column;
} location;

// Right, Up, Left, Down
int row_move[] = {0, -1, 0, 1};
int column_move[] = {1, 0, -1, 0};

void make_board(vector<vector<string>> &board, int line = 0)
{
    char tmp;

    if (line == board.size())
    {
        return;
    }

    for (int i = 0; i < board.size(); i++)
    {
        cin >> tmp;
        board[line][i] = tmp;
        board[line][i] += 'A' + line;
        board[line][i] += '1' + i;
    }

    make_board(board, line + 1);
}

void print_board(vector<vector<string>> &board, int i = 0)
{
    if (i >= board.size())
    {
        return;
    }

    for (int j = 0; j < board.size(); j++)
    {
        cout << board[i][j][0] << ' ';
    }
    cout << endl;
    print_board(board, i + 1);
}

void find_N(vector<vector<string>> &board, vector<location> &N_location, int i = 0, int j = 0)
{

    for (; i < board.size(); i++)
    {
        for (; j < board.size(); j++)
        {
            if (board[i][j][0] == 'N')
            {
                N_location.push_back({.row = i, .column = j});
            }
        }
        j = 0;
    }
}

bool check_move(vector<vector<string>> &board, int row, int column, int mode)
{
    int size = board.size();
    if (column < 1 || size - 2 < column)
    {
        return false;
    }
    if (row < 1 || size - 2 < row)
    {
        return false;
    }

    if (board[row][column][0] != 'N' || board[row + row_move[mode]][column + column_move[mode]][0] != 'O')
    {
        return false;
    }
    return true;
}

void make_move(vector<vector<string>> &board, location current, int mode, bool revurse_mode = false)
{
    if (revurse_mode)
    {
        // OON --> NON
        board[current.row][current.column][0] = 'N';
        // NON --> NOO
        board[current.row + (2 * row_move[mode])][current.column + (2 * column_move[mode])][0] = 'O';
        // NOO --> NNO
        board[current.row + row_move[mode]][current.column + column_move[mode]][0] = 'N';
    }
    else
    {
        // NNO --> NOO
        board[current.row + row_move[mode]][current.column + column_move[mode]][0] = 'O';
        // NOO --> NON
        board[current.row + (2 * row_move[mode])][current.column + (2 * column_move[mode])][0] = 'N';
        // NON --> OON
        board[current.row][current.column][0] = 'O';
    }
}

bool check_solution(vector<vector<string>> &board)
{
    vector<location> N_location;
    find_N(board, N_location);

    if (N_location.size() == 1 && board[N_location[0].row][N_location[0].column][1] == 'D' && board[N_location[0].row][N_location[0].column][2] == '4')
    {
        return true;
    }
    return false;
}

void make_answer(vector<vector<string>> &board, vector<string> &answer, location current, int mode, bool revurse_mode = false)
{

    if (revurse_mode)
    {
        answer.pop_back();
    }
    else
    {
        string tmp;
        switch (mode)
        {
        case Right:
            tmp = "RIGHT";
            break;
        case Up:
            tmp = "UP";
            break;
        case Left:
            tmp = "LEFT";
            break;
        case Down:
            tmp = "DOWN";
            break;
        }
        // place + move direction
        answer.push_back(board[current.row][current.column].substr(1) + ' ' + tmp + '\n');
    }
}

void print_answer(vector<string> &answer)
{
    int size_answer = answer.size();

    for (int i = 0; i < size_answer; i++)
    {
        cout << answer[i];
    }
}

bool solve(vector<vector<string>> &board, vector<string> &answer, location current)
{
    vector<location> N_location;
    find_N(board, N_location);
    int N_number = 0;

    while (true)
    {
        current = N_location[N_number];

        for (int mode = 0; mode < 4; mode++)
        {
            if (check_move(board, current.row + row_move[mode], current.column + column_move[mode], mode))
            {
                make_move(board, current, mode);
                make_answer(board, answer, current, mode);

                ///////////////////////////////
                // print_board(board);
                // cout << "----------------------" << endl;
                ///////////////////////////////

                if (solve(board, answer, current))
                {
                    return true;
                }
                else
                {
                    make_move(board, current, mode, true);
                    make_answer(board, answer, current, mode, true);

                    /////////////////////////////////
                    // print_board(board);
                    // cout << "----------------------" << endl;
                    /////////////////////////////////
                }
            }
        }

        if (check_solution(board))
        {
            return true;
        }

        // All Ns are Checked
        N_number++;
        if (N_number >= N_location.size())
        {
            return false;
        }
    }
    return false;
}

int main()
{
    location current = {0, 0};

    vector<string> answer;
    vector<vector<string>> board(LINE_SIZE, vector<string>(LINE_SIZE));

    make_board(board);

    if (solve(board, answer, current))
    {
        print_answer(answer);
    }
    else
    {
        cout << "Loser" << endl;
    }
}
