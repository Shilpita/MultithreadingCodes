// Using fork() create a child process and print its Parent and chid id


#include<unistd.h>

#include<stdio.h>




int main ()

{
  
 pid_t child_pid=  fork();
 
  if (child_pid <0)
  
   printf("Error in child creation");
 
  else
    
   printf ("The parent process id = %d , the self process id = %d and the child process id= %d \n",getppid(),getpid(),child_pid);

 //while(1);
 

return 0;


}


