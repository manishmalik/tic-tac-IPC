#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include <unistd.h>
#include <sys/shm.h>

int main(void)
{
    int shmid = shmget((key_t)1234,sizeof(shared),IPC_CREAT  | 0666);
    int i=0,j=0;
    if ( shmid == -1 )
    {
        fprintf(stderr,"shared memory could not be created!,program will now abort\n");
        exit(EXIT_FAILURE);
    }
    shared* data = (shared*)shmat(shmid,NULL,0);
    
    if ( data == NULL )
    {
        fprintf(stderr,"shared memory could not be attached!,program will now abort\n");
        exit(EXIT_FAILURE);
    }
    
    while ( 1 )
    {
        while ( data->turn == 1 )
        /*{
          sleep(10)  ;
          printf("turn is : %d\n",data->turn);
        }*/;
        printf("%c ",data->grid[0][0]);
        
        data->turn = 1;
    }
    
    if ( shmdt(data) != 0)
    {
        fprintf(stderr,"shared memory could not be detached!,program will now abort\n");
        exit(EXIT_FAILURE);   
    }
    return 0;
}