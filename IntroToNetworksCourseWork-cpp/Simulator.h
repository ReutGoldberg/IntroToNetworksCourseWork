#ifndef INTROTONETWORKSCOURSEWORK_EVENT_H
#define INTROTONETWORKSCOURSEWORK_EVENT_H

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <cstdlib>
#include <random>

using std::vector;
using std::queue;
using std::priority_queue;
using std::to_string;

class Event {
    double time;
public:
    explicit Event(double time) : time(time){}
    virtual ~Event() = default;
    virtual void process_event() = 0;
    double getTime();
};

struct compareEvent {
    bool operator() (Event *first, Event* second) const {
        return first->getTime() > second->getTime();
    }
};

class Simulator {
    double _time; //simulator time
    double runtime; //total run time
    int amount_queues;
    double _lambda;
    double myu;
    vector<double> probabilities;
    queue<double> patients_queue;
    vector<double> avg_time_queue_sizes;
    priority_queue<Event*, std::vector<Event *, std::allocator<Event*> >, compareEvent> events_queue;
    double last_treatment_time;
    double patients_on_hold_total_time;
    double patients_treatment_total_time;
    int amount_vaccinated;
    int amount_gave_up;
    static Simulator* instance; //TODO check if needed
    Simulator(int T, int M, double _lambda, double myu, const vector<double>& probabilities);
public:
    void setLast_treatment_time(double newVal);
    void addPatients_on_hold_total_time(double toAdd);
    void addPatients_treatment_total_time(double toAdd);
    void addAmount_vaccinated(int toAdd);
    void addAmount_gave_up(int toAdd);
    Simulator& operator=(Simulator&) = delete;
    Simulator& operator=(const Simulator&) = delete;
    Simulator(Simulator&) = delete;
    Simulator(const Simulator&) = delete;
    static Simulator* getInstance(int T = 0, int M = 0, double _lambda = 0, double myu = 0,
                                  const vector<double>& probabilities = vector<double>()) // make Simulator singleton
    {
        if(instance == nullptr)
            instance = new Simulator(T, M, _lambda, myu, probabilities);
        return instance;
    }
    queue<double>* getPatients_queue();
    double getMyu();
    void schedule_event (Event *ev);
    void run();
    void print_results();
    bool toVaccinateOrNotToVaccinate(int i);
};



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
