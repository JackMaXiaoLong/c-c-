#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h> /*perror, errno ,...*/
#include <stdlib.h>/*abs的头文件*/
#include <stdio.h> /*printf*/
#include <sys/mman.h>
#include <linux/input.h>
#include <stdlib.h>
#include <time.h> /*time */
#include <string.h>
#include <linux/fb.h>
#include <pthread.h>
#include <dirent.h> 
#include "linklist.h"

extern int* p;
struct headnode* create_head()
{
	struct headnode* currenthead = malloc(sizeof(*currenthead));
	currenthead->first = currenthead->last = NULL;
	return currenthead;
}
void add_node(struct headnode* currenthead, struct node *p)
{
	if (currenthead== NULL || p == NULL)
	{
		return ;
	}
	if (currenthead->first == NULL)
	{
		currenthead->first = p;
		currenthead->last = p;
	}
	else
	{
		currenthead->last->next = p;
		p->prev=currenthead->last;
		currenthead->last=p;	
	}	
}

void print_list(struct headnode* currenthead)
{
	struct node *p1 = currenthead->first;
	while (p1)
	{
		printf("%s\n", p1->data);
		p1 = p1->next;
	}
}
int geshu(struct headnode* currenthead)
{
	int k=0;
	struct node *p2 = currenthead->first;
	while (p2)
	{
		k++;
		p2 = p2->next;
	}
	return k;
}

void find_picture(char *name,struct headnode* currenthead)
{
	DIR*dir = opendir(name);
	if (dir == NULL)
	{
		perror("opendir error:");
		return ;
	}
	while(1)
	{
		struct dirent*dirent = readdir(dir);
		if(dirent == NULL)
		{
			break;
		}
		if(strcmp(dirent->d_name,".")==0||strcmp(dirent->d_name,"..")==0)
		{
			continue;
		}
		struct stat st;
		char wondelfulname[512]={0};
		sprintf(wondelfulname, "%s/%s", name, dirent->d_name);
		stat(wondelfulname, &st);
		if(S_ISREG(st.st_mode))
		{
			/*int fd = open(wondelfulname,O_RDONLY);
			if(fd == -1)
			{
				perror("error:");
				return ;
			}



			char data[2];
			read(fd,data,2);
			if(data[0] == 0x42 && data[1] == 0x4d )
			{
				struct node* p=malloc(sizeof(*p));
				strcpy(p->data,wondelfulname);
				p->next=p->prev=NULL;
				add_node(currenthead,p);
			}
			*/
			if(strcmp(dirent->d_name+strlen(dirent->d_name)-4,".bmp") == 0 ||strcmp(dirent->d_name+strlen(dirent->d_name)-4,".jpg")== 0)
			{
				struct node* p=malloc(sizeof(*p));
				strcpy(p->data,wondelfulname);
				p->next=p->prev=NULL;
				add_node(currenthead,p);
			}	
			//close(fd);
		}
		else if(S_ISDIR(st.st_mode))
		{
			find_picture(wondelfulname,currenthead);
		}
			
	}
	closedir(dir);		
}