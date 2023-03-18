#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
using namespace std;

#define MAX_COURSE_CLASS_DAY 2
#define MAX_CLASS_TIME 90
#define GROUP_A 1
#define GROUP_B 2
#define WORK_WEEK_DAYS 5
#define WORK_TIMES 3


struct Teacher
{
    string name;
    vector<int> free_day;
    vector<string> lesson;
};

struct Course
{
    string name;
    vector<int> class_day;
    pair<int, int> time[2]; // hh:mm
};

struct Schedule
{
    int days[2];
    int time;
    string teacher;
    string course;
};

struct Basic
{
    vector<Schedule> schedule;
    map<string, int> week;
    map<int, pair<int, int>> class_times;
};

struct Mix
{
    Teacher teacher;
    Course course;
};

void test(vector<Course> test, int time)
{
    for (auto i : test)
    {
        cout << i.name << endl;
        cout << time << endl;
    }
}


void get_teacher_schedule(Teacher &teacher, map<string, int> week)
{
    int number;

    cin >> teacher.name;

    cin >> number;
    for (int i = 0; i < number; i++)
    {
        string week_day;
        cin >> week_day;
        teacher.free_day.push_back(week[week_day]);
    }

    cin >> number;
    for (int i = 0; i < number; i++)
    {
        string lesson_name;
        cin >> lesson_name;
        teacher.lesson.push_back(lesson_name);
    }
}

void get_teachers_schedule(vector<Teacher> &teacher, map<string, int> week)
{
    int number;
    cin >> number;
    teacher.resize(number);

    for (int i = 0; i < number; i++)
    {
        get_teacher_schedule(teacher[i], week);
    }
}

void get_course_schedule(Course &course, map<string, int> week)
{
    cin >> course.name;

    for (int i = 0; i < MAX_COURSE_CLASS_DAY; i++)
    {
        string week_day;
        cin >> week_day;
        course.class_day.push_back(week[week_day]);
    }

    for (int i = 0; i < 2; i++)
    {
        string clock;
        cin >> clock;
        int hour = stoi(clock.substr(0, 2));
        int minute = stoi(clock.substr(3, 2));
        course.time[i] = {hour, minute};
    }
}

void get_courses_schedule(vector<Course> &course, map<string, int> week)
{
    int number;
    cin >> number;
    course.resize(number);

    for (int i = 0; i < number; i++)
    {
        get_course_schedule(course[i], week);
    }
}

void get_schedules(vector<Teacher> &teacher, vector<Course> &course, map<string, int> week)
{
    get_teachers_schedule(teacher, week);
    get_courses_schedule(course, week);
}

int conver_to_minute(pair<int, int> time)
{
    int result = 0;
    result += time.first * 60;
    result += time.second;
    return result;
}

bool is_course_on_time(pair<int, int> course_time[2], map<int, pair<int, int>> class_times, int class_time)
{
    int course_start = conver_to_minute(course_time[0]);
    int course_end = conver_to_minute(course_time[1]);
    int class_start = conver_to_minute(class_times[class_time]);
    int class_end = conver_to_minute(class_times[class_time + 3]);
    if (course_start <= class_start && class_end <= course_end)
        return true;
    return false;
}

bool is_course_in_day(Course course, int week_day)
{
    for (auto i : course.class_day)
    {
        if (i == week_day)
            return true;
    }
    return false;
}

