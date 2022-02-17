#include "particle_finder.h"
   
int glob_c = 0;
Point **results_p;
Point *part_results_p;
float *sub_x;
float *sub_y;
float *sub_z;

int main(int argc, char** argv ){
    Point start = {.x=0.0, .y=0.0, .z=0.0};
    MPI_File fh;
    
    int size;
    int rank;

    MPI_Init(&argc, &argv); // aby z konsoli przyjal procesy
    MPI_Comm_size( MPI_COMM_WORLD, &size); // z inita do tej zmiennej zapisze
    MPI_Comm_rank( MPI_COMM_WORLD, &rank); // wskaznik na numer watku
    //printf("HELLO. I am %d out of %d ... \n", rank, size);
    
    //Point start = {.x=0.0, .y=0.0, .z=0.0};
    //read_file(in_path);
    int i;
    int root = 0;
    int loc_c = 0;
    int size_per_proc = NUM_PARTS/size;
    sub_x = malloc(sizeof(float)*(size_per_proc));
    sub_y = malloc(sizeof(float)*(size_per_proc));
    sub_z = malloc(sizeof(float)*(size_per_proc));
    if(rank == root){
        read_file(in_path);
    }

    MPI_Scatter(particles_x, size_per_proc, MPI_FLOAT, sub_x, size_per_proc, MPI_FLOAT, root, MPI_COMM_WORLD);
    MPI_Scatter(particles_y, size_per_proc, MPI_FLOAT, sub_y, size_per_proc, MPI_FLOAT, root, MPI_COMM_WORLD);
    MPI_Scatter(particles_z, size_per_proc, MPI_FLOAT, sub_z, size_per_proc, MPI_FLOAT, root, MPI_COMM_WORLD);
        
    start_time();
    
    MPI_File_open(MPI_COMM_WORLD, "test.txt",MPI_MODE_CREATE | MPI_MODE_WRONLY,MPI_INFO_NULL,&fh);

    num_of_results = 0;
    for( i = 0; i < size_per_proc; i++){
        //printf("%f\n",sub_x[i]);
        find_neighbours(start, 100000.5, i);
        }
    loc_c = num_of_results;
    
        //FILE* f = fopen("test.txt","wb+");
        //if(f==NULL){
        //printf("failed to open file\n");exit(1);
        //}
        for (int i=0; i < 5; i++){
            char buf[42];
            //fprintf(f,"%d \n",i);
            snprintf(buf,42,"%d \n",i);
            MPI_File_write(fh,buf,strlen(buf), MPI_CHAR, NULL);
        }
        //        fclose(f);
        MPI_File_close(&fh);
    //printf("Hello, from %i \n", rank);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(&loc_c, &glob_c, 1, MPI_INT, MPI_SUM, root,  MPI_COMM_WORLD);
    
    save_results(out_path);
    if(rank == 0){
      	stop_time();
      	print_time("MPI elapsed in ");
    }
    free(sub_x);
    free(sub_y);
    free(sub_z);
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
    Point target = {.x=sub_x[index], .y=sub_y[index], .z=sub_z[index]};
    if(calc_distance(target, start) < distance){
         results[num_of_results] = target;
         num_of_results++;
         }
             
}

// read input data from file and store in particles array
void read_file(const char* path){
    FILE *fp;
    char row[MAXLENGTH];
    char header[MAXLENGTH];
    char *token;
    float *particle_px = particles_x;
    float *particle_py = particles_y;
    float *particle_pz = particles_z;

    fp = fopen(path,"r");

    fgets(header, MAXLENGTH, fp);

    while (feof(fp) != true){
        fgets(row, MAXLENGTH, fp);
        //printf("Row: %s", row);

        token = strtok(row, ",");
        if(token != NULL){
            *particle_px = atof(token);
            particle_px++;
        }

        token = strtok(NULL, ",");
        if(token != NULL){
            *particle_py = atof(token);
            particle_py++;
        }

        token = strtok(NULL, ",");
        if(token != NULL){
            *particle_pz = atof(token);
            particle_pz++;
        }
    }
}

// save found neighbours to file
void save_results(const char *path){
    MPI_File results_f;
    size_t glob_counter = 1;
    remove(path);
    //results_f = fopen(path,"w+");
    MPI_File_open(MPI_COMM_WORLD, path,MPI_MODE_CREATE | MPI_MODE_WRONLY,MPI_INFO_NULL,&results_f);

    for(size_t i=0; i < num_of_results; i++){
        char result[MAXLENGTH]; 
        sprintf(result,"Result %li: x=%f, y=%f, z=%f\n", glob_counter++, results[i].x, results[i].y, results[i].z);
        //fputs(result, results_f);
        MPI_File_write_shared(results_f,result,strlen(result), MPI_CHAR, NULL);
    }
    
        
        //        fclose(f);
    MPI_File_close(&results_f);
    //fclose(results_f);
    printf("Found %i neighbours.\n", glob_c);
}
