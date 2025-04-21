// A Chao Thao
// Program 2: Genetic Algorithms
// 04/20/2025

#include "GeneticAlgorithm.h"
#include <random>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>

random_device randomNumber; // Random generator device
mt19937 numGenerator(randomNumber()); // Efficient random number generator

// Global constant variables
const int POPULATION_SIZE = 500;
const int GENERATIONS_AMOUNT = 100;
const double MUTATION_RATE = 0.01;

// This vector contain all facilitators name
vector<string> facilitators = {
    "Lock", "Glen", "Banks", "Richards", "Shaw", "Singer", "Uther", "Tyler", "Numen", "Zeldin"
};

// This vector contain all the available time slots
vector<string> timeSlots = {
    "10AM", "11AM", "12PM", "1PM", "2PM", "3PM"
};

// This vector contain all the name of the room
vector<string> roomNames = {
    "Slater 003", "Roman 216", "Loft 206", "Roman 201", "Loft 310", "Beach 201", "Beach 301", "Logos 325", "Frank 119"
};

// This method will return a random choice from a given vector
string randomChoice(const vector<string>& options) {
    uniform_int_distribution<> dist(0, options.size() - 1);
    return options[dist(numGenerator)];
}

// This method will use the room name to find its capacity
int roomCapacity(const string& name, const vector<Room>& rooms) {
    for (const auto& r : rooms)
        if (r.name == name) return r.capacity;
    return 0;
}

// This methon will check to see if a facilitator is preferred for an activity
bool isPreferredFacilitator(const string& facilitator, const Activity& activity) {
    return find(activity.preferredFacilitator.begin(), activity.preferredFacilitator.end(), facilitator) != activity.preferredFacilitator.end();
}

// This method will check to see if a facilitator is acceptable
bool isOtherFacilitator(const string& facilitator, const Activity& activity) {
    return find(activity.otherFacilitator.begin(), activity.otherFacilitator.end(), facilitator) != activity.otherFacilitator.end();
}

// This method will convert the time string into an integer hour for comparison reason
int timeHour(const string& time) {
    if (time == "10AM") return 10;
    if (time == "11AM") return 11;
    if (time == "12PM") return 12;
    if (time == "1PM") return 13;
    if (time == "2PM") return 14;
    if (time == "3PM") return 15;

    return 0;
}

// This method will creates an initial population with random schedules
vector<Schedule> generateInitialPopulation(const vector<Activity>& activities, const vector<Room>& rooms) {
    vector<Schedule> population;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        Schedule s;
        for (const auto& act : activities) {
            ScheduleAssignment assign;
            assign.activityName = act.name;
            assign.facilitator = randomChoice(facilitators);
            assign.room = randomChoice(roomNames);
            assign.time = randomChoice(timeSlots);
            s.assignments.push_back(assign);
        }
        s.calculateFitness(activities, rooms, timeSlots);
        population.push_back(s);
    }
    return population;
}

// This method will randomly mutates some assignments in a schedule
void mutation(Schedule& schedule) {
    uniform_real_distribution<> prob(0.0, 1.0);
    for (auto& assign : schedule.assignments) {
        if (prob(numGenerator) < MUTATION_RATE) assign.facilitator = randomChoice(facilitators);
        if (prob(numGenerator) < MUTATION_RATE) assign.room = randomChoice(roomNames);
        if (prob(numGenerator) < MUTATION_RATE) assign.time = randomChoice(timeSlots);
    }
}

// This method will combines 2 parent schedule into a child  
Schedule crossover(const Schedule& parent1, const Schedule& parent2) {
    Schedule child;
    uniform_int_distribution<> dist(0, parent1.assignments.size() - 1);
    int crossoverPoint = dist(numGenerator);
    for (size_t i = 0; i < parent1.assignments.size(); ++i) {
        child.assignments.push_back(i < crossoverPoint ? parent1.assignments[i] : parent2.assignments[i]);
    }
    return child;
}

// This method is using softmax probability to selects an index from the population 
int selectIndexBySoftmax(const vector<Schedule>& population) {
    vector<double> expFitness;
    double maxFit = population[0].fitness;
    for (const auto& s : population)
        if (s.fitness > maxFit) maxFit = s.fitness;

    double sum = 0.0;
    for (const auto& s : population) {
        double e = exp(s.fitness - maxFit);
        expFitness.push_back(e);
        sum += e;
    }

    uniform_real_distribution<> dist(0.0, sum);
    double pick = dist(numGenerator);
    double acc = 0.0;
    for (int i = 0; i < expFitness.size(); ++i) {
        acc += expFitness[i];
        if (acc >= pick) return i;
    }
    return expFitness.size() - 1;
}

// This method will runs the genetic algorithm across multiple generations
Schedule runGeneticAlgorithm(vector<Schedule>& population, const vector<Activity>& activities, const vector<Room>& rooms) {
    for (int gen = 0; gen < GENERATIONS_AMOUNT; ++gen) {
        sort(population.begin(), population.end(), [](const Schedule& a, const Schedule& b) {
            return a.fitness > b.fitness;
            });

        vector<Schedule> nextGen;
        nextGen.insert(nextGen.end(), population.begin(), population.begin() + POPULATION_SIZE / 10);

        while (nextGen.size() < POPULATION_SIZE) {
            int i1 = selectIndexBySoftmax(population);
            int i2 = selectIndexBySoftmax(population);
            Schedule child = crossover(population[i1], population[i2]);
            mutation(child);
            child.calculateFitness(activities, rooms, timeSlots);
            nextGen.push_back(child);
        }

        population = nextGen;
        cout << "Generation " << gen + 1 << ": Best Fitness = " << population.front().fitness << endl;
    }
    return population.front();
}