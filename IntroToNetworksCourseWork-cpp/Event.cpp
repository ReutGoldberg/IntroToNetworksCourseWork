#include "Simulator.h"


void ArriveEvent::process_event() {
    Simulator* sim = Simulator::getInstance();
    int qsize = sim->getPatients_queue()->size();
    if(qsize==0){
        sim->getPatients_queue()->push(this->getTime());
        sim->schedule_event(new StartTreatmentEvent(this->getTime()));
    }
    else
    {
        if(sim->toVaccinateOrNotToVaccinate(qsize))
            sim->getPatients_queue()->push(this->getTime());
        else
            sim->addAmount_gave_up(1);
    }
}

void StartTreatmentEvent::process_event() {
    Simulator* sim = Simulator::getInstance();
    sim->addAmount_vaccinated(1);
    double r= (((double)rand() + 1)/(RAND_MAX));
    double treatment_time = (-1/sim->getMyu())*log(r);
    sim->addPatients_treatment_total_time(treatment_time);
    sim->addPatients_on_hold_total_time(this->getTime()-sim->getPatients_queue()->front());
    sim->schedule_event(new LeaveEvent(this->getTime()+treatment_time));
}

void LeaveEvent::process_event() {
    Simulator* sim = Simulator::getInstance();
    sim->setLast_treatment_time(this->getTime());
    sim->getPatients_queue()->pop();
    if(!sim->getPatients_queue()->empty())
        sim->schedule_event(new StartTreatmentEvent(this->getTime()));
}

double Event::getTime() {
    return this->time;
}


