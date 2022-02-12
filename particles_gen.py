import csv  
import random

range_min = -100.0
range_max = 100.0

def randomize():
    return random.uniform(range_min, range_max)

def particles_gen(num_of_parts, csv_path):
    header = ['x', 'y', 'z', 'mass']

    with open(csv_path, 'w') as f:
        writer = csv.writer(f)

        # write the header
        writer.writerow(header)

        # write the data
        for i in range(num_of_parts):
            data = [randomize(), randomize(), randomize()]
            writer.writerow(data)
        
        
if __name__ == "__main__":
    num_of_parts = 10000
    csv_path = "particles.csv"
    
    particles_gen(num_of_parts, csv_path)
    
