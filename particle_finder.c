#include <math.h>
#include <stdio.h>

#define NUM_OF_PARTS 1

typedef struct {
    float x;
    float y;
    float z;
} Point;

Point particles[NUM_OF_PARTS];
Point results[NUM_OF_PARTS];
size_t num_of_results;

// calculate euclidean metric in a 3D space
float calc_distance(Point A, Point B){
    float distance;
    distance = sqrt(pow((A.x - B.x),2) + pow((A.y - B.y),2) + pow((A.z - B.z),2));
    return distance;
}

// find all particles at a given distance from the starting point
void find_neighbours(Point start, float distance){
    num_of_results = 0;
    for(int i=0;i<NUM_OF_PARTS;i++){
        if(calc_distance(particles[i], start) < distance){
            results[num_of_results] = particles[i];
            num_of_results++;
        }
    }
}
int main(){

    Point first = {.x=1.0, .y=1.0, .z=0.0};
    Point second = {.x=0.0, .y=0.0, .z=1.0};
    printf("%f\n", calc_distance(first,second));
    return 0;
}