#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>

int N;//Dimensions of chess board(N*N)

// Helper struct,
typedef struct{
	int x,y;
} pair;

/*
PQargs contains the information about the next move, information about the next coordinates. If its a valid move, what is its priority.
It is passed as a argument to the multithreading function getpriority to calculate the prioity.
*/
typedef struct{
    int x, y;
    int **board;
    int score;
} PQargs;

/*
KTargs acts a arguments for the Knight Tout Multithreading Function.
It contains information about the current board configuration, current coordinates,
as well how many moves the knight has made(index).
*/
typedef struct{
    pair coords; //coordinates
    int index; //the number of moves Knight has made
    int **board; //current chess configuration
    unsigned int seed; //a random number
} KTargs;

int directions[8][2] = {{1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};
int **result; //resultant chess board
bool flag = false; //whether we have reached the result or not. Basically acts as a signal for other threads

//Function to check whether the coordinate is valid or not.
bool isValid(int x,int y,int** arr)
{
	return (x >= 0 && x < N && y >= 0 && y < N && arr[x][y]==0);
}

//function to copy the 2D array from source to result
void copyBoard(int** result,int ** source)
{
	for(int i=0;i<N;++i)
	{
		for(int j=0;j<N;++j)
		{
			result[i][j] = source[i][j];
		}
	}
	return;
}

//Initial test which tells us which arguments  dont have possible tour.
bool initialConditions(int StartX, int StartY)
{
	if(N <= 4||(N % 2 == 1 && (StartX + StartY) % 2 == 1))
	{
		return true;
	}
	return false;
}

//function to get the priority of all the next possible moves
void *getpriority(void *args)
{
    //check all neighbours of coordinates passed and return priority
    int temp = 0;
    PQargs *current = (PQargs *)args;
    if(!isValid(current->x, current->y, current->board))
    {
        current->score = 15;
        pthread_exit(0);
    }
    for(int i=0; i<8; i++)
    {
        if(isValid(current->x + directions[i][0], current->y + directions[i][1], current->board))
        {
            temp++;
        }
    }
    current->score =  temp;
    pthread_exit(0);
}

//main multithreading function
void *KnightTour(void *args)
{
    //perform the randomised Knight Tour from coordinates passed as args
    //typecasting the arguments to KT args
    KTargs *current = (KTargs *)args;
    
    if(flag)
    {
        pthread_exit(0);
    }
    //checking whether this coordinate is within the limits and hasn't been visited before
    if(!isValid(current->coords.x, current->coords.y, current->board))
    {
        pthread_exit(0);
    }

    current->board[current->coords.x][current->coords.y] = current->index;
    
    if(current->index == N*N)
    {
        flag = true;
        result = (int**)calloc(N, sizeof(int*));
        for(int i=0; i<N; i++)
        {
            result[i] = (int*)calloc(N, sizeof(int));
        }
        copyBoard(result, current->board); //copying the board.
        pthread_exit(0);
    }

    //getting the priority of the next moves
    pthread_t* tid=(pthread_t*)malloc(8*sizeof(pthread_t));
    PQargs *pq = (PQargs *)malloc(8*sizeof(PQargs));
    for(int i=0; i<8; i++)
    {
        pq[i].x = current->coords.x + directions[i][0];
        pq[i].y = current->coords.y + directions[i][1];
        pq[i].score = -1;
        pq[i].board = current->board;
        if(pthread_create(&tid[i], NULL, &getpriority, (void *)&pq[i])!=0)
        {
            perror("Error in creating the thread\n");
        }
    }

    int arr[8];
    for(int i=0; i<8; i++)
    {
        arr[i] = 25;
        if(pthread_join(tid[i],NULL)!=0)
        {
            perror("Error in joining the thread\n");
        }
        arr[i] = pq[i].score;
    }

    //getting what is the minimum prioirty among the 8 possible moves, and how many coordinates have that same minimum priority.
    int freq = 0, minval = 25;
    for(int i=0; i<8; i++)
    {
        if(arr[i] < minval)
        {
            minval = arr[i];
            freq=1;
        }
        else if(arr[i] == minval)
        {
            freq++;
        }
    }

    //if there is no possible move, we exit this thread to choose some other possible path.
    if(minval == 25)
    {
        pthread_exit(0);
    }
    srand(current->seed);
    
    //since there exits "freq" numbers of moves of moves which have same minimum priority, we are randomly choosing one of them.
    int choice=rand()%freq;
    for(int i=0;i<8;i++)
    {
        if(arr[i]==minval)
        {
            if(choice == 0)
            {
                //send to new coords
                current->coords.x = pq[i].x;
                current->coords.y = pq[i].y;
                current->index = current->index + 1;
                current->seed = current->seed + 1;
                KnightTour((void *)current);
                pthread_exit(0);
            }
            else
            {
                choice--;
            }
        }
    }
    
}

int main(int argc, char *argv[]) {
	
	if (argc != 4) {
		printf("Usage: ./Knight.out grid_size StartX StartY");
		exit(-1);
	}
	
	N = atoi(argv[1]);
	int StartX=atoi(argv[2]);
	int StartY=atoi(argv[3]);

    if(initialConditions(StartX,StartY))
	{
		printf("No Possible Tour\n");
        exit(0);
	}
    pthread_t KTthreads[20];
    KTargs arguments[20];
    unsigned int sd = 5786;
    while(flag == false)
    {
        for(int k=0; k<20; k++)
        {
            arguments[k].coords.x = StartX;
            arguments[k].coords.y = StartY;
            arguments[k].index = 1;
            arguments[k].board = (int**)calloc(N, sizeof(int*));
            arguments[k].seed = sd;
            sd++;
            for(int i=0; i<N; i++)
            {
                arguments[k].board[i] = (int*)calloc(N, sizeof(int));
            }
            if(pthread_create(&KTthreads[k], NULL, &KnightTour, (void *)&arguments[k])!=0)
            {
                perror("Error in creating the thread\n");
            }
        }
        for(int k=0; k<20; k++)
        {
            if(pthread_join(KTthreads[k], NULL)!=0)
            {
                perror("Error in joining the thread\n");
            }
        }
    }

    pair rs[2505];
    for(int i=0;i<N;++i)
    {
        for(int j=0;j<N;++j)
        {
            rs[result[i][j]-1].x = i;
            rs[result[i][j]-1].y = j;
        }
    }

    for(int i=0;i<N*N;++i)
    {
        printf("%d,%d|",rs[i].x,rs[i].y);
    }
    printf("\n");
	return 0;
}