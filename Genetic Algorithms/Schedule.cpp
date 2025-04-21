// A Chao Thao
// Program 2: Genetic Algorithms
// 04/20/2025

#include "Schedule.h"
#include <map>
#include <set>
#include <cmath>
#include <algorithm>

// This method will use the room name to find its capacity
int roomCapacity(const string& name, const vector<Room>& rooms);

// This methon will check to see if a facilitator is preferred for an activity
bool isPreferredFacilitator(const string& facilitator, const Activity& activity);

// This method will check to see if a facilitator is acceptable
bool isOtherFacilitator(const string& facilitator, const Activity& activity);

// This method will convert the time string into an integer hour for comparison reason
int timeHour(const string& time);

// This method will calculate a fitness score based on scheduling contraints and preferences
void Schedule::calculateFitness(const vector<Activity>& activities, const vector<Room>& rooms, const vector<string>& times) {
    map<string, Activity> activityMap;
    for (const auto& act : activities) activityMap[act.name] = act;

    fitness = 0.0; // fitness starts at 0
    map<string, set<string>> timeRoom; // Tracks all activities that share a room and time
    map<string, set<string>> facilitatorTime; // Track all facilitator scheduling conflicts
    map<string, int> facilitatorLoad; // Track all activities a facilitator has
    map<string, string> activityTime; // Store all time assignments
    map<string, string> activityRoom; // Store all room assignments

    for (const auto& assign : assignments) {
        const auto& activity = activityMap[assign.activityName];
        int roomCap = roomCapacity(assign.room, rooms);

        // Check to see if activity is schedule at the same time in the same room
        string key = assign.time + ":" + assign.room;
        if (!timeRoom[key].empty()) fitness -= 0.5;
        timeRoom[key].insert(assign.activityName);

        // Activities is in a room too small for its expected enrollment
        if (roomCap < activity.expectedEnrollment) fitness -= 0.5;
        // Activities is in a room with capacity > 6 times expected enrollment
        else if (roomCap > activity.expectedEnrollment * 6) fitness -= 0.4;
        // Activities is in a room with capacity > 3 times expected enrollment
        else if (roomCap > activity.expectedEnrollment * 3) fitness -= 0.2;
        else fitness += 0.3; // Otherwise

        // Activities is overseen by a preferred facilitator
        if (isPreferredFacilitator(assign.facilitator, activity)) fitness += 0.5;
        // Activities is overseen by another facilitator listed for that activity
        else if (isOtherFacilitator(assign.facilitator, activity)) fitness += 0.2;
        else fitness -= 0.1; // Activities is overseen by some other facilitator

        string fkey = assign.facilitator + ":" + assign.time;
        // Activity facilitator is scheduled for only 1 activity in this time slot
        if (facilitatorTime[fkey].empty()) fitness += 0.2;
        else fitness -= 0.2; // Activity facilitator is scheduled for more than one activity at the same time
        facilitatorTime[fkey].insert(assign.activityName);

        facilitatorLoad[assign.facilitator]++;
        activityTime[assign.activityName] = assign.time;
        activityRoom[assign.activityName] = assign.room;
    }

    for (const auto& f : facilitatorLoad) {
        if (f.first == "Tyler" && f.second < 2) continue;
        if (f.second > 4) fitness -= 0.5; // Facilitator is scheduled to oversee more than 4 activities total
        else if (f.second < 2) fitness -= 0.4; // Facilitator is scheduled to oversee 1 or 2 activities*
    }

    // Activity-specific adjustments:
    int t101A = timeHour(activityTime["SLA101A"]);
    int t101B = timeHour(activityTime["SLA101B"]);
    int t191A = timeHour(activityTime["SLA191A"]);
    int t191B = timeHour(activityTime["SLA191B"]);

    // The 2 sections of SLA 101 are more than 4 hours apart
    if (abs(t101A - t101B) >= 4) fitness += 0.5;
    // Both sections of SLA 101 are in the same time slot
    if (t101A == t101B) fitness -= 0.5;

    // The 2 sections of SLA 191 are more than 4 hours apart
    if (abs(t191A - t191B) >= 4) fitness += 0.5;
    // Both sections of SLA 191 are in the same time slot
    if (t191A == t191B) fitness -= 0.5;

    vector<pair<string, string>> pairs = {
        {"SLA191A", "SLA101A"}, {"SLA191B", "SLA101B"},
        {"SLA191A", "SLA101B"}, {"SLA191B", "SLA101A"}
    };

    for (auto& [a, b] : pairs) {
        int time1 = timeHour(activityTime[a]);
        int time2 = timeHour(activityTime[b]);
        if (abs(time1 - time2) == 1) {
            // A section of SLA 191 and a section of SLA 101 are overseen in consecutive time slots
            fitness += 0.5;
            bool inRomanOrBeach =
                (activityRoom[a].find("Roman") != string::npos || activityRoom[a].find("Beach") != string::npos ||
                    activityRoom[b].find("Roman") != string::npos || activityRoom[b].find("Beach") != string::npos);
            bool oneNotIn =
                (activityRoom[a].find("Roman") == string::npos && activityRoom[a].find("Beach") == string::npos) ||
                (activityRoom[b].find("Roman") == string::npos && activityRoom[b].find("Beach") == string::npos);
            // In this case only (consecutive time slots), one of the activities is in Roman or Beach,
            // and the other isn’t
            if (inRomanOrBeach && oneNotIn) fitness -= 0.4;
        }
        // A section of SLA 191 and a section of SLA 101 are taught separated by 1 hour 
        else if (abs(time1 - time2) == 2) {
            fitness += 0.25;
        }
        // A section of SLA 191 and a section of SLA 101 are taught in the same time slot
        else if (time1 == time2) {
            fitness -= 0.25;
        }
    }
}