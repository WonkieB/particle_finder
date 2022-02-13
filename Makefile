SINGLE_DIR = single
MPI_DIR = MPI
OPENMP_DIR = OpenMP
CUDA_DIR = CUDA

.PHONY: all
all: single openmp

.PHONY: single
single: comp_single run_single

.PHONY: comp_single
comp_single: $(SINGLE_DIR)/particle_finder 

$(SINGLE_DIR)/particle_finder:
	cd $(SINGLE_DIR); gcc particle_finder.c my_timers.c -o particle_finder -lm

.PHONY: run_single
run_single: particles.csv
	@cd $(SINGLE_DIR); ./particle_finder

.PHONY: openmp
openmp: comp_openmp run_openmp

.PHONY: comp_openmp
comp_openmp: $(OPENMP_DIR)/particle_finder 
	
$(OPENMP_DIR)/particle_finder:
	cd $(OPENMP_DIR); icc -qopenmp particle_finder.c my_timers.c -o particle_finder

.PHONY: run_openmp
run_openmp: particles.csv
	@cd $(OPENMP_DIR); ./particle_finder


.PHONY: clean
clean:
	rm */particle_finder *.csv

particles.csv:
	python3 particles_gen.py
