// A Chao Thao
// Program 2: Genetic Algorithms
// 04/20/2025

#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "Schedule.h"
#include <vector>
#include <string>
using namespace std;

// These method comment is in the .cpp file
string randomChoice(const vector<string>& options);
int roomCapacity(const string& name, const vector<Room>& rooms);
bool isPreferredFacilitator(const string& facilitator, const Activity& activity);
bool isOtherFacilitator(const string& facilitator, const Activity& activity);
int timeHour(const string& time);
vector<Schedule> generateInitialPopulation(const vector<Activity>& activities, const vector<Room>& rooms);
void mutation(Schedule& schedule);
Schedule crossover(const Schedule& parent1, const Schedule& parent2);
int selectIndexBySoftmax(const vector<Schedule>& population);
Schedule runGeneticAlgorithm(vector<Schedule>& population, const vector<Activity>& activities, const vector<Room>& rooms);

#endif
