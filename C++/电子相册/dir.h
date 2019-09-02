#ifndef _DIR_H
#define _DIR_H
#include "StringList.h"
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <cstring>

using namespace std;
class Dir
{
	private:
		DIR*pdir;
		string pathname;
	
	public:
		Dir(string pathname);
		~Dir();
		//获取目录中的所有的文件名存储在链表中
		StringList EntryList();

		
};



#endif