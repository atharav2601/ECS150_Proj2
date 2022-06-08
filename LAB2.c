#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

struct Readyqueue
{
  int dataR; //using number to represent the file name, this is also the index number to all record arrays
  struct Readyqueue *next;
};
struct IOqueue
{
  int dataIO; //using number to represent the file name, this is also the index number to all record arrays
  struct IOqueue *next;
};

 

void appendR(struct Readyqueue** head, int newData)//to initialzie the ready queue, use for loop to add the process number one by one
{
   
    struct Readyqueue* newNodeR = (struct Readyqueue*) malloc(sizeof(struct Readyqueue));
    struct Readyqueue *current = *head;  
    newNodeR->dataR  = newData;//assign to the end of ready queue
 
    newNodeR->next = NULL;//if ready queue is empty, the head node is the new node
    if (*head == NULL)
    {
       *head = newNodeR;
       return;
    }
 
    while (current->next != NULL)//go to the end of item and connect it to the new node 
        current = current->next;
    current->next = newNodeR;
    return;
}

void popR(struct Readyqueue **head) { //Readyqueue pop
  struct Readyqueue *tmp;

  //Linked list does not exist or the list is empty
  if(*head == NULL) {
      return;
  } 
  
  tmp = *head;
  *head = (*head)->next;//move head to next
  free(tmp);//remove first item
}

void appendIO(struct IOqueue** head, int newData)//IOqueue, it is empty initially
{
   
    struct IOqueue* newNodeIO = (struct IOqueue*) malloc(sizeof(struct IOqueue));
    struct IOqueue *currentIO = *head;  
    newNodeIO->dataIO  = newData;//assign to the end of IO queue
 
    newNodeIO->next = NULL;//if ready queue is empty, the head node is the new node
    if (*head == NULL)
    {
       *head = newNodeIO;
       return;
    }
 
    while (currentIO->next != NULL)//go to the end of item and connect it to the new node 
        currentIO = currentIO->next;
    currentIO->next = newNodeIO;
    return;
}

void popIO(struct IOqueue **head) { //IO queue pop
  struct IOqueue *tmp;

  //Linked list does not exist or the list is empty
  if(*head == NULL) {
      return;
  } 
  
  tmp = *head;
  *head = (*head)->next;//move head to next
  free(tmp);//remove first item
}

void printList(struct Readyqueue* n)
{
    while (n != NULL) {
        printf(" %d ", n->dataR);
        n = n->next;
    }
}
  

int Check_if_blocked(float p, int t)
{
    int random;
    int high =1, low =0;
    random=(rand() % (high - low + 1)) + low;
    if(t>2)
    {
        if(random<p)
        {
            return 1;//return true
        }
    }
    return 0;//return false
}

