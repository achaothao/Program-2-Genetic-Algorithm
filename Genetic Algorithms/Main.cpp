// A Chao Thao
// Program 2: Genetic Algorithms
// 04/20/2025

#include <iostream>
#include <fstream>
#include "Activity.h"
#include "Schedule.h"
#include "GeneticAlgorithm.h"

using namespace std;

int main() {
    cout << "Genetic Scheduler Initialized.\n";

    // This is the input data for the activities
    vector<Activity> activities = {
        {"SLA101A", 50, {"Glen", "Lock", "Banks", "Zeldin"}, {"Numen", "Richards"}},
        {"SLA101B", 50, {"Glen", "Lock", "Banks", "Zeldin"}, {"Numen", "Richards"}},
        {"SLA191A", 50, {"Glen", "Lock", "Banks", "Zeldin"}, {"Numen", "Richards"}},
        {"SLA191B", 50, {"Glen", "Lock", "Banks", "Zeldin"}, {"Numen", "Richards"}},
        {"SLA201", 50, {"Glen", "Banks", "Zeldin", "Shaw"}, {"Numen", "Richards", "Singer"}},
        {"SLA291", 50, {"Lock", "Banks", "Zeldin", "Singer"}, {"Numen", "Richards", "Shaw", "Tyler"}},
        {"SLA303", 60, {"Glen", "Zeldin", "Banks"}, {"Numen", "Singer", "Shaw"}},
        {"SLA304", 25, {"Glen", "Banks", "Tyler"}, {"Numen", "Singer", "Shaw", "Richards", "Uther", "Zeldin"}},
        {"SLA394", 20, {"Tyler", "Singer"}, {"Richards", "Zeldin"}},
        {"SLA449", 60, {"Tyler", "Singer", "Shaw"}, {"Zeldin", "Uther"}},
    };

    // This is the input data for available rooms 
    vector<Room> rooms = {
        {"Slater 003", 45}, {"Roman 216", 30}, {"Loft 206", 75}, {"Roman 201", 50},
        {"Loft 310", 108}, {"Beach 201", 60}, {"Beach 301", 75}, {"Logos 325", 450}, {"Frank 119", 60}
    };

    // Generate initial random population
    vector<Schedule> population = generateInitialPopulation(activities, rooms);
    cout << "Initial population generated.\n";

    // Run genetic algorithm and find the best schedule
    Schedule best = runGeneticAlgorithm(population, activities, rooms);

    // Save best result to a file (If I got it right)
    ofstream out("best_schedule.txt");
    for (const auto& assign : best.assignments) {
        out << "Activity Name: " << assign.activityName << "\n"
            << "Facilitator: " << assign.facilitator << "\n"
            << "Room: " << assign.room << "\n"
            << "Time: " << assign.time << "\n\n";
    }
    out.close();

    cout << "Best schedule saved to best_schedule.txt\n";
    return 0;
}


