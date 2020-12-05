import heapq
from queue import Queue
from Event import Event, ArriveEvent
from random import expovariate, uniform


def singleton(cls):
    return cls()


@singleton
class Simulator(object):
    # _instance = None
    #
    # def __new__(cls, *args, **kwargs):
    #     if not isinstance(cls._instance, Simulator):
    #         cls._instance = object.__new__(cls, *args, **kwargs)
    #     return cls._instance
    #
    # def get_instance(self):
    #     return self

    def __init__(self, T: int, M: int, _lambda: float, myu: float, probabilities: list):
        self.runtime: int = T
        self.amount_queues: int = M
        self._lambda: float = _lambda
        self.myu: float = myu
        self.probabilities: list = probabilities
        self.last_treatment_time: float = 0     # done
        self.amount_vaccinated: int = 0         # done
        self.amount_gave_up: int = 0            # done
        self.patients_queue = Queue()
        self.patients_on_hold_times: list = []   # done
        self.patients_treatment_time: list = []  # done
        self.avg_time_queue_sizes: list = []     # done
        self.events_queue: list = []
        self.time: float = 0

    def scheduleEvent(self, event: Event):
        heapq.heappush(self.events_queue, event)

    def run(self):
        t = expovariate(self._lambda)
        while t < self.runtime:
            heapq.heappush(self.events_queue, ArriveEvent(t))
            t += expovariate(self._lambda)

        while len(self.events_queue) != 0:
            event = heapq.heappop(self.events_queue)
            self.avg_time_queue_sizes[self.patients_queue.qsize()] = event.time - self.time
            self.time = event.time
            event.processEvent()

        A_Ti = [self.avg_time_queue_sizes[0]]
        for i in range(1, len(self.avg_time_queue_sizes)):
            A_Ti[i] = self.avg_time_queue_sizes[i] / self.amount_queues
        Z_i = []
        for i in range(len(self.avg_time_queue_sizes)):
            Z_i.append(self.avg_time_queue_sizes[i] / self.last_treatment_time)

        Tw = sum(self.patients_on_hold_times) / len(self.patients_on_hold_times)
        Ts = sum(self.patients_treatment_time) / len(self.patients_treatment_time)
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





