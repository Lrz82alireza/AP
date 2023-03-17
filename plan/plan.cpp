#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
using namespace std;

#define MAX_COURSE_CLASS_DAY 2
#define MAX_CLASS_TIME 90

struct Occupied_class
{
    int day;
    string teacher_name;
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
    vector<Occupied_class> occupied_class;
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
    if (time1.first == time2.first && time1.second <= time2.second)
        return true;
    if (time1.first < time2.second)
        return true;
    return false;
}

bool is_course_on_time(pair<int, int> course_time[2], map<int, pair<int, int>> &class_times, int class_time)
{
    int class_hour = class_times[class_time].first,
        class_minute = class_times[class_time].second;

    if (course_time[0].first == class_hour && course_time[0].second <= class_minute)
    {
        pair<int, int> end_time = time_plus(course_time[0]);
        if (is_time1_before_time2(end_time, course_time[1]))
            return true;
    }

    if (course_time[0].first < class_hour)
        return true;
    return false;
}

bool is_this_course_better(Course current_course ,Course new_course)
{
    // 
}

Course find_best_course(vector<Course> &valid_courses)
{
    Course best_course = valid_courses[0];
    if (valid_courses.size() == 1)
        return valid_courses[0];
    
    for (auto i = valid_courses.begin() + 1; i != valid_courses.end(); i++)
    {
        if (is_this_course_better()) // قراره چک کنه که کدومشون بهترن
        {
            // 
        }
    }
}

Course find_valid_courses(vector<Teacher> &teacher, vector<Course> &course, map<string, int> &week, map<int, pair<int, int>> &class_times, int class_time, int week_day = 0)
{
    vector<Course> valid_courses;
    for (auto i : course)
    {
        if (is_course_on_time(i.time, class_times, class_time))
        {
            valid_courses.push_back(i);
        }
    }

    return find_best_course(valid_courses);
}

void make_schedule(vector<Teacher> &teacher, vector<Course> &course, map<string, int> &week, map<int, pair<int, int>> &class_times, int week_day = 0)
{
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

    // get_schedules(teacher, course, week);

    // cout << course[0].time[1].second << endl;
}