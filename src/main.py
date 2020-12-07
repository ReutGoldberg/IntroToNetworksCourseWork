from Simulator import Simulator
from sys import argv

probabilities = [float(prob) for prob in argv[5:]]
simulator = Simulator(float(argv[1]), int(argv[2]), float(argv[3]), float(argv[4]), list(probabilities))
simulator.run()
simulator.print_results()



