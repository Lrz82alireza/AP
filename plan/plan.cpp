#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
using namespace std;

#define MAX_COURSE_CLASS_DAY 2
#define MAX_CLASS_TIME 90
#define GROUP_A 1
#define GROUP_B 2
#define WORK_WEEK_DAYS 5
#define WORK_TIMES 3

struct Basic
{
    vector<Schedule> schedule;
    map<string, int> week;
    map<int, pair<int, int>> class_times;
};

struct Schedule
{
    int days[2];
    int time;
    string teacher;
    string course;
};

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

pair<int, int> time_plus(pair<int, int> course_time, int plus_minute = MAX_CLASS_TIME)
{
    pair<int, int> result = {course_time.first + (plus_minute / 60), course_time.second + (plus_minute % 60)};

    if (result.second >= 60)
    {
        result.first++;
        result.second -= 60;
    }

    return result;
}

bool is_time1_before_time2(pair<int, int> time1, pair<int, int> time2)
{
    if (time1.first == time2.first && time1.second < time2.second)
        return true;
    if (time1.first < time2.second)
        return true;
    return false;
}

bool is_time1_equal_time2(pair<int, int> time1, pair<int, int> time2)
{
    if (time1.first == time2.first && time1.second == time2.second)
        return true;
    return false;
}

bool is_course_on_time(pair<int, int> course_time[2], map<int, pair<int, int>> class_times, int class_time)
{
    int class_hour = class_times[class_time].first,
        class_minute = class_times[class_time].second;
    pair<int, int> end_time = time_plus(course_time[0]);

    if (is_time1_before_time2(end_time, course_time[1]))
        return false;
    if (!is_time1_before_time2(course_time[0], class_times[class_time]) &&
        !is_time1_equal_time2(course_time[0], class_times[class_time]))
        return false;

    return true;
}

bool is_this_course_better(Course current_course, Course new_course) /////////////////////////////
{
    //
}

Course find_best_course(vector<Course> valid_courses) /////////////////////////////
{
    Course best_course = valid_courses[0];
    if (valid_courses.size() == 1)
        return valid_courses[0];

    for (auto i = valid_courses.begin() + 1; i != valid_courses.end(); i++) //////////////////////////////////////////////
    {
        if (is_this_course_better()) // قراره چک کنه که کدومشون بهترن
        {
            //
        }
    }
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
        if (counter >= 1)
            return true;
        break;

    case GROUP_B:
        if (counter >= 2)
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
            for (auto course_day : valid_course.class_day)
            {
                if (schedule_day == course_day && i.time == class_time && i.teacher == teacher.name)
                    return true;
            }
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

vector<Course> find_valid_courses(Basic &basic, vector<Teacher> &teacher, vector<Course> &course,
                                  int class_time, int group, int week_day = 0)
{
    vector<Course> valid_courses;
    for (auto i : course)
    {
        if (is_course_valid(basic, i, class_time, week_day, group))
            valid_courses.push_back(i);
    }

    return valid_courses;
}

void make_schedule_of_day(Basic &basic, vector<Teacher> &teacher, vector<Course> &course, int group, int week_day, int class_time = 0)
{
    if (class_time >= WORK_TIMES)
        return;

    //////////////////////////////////////////////////////// making schedule

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
    if (group != GROUP_A && group != GROUP_B)
        return;
    make_schedule_of_week(basic, teacher, course, group);

    make_schedule_of_group(basic, teacher, course, group + 1);
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

    vector<Teacher> teacher;
    vector<Course> course;
    vector<Schedule> schedule;

    Basic basic = {.schedule = schedule, .week = week, .class_times = class_times};

    // get_schedules(teacher, course, week);

    // cout << course[0].time[1].second << endl;
}