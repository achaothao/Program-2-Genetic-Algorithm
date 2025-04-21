// A Chao Thao
// Program 2: Genetic Algorithms
// 04/20/2025

#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <string>
#include <vector>
using namespace std;

// This struct represent an activity with its expected enrollment, preferred faciliatators,
// and other faciliatator
struct Activity {
    string name;
    int expectedEnrollment;
    vector<string> preferredFacilitator;
    vector<string> otherFacilitator;
};

// This struct maintain room name and its capacity information
struct Room {
    string name;
    int capacity;
};

#endif