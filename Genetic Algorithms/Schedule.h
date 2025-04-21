// A Chao Thao
// Program 2: Genetic Algorithms
// 04/20/2025

#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "ScheduleAssignment.h"
#include "Activity.h"
#include <vector>
#include <string>
using namespace std;

// This struct will contain the full schedule
struct Schedule {
    vector<ScheduleAssignment> assignments;
    double fitness = 0.0;

    // Calculation is base on constraints and preferences
    void calculateFitness(const vector<Activity>& activities, const vector<Room>& rooms, const vector<string>& times);
};

#endif
