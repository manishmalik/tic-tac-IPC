/*
    this program controls movement for player 1
    also decides who moves first and ergo,should be the first program to execute
*/
#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "header.h"
#include <time.h>

int main(void)
{
    int shmid = shmget((key_t)1234,sizeof(shared),IPC_CREAT | 0666);
    int i=0,j=0;
    int posx,posy,choice;
 
    srand(time(NULL));   
    if ( shmid == -1 )
    {
        fprintf(stderr,"shared memory could not be created!,program will now abort\n");
        exit(EXIT_FAILURE);
    }
    shared* data = (shared*)shmat(shmid,NULL,SHM_RND);
    
    if ( data == NULL )
    {
        fprintf(stderr,"shared memory could not be attached!,program will now abort\n");
        exit(EXIT_FAILURE);
    }
    clear();
    initialize(data);
     
    if ( rand() % 2 == 0 )
        data->turn=1;
    else
        data->turn=2;
        
    data->turn = 1;
    
	while (1)
	{
		while (data->turn == 2)   {
			printf("Player 2 is moving,plese wait.....\n");
			sleep(2);
		}
		display(data);
		if (!check(data))
		{
			//player1 moves  
			if (data->count >= 8)
			{
				printf("\n Its a Tie !!\n");
				break;
			}
			else
			{
				do
				{
					printf("Choose where to place mark : ");
					scanf("%d", &choice);

					posx = --choice / 3;
					posy = choice % 3;
				} while (choice<0 || choice>9 || data->grid[posx][posy] > '9');
				data->grid[posx][posy] = 'x';
				//player 1 has moved

				display(data);
				data->count = data->count + 1;
				data->turn = 2;
			}
		}
		else
		{
			printf("\n Player %d is the Winner of the game!",((data->count)-1)%2 + 1);
			data->turn = 2;
			break;
		}
	}
     //detaching the shared memory
    if ( shmdt(data) != 0)
    {
        fprintf(stderr,"shared memory could not be detached!,program will now abort\n");
        exit(EXIT_FAILURE);   
    }
    return 0;
}
