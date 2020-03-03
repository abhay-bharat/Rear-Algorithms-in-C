#include "assignment_2.h"
#include<stdlib.h>
#include<stdio.h>
#include<limits.h>

typedef struct{
  int* arr; //each node will have array with size equal to the level at which it is created(i.e. at a given time only
            // level number of elements will be there)
}node_t;

typedef struct{
  node_t* head; //To point to the node created of type node_t
  int count; //To know when a comparison match to be played between the nodes, if 1 play a match, if 0 just store in that node
}level_headers; //An array(of size (logn +1)) of this level_headers will be created.

level_headers* initialise(int level)//Function to initialise the headers of the nodes
{
  level_headers* head_array = (level_headers*)malloc(level*sizeof(level_headers));
  for(int i =0;i<level; ++i)
  {
    head_array[i].head = NULL;
    head_array[i].count = 0;
  }
  return head_array;
}

node_t* create_node(int level_number)  //This is called only when head of that level is pointing to NULL, otherwise just overwritten
{
  node_t* new_node = (node_t*)malloc(sizeof(node_t));
  new_node->arr = (int*)malloc(level_number*sizeof(int)); //An array is created

  return new_node;
}

int ispowerOf2(int n) //Function to check if a number is a power of 2
{
  while (n != 1)
  {
      if(n%2 != 0)
         return 0;
      n = n/2;
  }
  return 1;
}

void find_second_greatest(int *numbers, int length)
{
  int i,j;
  if(length == 1)
    return;
  int level = 1;  //number of levels in a tournament tree + 1
  int n = length;
  while(n != 1)
  {
    level+=1;
    n = n/2;
  }

  level_headers* head_arr = initialise(level);
  int* temp_arr = (int*)malloc((level+1)*sizeof(int)); //This array at any moment will hold the most current status of the largest element and other
  for(i = 0;i<length;++i)                              //elements which lost to it.
  {
    j = 0; //To keep count of current level
    while(j!= level)
    {
      if(j+1 == 1)
      {
        if(head_arr[j].head == NULL)  //If that level is encountered for first time then a node is created, with array having size of level
        {
          head_arr[j].head = create_node(j+1);
        }
        if(head_arr[j].count == 0)  //This when just the element is stored and no match is played
        {
          temp_arr[0] = numbers[i];
          head_arr[j].head->arr[0] = numbers[i];
          head_arr[j].count++;
          j++;
          for(int k =1;k<level;++k)    //Just for the convenience in the end to print the final status.
          {
            temp_arr[k] = INT_MIN;
          }
          break;   //Since element is just stored hence no further comparison with other level will be done
        }
        else if(head_arr[j].count == 1)  //This is when a comparison match is played
        {
          int temp = head_arr[j].head->arr[0];
          head_arr[j].count = 0;
          printf("%d x %d = ",temp,numbers[i]);
          if(temp > numbers[i])
          {
            temp_arr[0] = temp;
            temp_arr[1] = numbers[i];
          }
          else
          {
            temp_arr[0] = numbers[i];
            temp_arr[1] = temp;
          }
          for(int k = 2;k<=(level);++k)
          {
            temp_arr[k] = INT_MIN;
          }
          printf("%d %d\n",temp_arr[0],temp_arr[1]);
        }
      }
      else if((j+1) > 1)   //This is to handle a bit differently with respect to the printing format from the case when just 2 elements are compared
      {
        if(head_arr[j].head == NULL)
        {
          head_arr[j].head = create_node(j+1);
        }
        if(head_arr[j].count == 0)  //Just store elements from the temp_arr(this is the most current status of the problem, which is held in it).
        {
          for(int k = 0;k<(j+1); ++k)
          {
            head_arr[j].head->arr[k] = temp_arr[k];
          }
          head_arr[j].count++;
          j++;
          break;  //Since just stored, hence break out of the while loop without any comparison required
        }
        else if(head_arr[j].count == 1)
        {
          for(int k = 0; k< (j+1); ++k)  //As per the asked format print statements are written
          {
            printf("%d ",head_arr[j].head->arr[k]);
          }
          printf(" x ");
          for(int k =0;k< (j+1);++k)
          {
            printf("%d ",temp_arr[k]);
          }
          printf(" = ");
          if(head_arr[j].head->arr[0] > temp_arr[0])  //Updating of the temp_arr, since a comparison match is played
          {
            int temp = temp_arr[0];
            temp_arr[0] = head_arr[j].head->arr[0];
            temp_arr[1] = temp;
            for(int k = 2; k<=(j+1); ++k)
            {
              temp_arr[k] = head_arr[j].head->arr[k-1];
            }
            for(int k = j+2;k<=level;++k)
            {
              temp_arr[k] = INT_MIN;
            }
          }
          else if(head_arr[j].head->arr[0] < temp_arr[0])
          {
            int q = j;
            while(q != 0)
            {
              temp_arr[q+1] = temp_arr[q];
              q--;
            }
            temp_arr[q+1] = head_arr[j].head->arr[0];
          }
          for(int k = 0;k<=(j+1);++k)
          {
            if(temp_arr[k] != INT_MIN)
              printf("%d ",temp_arr[k]);
          }
          printf("\n");
          head_arr[j].count = 0;  //After a match is played the node is free again to store the next elements
        }
      }
        ++j;
    }
  }
  if(!ispowerOf2(length)) //To handle case when number is not power of 2, when last element enountered
    {
      while(j < level) //While the current level(i.e. j) is less than the level
      {
        int k =0;
        if(head_arr[j].count == 1) //A comparison match played with the next nodes only whenever it is filled with values.
        {
          for(int p = 0;p <= j;++p)
          {
            printf("%d ",head_arr[j].head->arr[p]);
          }
          printf("x ");
          while(temp_arr[k] != INT_MIN)
          {
            printf("%d ",temp_arr[k]);
            k++;
          }
          printf(" = ");
          if(temp_arr[0] > head_arr[j].head->arr[0]) //Handling of the cases seperately
          {
            int l = level-1;
            while(l != 0)
            {
              temp_arr[l+1] = temp_arr[l];
              l--;
            }
            temp_arr[1] = head_arr[j].head->arr[0];
          }
          else if(head_arr[j].head->arr[0] > temp_arr[0])
          {
            temp_arr[1] = temp_arr[0];
            temp_arr[0] = head_arr[j].head->arr[0];
            for(int q = 1; q<=j;++q)
            {
              temp_arr[q+1] = head_arr[j].head->arr[q];
            }
          }
          int p = 0;
          while(p != (level+1) && temp_arr[p] != INT_MIN)
          {
            printf("%d ",temp_arr[p]);
            p++;
          }
          printf("\n");
        }
        j++;
    }
  }
    int m =2; //Because 1st element is the max element
    int second_max = temp_arr[1];
    while(m != level+1 && temp_arr[m] != INT_MIN) //This is for the last (logn - 1) comparisons for second highest element
    {
      if(temp_arr[m] > second_max)
      {
        second_max = temp_arr[m];
      }
      ++m;
    }
    printf("\n%d",second_max);
    free(temp_arr); //Freeing the array created
    free(head_arr); //Freeing of the head arrays
}
