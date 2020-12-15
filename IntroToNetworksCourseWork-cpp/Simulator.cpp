#include "Simulator.h"
#include <queue>
#include <ctime>

Simulator* Simulator::instance = nullptr;

Simulator::Simulator(int T, int M, double _lambda, double myu, const vector<double>& probabilities){
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
    this->events_queue = std::priority_queue<Event*, std::vector<Event *, std::allocator<Event*> >, compareEvent>();
    this->_time=0;
    Simulator::instance = nullptr;
}

double Simulator::getMyu() {
    return this->myu;
}
void Simulator::schedule_event(Event *ev) {
    this->events_queue.push(ev);
}
void Simulator::run() {
    srand((unsigned)time(nullptr));
    double r= (((double)rand() + 1)/(RAND_MAX));
    double t=(-1/_lambda)*log(r);
    while(t< this->runtime){
        this->events_queue.push(new ArriveEvent(t));
        r= (((double)rand() + 1)/(RAND_MAX));
        t+=(-1/_lambda)*log(r);
    }
    while (!this->events_queue.empty()){
        Event* event = events_queue.top();
        events_queue.pop();
        avg_time_queue_sizes[patients_queue.size()] += event->getTime() - this->_time;
        this->_time=event->getTime();
        event->process_event();
        delete event;
    }
}

void Simulator::print_results() {
    vector<double> A_Ti;
    A_Ti.push_back(avg_time_queue_sizes[0]);
    for(int i = 1; i < this->avg_time_queue_sizes.size(); i++)
        A_Ti.push_back(avg_time_queue_sizes[i]/amount_queues);
    vector<double> Zi;

    for(int i=0;i<avg_time_queue_sizes.size();i++)
        Zi.push_back(A_Ti[i]/last_treatment_time);

    double Tw=patients_on_hold_total_time/amount_vaccinated;
    double Ts=patients_treatment_total_time/amount_vaccinated;
    double avg_lambda=amount_vaccinated/last_treatment_time;
    std::string output;
    output+= to_string(amount_vaccinated)+" "+to_string(amount_gave_up)+" "+to_string(last_treatment_time)+" ";
    for(double i : A_Ti)
        output += (to_string(i) + " ");
    for(double i : Zi)
        output += (to_string(i) + " ");
    output += to_string(Tw)+" "+to_string(Ts)+" "+to_string(avg_lambda);
    std::cout << output << std::endl;
}


queue<double>* Simulator::getPatients_queue() {
    return &patients_queue;
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

bool Simulator::toVaccinateOrNotToVaccinate(int i) {
    double r = ((double)rand()/(RAND_MAX));
    return r <= this->probabilities[i];
}

