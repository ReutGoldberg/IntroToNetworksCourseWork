import abc
from Simulator import *


class Event(abc.ABC):
    def __init__(self, time):
        self.time = time
        self.simulator = singleton(Simulator)

    def __lt__(self, other):
        return self.time < other.time

    @abc.abstractmethod
    def processEvent(self):
        ...


class ArriveEvent(Event):
    def __init__(self, time):
        super(Event, self).__init__(time)

    def processEvent(self):
        qsize = self.simulator.patients_queue.qsize()
        if qsize == 0:
            self.simulator.patients_queue.put(self.time)
            self.simulator.scheduleEvent(self.simulator, StartTreatmentEvent(self.time))
        else:
            if uniform(0, 1) <= self.simulator.probabilities[qsize]:
                self.simulator.patients_queue.put(self.time)
            else:
                self.simulator.amount_gave_up += 1


class StartTreatmentEvent(Event):
    def __init__(self, time):
        super(Event, self).__init__(time)

    def processEvent(self):
        self.simulator.amount_vaccinated += 1
        treatment_time = expovariate(self.simulator.myu)
        self.simulator.patients_treatment_time.append(treatment_time)
        on_hold_time = self.simulator.time - self.simulator.patients_queue[0]
        self.simulator.patients_on_hold_times.append(on_hold_time)
        self.simulator.scheduleEvent(self.simulator, LeaveEvent(self.time + treatment_time))


class LeaveEvent(Event):
    def __init__(self, time):
        super(Event, self).__init__(time)

    def processEvent(self):
        heapq.heappop(self.simulator.events_queue)
        self.simulator.last_treatment_time = self.time
        self.simulator.patients_queue.get()
        if not self.simulator.patients_queue.empty():
            self.simulator.scheduleEvent(self.simulator, StartTreatmentEvent(self.time))


