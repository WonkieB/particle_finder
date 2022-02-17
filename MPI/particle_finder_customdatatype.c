#include "particle_finder.h"
   
int glob_c = 0;

int main(int argc, char** argv ){
    Point start = {.x=0.0, .y=0.0, .z=0.0};
    read_file(in_path);
    MPI_Init(&argc, &argv); // aby z konsoli przyjal procesy
    MPI_Comm_size( MPI_COMM_WORLD, &size); // z inita do tej zmiennej zapisze
    MPI_Comm_rank( MPI_COMM_WORLD, &rank); // wskaznik na numer watku
    //printf("HELLO. I am %d out of %d ... \n", rank, size);
  
    int i;
    int loc_c = 0; 

    MPI_Datatype point_type;
    int lengths[3] = { 1, 1, 1};
    MPI_Aint displacements[3];
    Point dummy_point;
    MPI_Aint base_address;
    MPI_Get_address(&dummy_point, &base_address);
    MPI_Get_address(&dummy_point.x, &displacements[0]);
    MPI_Get_address(&dummy_point.y, &displacements[1]);
    MPI_Get_address(&dummy_point.z, &displacements[2]);
    displacements[0] = MPI_Aint_diff(displacements[0], base_address);
    displacements[1] = MPI_Aint_diff(displacements[1], base_address);
    displacements[2] = MPI_Aint_diff(displacements[2], base_address);

    MPI_Datatype types[3] = { MPI_FLOAT, MPI_FLOAT, MPI_FLOAT };
    MPI_Type_create_struct(3, lengths, displacements, types, &point_type);
    MPI_Type_commit(&point_type);

    start_time();
    
    loc_c = find_neighbours(start, 10000.5);
        
    printf("Hello, local %i , global %i \n", loc_c, glob_c);
    
    MPI_Reduce(&loc_c, &glob_c, 1, MPI_INT, MPI_SUM, 0,  MPI_COMM_WORLD);
    MPI_Gather(results, loc_c, point_type, global_results, glob_c, point_type, 0, MPI_COMM_WORLD);
    if(rank == 0){
      	save_results(out_path);
      	stop_time();
      	print_time("MPI elapsed in ");
    }
    MPI_Finalize();

    
    return 0;
}

// calculate euclidean metric in a 3D spaceS
float calc_distance(Point A, Point B){
    float distance;
    distance = sqrt(pow((A.x - B.x),2) + pow((A.y - B.y),2) + pow((A.z - B.z),2));
    return distance;
}

// find all particles at a given distance from the starting point
int find_neighbours(Point start, float distance){
    num_of_results = 0;  
    for(size_t i=0;i<NUM_PARTS;i++){
        if(calc_distance(particles[i], start) < distance){
            results[num_of_results] = particles[i];
            num_of_results++;
        }
    return num_of_results;
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
