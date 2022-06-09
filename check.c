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
  int dataIO; 
  struct IOqueue *next;
};
struct CPUlist
{
  int dataC; 
  struct CPUlist *next;
};
void Rprint(struct Readyqueue* n)//Print Readyqueue
{
    while (n != NULL) {
        printf(" %d ", n->dataR);
        n = n->next;
    }
}

void CPUprint(struct CPUlist* n)//print CPUlist
{
    while (n != NULL) {
        printf(" %d ", n->dataC);
        n = n->next;
    }
}

void IOprint(struct IOqueue* n)//print IOqueue
{
    while (n != NULL) {
        printf(" %d ", n->dataIO);
        n = n->next;
    }
}

int getlengthR(struct Readyqueue* n)//get Readyqueue length
{
    int lengthR = 0;
    while (n != NULL) {
        lengthR+=1;
        n = n->next;
    }
    
    return lengthR;
}

int getlengthC(struct CPUlist* n)//get CPU length
{
    int lengthC = 0;
    while (n != NULL) {
        lengthC+=1;
        n = n->next;
    }
    
    return lengthC;
}

int getlengthIO(struct IOqueue* n)//get IO length
{
    int lengthIO = 0;
    while (n != NULL) {
        lengthIO+=1;
        n = n->next;
    }
    
    return lengthIO;
}
void appendR(struct Readyqueue** head, int newData)//to initialzie the ready queue, use for loop to add the process number one by one
{
   
    struct Readyqueue* newNodeR = (struct Readyqueue*)malloc(sizeof(struct Readyqueue));
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

void appendC(struct CPUlist** head, int newData)//
{
   
    struct CPUlist* newNodeC = (struct CPUlist*) malloc(sizeof(struct CPUlist));
    struct CPUlist *currentC = *head;  
    newNodeC->dataC  = newData;//assign to the end of CPU queue
 
    newNodeC->next = NULL;//if CPU queue is empty, the head node is the new node
    if (*head == NULL)
    {
       *head = newNodeC;
       return;
    }
 
    while (currentC->next != NULL)//go to the end of item and connect it to the new node 
        currentC = currentC->next;
    currentC->next = newNodeC;
    return;
}

void popC(struct CPUlist**head) { //CPUqueue pop
  struct CPUlist *tmp;

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
float f_random_generator() 
{
    return (float)random()/RAND_MAX;//value between 0 and 1
}
int i_random_generator(int time) 
{
    return (random()%time)+1;
}
int Check_if_blocked(float p, int t)
{
    float random;
    int high =1, low =0;
    //random=(rand() % (high - low + 1)) + low;//between 0 and 1
    if(t>2)
    {
        random = f_random_generator();
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
    struct CPUlist* headC = NULL;

    (void) srandom(12345);
    FILE *file1, *file_count;
    char c;
    int num = 1;
    //char *input = argv[1];
    char input = *argv[2];
    file_count=fopen(argv[2],"r");
    int i=0;
    if (file_count == 0)
    {
        //fopen returns 0, the NULL pointer, on failure
        perror(argv[2]);
        exit(1);
    }
    else
    {
        // use fscanf to get data by the form
        // while(fscanf("%s %s %s", name, time, block) != EOF)
        // cast the values appropriately and error che
        while((c=fgetc(file_count)) != EOF)
        {
            if(c=='\n')
            {
                num=num+1;
            }
        }
    }
    fclose(file_count);
    char name_process [num][10];//process name array to store process names
    int name_index = 0;
    int time_process [num];//runtime array
    char probability_string [num][4];//probability to block array, "0.xx" is 4 characters
    float probability[num];
    int j=0,k=0;
    int error=0;
    int high=30,low=1;
    int completed=0;

    //fine
    //initialize time_process to 0
    file1 = fopen(argv[2], "r");
    i=0;
    int name_length[num];
    int check_1=0;
    for(i=0;i<num;i++)
    {
        time_process[i]=0;
    }
    if (file1 == 0)
    {
        //fopen returns 0, the NULL pointer, on failure
        perror("Cannot open input file\n");
        exit(-1);
    }
    else
    {
        k=1;
        j=0;
        i=0;
        while((c=fgetc(file_count)) != EOF)
        {
            if((k%3)==1)
            {
                if(isspace(c) && check_1 == 0)
                {
                    k=k+1;
                    check_1=1;
                    name_length[i]=j;
                    j=0;
                }
                else if(!isspace(c))
                {
                    if(isalpha(c))
                    {
                        name_process[i][j]=c;
                    }
                    j=j+1;
                    check_1=0;
                    if(j>10)
                    {
                        printf("error1   %c   %d  %i",c,j,i);
                    }
                }
            }
            else if((k%3)==2)
            {
                if(isspace(c) && check_1==0)
                {
                    k=k+1;
                    check_1=1;
                }
                else if(isdigit(c))
                {
                    time_process[i]=(time_process[i]*10)+(int)((((int)c)+((int)'0')));
                    time_process[i]=time_process[i]-96;
                    check_1=0;
                }
                else if(isalpha(c))
                {
                    //error
                    printf("error2   ");
                }
            }
            else 
            {
                if(isspace(c) && check_1==0)
                {
                    k=k+1;
                    j=0;
                    check_1=1;
                }
                else if(!isspace(c) && j<4)
                {
                    probability_string[i][j]=c;
                    j=j+1;
                    check_1=0;
                }
                if(j>4)
                {
                    printf("error3   ");
                }
            }
            if(c=='\n')
            {
                i=i+1;
            }
        }
    }
    for(i=0;i<num;i++)
    {
        if(probability_string[i][0]=='0' && probability_string[i][1]=='.')//also check if position 2 and 3 are digits 
        {
            //probability[i]= atoi(strcat(probability_string[i][2],probability_string[i][3])); //if 0.87, will give 87
            probability[i]=(int)(probability_string[i][2]-'0')*10;
            probability[i]=probability[i]+(int)(probability_string[i][3]-'0');
            probability[i]=(probability[i]/100.0);
        }
    }
    //removing extra chars in name
    for(i=0;i<num;i++)
    {
        for(j=0;j<name_length[i];j++)
        {
            continue;
        }
        for(k=j;k<10;k++)
        {
            name_process[i][k]='\0';
        }
    }

    for( int v =0; v<num; v++)//i should be the total number of files, the link list for Readyqueue here will record the files in form of interger
    {
        printf("%s  ",name_process[v]);
        printf("%d  ",time_process[v]);
        printf("%f\n",probability[v]);
        //appendR(&headR,v);
    }
    fclose(file1);

    for( int v =0; v<num; v++)//i should be the total number of files, the link list for Readyqueue here will record the files in form of interger
    {
        appendR(&headR,v);
    }
    int track = 0;
    float random=0.00;

    //variables for processes
    int runtime[num]; //runtime for different processes 
    int block_time[num]; // block time for different processes
    int dispatches[num];//number of times process was given cpu
    int time_completed[num]; // time when the process is completed
    int blocked_counter[num]; // number of times the process was blocked by IO

    //variables for system 
    int system_clock_time = 0;

    //variables for CPU and IO 
    int CPU_runtime=0, CPU_idle_time=0, CPU_dispatch=0, IO_runtime=0, IO_idle_time=0 , IO_dispatch=0; 

    //other variables
    int run_now=0,run_now_cpu=0,run_now_IO=0;
    int check_block[num];  // check if the process needs to remain blocked or not
    int check_run[num];//if a process needs to run or not
    int block[num];
    int preblock_run[num];
    int quantum = 0; //keep track of quantum in loop 
    //initialize ready queue
    //take input from user and store in in char input

    for(i=0;i<num;i++) //initialize everything to 0
    {
        runtime[i]=0;
        block_time[i]=0;
        dispatches[i]=0;
        time_completed[i]=0;
        blocked_counter[i]=0;
        check_block[i]=0;
        check_run[i]=0;
        block[i]=0;
        preblock_run[i]=0;
    }

    
    //completed!=num
    while(completed!=num) // and check if length of completed queue is equal to num 
    {
        if(getlengthR(headR)!=0 ) //check if ready queues has any value 
        {
            if(input=='f')
            {
                track=headR->dataR;//first value of the ready list 
                appendC(&headC,track);//append track value to the cpu list
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
                        random=i_random_generator(high);//generate random number from 1 to (time_process[track]-runtime[track]) for time to run before block
                        if(time_process[track]-random>0)  //to check that random number generated is not equal to the time process
                        {
                            check_run[track]=random;
                            preblock_run[track]=1;
                            dispatches[track]=dispatches[track]+1;
                        }
                        else //if random number generated is equal to the process time 
                        {
                            block[track]=0;
                            check_run[track]=time_process[track];
                        }
                    }
                    else if(check_run[track]==0)
                    {
                        appendIO(&headIO,track);//append track value from cpu queue to IO queue
                        popC(&headC);//pop first value of cpu queue
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
                printf("check run time %d %d\n",track,check_run[track]);
            }
            else if(input == 'r')
            {
                printf("r\n");
                // store an interrupt time in the cpu struct (if you have that)
                // set interrupt time to min(5, cur_job.time_remaining)
                // do this when you dispatch a job to the cpu
                // decrement interrupt time on each tick until it goes to 0 and on 0; remove the job from the cpu and push rq
                track=headR->dataR;//first value of the ready list add if condition 
                appendC(&headC,track);//append track value to the cpu list
                popR(&headR);//pop value from the ready list
                if(preblock_run[track]==0)
                {
                    block[track]=Check_if_blocked(probability[track],(time_process[track]-runtime[track]));
                }
                if(block[track]==1) //blocked
                {
                    if(preblock_run[track]==0 && check_run[track]==0)        
                    {
                        random=i_random_generator(5);//generate random number from 1 to 5 or (time_process[track]-runtime[track]) if time remaining less than 5  - for time to run before block
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
                        appendIO(&headIO,track);//append track value from cpu queue to IO queue
                        popC(&headC);//pop first value of cpu queue
                        blocked_counter[track]=blocked_counter[track]+1;
                        preblock_run[track] = 0; //resetting the value in case the process gets blocked again
                        break;
                    }
                }
            }
        }
        if(getlengthC(headC)!=0 && getlengthIO(headIO)!=0) //check if both queues have values
        {
            printf("hi!\n");
            run_now_cpu=headC->dataC;//get first index value in cpu queue here
            run_now_IO=headIO->dataIO;//get first index value of IO queue here
            CPU_runtime=CPU_runtime+1;
            IO_runtime=IO_runtime+1;
            if(runtime[run_now_cpu]==time_process[run_now_cpu]-1)
            {
                printf("lol2");
                time_completed[run_now_cpu]=system_clock_time;
                completed+=1;
                //append the first value of the cpu queue to the completed queue
                popC(&headC);//remove the first value of the CPU queue over here using pop
                CPU_dispatch+=1;
            }
            else
            {
                runtime[run_now_cpu]=runtime[run_now_cpu]+1;
                check_run[run_now_cpu]=check_run[run_now_cpu]-1;
                if(check_run[run_now_cpu]==0)
                {
                    appendR(&headR,headC->dataC);//append from cpu queue to ready queue 
                    popC(&headC);//pop from cpu queue 
                    CPU_dispatch+=1;
                }
            }
            if(check_block[run_now_IO]==0) //not blocked 
            {
                if(block_time[run_now_IO]!=0) //had been blocked but not blocked anymore
                {

                    appendR(&headR,headIO->dataIO);//append item from IO queue to ready queue 
                    popIO(&headIO);//pop the item from the IO queue
                    dispatches[run_now_IO]=dispatches[run_now_IO]+1;   //passed from io to cpu
                    block[run_now_IO]=0;//process not blocked anymore
                    IO_dispatch+=1;
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
        else if(getlengthC(headC)!=0) //check only cpu queue has values
        {
            printf("hi!\n");
            CPU_runtime=CPU_runtime+1;
            IO_idle_time=IO_idle_time+1;
            run_now_cpu=headC->dataC;// get CPU queue's first value here
            if(runtime[run_now_cpu]==time_process[run_now_cpu]-1)
            {
                printf("lol1");
                time_completed[run_now_cpu]=system_clock_time;
                completed+=1;
                //append the first value of the cpu queue to the completed queue
                popC(&headC);//remove the first value of the CPU queue over here
                CPU_dispatch+=1;
            }
            else
            {
                runtime[run_now_cpu]=runtime[run_now_cpu]+1;
                check_run[run_now_cpu]=check_run[run_now_cpu]-1;
                if(check_run[run_now_cpu]==0)
                {
                    appendR(&headR,headC->dataC);//append from cpu queue to ready queue 
                    popC(&headC);//pop from cpu queue
                    CPU_dispatch+=1; 
                }
            }
        }
        else //only io queue has values
        {
            run_now_IO=headIO->dataIO;
            IO_runtime=IO_runtime+1;
            CPU_idle_time=CPU_idle_time+1;
            if(check_block[run_now_IO]==0) //not blocked 
            {
                if(block_time[run_now_IO]!=0) //had been blocked but not blocked anymore
                {
                    appendR(&headR,headIO->dataIO);//append item from IO queue to ready queue 
                    popIO(&headIO);//pop the item from the IO queue
                    dispatches[run_now_IO]=dispatches[run_now_IO]+1; //passed from io to cpu
                    block[run_now_IO]=0; //process not blocked anymore
                    IO_dispatch+=1;
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
        system_clock_time+=1;
    }
    printf("ready");
    Rprint(headR);
    printf("cpu");
    CPUprint(headC);
    printf("IO");
    IOprint(headIO);
    printf("cpu runtime %d\n",CPU_runtime);
    printf("cpu idle time %d\n",CPU_idle_time);
    for(i=0;i<num;i++)
    {
        printf("%d \n",time_completed[i]);
    }
}


   /*
       printf("Program output (to stdout):\n------------------\n");
     header line 
    printf("Processes:\n\n");

    printf("Name    CPU time    When done  Dispatches  Block for I/O    I/O time\n");
    for (int v=0;v<num;v++)
    {
        
        printf("%s  ",name_process[v]);
        printf("%d",time_process[v]);
        printf("\n");
    }
    */

