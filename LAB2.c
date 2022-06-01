

int main(int argc, string argv[])
{
   int num = (argc-1)/3;//number of process total
   int i;
   int j=0;
   char name_process [num][10];//process name array to store process names
   int name_index = 0;
   char runtime_process [num][2];//runtime array, assume the runtime can be 1 to 99
   char probobility_process [num][4];//probability to block array, "0.xx" is 4 characters

//for checking the format, we can get the strlen for argv[i][j] and use loop to check if they all number and etc.

   for(i=1, i<num, i+=3)//only 1th, 4th, 7th... are process names
   {    
    name_process [name_index] = argv[i];//store the process name in the array   
   }

    for(i=2, i<num, i+=3)//only 2th, 5th, 8th... are process runtime
   {
    name_process [name_index] = argv[i];//store the process name in the array
    }

   for(i=3, i<num, i+=3) //3th, 6th, 9th... are probability of block
   {
    probobility_process [num]=argv[i];// store the probability of block
   }
//note, all three array are character type still, make sure to do the convertion for later used
   

}

struct Node
{
  int data; //using number to represent the file name, this is also the index number to all record arrays
  struct Node *next;
};
 

void append(struct Node** head, int newData)//to initialzie the ready queue, use for loop to add the process number one by one
{
   
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    struct Node *current = *head;  
    newNode->data  = newData;//assign to the end of ready queue
 
    newNode->next = NULL;//if ready queue is empty, the head node is the new node
    if (*head == NULL)
    {
       *head = newNode;
       return;
    }
 
    while (current->next != NULL)//go to the end of item and connect it to the new node 
        current = current->next;
    current->next = newNode;
    return;
}

void pop(struct node **head) {
  struct node *tmp;

  /*Linked list does not exist or the list is empty*/
  if(*head == NULL) {
      return;
  } 
  
  tmp = *head;
  *head = (*head)->next;//move head to next
  free(tmp);//remove first item
}
