#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>


const clock_t MaxDelay = 1000000;
//if the delay is too big the thread outputs will be shown at the proper time
int x; //a global variable	for the processes
int x1 = 10;//global shared variable for the threads
const int INC = +2, DEC = -1; 

void delay (clock_t ticks) 
{
		clock_t start = clock();
		do
		; while (clock() < start + ticks);
}

void change_number(char * legend, int i)// for the 2 processes
{
	int u = 1;
	while (u != 0) /*loop once*/
	{
			x += i;
			printf("%s: %i\n", legend, x);
			delay(rand()%MaxDelay);	
			u--;
	}
}
void * change_number1(void *n)
{  
	int u = 1;
	
	int *i = (int *)n;
	while (u != 0)   // loop once
	{   
		x1 += *i;
		printf("adjustment = %2i; x1 = %i\n", *i, x1);		
		u--;
	}
	return(n);
}

int main (void) 
{ 
    pid_t c;//fork return value 
    pthread_t up_thread, dn_thread; 
    srand (time(NULL));//initialize seed for rand 
    //Get user input - note: scanf is a bad solution and user input needs to be validated 
    printf("Please input an integer value for the 2 processes: "); 
    scanf("%d", &x); 
    printf("You entered x = %d\n", x); 
    c = fork();
    
    if (c == -1)  
    { 
        perror ("Error creating process."); 
        exit(EXIT_FAILURE);
    }
    else if (c != 0) { //parent
        printf("Process %i created.\n", c);
    }
         
    for (int i = 0; i <=1; i++) //loop 2 times
    {
        
        if (c==0) 
        { //Child process 
            change_number("child", 2);
            //FIXME Process is blocking here 
            if(pthread_create(&up_thread,NULL, change_number1, (void *)&INC) != 0) 
            { 
                perror ("Error creating up thread."); 
                exit(EXIT_FAILURE); 
            } 
            if(pthread_create(&dn_thread,NULL, change_number1, (void *)&DEC) != 0) 
            { 
                perror ("Error creating down thread."); 
                exit(EXIT_FAILURE); 
            } 
                if(pthread_join(up_thread, NULL) != 0) 
                { 
                    perror ("Error joining up thread."); 
                    exit(EXIT_FAILURE); 
                } 
                if(pthread_join(dn_thread, NULL) != 0) 
                { 
                    perror ("Error joining up thread."); 
                    exit(EXIT_FAILURE); 
                } 
        } 
        else 
        { 
            //Parent process 
            //printf("Creating threads:\n"); 
            change_number("parent", -1); 
            
        } 

    }
            
    return 0;
}
