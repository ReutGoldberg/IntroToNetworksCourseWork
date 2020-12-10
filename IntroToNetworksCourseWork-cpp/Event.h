//
// Created by reutg on 09/12/2020.
//

#ifndef INTROTONETWORKSCOURSEWORK_EVENT_H
#define INTROTONETWORKSCOURSEWORK_EVENT_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <array>
#include <list>
#include <stack>
#include <queue>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unordered_map>
#include <math.h>
#include <random> //TODO check random library c++ 11

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::list;
using std::array;
using std::stack;
using std::unordered_map;
using std::queue;
using std::to_string;

class Event {
    double time;
public:
    explicit Event(double time) : time(time){}
    virtual ~Event() = default;
    virtual void process_event(){}
    double getTime();
};

class Simulator {
    double time; //simulator time
    double runtime; //total run time
    int amount_queues;
    double _lambda;
    double myu;
    vector<double> probabilities;
    queue<double> patients_queue;
    vector<double> avg_time_queue_sizes;
    list<Event> events_queue;
    double last_treatment_time;
    double patients_on_hold_total_time;
    double patients_treatment_total_time;
    int amount_vaccinated;
    int amount_gave_up;
    static Simulator* instance; //TODO check if needed
    Simulator(int T, int M, double _lambda, double myu, vector<double> probabilities);
public:
    void addLast_treatment_time(double toAdd);
    void addAvg_time_queue_sizes(int i, double toAdd);
    void setLast_treatment_time(double newVal);
    void addPatients_on_hold_total_time(double toAdd);
    void addPatients_treatment_total_time(double toAdd);
    void addAmount_vaccinated(int toAdd);
    void addAmount_gave_up(int toAdd);
    //Simulator(Simulator&) = delete;
    Simulator& operator=(Simulator&) = delete;
    //Simulator(const Simulator&) = delete;
    Simulator& operator=(const Simulator&) = delete;
    ~Simulator()=default;
    static Simulator* getInstance(int T = 0, int M = 0, double _lambda = 0, double myu = 0,
                                  vector<double> probabilities = vector<double>()) // make Simulator singleton
    {
        if(instance == nullptr)
            instance = new Simulator(T, M, _lambda, myu, probabilities);
        return instance;
    }
    queue<double>* getPatients_queue();
    double getTime();
    double getMyu();
    void schedule_event (Event* ev); //TODO check is its ok
    void run();
    void print_results();
    bool toVaccinateOrNotToVaccinate(int i);
};

bool sortEvent (Event& first, Event& second);

class ArriveEvent : public Event{
public:
    explicit ArriveEvent(double time) : Event(time){}
    void process_event() override;
    ~ArriveEvent() override = default;

};

class StartTreatmentEvent : public Event{
public:
    explicit StartTreatmentEvent(double time) : Event(time){}
    void process_event() override;
    ~StartTreatmentEvent() override = default;

};

class LeaveEvent : public Event{
public:
    explicit LeaveEvent(double time) : Event(time){}
    void process_event() override;
    ~LeaveEvent() override = default;

};

#endif //INTROTONETWORKSCOURSEWORK_EVENT_H
