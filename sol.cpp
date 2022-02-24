#include <bits/stdc++.h>
using namespace std;

typedef string skillName;

struct Contributor
{
    string name;
    bool isAvailable;
    int noOfSkills;
    map<skillName, int> skills;
};

struct Project
{
    string name;
    int duration;
    int score;
    int deadline;
    int noOfRoles;
    map<skillName, int> roles;
};

void solve()
{
    int C, P; // no of contributors, no of projects
    vector<Contributor> contributors(C);
    vector<Project> projects(P);

    for (int i = 0; i < C; i++)
    {
        cin >> contributors[i].name >> contributors[i].noOfSkills;
        for (int j = 0; j < contributors[i].noOfSkills; j++)
        {
            string skill;
            int level;
            cin >> skill >> level;
            contributors[i].skills[skill] = level;
        }
    }

    for (int i = 0; i < P; i++)
    {
        cin >> projects[i].name >> projects[i].duration >> projects[i].score >> projects[i].deadline >> projects[i].noOfRoles;
        for (int j = 0; j < projects[i].noOfRoles; j++)
        {
            string skill;
            int level;
            cin >> skill >> level;
            projects[i].roles[skill] = level;
        }
    }
}

int main()
{
    solve();
    return 0;
}