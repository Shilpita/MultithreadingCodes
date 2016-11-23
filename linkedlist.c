#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"

void initHead(struct node* head)
{
 head->next = NULL;
}

struct node* append(struct node* new_node, struct node* head)
{
  new_node->next= head;
  head = new_node;     
  return head;
}

int search(char word[], struct node* head)
{
  while(head!=NULL)
{
   if(strcmp((char*)head->word,(char*)word)==0)
   {
       head->count++;
       return 1;
   }
   else
  {
      head= head->next;   
  }
   
} 
  return 0;    
}

void printlist(struct node* head)
{
   while(head!=NULL) {
   	printf("%s\t %d\n",head->word,head->count);
   	head= head->next;
   }
}

void  deletelist(struct node* head)
{
  struct node* temp_prev ;
  while(head!=NULL)
  {
     temp_prev = head;
     head= head->next;
     free(temp_prev);  
  }   
}


/*
int main(void)
{
  struct node* head = (struct node*)malloc(sizeof(struct node));
  strncpy((char*)&head->word, "tester", 32);
  initHead(head);

  struct node* next_node = (struct node*)malloc(sizeof(struct node));
  strncpy((char*)&next_node->word, "sample", 32);
  head = append(next_node,head);

  printlist(head); 
  int ret= search("sample",head);

  printlist(head); 
 deletelist(head);
  head=NULL;
  printlist(head);
  return 0 ;
}*/
