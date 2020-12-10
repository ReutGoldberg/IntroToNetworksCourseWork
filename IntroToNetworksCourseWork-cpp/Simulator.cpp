//
// Created by reutg on 09/12/2020.
//


#include "Event.h"

//Simulator* Simulator::instance = nullptr;

Simulator::Simulator(int T, int M, double _lambda, double myu, vector<double> probabilities){
    this->runtime = T;
    this->amount_queues = M;
    this->_lambda=_lambda;
    this->myu=myu;
    this->probabilities = vector<double>(probabilities);
    this->last_treatment_time=0;
    this->amount_vaccinated=0;
    this->amount_gave_up=0;
    this->patients_queue = queue<double>();
    this->patients_on_hold_total_time=0;
    this->patients_treatment_total_time=0;
    this->avg_time_queue_sizes = vector<double>(probabilities.size(),0);
    this->events_queue = list<Event>();
    this->time=0;
    //this->instance= nullptr;
}

double Simulator::getTime() {
    return this->time;
}
double Simulator::getMyu() {
    return this->myu;
}
void Simulator::schedule_event(Event* ev) {
    this->events_queue.push_back(*ev);
}
void Simulator::run() {
    double r= ((double)rand()/(RAND_MAX));
    double t=(-1/_lambda)*log(r);
    while(t< this->runtime){
        auto* av = new ArriveEvent(t);
        this->events_queue.push_back(*av);
        r= ((double)rand()/(RAND_MAX));
        t+=(-1/_lambda)*log(r);
    }
    this->events_queue.sort(sortEvent);
    while (!this->events_queue.empty()){
        Event& event = events_queue.front();
        avg_time_queue_sizes[patients_queue.size()] += event.getTime() - this->time;
        this->time=event.getTime();
        events_queue.pop_front();
        event.process_event();
    }
}

void Simulator::print_results() {
    vector<double> A_Ti;
    for(int i=0;i<avg_time_queue_sizes.size();i++)
        A_Ti.push_back(avg_time_queue_sizes[i]);
    vector<double> Zi;
    Zi.push_back(avg_time_queue_sizes[0]/amount_queues);
    for(int i=1;i<avg_time_queue_sizes.size();i++)
        Zi.push_back(avg_time_queue_sizes[i]);
    double Tw=patients_on_hold_total_time/amount_vaccinated;
    double Ts=patients_treatment_total_time/amount_vaccinated;
    double avg_lambda=amount_vaccinated/last_treatment_time;
    string output = "";
    output+= to_string(amount_vaccinated)+" "+to_string(amount_gave_up)+" "+to_string(last_treatment_time)+" ";
    for(int i=0; i<A_Ti.size();i++)
        output += (to_string(A_Ti[i]) + " ");
    for(int i=0; i<Zi.size();i++)
        output += (to_string(Zi[i]) + " ");
    output += to_string(Tw)+" "+to_string(Ts)+" "+to_string(avg_lambda);
    std::cout << output;
}


queue<double>* Simulator::getPatients_queue() {
    return &patients_queue;
}

void Simulator::addLast_treatment_time(double toAdd) {
    this->last_treatment_time+=toAdd;
}

void Simulator::addPatients_on_hold_total_time(double toAdd) {
    this->patients_on_hold_total_time+=toAdd;
}

void Simulator::addPatients_treatment_total_time(double toAdd) {
    this->patients_treatment_total_time+=toAdd;
}

void Simulator::addAmount_vaccinated(int toAdd) {
    this->amount_vaccinated+=toAdd;
}

void Simulator::addAmount_gave_up(int toAdd) {
    this->amount_gave_up+=toAdd;
}

void Simulator::setLast_treatment_time(double newVal) {
    this->last_treatment_time=newVal;
}

void Simulator::addAvg_time_queue_sizes(int i, double toAdd) {

}

bool Simulator::toVaccinateOrNotToVaccinate(int i) {
    double r= ((double)rand()/(RAND_MAX));
    return r <= this->probabilities[i];
}

bool sortEvent( Event &first, Event &second) {
    return first.getTime() < second.getTime();
}
