#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


int Check_if_blocked(float p, int t)
{
    int random;
    random=0;//generate random number here between 0 and 1 
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
        fclose(file1);
        //convert probability string to float
        for(i=0;i<num;i++)
        {
            probability[i]= atoi(strcat(probability_string[i][2],probability_string[i][3])); //if 0.87, will give 87
            probability[i]=(probability[i]/100.0);
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
                //pop value from the ready list
                if(preblock_run[track]==0)
                {
                    block[track]=Check_if_blocked(probability[track],(time_process[track]-runtime[track]));
                }
                if(block[track]==1) //blocked
                {
                    if(preblock_run[track]==0 && check_run[track]==0)        
                    {
                        random=0;//generate random number from 1 to (time_process[track]-runtime[track]) for time to run before block
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
                        //append track value from cpu queue to IO queue
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
                //pop value from the ready list
                if(preblock_run[track]==0)
                {
                    block[track]=Check_if_blocked(probability[track],(time_process[track]-runtime[track]));
                }
                if(block[track]==1) //blocked
                {
                    if(preblock_run[track]==0 && check_run[track]==0)        
                    {
                        random=0;//generate random number from 1 to 5 or (time_process[track]-runtime[track]) if time remaining less than 5  - for time to run before block
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
                        //append track value from cpu queue to IO queue
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
                //remove the first value of the CPU queue over here using pop
            }
            else
            {
                runtime[run_now_cpu]=runtime[run_now_cpu]+1;
                check_run[run_now_cpu]=check_run[run_now_cpu]-1;
                if(check_run[run_now_cpu]==0)
                {
                    //append from cpu queue to ready queue
                    //pop from cpu queue
                }
            }
            if(check_block[run_now_IO]==0) //not blocked 
            {
                if(block_time[run_now_IO]!=0) //had been blocked but not blocked anymore
                {
                    //append item from IO queue to ready queue 
                    //pop the item from the IO queue
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
                //remove the first value of the CPU queue over here
            }
            else
            {
                runtime[run_now_cpu]=runtime[run_now_cpu]+1;
                check_run[run_now_cpu]=check_run[run_now_cpu]-1;
                if(check_run[run_now_cpu]==0)
                {
                    //append from cpu queue to ready queue
                    //pop from cpu queue
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
                    //append item from IO queue to ready queue 
                    //pop the item from the IO queue
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
}


int CPU_function(int time, char type)     //type is for FCFS or RR
{
    int runtime=0;
    if(type=='f')
    {  
        runtime=time;
    }
    if(type =='r')
    {

    }
    return time;
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