int main(int argc, char **argv)
{


    struct Readyqueue* headR = NULL;
    struct IOqueue* headIO = NULL;


    (void) srandom(12345);
    FILE *file1, *file_count;
    char c;
    int num = 0;
    //char *input = argv[1];

    file_count=fopen(argv[2],"r");
    if (file_count == 0)
    {
        //fopen returns 0, the NULL pointer, on failure
        perror("Cannot open input file\n");
        exit(-1);
    }
    else
    {
        while((c=fgetc(file_count)) != EOF)
        {
            while((c=fgetc(file_count)) != '\n')
            {
                continue;
            }
            num=num+1;
        }
    }
    fclose(file_count);
    char name_process [num][10];//process name array to store process names
    int name_index = 0;
    int time_process [num];//runtime array
    char probability_string [num][4];//probability to block array, "0.xx" is 4 characters
    float probability[num];
    int i=0,j=0,k=0;
    int error=0;
    int high=30,low=1;
    //initialize time_process to 0
    for(i=0;i<num;i++)
    {
        time_process[i]=0;
    }
    file1 = fopen(argv[2], "r");

    if (file1 == 0)
    {
        //fopen returns 0, the NULL pointer, on failure
        perror("Cannot open input file\n");
        exit(-1);
    }
    else
    {
        while ((c =fgetc(file1)) != EOF )
        {
            while(!isspace(c))
            {
                if(k==0)   //name of the process is inputted
                {
                    while(1)
                    {
                        if(isalpha(c) || isdigit(c))
                        {
                            name_process[i][j]=c;
                            j=j+1;
                            if(j>10)
                            {
                                //error = 1
                                //store the error name using perror
                                //exit(-1)
                            }
                        }
                        else
                        {
                            break;
                        }
                        c = fgetc(file1);
                    }
                    j=0;
                    k=k+1;
                }
                else if(k==1) 
                {
                    while(1)
                    {
                        if(isdigit(c))
                        {
                            time_process[i]=time_process[i]*10+(c-'0');
                        }
                        else if(isspace(c))
                        {
                            break;
                        }
                        else
                        {
                            //error = 1
                            //store the error name using perror
                            //exit(-1)
                        }
                        c = fgetc(file1);
                    }
                    k=k+1;
                }
                else
                {
                    while(1)
                    {
                        if(!isspace(c))
                        {
                            probability_string[i][j]=c;
                            j=j+1;
                        }
                        else 
                        {
                            break;
                        }
                        c = fgetc(file1);
                        i=i+1;
                        if(j>=4) //probability should be 4 char, one 0 one "." and two digits
                        {
                            //error = 1
                            //store the error name using perror
                            //exit(-1)
                        }
                    }
                    j=0;
                    k=0;
                    i=0;
                }
            }


        }
        for( int v =0;v<i;v++)//i should be the total number of files, the link list for Readyqueue here will record the files in form of interger
        {
          appendR(&headR,v);
        }
        fclose(file1);
        //convert probability string to float
        for(i=0;i<num;i++)
        {
            if(probability_string[i][0]=='0' && probability_string[i][1]=='.')//also check if position 2 and 3 are digits 
            {
                probability[i]= atoi(strcat(probability_string[i][2],probability_string[i][3])); //if 0.87, will give 87
                probability[i]=(probability[i]/100.0);
            }
        }
    }
    int track = 0;
    float random=0.00;
    char input='f';

    //variables for processes
    int runtime[num]; //runtime for different processes 
    int block_time[num]; // block time for different processes
    int dispatches[num];//number of times process was given cpu
    int time_completed[num]; // time when the process is completed
    int blocked_counter[num]; // number of times the process was blocked by IO

    //variables for system 
    int system_clock_time = 0;

    //variables for CPU and IO 
    int CPU_runtime=0, CPU_idle_time=0, IO_runtime=0, IO_idle_time=0; 

    //other variables
    int run_now=0,run_now_cpu=0,run_now_IO=0;
    int check_block[num];  // check if the process needs to remain blocked or not
    int check_run[num];//if a process needs to run or not
    int block[num];
    int preblock_run[num];
    int quantum = 0; //keep track of quantum in loop 
    //initialize ready queue
    //take input from user and store in in char input

    for(i=0;i<num;i++)
    {
        runtime[num]=0;
        block_time[num]=0;
        dispatches[num]=0;
        time_completed[num]=0;
        blocked_counter[num]=0;
        check_block[num]=0;
        check_run[num]=0;
        block[num]=0;
        preblock_run[num]=0;
    }

    while(error==0) // and check if length of completed queue is equal to num 
    {
        if(1) //check if ready queues has any value 
        {
            if(input=='f')
            {
                track=0;//first value of the ready list add if condition 
                //append track value to the cpu list
                popR(&headR);//pop value from the ready list
                if(preblock_run[track]==0)
                {
                    block[track]=Check_if_blocked(probability[track],(time_process[track]-runtime[track]));
                }
                if(block[track]==1) //blocked
                {
                    if(preblock_run[track]==0 && check_run[track]==0)        
                    {
                        high=time_process[track]-runtime[track];
                        random=(rand() % (high - low + 1)) + low;//generate random number from 1 to (time_process[track]-runtime[track]) for time to run before block
                        if(time_process[track]-random>0)  //to check that random number generated is not equal to the time process
                        {
                            check_run[track]=random;
                            preblock_run[track]=1;
                            dispatches[track]=dispatches[track]+1;
                        }
                        else //if random number generated is equal to the process time 
                        {
                            block[track]=0;
                        }
                    }
                    else if(check_run[track]==0)
                    {
                        appendIO(&headIO,track)//append track value from cpu queue to IO queue
                        //pop first value of cpu queue
                        blocked_counter[track]=blocked_counter[track]+1;
                        preblock_run[track] = 0; //resetting the value in case the process gets blocked again
                        break;
                    }
                }
                else    //not blocked
                {
                    check_run[track]=time_process[track]-runtime[track];
                    preblock_run[track]=1;
                }
            }
            else if(input == 'r')
            {
                track=0;//first value of the ready list add if condition 
                //append track value to the cpu list
                popR(&headR);//pop value from the ready list
                if(preblock_run[track]==0)
                {
                    block[track]=Check_if_blocked(probability[track],(time_process[track]-runtime[track]));
                }
                if(block[track]==1) //blocked
                {
                    if(preblock_run[track]==0 && check_run[track]==0)        
                    {
                        random=(rand() % 5) + low;//generate random number from 1 to 5 or (time_process[track]-runtime[track]) if time remaining less than 5  - for time to run before block
                        if(time_process[track]-random>0)  //to check that random number generated is not equal to the time process
                        {
                            check_run[track]=random;
                            preblock_run[track]=1;
                            dispatches[track]=dispatches[track]+1;
                        }
                        else //if random number generated is equal to the process time 
                        {
                            block[track]=0;
                        }
                    }
                    else if(check_run[track]==0)
                    {
                        appendIO(headIO,track);//append track value from cpu queue to IO queue
                        //pop first value of cpu queue
                        blocked_counter[track]=blocked_counter[track]+1;
                        preblock_run[track] = 0; //resetting the value in case the process gets blocked again
                        break;
                    }
                }
                else   //not blocked and has no more time to run left
                {
                    if(time_process[track]-runtime[track]<=5)
                    {
                        check_run[track]=time_process[track]-runtime[track];
                    }
                    else
                    {
                        check_run[track]=5;
                    }
                    preblock_run[track]=1;
                }
            }
              
        }
        //check if IO and CPU queue has values, if they both do
        {
            run_now_cpu=0;//get first index value in cpu queue here
            run_now_IO=0;//get first index value of IO queue here
            CPU_runtime=CPU_runtime+1;
            IO_runtime=IO_runtime+1;
            if(runtime[run_now_cpu]==time_process[run_now_cpu]-1)
            {
                time_completed[run_now_cpu]=system_clock_time;
                //append the first value of the cpu queue to the completed queue
                popR(&headR);//remove the first value of the CPU queue over here using pop
            }
            else
            {
                runtime[run_now_cpu]=runtime[run_now_cpu]+1;
                check_run[run_now_cpu]=check_run[run_now_cpu]-1;
                if(check_run[run_now_cpu]==0)
                {
                    //append from cpu queue to ready queue ???
                    //pop from cpu queue ???
                }
            }
            if(check_block[run_now_IO]==0) //not blocked 
            {
                if(block_time[run_now_IO]!=0) //had been blocked but not blocked anymore
                {

                    appendR(headR,headIO->dataIO);//append item from IO queue to ready queue 
                    popIO(&headIO);//pop the item from the IO queue
                    dispatches[run_now_IO]=dispatches[run_now_IO]+1;   //passed from io to cpu
                    block[run_now_IO]=0;//process not blocked anymore
                }
                else                          //was not blocked  
                {
                    check_block[run_now_IO]=0;//generate random number between 1 and 30 
                }
            }
            else
            {
                check_block[run_now_IO]=check_block[run_now_IO]-1;
                block_time[run_now_IO]=block_time[run_now_IO]+1;
            }
        }
        //only CPU queue has value 
        {
            CPU_runtime=CPU_runtime+1;
            IO_idle_time=IO_idle_time+1;
            run_now_cpu=0;// get CPU queue's first value here
            if(runtime[run_now_cpu]==time_process[run_now_cpu]-1)
            {
                time_completed[run_now_cpu]=system_clock_time;
                //append the first value of the cpu queue to the completed queue
                popR(&headR);//remove the first value of the CPU queue over here
            }
            else
            {
                runtime[run_now_cpu]=runtime[run_now_cpu]+1;
                check_run[run_now_cpu]=check_run[run_now_cpu]-1;
                if(check_run[run_now_cpu]==0)
                {
                    //append from cpu queue to ready queue ???
                    //pop from cpu queue ???
                }
            }
        }
        //only IO queue has value
        {
            IO_runtime=IO_runtime+1;
            CPU_idle_time=CPU_idle_time+1;
            if(check_block[run_now_IO]==0) //not blocked 
            {
                if(block_time[run_now_IO]!=0) //had been blocked but not blocked anymore
                {
                    appendR(headR,headIO->dataIO);//append item from IO queue to ready queue 
                    popIO(&headIO);//pop the item from the IO queue
                    dispatches[run_now_IO]=dispatches[run_now_IO]+1; //passed from io to cpu
                    block[run_now_IO]=0; //process not blocked anymore
                }
                else                          //was not blocked  
                {
                    check_block[run_now_IO]=2;//generate random number between 1 and 30 
                }
            }
            else
            {
                check_block[run_now_IO]=check_block[run_now_IO]-1;
                block_time[run_now_IO]=block_time[run_now_IO]+1;
            }
        }
        system_clock_time=system_clock_time+1;
        quantum=quantum+1;
    }

    printf("Processes:\n");
    printf("Name    CPU time    When done  Dispatches  Block for I/O    I/O time\n");
    for (int v=0;v<i;v++)
    {
        
        printf("%s  ",name_process[v]);
        printf("%d  %d  %d  %d  %d\n",runtime[v],time_completed[v],dispatches[v],blocked_counter[v],block_time[v]);
    }
    printf("\n");//blank line for system
    
    printf("system:\n");
    printf("The wall clock time at which the simulation finished: ")
    printf("%d\n",system_clock_time);
    printf("\n");//blank line for CPU status
    
    printf("CPU:\n");
    printf("Total time spent busy: %d\n",CPU_runtime);
    printf("Total time spent idle: %d\n",CPU_idle_time);
    float utCPU=CPU_runtime/system_clock_time;//nn.nn
    printf("CPU utilization:%f\n",utCPU);
    printf("Number of dispatches:%d\n",)//CPU dispatchname??
    float thCPU= //Overall throughput = number of processes / total time
    printf("Overall throughput:%d\n",)//need value
    
    printf("\n");//blank line for IO status
    
    printf("I/O device:\n");
    printf("Total time spent busy: %d\n",IO_runtime);
    printf("Total time spent idle: %d\n",IO_idle_time);
    float utIO=IO_runtime/system_clock_time;//nn.nn
    printf("I/O device utilization: %f\n",utIO);
    printf("Number of dispatches:%d\n",)//CPU dispatchname??
    float thIO= //Overall throughput = number of processes / total time
    printf("Overall throughput:%d\n",)//need value
    //utCPU, utIO, thCPU,thIO all are float nn.nn
    //all interger or float digits are italics
    

}













//extra code

/*check if ready queue has values in it
            {
                run_now=0;//put the ready queue value over here
                block = Check_if_blocked(run_now, probability[run_now], time_process[run_now]);
            }
            //else
            {
                break;
            }
            if(block==1)
            {
                //append value from ready queue into IO queue over here
                //pop the ready queue value over here
            }
            else
            {
                //append value of ready queue in CPU queue
                //pop value from ready queue
                break;
            }  
*/
