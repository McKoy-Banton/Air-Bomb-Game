/*
AUTHOR: Mckoy BANTON
*/

#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <signal.h>

void plane();
void printArray(int arr[], int maximum);

int fuel=100;
pid_t parent_ID_check=0; //0 for parent; OTHERWISE child
int childrenProcessID[100];//tracks the instances of child process(es)



void main()
{
    void bomb(int);
    void refuel(int);
    
    char input;
    
    int i=1;//default for parent

    int index=0;//records the number of planes launched

    signal(SIGUSR1,bomb);//install the handler for SIGUSR1-(bombing)
    signal(SIGUSR2,refuel);//install the handler for SIGUSR2-(refuel)

    while(1)
    {
        if(parent_ID_check==0) //access: Parent ONLY
        {
            printf("Enter a command l (launch), b(bomb), r(refuel), q(quit): ");
            scanf("%s",&input);

            //check input
            while (input !='q' &&  input !='b' && input !='r' && input !='l')
            {
                printf("\nIncorrect command!\nThe acceptable commands are (l,b,r,q ONLY)\n");
                printf("Enter a command l (launch), b(bomb), r(refuel), q(quit): ");
                scanf("%s", &input);
            }
        }
        
        if ((input=='l') && parent_ID_check==0) //access: parent ONLY
        {
            i=fork();//the process of LAUNCHING (l) the plane
            childrenProcessID[index]=i;
            index++;

        }

        if (i==0)  //access: Child ONLY 
        {            
            plane();
            exit(0);
        }

        if ((input=='b') && parent_ID_check==0) //access: parent ONLY
        {   
            //the process of a plane BOMBING (b)
            int inputProcessID;
            if (index==0)
            {
                printf("No planes flying, can't drop bomb\n");
            }
            else
            {
                printf("\nChoose a process ID\n");
                printArray(childrenProcessID,index);
                scanf("%d", &inputProcessID);
                kill(inputProcessID,SIGUSR1);//is the process of base sending the BOMB (b).
            }
            
        }

        if ((input=='r') && parent_ID_check==0) //access: parent ONLY
        {   
            //the process of a plane REFUELLING (r)
            int inputProcessID;
            if (index==0)
            {
                printf("No planes were launched, none to refuel!\n");
            }
            else
            {
                printf("\nChoose a process ID\n");
                printArray(childrenProcessID,index);
                scanf("%d", &inputProcessID);
                kill(inputProcessID,SIGUSR2);//is the process of base sending the BOMB (b).
            }
            
        }
       if ((input=='q') && parent_ID_check==0) //access: parent ONLY
       {    
            //QUITTING (q) the base and plane 
            exit(0);
       }

    }

}

void plane()
{
    pid_t childPID=getpid();
    pid_t parent_ID_check=getppid();
    /* **fuel control function**  */
    while (fuel!=0 && fuel>-1)
    {
        sleep(3);
        if (fuel>=15)
        {
            fuel=fuel-15;
        }else
        {
            fuel=0;
        }
        printf("Bomber %d to base %d %% of fuel left\n", childPID, fuel);
    }

    if (fuel==0)
    {
        printf("SoS! Plane %d is going to crash\n",childPID);
        exit(EXIT_SUCCESS);
    }
    
}

void bomb(int signum)
{
    //The signal handler response to SIGUSR1 signal
    pid_t pID=getpid();
    printf("Bomber %d to base, bomb away!\n", pID);
}

void refuel(int signum)
{   //The signal handler response to SIGUSR2 signal
    pid_t pID=getpid();
    fuel=100;
    printf("Plane %d has been refueled.\n",pID);
}

void printArray(int arr[], int maximum)
{
    printf("\n[ ");
    for (int x=0; x<maximum; x++)
    {
        printf("%d, ",arr[x]);
    }
    printf(" ]\n");
}