#include "particle_finder.h"
   
int glob_c = 0;

int main(int argc, char** argv ){
    Point start = {.x=0.0, .y=0.0, .z=0.0};
    read_file(in_path);
    MPI_Init(&argc, &argv); // aby z konsoli przyjal procesy
    MPI_Comm_size( MPI_COMM_WORLD, &size); // z inita do tej zmiennej zapisze
    MPI_Comm_rank( MPI_COMM_WORLD, &rank); // wskaznik na numer watku
    //printf("HELLO. I am %d out of %d ... \n", rank, size);
  
    //Point start = {.x=0.0, .y=0.0, .z=0.0};
    //read_file(in_path);
    int i;
    int loc_c = 0;

    start_time();
    if(rank !=0){
        num_of_results = 0;
        for( i = rank - 1; i < NUM_PARTS; i += size - 1){
            find_neighbours(start, 100000.5, i);
            }
        loc_c = num_of_results;
        
        //printf("Hello, from %i \n", rank);
    }
    MPI_Reduce(&loc_c, &glob_c, 1, MPI_INT, MPI_SUM, 0,  MPI_COMM_WORLD);
    if(rank == 0){
      	save_results(out_path);
      	stop_time();
      	print_time("MPI elapsed in ");
    }
    MPI_Finalize();

    
    return 0;
}

// calculate euclidean metric in a 3D space
float calc_distance(Point A, Point B){
    float distance;
    distance = sqrt(pow((A.x - B.x),2) + pow((A.y - B.y),2) + pow((A.z - B.z),2));
    return distance;
}

// find all particles at a given distance from the starting point
void find_neighbours(Point start, float distance, int index ){
    if(calc_distance(particles[index], start) < distance){
         results[num_of_results] = particles[index];
         num_of_results++;
         }
             
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
    for(size_t i=0; i < glob_c; i++){
        char result[MAXLENGTH]; 
        sprintf(result,"Result %li: x=%f, y=%f, z=%f\n", i+1, results[i].x, results[i].y, results[i].z);
        fputs(result, results_f);
    }
    fclose(results_f);
    printf("Found %i neighbours.\n", glob_c);
}
