from heapq import heapify, heappop, heappush
from collections import deque
from Event import Event, ArriveEvent
from random import expovariate, uniform
import time

class Simulator(object):

    def __init__(self, T, M, _lambda, myu, probabilities):
        self.runtime = T
        self.amount_queues = M
        self._lambda = _lambda
        self.myu = myu
        self.probabilities = probabilities
        self.last_treatment_time = 0
        self.amount_vaccinated = 0
        self.amount_gave_up = 0
        self.patients_queue = deque()
        self.patients_on_hold_total_time = 0
        self.patients_treatment_total_time = 0
        self.avg_time_queue_sizes = [0] * len(probabilities)
        self.events_queue = []
        self.time = 0

    def scheduleEvent(self, event):
        heappush(self.events_queue, event)

    def run(self):
        t = expovariate(self._lambda)
        while t < self.runtime:
            #heapq.heappush(self.events_queue, ArriveEvent(t, self))
            self.events_queue.append(ArriveEvent(t, self))
            t += expovariate(self._lambda)
        heapify(self.events_queue)

        while len(self.events_queue) != 0:
            event = heappop(self.events_queue)
            self.avg_time_queue_sizes[len(self.patients_queue)] += (event.time - self.time)
            self.time = event.time
            event.processEvent()

        A_Ti = [self.avg_time_queue_sizes[0]]
        for i in range(1, len(self.avg_time_queue_sizes)):
            A_Ti.append(self.avg_time_queue_sizes[i] / self.amount_queues)

        Z_i = [self.avg_time_queue_sizes[0] / self.last_treatment_time]
        for i in range(1, len(self.avg_time_queue_sizes)):
            Z_i.append(self.avg_time_queue_sizes[i] / (self.last_treatment_time * self.amount_queues))

        Tw = self.patients_on_hold_total_time / self.amount_vaccinated
        Ts = self.patients_treatment_total_time / self.amount_vaccinated
        avg_lambda = self.amount_vaccinated / self.last_treatment_time

        output = ""
        for i in range(len(A_Ti)):
            output += str(A_Ti[i]) + ' '

        for i in range(len(Z_i)):
            if i != len(Z_i)-1:
                output += str(Z_i[i]) + ' '
            else:
                output += str(Z_i[i])
        print(self.amount_vaccinated, self.amount_gave_up, self.last_treatment_time, output, Tw, Ts, avg_lambda)





