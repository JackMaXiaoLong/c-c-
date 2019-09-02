#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<pthread.h>
#include<bits/pthreadtypes.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<signal.h>
int main()
{	
	int choose=0;
	pid_t pid;
	int flag=0; 
	while(1)
    {
    	system("clear");
    	printf("menu: 1.play 2.stop/continue 3.next 4.prev 5.real_stop\n");
    	printf("please input: ");
		scanf("%d",&choose);
		switch(choose)
		{
			 case 1:
			 {
				 flag=1;
				 pid =fork();
				 if(pid==0)
				 {	 	
                     close(0);                   
					 execlp("madplay","madplay","1.mp3","-r",NULL);	 
				 }
			     break;
			}	
			 case 2:
			 {
			     if(flag==1)
				 {
					 kill(pid,19);
					 flag=2;
				 }
				 else if(flag==2)
				 {
					 kill(pid,18);
					 flag=1;
				 }
			     break;
			 }
			 case 3:
			    {
				 kill(pid,9);
				 flag=1;
				 sleep(1);
				 pid =fork();
				 if(pid==0)
				 {
					 close(0); 
					 execlp("madplay","madplay","2.mp3","-r",NULL);
				 }
			     break;
			    }
			 case 4:
			    {
				 kill(pid,9);
				 flag=1;
				 sleep(1);
				 pid =fork();
				 if(pid==0)
				 {
					 close(0); 
					 execlp("madplay","madplay","3.mp3","-r",NULL);
				 }
			     break;
			    }
				 case 5:
				 {
				     kill(pid,9);
				     break;
				 }
			  	default:
				printf("error\n");
		}
		sleep(1);
	}
	return 0;
}