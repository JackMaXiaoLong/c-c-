
#include "dir.h"
Dir::Dir(string pathname):pathname(pathname)
{
	//打开目录
	pdir = opendir(pathname.c_str());//const char*
	if(pdir == NULL)
	{
		//perror("opendir error:");
		cerr << strerror(errno) << endl;
		throw strerror(errno);//可以获取到系统的错误提示
	}
}

Dir::~Dir()
{
	cout << "目录的析构函数" << endl;
}

/*
	EntryList:获取目录中的所有的文件名存储在链表中
*/
StringList Dir::EntryList()
{
	StringList list;
	struct dirent*p = NULL;
	while(p = readdir(pdir))
	{
		string filname = pathname + "/" + p->d_name;
		list.push_back(filname);
	}
	
	return list;
}
