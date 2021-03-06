#ifndef PARTICLE_FINDER_H
#define PARTICLE_FINDER_H

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <mpi.h>
#include "my_timers.h"

#define NUM_PARTS 10000000
#define MAXLENGTH 100
const char* in_path = "../particles.csv";
const char* out_path = "results.txt";

typedef struct {
    float x;
    float y;
    float z;
} Point;

float particles_x[NUM_PARTS];
float particles_y[NUM_PARTS];
float particles_z[NUM_PARTS];
Point results[NUM_PARTS];
size_t num_of_results;
int size, rank;

float calc_distance(Point A, Point B);
void find_neighbours(Point start, float distance, int index);
void read_file(const char* path);
void save_results(const char *path);

#endif // PARTICLE_FINDER_H
