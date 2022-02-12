all: comp run

comp: 
	gcc particle_finder.c -o particle_finder -lm

run: particles.csv
	./particle_finder

clean:
	rm particle_finder

particles.csv:
	python3 particles_gen.py