bool is_this_class_used(Basic basic, Course &course, int class_time)
{
    for (auto it : basic.schedule)
    {
        for (auto sch_day : it.days)
        {
            for (auto crs_day : course.class_day)
            {
                if (sch_day == crs_day && it.time == class_time)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool is_course_used(Basic basic, Course &course, int week_day, int group)
{
    int counter = 0;
    for (auto i : basic.schedule)
    {
        if (i.course == course.name && (i.days[0] == week_day || i.days[1] == week_day))
        {
            counter++;
        }
    }

    switch (group)
    {
    case GROUP_A:
        if (counter == GROUP_A)
            return true;
        break;

    case GROUP_B:
        if (counter == GROUP_B)
            return true;
        break;
    }
    return false;
}

bool is_course_valid(Basic basic, Course &course, int class_time, int week_day, int group)
{
    if (!is_course_in_day(course, week_day))
        return false;
    if (is_course_used(basic, course, week_day, group))
        return false;
    if (is_this_class_used(basic, course, class_time))
        return false;
    if (is_course_on_time(course.time, basic.class_times, class_time))
        return true;
    return false;
}

bool has_this_course(Teacher teacher, Course valid_course)
{
    for (auto i : teacher.lesson)
    {
        if (i == valid_course.name)
            return true;
    }
    return false;
}

bool has_courses_days(Teacher teacher, Course valid_course)
{
    int counter = 0;
    for (auto i : valid_course.class_day)
    {
        for (auto j : teacher.free_day)
        {
            if (i == j)
                counter++;
        }
    }

    if (counter == MAX_COURSE_CLASS_DAY)
        return true;
    return false;
}

bool had_this_class_time(Basic basic, Teacher teacher, Course valid_course, int class_time)
{
    for (auto i : basic.schedule)
    {
        for (auto schedule_day : i.days)
        {
            if (schedule_day != valid_course.class_day[0] && schedule_day != valid_course.class_day[1])
                continue;
            if (i.time != class_time)
                continue;
            if (i.teacher == teacher.name)
                return true;
        }
    }
    return false;
}

bool is_teacher_valid(Basic basic, Teacher teacher, Course valid_course, int class_time)
{
    if (!has_this_course(teacher, valid_course))
        return false;
    if (!has_courses_days(teacher, valid_course))
        return false;
    if (had_this_class_time(basic, teacher, valid_course, class_time))
        return false;
    return true;
}

vector<Course> find_valid_courses(Basic &basic, vector<Teacher> &teacher, vector<Course> &course,
                                  int class_time, int group, int week_day)
{
    vector<Course> valid_courses;
    for (auto i : course)
    {
        if (is_course_valid(basic, i, class_time, week_day, group))
            valid_courses.push_back(i);
    }

    return valid_courses;
}

string to_lower(string s)
{
    string new_s = s;
    for (int i = 0; i < s.size(); i++)
    {
        new_s[i] = tolower(s[i]);
    }
    return new_s;
}

vector<Teacher> find_valid_teachers(Basic basic, vector<Teacher> teacher, Course valid_course, int class_time)
{
    vector<Teacher> valid_teachers;
    for (auto i : teacher)
    {
        if (is_teacher_valid(basic, i, valid_course, class_time))
            valid_teachers.push_back(i);
    }

    return valid_teachers;
}

int compare_alphabetically(string s1, string s2)
{
    s1 = to_lower(s1);
    s2 = to_lower(s2);

    for (int i = 0; i != s1.size() && i != s2.size(); i++)
    {
        if (s1[i] < s2[i])
            return 1;
        else if (s1[i] > s2[i])
            return 2;
    }
    if (s1 == s2)
        return 0;
    if (s1.size() > s2.size())
        return 1;
    return 2;
}

bool is_this_teacher_better(Teacher current_teacher, Teacher new_teacher)
{
    if (new_teacher.free_day.size() < current_teacher.free_day.size())
        return true;
    if (new_teacher.free_day.size() == current_teacher.free_day.size() &&
        compare_alphabetically(new_teacher.name, current_teacher.name) == 1)
        return true;
    return false;
}

Teacher find_best_teacher(vector<Teacher> valid_teachers)
{
    Teacher best_teacher = valid_teachers.front();
    for (auto i : valid_teachers)
    {
        if (i.name == valid_teachers.front().name)
            continue;
        if (is_this_teacher_better(best_teacher, i))
            best_teacher = i;
    }
    return best_teacher;
}

Mix make_mix(Teacher best_teacher, Course valid_course)
{
    Mix mix;
    mix.course = valid_course;
    mix.teacher = best_teacher;
    return mix;
}

vector<Mix> make_valid_mixes(Basic &basic, vector<Teacher> teacher, vector<Course> valid_courses, int class_time)
{
    vector<Mix> valid_mixes;
    for (auto i : valid_courses)
    {
        vector<Teacher> valid_teachers = find_valid_teachers(basic, teacher, i, class_time);
        if (valid_teachers.size() == 0)
            continue;
        
        Teacher best_teacher = find_best_teacher(valid_teachers);
        valid_mixes.push_back(make_mix(best_teacher, i));
    }
    return valid_mixes;
}

bool is_mix1_equal_mix2(Mix mix1, Mix mix2)
{
    if (mix1.course.name != mix2.course.name)
        return false;
    if (mix1.teacher.name != mix2.teacher.name)
        return false;
    return true;
}

bool is_this_mix_better(Mix current_mix, Mix new_mix)
{
    if (compare_alphabetically(current_mix.course.name, new_mix.course.name) == 1)
        return true;
    return false;
}

Mix find_best_mix(vector<Mix> valid_mixes)
{
    Mix best_mix = valid_mixes.front();
    for (auto i : valid_mixes)
    {
        if (is_mix1_equal_mix2(i, best_mix))
            continue;
        if (is_this_mix_better(i, best_mix))
            best_mix = i;
    }
    return best_mix;
}

Schedule make_a_schedule(Mix mix, int class_time)
{
    Schedule new_schedule;
    new_schedule.course = mix.course.name;
    new_schedule.teacher = mix.teacher.name;
    new_schedule.time = class_time;
    for (int i = 0; i < MAX_COURSE_CLASS_DAY; i++)
    {
        new_schedule.days[i] = mix.course.class_day[i];
    }
    return new_schedule;
}

void make_schedule_of_class_time(Basic &basic, vector<Teacher> &teacher, vector<Course> &course, int group, int week_day, int class_time)
{
    vector<Course> valid_courses = find_valid_courses(basic, teacher, course, class_time, group, week_day);
    if (valid_courses.size() == 0)
        return;

    vector<Mix> valid_mixes = make_valid_mixes(basic, teacher, valid_courses, class_time);
    if (valid_mixes.size() == 0)
        return;

    Mix best_mix = find_best_mix(valid_mixes);

    Schedule new_schedule = make_a_schedule(best_mix, class_time);
    basic.schedule.push_back(new_schedule);
}

void make_schedule_of_day(Basic &basic, vector<Teacher> &teacher, vector<Course> &course, int group, int week_day, int class_time = 0)
{
    if (class_time >= WORK_TIMES)
        return;

    make_schedule_of_class_time(basic, teacher, course, group, week_day, class_time);

    make_schedule_of_day(basic, teacher, course, group, week_day, class_time + 1);
}

void make_schedule_of_week(Basic &basic, vector<Teacher> &teacher, vector<Course> &course, int group, int week_day = 0)
{
    if (week_day >= WORK_WEEK_DAYS)
        return;
    make_schedule_of_day(basic, teacher, course, group, week_day);

    make_schedule_of_week(basic, teacher, course, group, week_day + 1);
}

void make_schedule_of_group(Basic &basic, vector<Teacher> &teacher, vector<Course> &course, int group = GROUP_A)
{
    if (group >= GROUP_B + 1)
        return;
    make_schedule_of_week(basic, teacher, course, group);

    make_schedule_of_group(basic, teacher, course, group + 1);
}

void make_plan(Basic &basic, vector<Teacher> &teacher, vector<Course> &course)
{
    get_schedules(teacher, course, basic.week);
    make_schedule_of_group(basic, teacher, course);
}

bool had_this_name(vector<string> course_name, string name)
{
    for (auto i : course_name)
    {
        if (i == name)
            return true;
    }
    return false;
}

vector<string> find_courses_names(vector<Course> course)
{
    vector<string> course_name;
    course_name.push_back(course.front().name);

    for (auto i : course)
    {
        if (had_this_name(course_name, i.name))
            continue;
        course_name.push_back(i.name);
    }
    return course_name;
}

void print_class_time(int class_time)
{
    switch (class_time)
    {
    case 0:
        cout << "07:30 09:00" << endl;
        break;
    case 1:
        cout << "09:30 11:00" << endl;
        break;
    case 2:
        cout << "11:30 13:00" << endl;
        break;
    }
}

void print_course_plan(Basic &basic, string courses_name)
{
    int counter = 0;
    cout << courses_name << endl;
    for (auto i : basic.schedule)
    {
        if (i.course == courses_name)
        {
            counter++;
            cout << i.teacher << ": ";
            print_class_time(i.time);
        }
    }

    for (int i = 0; i < MAX_COURSE_CLASS_DAY - counter; i++)
    {
        cout << "Not Found" << endl;
    }
}

void print_plan(Basic &basic, vector<Course> course)
{
    cout << basic.schedule.size() << endl;

    vector<string> courses_name = find_courses_names(course);
    int counter = 0;
    string cur_course = courses_name[counter];
    print_course_plan(basic, cur_course);

    for (auto i : basic.schedule)
    {
        if (cur_course != i.course)
        {
            print_course_plan(basic, i.course);

            counter++;
            if (counter == courses_name.size())
                break;

            cur_course = courses_name[counter];

        }
    }
}

int main()
{
    map<string, int> week;
    week["Saturday"] = 0;
    week["Sunday"] = 1;
    week["Monday"] = 2;
    week["Tuesday"] = 3;
    week["Wednesday"] = 4;

    map<int, pair<int, int>> class_times;
    class_times[0] = {7, 30};
    class_times[1] = {9, 30};
    class_times[2] = {11, 30};

    class_times[3] = {9, 0};
    class_times[4] = {11, 0};
    class_times[5] = {13, 0};

    vector<Teacher> teacher;
    vector<Course> course;
    vector<Schedule> schedule;

    Basic basic = {.schedule = schedule, .week = week, .class_times = class_times};

    make_plan(basic, teacher, course);
    /*
    for (auto i : basic.schedule)
    {
        cout << i.course << endl;
        cout << i.teacher << endl;
        cout << i.time << endl;
        cout << i.days[0] << " " << i.days[1] << endl;
        cout << "////////////////////////////////" << endl;
    }
    */
    print_plan(basic, course);
}