#include "assignment_1.h"
#include<stdlib.h>
#include<stdio.h>

typedef struct book_keeping{
  int status; //0-free and 1-allocated
  struct book_keeping* next; //pointer to a structure type, to point to next bookkeeping structure
  int num_bytes; // To store the number of bytes allocated or is free
  char* pointer_location; //To hold the address of the location to which this book belongs to
}t_bookKeeping;

extern t_bookKeeping* head; //head is the bookkeeping node,it will point to the initial book node which holds base address of memory allocated
t_bookKeeping* head = NULL;
//Defining the extern variable declared in the .h file
char* p;
int size;

void allocate(int n)
{
  size = n; // The size of the character array 'p'
  p = (char*)malloc(size*sizeof(char)); //dynamic allocation of memory
  head = (t_bookKeeping*)(p + sizeof(t_bookKeeping));//Explicit typecasting of the memory allocated to store the structure variable
  head->status = 0;//Initially the memory allocated is free
  head->next = NULL;
  head->pointer_location = p + sizeof(t_bookKeeping);
  head->num_bytes = n-sizeof(t_bookKeeping); //Because out of n bytes sizeof structure byte is used as bookkeeping node
}

void* mymalloc(int space)
{
  t_bookKeeping* pres = head;
  t_bookKeeping* prev = NULL;
  t_bookKeeping* temp; // To create new node of bookkeeping structure
  char* ptr = NULL;//To temporarily point to the starting of the memory allocated
  int book = sizeof(t_bookKeeping);
  while(pres != NULL)
  {
    if(pres->status == 0 && pres->num_bytes >= space)//When there is space available to allocate
    {
      if(pres->num_bytes >= space + book)//When there is space sufficient to both get allocated and along with a bookkeeping
      {
        ptr = pres->pointer_location + book;
        pres->num_bytes -= (book + space); //Taking out space amount of bytes from the free space
        pres->pointer_location += (book + space);//The free space it points to is now certain offset ahead
        temp = (t_bookKeeping*)(ptr + book); //Creating a new to store the details of space allocated
        temp->status = 1;
        temp->num_bytes = space;
        temp->pointer_location = ptr;
        if(prev == NULL) //Placing node at the appropriate position(here in the beginning)
        {
          temp->next = head;
          head = temp;
        }
        else
        {
          prev->next = temp;
          temp->next = pres;
        }
      }
      else //When number of free bytes is just sufficient to allocate space and not to store in new bookkeeping node
      {    //Hence no new book is created just the book which held details of free space is used
        pres->status = 1; //The memory that was marked as free is now marked as allocated
        return pres->pointer_location;
      }
      int offset = ptr - p;
      return p + offset; //Returning the location to space allocated
    }
    prev = pres;
    pres = pres->next;
  }
  return NULL; //When memory allocation is not possible
}

void myfree(void *b)
{
  if (b == NULL)
    return;
  int book = sizeof(t_bookKeeping);
  t_bookKeeping* pres = head;
  t_bookKeeping* prev = NULL;
  while(pres != NULL && pres->pointer_location != b)//Checking if it is NULL first always,  otherwise it could cause seg fault
  {                                                 //then checking if appropriate pointer location is reached
    prev = pres;
    pres = pres->next;
  }
  //To check if pres is NULL
  if(!pres){
    return;
  }
  //To check if it is already free
  if(pres->status == 0){
    return;
  }
  pres->status = 0;
  //Merging of adjacent free spaces
  if(pres->next != NULL && pres->next->status == 0) //If the space next to the allocated space(the one which is freed now) is free space
  {
    pres->num_bytes += pres->next->num_bytes + book;
    pres->next = pres->next->next;
  }
  if(prev != NULL && prev->status == 0)//If there is an free space of bytes before the space which is freed now, then it is merged into a single free space
  {
    prev->num_bytes += pres->num_bytes + book;
    prev->next = pres->next;
  }
}

void print_book()
{
  printf("%d\n",sizeof(t_bookKeeping));
}

void display_mem_map()
{
  t_bookKeeping* temp = head;//Temporary pointer variable to hold the node in linked list
  int init_byte = 0; //The initial byte number out of the allocated byte from where book or memory is allocated(also from where the free bytes begin)
  int count = 0; //To know when to print the memory used up for bookkeeping(i.e.when count is even)
  int book_size = sizeof(t_bookKeeping);
  while(temp != NULL)
  {
    printf("%d\t",init_byte);
    if(count%2 == 0)
    {
      init_byte += book_size;
      printf("%d\t",book_size);
      printf("book\n");
    }
    else
    {
      init_byte += temp->num_bytes;
      printf("%d\t",temp->num_bytes);
      if(temp->status == 0)
      {
        printf("free\n");
      }
      else
      {
        printf("allocated\n");
      }
      temp = temp->next; //Move to next node only after the present node data is printed(not when book is printed).
    }
    count++;
  }
}
