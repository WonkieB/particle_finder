SINGLE_DIR = single
MPI_DIR = MPI
OPENMP_DIR = OpenMP
CUDA_DIR = CUDA

single: comp_single run_single

comp_single: 
	cd $(SINGLE_DIR); gcc particle_finder.c my_timers.c -o particle_finder -lm

run_single: particles.csv
	@cd $(SINGLE_DIR); ./particle_finder

clean:
	cd $(SINGLE_DIR); rm particle_finder
	rm particles.csv

particles.csv:
	python3 particles_gen.py