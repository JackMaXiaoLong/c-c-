#ifndef _StringList_H
#define _StringList_H
#include <iostream>
#include <cstdio>
using namespace std;
typedef string ElemType;

class Node//数据节点
{
	private:
		ElemType data;
		Node*next;
		//...
		
	public:
		Node(ElemType d);
		~Node();
		void show();
		
		//声明友元类
		friend class StringList;
		friend class Iterator;
		//...
};

class Iterator
{
	public:
		Iterator(Node*p):p(p)
		{}
		//自加(前置)
		void operator++()
		{
			p = p->next;
		}
		bool operator!=(const Iterator&other)
		{
			return p != other.p;
		}
		ElemType operator*()
		{
			return p->data;
		}	
		friend class StringList;			
	private:
		Node*p;
};




class StringList//链表
{
	private:
		Node*first;
		Node*last;
		int num;
		string name;
		//....
	
	public:
		StringList();
		~StringList();
		
		//尾插法
		void push_back(ElemType d);
		
		//头插法
		
		//从头删除
		//从尾删除
		//是否为空
		bool isEmpty();
		//遍历这个链表
		void Print();
		
		//返回链表中元素个数
		int Elem_num()const
		{
			return num;
		}
		//...
		//返回第一个元素所在的位置
		Iterator begin()
		{
			return Iterator(first);
		}
		//返回最后一个元素的下一个位置
		Iterator end()
		{
			return Iterator(last->next);
		}
};




#endif