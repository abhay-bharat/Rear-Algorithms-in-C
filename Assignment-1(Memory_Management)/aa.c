#include "assignment_1.h"
#include<stdlib.h>
#include<stdio.h>

typedef struct book_keeping{
  int status; //0-free and 1-allocated
  struct book_keeping* next; //pointer of structure type next, to point to next bookkeeping structure
  int num_bytes; // To store the number of bytes allocated or is free
}t_bookKeeping;

extern t_bookKeeping* head;
t_bookKeeping* head = NULL;
char* p;
int size;

void allocate(int n)
{
  size = n; // The size of the character array 'p'
  p = (char*)malloc(size*sizeof(char));
  head = (t_bookKeeping*)(p + sizeof(t_bookKeeping)); //head is the bookkeeping node whose address is the base address of memory allocated
  head->status = 0;
  head->next = NULL;
  head->num_bytes = n-sizeof(t_bookKeeping); //Because out of n bytes sizeof structure byte is used as bookkeeping node
}

void* mymalloc(int space)
{
  t_bookKeeping* pres = head;
  t_bookKeeping* prev = NULL;
  t_bookKeeping* temp; // To create new node of bookkeeping structure
  char* ptr = p;
  int book = sizeof(t_bookKeeping);

  ptr = ptr + book;
  while(pres != NULL)
  {
    if(pres->status == 0 && pres->num_bytes >= space)//When available space is sufficient to create a book and allocate bytes
    {
      if(pres->num_bytes >= space + book)
      {
        pres->num_bytes -= (book + space); //Taking out space amount of bytes from the free space

        temp = (t_bookKeeping*)(ptr + book); //Creating a new to store the details of space allocated
        temp->status = 1;
        temp->num_bytes = space;
        ptr = ptr + book;

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
      {
        pres->status = 1;
      }
      int offset = ptr - p;
      return p + offset; //Returning the location to space allocated
    }
    else
    {
      ptr = ptr + pres->num_bytes + book;
    }
    prev = pres;
    pres = pres->next;
  }
  return NULL;
}

void display_mem_map()
{
  t_bookKeeping* temp = head;
  int init_byte = 0;
  int count = 0; //To know when to print the memory used up for bookkeeping(i.e.when count is even)
  int book_size = sizeof(t_bookKeeping);
  //printf("%d\n",temp->num_bytes);
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
      temp = temp->next;
    }
    count++;
  }
}






#include "assignment_1.h"
#include<stdlib.h>
#include<stdio.h>

typedef struct book_keeping{
  int status; //0-free and 1-allocated
  struct book_keeping* next; //pointer of structure type next, to point to next bookkeeping structure
  int num_bytes; // To store the number of bytes allocated or is free
}t_bookKeeping;

typedef struct{
  t_bookKeeping* head;
}memoryManagement;

extern memoryManagement* mem_list;
memoryManagement* mem_list = NULL;;
//mem_list->head = NULL;
char* p;
int size;

/*static t_bookKeeping* create_Node()
{
  //t_bookKeeping* temp = (t_bookKeeping*)mymalloc(sizeof(t_bookKeeping));
  temp->status = 0;
  temp->next = NULL;
  return temp;
}
*/
void allocate(int n)
{
  size = n; // The size of the character array 'p'
  p = (char*)malloc(size*sizeof(char));
  mem_list = (memoryManagement*)(p + sizeof(memoryManagement));
  mem_list->head = NULL;
  //p = p + sizeof(memoryManagement);
  t_bookKeeping* temp = (t_bookKeeping*)(p + sizeof(t_bookKeeping)); //temp is the bookkeeping node whose address is the base address of memory allocated
  temp->status = 0;
  temp->next = NULL;
  temp->num_bytes = n-(sizeof(t_bookKeeping) + sizeof(memoryManagement)); //Because out of n bytes sizeof structure byte is used as bookkeeping node
  mem_list->head = temp;
  //p = p+sizeof(t_bookKeeping);
}

void* mymalloc(int SIZE)
{
  t_bookKeeping* pres = mem_list->head;
  t_bookKeeping* prev = NULL;
  t_bookKeeping* temp; // To create new node of bookkeeping structure
  char* ptr = p;
/*  if(temp->num_bytes < SIZE)
  {
    return NULL;
  }
  else
  {
  */
//  int count = 1; //To keep the count of number of nodes after which free space is found
    int book_size = sizeof(t_bookKeeping);
    temp =(t_bookKeeping*)(p + book_size);
    //char* p = p;
    while(pres != NULL)//This loop is to find the structure holding the first
    {                  //fit free space from which it is to be allocated
      if(pres->status == 0 && pres->num_bytes >= (SIZE + book_size))
        break;
      prev = pres;
      pres = pres->next;
      temp = temp + book_size + prev->num_bytes;
      ptr = ptr + book_size + prev->num_bytes;
    }
    if(pres == NULL)
    {
      return NULL;
    }
    else
    {
      pres->num_bytes -= (SIZE + book_size);
      temp->status = 1;
      temp->num_bytes = SIZE;
      if(prev == NULL)
      {
        temp->next = mem_list->head;
        mem_list->head = temp;
      }
      else
      {

        prev->next = temp;
        temp->next = pres;
      }
    }
    int offset = ptr-p;
    return p + offset;
}

void display_mem_map()
{
  t_bookKeeping* temp = mem_list->head;
  int init_byte = 0;
  int count = 0; //To know when to print the memory used up for bookkeeping(i.e.when count is even)
  int book_size = sizeof(t_bookKeeping);
  //printf("%d\n",temp->num_bytes);
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
    }
    count++;
    temp = temp->next;
  }
}
