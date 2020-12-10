//
// Created by reutg on 09/12/2020.
//

#include "Event.h"


void ArriveEvent::process_event() {
    Simulator* sim = sim->getInstance();
    int qsize = sim->getPatients_queue()->size();
    if(qsize==0){
        sim->getPatients_queue()->push(this->getTime());
        sim->schedule_event(new StartTreatmentEvent(this->getTime()));
    } else{
        double r= (double)rand()/(RAND_MAX);
        if(sim->toVaccinateOrNotToVaccinate(qsize))
            sim->getPatients_queue()->push(this->getTime());
        else
            sim->addAmount_gave_up(1);
    }
}

void StartTreatmentEvent::process_event() {
    Simulator* sim = sim->getInstance();
    sim->addAmount_vaccinated(1);
    double treatment_time = (-1/sim->getMyu())*log(rand());
    sim->addPatients_treatment_total_time(treatment_time);
    sim->addPatients_on_hold_total_time(this->getTime()-sim->getPatients_queue()->front());
    sim->schedule_event(new LeaveEvent(this->getTime()+treatment_time));
}

void LeaveEvent::process_event() {
    Simulator* sim = sim->getInstance();
    sim->setLast_treatment_time(this->getTime());
    sim->getPatients_queue()->pop(); //TODO check poop right side
    if(!sim->getPatients_queue()->empty())
        sim->schedule_event(new StartTreatmentEvent(this->getTime()));
}

double Event::getTime() {
    return this->time;
}


