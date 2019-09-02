#ifndef _LINKLIST_H
#define _LINKLIST_H

struct node
{
	struct node* next;
	struct node* prev;
	char data[512];
};
struct headnode
{
	struct node* first;
	struct node* last;	
};
struct headnode* create_head();
void find_file(char *name,struct headnode* currenthead);

void add_node(struct headnode* currenthead, struct node *p);


void print_list(struct headnode* currenthead);

int geshu(struct headnode* currenthead);


void find_picture(char *name,struct headnode* currenthead);
void qingkong(struct headnode* currenthead);
#endif