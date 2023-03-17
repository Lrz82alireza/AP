#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

void command(string input, bool *flag);
int convert(string input);
void longest_day();
void summary(string note);
void best_day();

vector<vector<string>> diary(1);

int main()
{
    string input;
    bool flag = true;
    while (true)
    {
        cin >> input;
        if (input == "biroon")
        {
            break;
        }
        command(input, &flag);
    }
}

// manage inputs
void command(string input, bool *flag)
{
    string tmp;
    switch (convert(input))
    {
    case 1: // start_day

        if (*flag)
        {
            cin >> tmp;
            diary.back().push_back(tmp);

            // make a place for note
            diary.back().push_back("");
            *flag = false;
            break;
        }

        // new day
        diary.resize(diary.size() + 1);
        cin >> tmp;
        diary.back().push_back(tmp);

        // make a place for note
        diary.back().push_back("");

        break;
    case 2: // show_day

        cin >> tmp; // get date

        for (int i = 0; i < diary.size(); i++)
        {
            if (diary[i][0] == tmp)
            {
                cout << diary[i].back();
                break;
            }
        }
        break;
    case 3: // show_the_longest_day

        longest_day();

        break;
    case 4: // show_the_best_day
        best_day();
        break;
    case 5: // note

        diary.back().back() += input;

        getline(cin, tmp);
        diary.back().back() += (tmp + '\n');

        break;
    }
}

void best_day()
{
    string textword, fileword;
    int best_diary = 0, co1 = 0;

    if (diary.size() == 1)
    {
        cout << diary[0][0] << '\n';
        summary(diary[0].back());
        exit;
    }

    for (int i = 0, co2 = 0; i < diary.size(); i++)
    {
        stringstream text(diary[i].back());

        // find positive words
        while (getline(text, textword, ' '))
        {

            ifstream myfile("positive-words.txt");

            while (getline(myfile, fileword, '\n'))
            {
                if (textword == fileword)
                {
                    co2++;
                }
            }

            myfile.close();
        }

        // compare between former diary'word and new one
        if (co1 <= co2)
        {
            if (co1 != co2)
                best_diary = i;
            co1 = co2;
            co2 = 0;
        }
    }
    cout << diary[best_diary][0] << '\n';
    summary(diary[best_diary].back());
}

// convert input to number
int convert(string input)
{
    if (input == "start_day")
    {
        return 1;
    }
    else if (input == "show_day")
    {
        return 2;
    }
    else if (input == "show_the_longest_day")
    {
        return 3;
    }
    else if (input == "show_the_best_day")
    {
        return 4;
    }
    else // note
    {
        return 5;
    }
}

void longest_day()
{
    int longest = 0;

    if (diary.size() == 1)
    {
        cout << diary[longest][0] << '\n';
        summary(diary[longest].back());
        exit;
    }

    // find longest day
    for (int i = 1; i < diary.size(); i++)
    {
        if (diary[longest].back().size() < diary[i].back().size())
        {
            longest = i;
        }
    }
    // print it
    cout << diary[longest][0] << '\n';
    summary(diary[longest].back());
}

// print the summary of day's note
void summary(string note)
{
    if (note.size() <= 20)
    {
        cout << note << '\n';
    }
    else
    {
        for (int i = 0; i < 20; i++)
        {
            cout << note[i];
        }
        cout << "..." << '\n';
    }
}