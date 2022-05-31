while(ready queue is not empty or IO queue is not empty or CPU is busy or IO is busy){

if pCurrent != 0 and iCurrent =0//CPU is busy and IO is idle
	pCurrent -=1;
    cpubusytime +=1;
    ioidletime +=1;
    wallclock +=1;

    if pCurrent =0
        if pRemain = 0
            terminate the process
            processendtime = Wallclock;
        else

            if PgotoIO = true
                IOqueue.push//function to add item at the end of link list IO queue
            else
                readyqueue.push //function to add item at the end of link list ready queue

        if readyqueue is not empty//only pCurrent go to 0, CPU finish the job
            dispatchToCPU();
    if IOqueue is not empty
        serveIO();

if pCurrent =0 and iCurrent !=0//IO is busy and CPU is idle
    iCurrent -=1;
    cpuideletime +=1;
    iobusytime +1=1;
    wallclock +=1;
    

    if iCurrent=0
        readyqueue.push //function to add item at the end of link list ready queue
        if IOqueue is not empty
            serveIO();

    if readyqueue is not empty
        dispatchToCPU();

if pCurrent !=0 and iCurrent !=0//both busy make sure if CPU or IO finish the current job, send them to either queue and check if new process coming in
    iCurrent -=1;
    pCurrent -=1;
    cpubusytime +=1;
    iobusytime +1=1;
    wallclock +=1;

    if pCurrent =0
        if pRemain = 0
            terminate the process
            processendtime = Wallclock;
        else

            if PgotoIO = true
                IOqueue.push
            else
                readyqueue.push //function to add item at the end of link list ready queue


        if readyqueue is not empty
            dispatchToCPU();

    
    if iCurrent=0
        IOqueue.push
        if IOqueue is not empty
            serveIO();
}

void dispatchTOCPU(process_number)
{
    process_dispatchtime +=1;
    if pRemain<2
        pCurrent = pRemain;
        pRemain =0; //run if only 1 or 0 units 

    getrandom to decide if it blocks//compare with given probability
    if block
        pCurrent = getrandom running time;
        pgotoIO = true;
       
    else
        pCurrent = remaning time or quntum 5
        pgotoIO = false;
      
    pRemain=pRemain-pCurrent; //update the remaning time
    
}

void serveIO(provess_number)
{
    process_IOtime +=1;
    iCurrent =  getrandomforIO//1 to 30 ;
    
}