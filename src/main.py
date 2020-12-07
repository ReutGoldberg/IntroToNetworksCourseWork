from Simulator import Simulator
import time
from sys import argv

t = time.time()
probabilities = [float(prob) for prob in argv[5:]]
simulator = Simulator(float(argv[1]), int(argv[2]), float(argv[3]), float(argv[4]), list(probabilities))
simulator.run()
print(time.time() - t)


