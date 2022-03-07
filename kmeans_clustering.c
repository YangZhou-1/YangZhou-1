#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <omp.h>
#include <math.h> 
#include <float.h>

#define MAXCHAR 25
#define DEBUG   1
#define MAXVALUE 1.0E10

int whichCentroid(double points_x, double points_y, double* centroids_x, double* centroids_y, int num_centroids);
int whichCentroid(double points_x, double points_y, double* centroids_x, double* centroids_y, int num_centroid) {
        double euclideanDistance = MAXVALUE;
        double distance = 0;
        int index = -1;
        for (int i = 0; i < num_centroid; i++) {
            distance = (points_x - centroids_x[i])*(points_x - centroids_x[i]) + (points_y - centroids_y[i])*(points_y - centroids_y[i]);
            if (distance < euclideanDistance) {
                euclideanDistance = distance;
                index = i;
            }
        }
        return index;
    }


int main(int argc, char* argv[]) {
    // Catch console errors
    if (argc != 8) {
        printf("USE LIKE THIS: kmeans_clustering n_points points.csv n_centroids centroids.csv output.csv time.csv num_threads\n");
        exit(-1);
    }

    // points ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    int num_points = strtol(argv[1], NULL, 10);
    FILE* pointsFile = fopen(argv[2], "r");
    if (pointsFile == NULL) {
        printf("Could not open file %s", argv[2]);
        exit(-2);
    }

    // centroids ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    int num_centroids = strtol(argv[3], NULL, 10);
    FILE* centroidsFile = fopen(argv[4], "r");
    if (centroidsFile == NULL) {
        printf("Could not open file %s", argv[4]);
        fclose(pointsFile);
        exit(-3);
    }

    // output ~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    FILE* outputFile = fopen(argv[5], "w");
    FILE* timeFile = fopen(argv[6], "w");

    // threads ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    int num_threads = strtol(argv[7], NULL, 10);

    // array ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    double* points_x = malloc(num_points * sizeof(double));
    double* points_y = malloc(num_points * sizeof(double));

    // centroid array /////////////////////////////////////////
    double* centroids_x = malloc(num_centroids * sizeof(double));
    double* centroids_y = malloc(num_centroids * sizeof(double));



    // Store values ~~~~~~~~ //  
    // temporarily store values
    char str[MAXCHAR];

    // Storing point values //
    int k = 0;
    while (fgets(str, MAXCHAR, pointsFile) != NULL) {
        sscanf(str, "%lf,%lf", &(points_x[k]), &(points_y[k]));
        k++;
    }
    fclose(pointsFile);

    // Storing centroid values //
    k = 0;
    while (fgets(str, MAXCHAR, centroidsFile) != NULL) {
        sscanf(str, "%lf,%lf", &(centroids_x[k]), &(centroids_y[k]));;
        k++;
    }
    fclose(centroidsFile);

    

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    // start time
    double start = omp_get_wtime();



    int* points_z = malloc(num_points * sizeof(int));
    int* centroids_count = malloc(num_centroids * sizeof(int));
    double* new_centroids_x = malloc(num_centroids * sizeof(double));
    double* new_centroids_y = malloc(num_centroids * sizeof(double));

    double* new_centroids_x_sum = malloc(num_centroids * sizeof(double));
    double* new_centroids_y_sum = malloc(num_centroids * sizeof(double));
    double* centroids_moved_distance = malloc(num_centroids * sizeof(double));
    
    double average_moving_distance = 1000;

    int* local_centroids_count = malloc(num_centroids * num_threads * sizeof(int));
    double* local_new_centroids_x_sum = malloc(num_centroids * num_threads * sizeof(double));
    double* local_new_centroids_y_sum = malloc(num_centroids * num_threads * sizeof(double));

    // TODO: implement algorithm here :)
       #pragma omp parallel  num_threads(num_threads) 
    {
        int my_rank =omp_get_thread_num();
        int my_offset = my_rank * num_centroids;

    while (average_moving_distance > 1)
    {
        
      #pragma omp for
        //initial variables change
        for (int i = 0; i < num_centroids; i++)
        {
            centroids_count[i] = 0;
            new_centroids_x[i] = 0;
            new_centroids_y[i] = 0;
            new_centroids_x_sum[i] = 0;
            new_centroids_y_sum[i] = 0;
            for (int j = 0; j < num_threads; j++)
            {
                local_centroids_count[j*num_centroids + i] = 0;
                local_new_centroids_x_sum[j*num_centroids + i] = 0;
                local_new_centroids_y_sum[j*num_centroids + i] = 0;
            }

        }
        #pragma omp single
        {    
            average_moving_distance=0;    
        }
        //first for loop map reduce calculate local variables
        #pragma omp for
        for (int i = 0; i < num_points; i++)
        {
            points_z[i] = whichCentroid(points_x[i], points_y[i], centroids_x, centroids_y, num_centroids);
            
            local_new_centroids_x_sum[my_offset + points_z[i]] += points_x[i];
            local_new_centroids_y_sum[my_offset + points_z[i]] += points_y[i];  
            local_centroids_count[my_offset + points_z[i]]++;
            
        }
        //first for loop map reduce calculate sum
        #pragma omp for  
        for (int i = 0; i < num_centroids; i++)
        {
            for (int j = 0; j < num_threads; j++)
            {
                centroids_count[i] += local_centroids_count[j*num_centroids + i];
                new_centroids_x_sum[i] += local_new_centroids_x_sum[j*num_centroids + i];
                new_centroids_y_sum[i] += local_new_centroids_y_sum[j*num_centroids + i];
            }
        } 
           
        //second for loop
        #pragma omp for reduction(+:average_moving_distance)
        for (int i = 0; i < num_centroids; i++)
        {  
            new_centroids_x[i] = (double)new_centroids_x_sum[i]/centroids_count[i];
            new_centroids_y[i] = (double)new_centroids_y_sum[i]/centroids_count[i];
            centroids_moved_distance[i] = sqrt((new_centroids_x[i] - centroids_x[i])*(new_centroids_x[i] - centroids_x[i]) + (new_centroids_y[i] - centroids_y[i])*(new_centroids_y[i] - centroids_y[i]));
            centroids_x[i] = new_centroids_x[i];
            centroids_y[i] = new_centroids_y[i];      
            average_moving_distance += (double)centroids_moved_distance[i]/num_centroids; 
        }       

        }
    }

    // end time
    double end = omp_get_wtime();
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    


    // print time //
    double time_passed = end - start -1;
    fprintf(timeFile, "%f", time_passed);
//	printf("total time consumed : %f \n", time_passed);
    // print centroids //
    for (int c = 0; c < num_centroids; ++c) {
        fprintf(outputFile, "%f, %f", centroids_x[c], centroids_y[c]);
        if (c != num_centroids - 1) fprintf(outputFile, "\n");
    }



    // close files //
    fclose(outputFile);
    fclose(timeFile);

    // free memory //
    free(points_x);
    free(points_y);
    free(centroids_x);
    free(centroids_y);

    // return //
    return 0;
}
