import abc
from Simulator import *
from random import expovariate, uniform


class Event(abc.ABC):
    def __init__(self, time, simulator):
        self.time = time
        self.simulator = simulator

    def __lt__(self, other):
        return self.time < other.time

    @abc.abstractmethod
    def processEvent(self):
        ...


class ArriveEvent(Event):
    def __init__(self, time, simulator):
        Event.__init__(self, time, simulator)

    def processEvent(self):
        qsize = len(self.simulator.patients_queue)
        if qsize == 0:
            self.simulator.patients_queue.append(self.time)
            self.simulator.scheduleEvent(StartTreatmentEvent(self.time, self.simulator))
        else:
            if uniform(0, 1) <= self.simulator.probabilities[qsize]:
                self.simulator.patients_queue.append(self.time)
            else:
                self.simulator.amount_gave_up += 1


class StartTreatmentEvent(Event):
    def __init__(self, time, simulator):
        Event.__init__(self, time, simulator)

    def processEvent(self):
        self.simulator.amount_vaccinated += 1
        treatment_time = expovariate(self.simulator.myu)
        self.simulator.patients_treatment_time.append(treatment_time)
        on_hold_time = self.time - self.simulator.patients_queue[0]
        self.simulator.patients_on_hold_times.append(on_hold_time)
        self.simulator.scheduleEvent(LeaveEvent(self.time + treatment_time, self.simulator))


class LeaveEvent(Event):
    def __init__(self, time, simulator):
        Event.__init__(self, time, simulator)

    def processEvent(self):
        self.simulator.last_treatment_time = self.time
        self.simulator.patients_queue.popleft()
        if len(self.simulator.patients_queue) != 0:
            self.simulator.scheduleEvent(StartTreatmentEvent(self.time, self.simulator))


