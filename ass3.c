#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <mpi.h>



int main ( int argc , char * argv []){
     int    numprocs , i,rank;
     int numOfPoints ;
        int numOfbars ;
       int max;
        int arr[100];
       
   int valuePerProcess;
      //int arr[numOfPoints];
  for(i = 0 ; i<100; i++){
    arr[i]=0;
  }
  int range;
  int g;
  int j;
  int threads_num = omp_get_max_threads() ;  
    MPI_Init(& argc ,& argv );
    MPI_Comm_size( MPI_COMM_WORLD ,& numprocs );
    MPI_Comm_rank( MPI_COMM_WORLD ,& rank );



    if(rank == 0 ){

             printf("Enter the total number of points : ");
            scanf("%d",&numOfPoints);

    FILE* Myfile  = fopen("/shared/dataset.txt","r");


     if (NULL == Myfile)
    {
            printf("file can't be opened \n");
    }
    for(i = 0 ; i < numOfPoints ; i++)
    {
        fscanf(Myfile, "%d", &arr[i]);
    }



    max = arr[0];
    for(i = 0 ; i < numOfPoints ; i++)
    {
          if(max<arr[i])
            max=arr[i];
    }


     printf("Enter the total number of bars : ");

    scanf("%d",&numOfbars);

    }


    MPI_Bcast(&numOfPoints, 1, MPI_INT , 0 , MPI_COMM_WORLD );
    MPI_Bcast(&numOfbars, 1, MPI_INT , 0 , MPI_COMM_WORLD );
    MPI_Bcast(&max, 1, MPI_INT , 0 , MPI_COMM_WORLD );
  //MPI_Bcast(&count, numOfbars, MPI_INT , 0 , MPI_COMM_WORLD );

    if(numOfPoints % numprocs==0){  
    valuePerProcess = numOfPoints / numprocs;  
  }  
  else{  
    valuePerProcess = (numOfPoints / numprocs)+1;  
  } 
   
    int arr2[numOfPoints];
  for(i = 0 ; i<numOfPoints; i++){
    arr2[i]=0;
  }

    MPI_Scatter(arr,valuePerProcess,MPI_INT,arr2,valuePerProcess,MPI_INT,0,MPI_COMM_WORLD);
	
    
  int range2=0;  
  if(max%numOfbars==0){  
  range = max/numOfbars;  
  }  
else{  
  range=(max/numOfbars)+1;  
}  
	int count[numOfbars];

   for(i = 0 ; i < numOfbars ; i++)
   {  
      
    count[i]=0;  
  } 

    #pragma omp parallel private(i,j)num_threads(threads_num)
    {

    for(i = 0 ; i < numOfbars ; i++)
    {
    
      for(j = 0 ; j < valuePerProcess ; j++){
          if(arr2[j]>range2&&arr2[j]<=range+range2){
            count[i]++;
        
      }
    }
    range2+=range;

      //printf(" the number is   : %d  \n",arr2[0]);
    }

  }

int count2[numOfbars*numprocs]; 



  
MPI_Gather(count, numOfbars, MPI_INT, count2, numOfbars, MPI_INT, 0, MPI_COMM_WORLD);  


	int count3[numOfbars];  
  
	for( i = 0 ; i < numOfbars ; i++){   
	  count3[i] = 0 ;    
	}
  
  if(rank==0){  

    int var=0,temp=numOfbars;  
    int total = numOfbars*numprocs;  
    for(i=0;i<total;i++){  
          
      count3[i-var]+=count2[i];   
        if(i==temp-1) {  
        var+=numOfbars;   
        temp+=numOfbars;  
        }  
    }  
      
    int rangeStart=0;
	int rangeEnd=range;  
      
    for(i=0;i<numOfbars ;i++){  
       printf("The range starts with %d , end with %d with count = %d   \n",rangeStart,rangeEnd,count3[i]);  
        rangeStart=rangeEnd;  
      rangeEnd+=range;  
      }  
      
  }  
    



    MPI_Finalize();
    return EXIT_SUCCESS;
}

