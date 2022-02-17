SINGLE_DIR = single
MPI_DIR = MPI
OPENMP_DIR = OpenMP
CUDA_DIR = CUDA

.PHONY: all
all: single openmp mpi

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
	cd $(OPENMP_DIR); gcc -fopenmp particle_finder.c my_timers.c -o particle_finder -lm

.PHONY: run_openmp
run_openmp: particles.csv
	@cd $(OPENMP_DIR); ./particle_finder

.PHONY: mpi
mpi: comp_mpi run_mpi

.PHONY: comp_mpi
comp_mpi: $(MPI_DIR)/particle_finder 
	
$(MPI_DIR)/particle_finder:
	cd $(MPI_DIR); mpicc particle_finder.c my_timers.c -o particle_finder -lm

.PHONY: run_mpi
run_mpi: particles.csv
	@cd $(MPI_DIR); mpiexec --use-hwthread-cpus -np 4 ./particle_finder
	
.PHONY: clean
clean:
	rm */particle_finder *.csv

particles.csv:
	python3 particles_gen.py
