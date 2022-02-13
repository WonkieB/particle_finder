#include "particle_finder.h"

int main(){
    omp_set_num_threads(8);
    Point start = {.x=0.0, .y=0.0, .z=0.0};
    read_file(in_path);
    start_time();
    find_neighbours(start, 100000.5);
    stop_time();
    save_results(out_path);
    
    print_time("OpenMP elapsed in ");
    return 0;
}

// calculate euclidean metric in a 3D space
float calc_distance(Point A, Point B){
    float distance;
    distance = sqrt(pow((A.x - B.x),2) + pow((A.y - B.y),2) + pow((A.z - B.z),2));
    return distance;
}

// find all particles at a given distance from the starting point
void find_neighbours(Point start, float distance){
    num_of_results = 0;
    omp_lock_t lock;

    omp_init_lock(&lock);

    #pragma omp parallel for schedule(guided, 100)
    for(size_t i=0;i<NUM_PARTS;i++){
        if(calc_distance(particles[i], start) < distance){
            omp_set_lock(&lock);
            results[num_of_results] = particles[i];
            num_of_results++;
            omp_unset_lock(&lock);
        }
    }
    omp_destroy_lock(&lock);
}

// read input data from file and store in particles array
void read_file(const char* path){
    FILE *fp;
    char row[MAXLENGTH];
    char header[MAXLENGTH];
    char *token;
    Point *particle_p = particles;

    fp = fopen(path,"r");

    fgets(header, MAXLENGTH, fp);

    while (feof(fp) != true){
        fgets(row, MAXLENGTH, fp);
        //printf("Row: %s", row);

        token = strtok(row, ",");
        if(token != NULL){
            particle_p->x = atof(token);
        }

        token = strtok(NULL, ",");
        if(token != NULL){
            particle_p->y = atof(token);
        }

        token = strtok(NULL, ",");
        if(token != NULL){
            particle_p->z = atof(token);
            //printf("Particle: x=%f, y=%f, z=%f\n\n", particle_p->x, particle_p->y, particle_p->z);
            particle_p++;
        }
    }
}

// save found neighbours to file
void save_results(const char *path){
    FILE *results_f;
    results_f = fopen(path,"w");
    for(size_t i=0; i < num_of_results; i++){
        char result[MAXLENGTH]; 
        sprintf(result,"Result %li: x=%f, y=%f, z=%f\n", i+1, results[i].x, results[i].y, results[i].z);
        fputs(result, results_f);
    }
    fclose(results_f);
    printf("Found %li neighbours.\n", num_of_results);
}
