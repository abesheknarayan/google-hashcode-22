// g++ -o ./bin/sol sol.cpp

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

typedef string skillName;
const int inf = 1e8;

struct Contributor
{
    string name;
    int index;
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
    map<skillName, int> printmap;
    map<skillName, vector<int>> roles;
    map<pair<skillName, int>, vector<int>> ordered_roles;
};

struct Output
{
    ll score;
    int noOfProjects;
    map<string, vector<string>> matching;
    map<string, vector<pair<int, int>>> debug_level;
};

Output greedyMatch(vector<Project> &projects, vector<Contributor> &contributors)
{

    Output op;
    op.score = 0;
    op.noOfProjects = 0;
    // we have ordering of projects
    map<skillName, set<pair<int, int>>> freeContributors;

    int i = 0;
    for (auto c : contributors)
    {
        // remove assert at end;
        // assert(c.skills.size() == 1);
        for (auto skill : c.skills)
        {
            freeContributors[skill.first].insert({skill.second, i++});
        }
    }

    multiset<pair<int, int>> usedContributors;

    set<int> timePoints;

    int time = 0;

    for (auto project : projects)
    {

        // take all finished contributors
        while (!usedContributors.empty())
        {
            auto nowc = *usedContributors.begin();
            if (nowc.first <= time)
            {
                contributors[nowc.second].isAvailable = true;
                auto onlySkill = *contributors[nowc.second].skills.begin();
                freeContributors[onlySkill.first].insert({onlySkill.second, contributors[nowc.second].index});
                usedContributors.erase(usedContributors.begin());
            }
            else
            {
                break;
            }
        }

        // asign contributors
        int assigned = 0;
        map<skillName, pair<int, int>> added;
        vector<pair<string, int>> ordered_contributors;
        vector<pair<int, int>> debug_level;
        int f = 0;
        for (auto r : project.roles)
        {
            string rskillname = r.first;
            sort(r.second.begin(),r.second.end());
            for (auto rlevel : r.second)
            {

                auto foundc = freeContributors[rskillname].lower_bound({rlevel, -inf});
                if (foundc != freeContributors[rskillname].end())
                {
                    // found some contri
                    assigned++;
                    added[rskillname] = *foundc;
                    freeContributors[rskillname].erase(foundc);
                    assert(foundc->first >= rlevel);
                    ordered_contributors.push_back({contributors[foundc->second].name, project.ordered_roles[{rskillname, rlevel}].back()});
                    project.ordered_roles[{rskillname,rlevel}].pop_back();
                    debug_level.push_back({foundc->first, rlevel});
                }
                else
                {
                    f = 1;
                    break;
                }
            }
            if(f)break;
        }
        if (assigned == project.noOfRoles)
        {
            int finish = time + project.duration;
            op.score += max(0, project.score - (max(0, finish - project.deadline)));
            op.noOfProjects++;
            assert(assigned == ordered_contributors.size());
            // add them to used
            for (auto ppl : added)
            {
                usedContributors.insert({finish, ppl.second.second});
            }
            vector<string> reordered_contributors(assigned);
            for (auto ppl : ordered_contributors)
            {
                reordered_contributors[ppl.second] = ppl.first;
            }
            op.matching[project.name] = reordered_contributors;
            op.debug_level[project.name] = debug_level;
        }
        else
        {
            // free added ppl
            for (auto ppl : added)
            {
                freeContributors[ppl.first].insert(ppl.second);
            }
        }
    }

    return op;
}

void print(Output &op)
{
    cout << op.noOfProjects << "\n";
    for (auto project : op.matching)
    {
        cout << project.first << "\n";
        for (auto name : project.second)
        {
            cout << name << " ";
        }
        cout << "\n";
    }
}

void solve()
{
    int C, P; // no of contributors, no of projects

    cin >> C >> P;

    vector<Contributor> contributors(C);
    vector<Project> projects(P);

    for (int i = 0; i < C; i++)
    {
        contributors[i].index = i;
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
            projects[i].ordered_roles[{skill, level}].push_back(j);
            projects[i].roles[skill].push_back(level);
        }
    }

    // for d and e
    ll score = 0;

    {
        Output op1;
        // sorting with more score + less deadline
        {
            vector<Project> nowprojects = projects;
            vector<Contributor> nowcontributors = contributors;

            sort(nowprojects.begin(), nowprojects.end(), [](const Project &a, const Project &b)
                 {
                if(a.score == b.score) {
                    return a.deadline < b.deadline;
                }
                return a.score > b.score; });

            op1 = greedyMatch(nowprojects, nowcontributors);
        }

        Output op2;
        // sorting with more score + more deadline
        {
            vector<Project> nowprojects = projects;
            vector<Contributor> nowcontributors = contributors;

            sort(nowprojects.begin(), nowprojects.end(), [](const Project &a, const Project &b)
                 {
                if(a.score == b.score) {
                    return a.deadline > b.deadline;
                }
                return a.score > b.score; });

            op2 = greedyMatch(nowprojects, nowcontributors);
        }


        if (op1.score > op2.score)
        {
            print(op1);
        }
        else
        {
            print(op2);
        }
    }
}

int main()
{
    solve();
    return 0;
}
