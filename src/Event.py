from random import random
from math import log


class Event:
    def __init__(self, time, simulator):
        self.time = time
        self.simulator = simulator

    def __lt__(self, other):
        return self.time < other.time

    def process_event(self):
        pass


class ArriveEvent(Event):
    def __init__(self, time, simulator):
        Event.__init__(self, time, simulator)

    def process_event(self):
        qsize = len(self.simulator.patients_queue)
        if qsize == 0:
            self.simulator.patients_queue.append(self.time)
            self.simulator.schedule_event(StartTreatmentEvent(self.time, self.simulator))
        else:
            if random() <= self.simulator.probabilities[qsize]:
                self.simulator.patients_queue.append(self.time)
            else:
                self.simulator.amount_gave_up += 1


class StartTreatmentEvent(Event):
    def __init__(self, time, simulator):
        Event.__init__(self, time, simulator)

    def process_event(self):
        self.simulator.amount_vaccinated += 1
        treatment_time = -1/self.simulator.myu * log(random())
        self.simulator.patients_treatment_total_time += treatment_time
        self.simulator.patients_on_hold_total_time += self.time - self.simulator.patients_queue[0]
        self.simulator.schedule_event(LeaveEvent(self.time + treatment_time, self.simulator))


class LeaveEvent(Event):
    def __init__(self, time, simulator):
        Event.__init__(self, time, simulator)

    def process_event(self):
        self.simulator.last_treatment_time = self.time
        self.simulator.patients_queue.popleft()
        if len(self.simulator.patients_queue) != 0:
            self.simulator.schedule_event(StartTreatmentEvent(self.time, self.simulator))


