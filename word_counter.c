#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<string.h>
#include<assert.h>
#include "linkedlist.h"

struct node *head_gv;

#define MAX_WORD_SIZE     32
#define MAX_WORDS_ROW     1024
#define MAX_ROWS          30          
#define MAX_WORD_ALLOCATION 10

char   work_set[MAX_ROWS][MAX_WORDS_ROW * MAX_WORD_SIZE];

#define MAX_THREAD_COUNT  4

struct thread_arg{
    int start_row ;
    int n_sentence;
};

pthread_mutex_t mutex =  PTHREAD_MUTEX_INITIALIZER;

int printWordCount(char *sentence)
{
	int   ret = 0;
	int   letter = 0;
	int   word_count  = 0;
	char  word_buf[MAX_WORD_SIZE];
	char  *chp  = sentence;
	struct node* next_node;
	int    flag = 0;
	int    word_end =  0;

	if (!chp) {
		ret = -EINVAL;
		return ret;
	}

	// chp till senetence end
	while((*chp != '\0') && letter < MAX_WORD_SIZE)
	{
		word_end = (*chp ==' '|| *chp =='\t'|| *chp =='\n');

		if(flag && word_end) {
			word_buf[letter] = '\0';
			//printf ("%s\n", word_buf);
			pthread_mutex_lock(&mutex);
			ret= search(word_buf, head_gv);
  			pthread_mutex_unlock(&mutex);
			if (!ret) {
				next_node = (struct node*)malloc(sizeof(struct node));
			        pthread_mutex_lock(&mutex);
				strncpy((char*)next_node->word, word_buf, MAX_WORD_SIZE);
				next_node->count = 1;
		//		pthread_mutex_lock(&mutex);
				head_gv = append(next_node, head_gv);
				pthread_mutex_unlock(&mutex);
			}
			word_count++;
			letter = 0;
			flag = 0;
		} else if ((!flag && !word_end) || (flag)) {      
			word_buf[letter] = *chp;
			letter++;
			flag = 1;
		} 	
		chp++;
	}

	if (letter >= MAX_WORD_SIZE) {
		ret = -EINVAL;
	} else {
		//printf ("Word Count :%d\n", word_count);
	}	

	return ret;
}

void* thread_work(void* arg)
{
   int print_ret=0;
   struct thread_arg* th_arg = (struct thread_arg*) arg;
   
  int i =0;
  for (i=th_arg->start_row; i< th_arg->start_row + th_arg->n_sentence; i++)
  { 
 //   pthread_mutex_lock(&mutex);
      print_ret = printWordCount(work_set[i]);
    //printf("%s : %d\n", work_set[i], th_arg->start_row);	
   // pthread_mutex_unlock(&mutex);
  }
 pthread_exit(0);
}


int    work_item = 0;

int main(void) { 
	int    ret = 0;
	char  *str = 0;
	char   sentence[MAX_WORDS_ROW * MAX_WORD_SIZE];
        int    tid = 0;

	pthread_t         threads[MAX_THREAD_COUNT];
        struct thread_arg arg[MAX_THREAD_COUNT];
	int              exit_code[MAX_THREAD_COUNT];
	int            *exit_ptr[MAX_THREAD_COUNT];

	int  rows_per_thread;
        int  req_threads;
	

	// init link_list		

	head_gv = (struct node*)malloc(sizeof(struct node));
	strcpy((char*)head_gv->word, "\0");
	initHead(head_gv);

	FILE *fp;
	fp = fopen("sampletext2.txt", "r");

	if (!fp) {
		printf("ERR File :%d\n", errno);	
		return errno;
	}

	do {	
		str = fgets(sentence, MAX_WORDS_ROW * MAX_WORD_SIZE, fp);
		if (!str)  {
			break;
		} else {
			strcpy((char*)work_set[work_item], str);
			work_item++;
		}

	} while (str && (work_item < MAX_ROWS));	

	fclose(fp);	
 //       printf("%d \n",work_item);      
	// capped X
        req_threads = (work_item/MAX_WORD_ALLOCATION) + 1;
   //     printf("%d\n",req_threads);
	if (req_threads > MAX_THREAD_COUNT) {
		req_threads = MAX_THREAD_COUNT;
	}
  //	printf (" Max req threads :%d\n", req_threads);
 
	rows_per_thread = work_item/req_threads;  //MAX_THREAD_COUNT;
        
    //    printf ("%d \n",rows_per_thread);

        for(tid =0; tid < req_threads ; tid++)
       {
         arg[tid].start_row = rows_per_thread * tid;
	 assert(arg[tid].start_row < work_item);
         arg[tid].n_sentence = rows_per_thread;

	 if (tid == req_threads) {
		arg[tid].n_sentence+=work_item % req_threads;// MAX_THREAD_COUNT; 
	 }

	 pthread_create(&threads[tid], NULL, &thread_work, &arg[tid]);
       }

        for(tid =0; tid < MAX_THREAD_COUNT;tid++) {
		exit_ptr[tid] = &exit_code[tid];
		pthread_join(threads[tid], (void*)&exit_ptr[tid]);	
	}
        printf("Word List\t\tFrequency\n");
	printlist(head_gv);
	deletelist(head_gv);
	head_gv = 0;
	return 0; 	
}

		






