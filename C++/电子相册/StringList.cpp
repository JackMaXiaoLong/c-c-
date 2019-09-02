#include "StringList.h"
/*
	数据节点的构造函数
*/
Node::Node(ElemType d)
{
	data = d;
	next = NULL;
}

Node::~Node()
{
	cout << "~Node" << endl;
}

void Node::show()
{
	cout << data << endl; 
}

StringList::StringList()
{
	first = NULL;
	last = NULL;
	num = 0;
	name = "pxl";
}

StringList::~StringList()
{
	cout << "~StringList" << endl;
}
/*
	尾插法
*/
void StringList::push_back(ElemType d)
{
	//创建一个新节点
	Node *new_node = new Node(d);
	
	//判断是否为空链表
	if(this->first == NULL)
	{
		first = new_node;
		last = new_node;
	}
	else
	{
		last->next = new_node;
		last = new_node;
	}
	
	num++;
	
}

void StringList::Print()
{
	//遍历指针
	Node*p = first;
	while(p)
	{
		cout << p->data << " ";
		p = p->next;
	}
	
	cout << endl;
}

bool StringList::isEmpty()
{
	return (num == 0);
}
