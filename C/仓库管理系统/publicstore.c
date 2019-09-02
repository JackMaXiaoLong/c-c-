#include <stdio.h>
#include "sqlite3.h"
#include <string.h>
#include<time.h>

char jiluzhe[32]={0};//记录登录用户名

void jilu(char *buf)
{
	sqlite3 * ppDb = NULL;
	int re = sqlite3_open("publicstore.db",&ppDb);
	if(re != 0)
	{
		perror("sqlite3_open error:");
		return ;
	}

	 const char*pzTail = NULL;	
	 sqlite3_stmt *ppStmt = NULL;

	char * sql = "insert into jilu(abc,time) values(?1,?2);"; 
	re = sqlite3_prepare_v2(ppDb,sql,-1,&ppStmt,&pzTail);
	//re = sqlite3_step(ppStmt);
	if(re != 0)
	{
		printf("sqlite3_prepare_v2 error:%s\n",sqlite3_errmsg(ppDb));
		return ;
	}	
	
	time_t timep;
	struct tm *p;
	time(&timep);
	p=gmtime(&timep);
	int y,m,d;
	y=p->tm_year+1900;
	m=p->tm_mon+1;
	d=p->tm_mday;
   char time1[32]={0};
   sprintf(time1,"%d-%d-%d",y,m,d);
	
	//获取sql参数的索引值
	int index[2];//保存参数的索引值
	index[0] =  sqlite3_bind_parameter_index(ppStmt, "?1");
	index[1] =  sqlite3_bind_parameter_index(ppStmt, "?2");
	//index[2] =  sqlite3_bind_parameter_index(ppStmt, "?3");
	
	//printf("%d %d %d \n",index[0],index[1],index[2]);
	//给参数绑定值
	sqlite3_bind_text(ppStmt,index[0],buf,strlen(buf),NULL);
	sqlite3_bind_text(ppStmt,index[1],time1,strlen(time1),NULL);
	//sqlite3_bind_int(ppStmt,index[2],num);
	
	//printf("sql = %s\n",sql);
	//printf("%c\n",*pzTail);
	//sqlite3_step :用来执行编译好QL语句对象
			
	re = sqlite3_step(ppStmt);
	if(re != SQLITE_DONE)
	{
		printf("sqlite3_step err%s\n",sqlite3_errmsg(ppDb));
		return ;
	}	


	//复位
	sqlite3_reset(ppStmt);
	
	
	
					
	// sqlite3_finalize :用来销毁一个语句对象
	re = sqlite3_finalize(ppStmt);
	if(re != 0)
	{
		perror("sqlite3_finalize error:");
		return ;
	}	
	
	//关闭数据库连接对象
	sqlite3_close(ppDb);
}

int chakanjilu()
{
	sqlite3 * ppDb = NULL;
	int re = sqlite3_open("publicstore.db",&ppDb);
	if(re != 0)
	{
		printf("error:%s\n",sqlite3_errmsg(ppDb));
		return -1;
	}				
	sqlite3_stmt *ppStmt = NULL;
	const char*pzTail = NULL;	
	char *sql = "select *from jilu;"; 	
	re = sqlite3_prepare_v2(ppDb,sql,-1,&ppStmt,&pzTail);
	if(re != 0)
	{
		perror("sqlite3_prepare_v2 error:");
		return -1;
	}	
	
	
	//运行
	 printf("************************\n");
	while(1)
	{
		int re = sqlite3_step(ppStmt);
		if(re == SQLITE_DONE)
		{
			break;
		}
		else if(re == SQLITE_ROW)
		{
			//获取多少列
			int n = sqlite3_column_count(ppStmt);
			
			int i;
		
			//获取每列的类型，才输出列值
			for(i = 0;i < n;i++)
			{
				//现获取类型//根据类型取获取值
				int type = sqlite3_column_type(ppStmt,i);	
				if(type == SQLITE_INTEGER)
				{
					printf("%d\t",sqlite3_column_int(ppStmt,i));
				}
				else if(type ==  SQLITE_FLOAT)
				{
					printf("%f\t",sqlite3_column_double(ppStmt,i));
				}
				else if(type ==  SQLITE_TEXT)
				{
					printf("%s\t",sqlite3_column_text(ppStmt,i));
				}
				else 
				{
					printf("\t");
				}
				
				
			}
			printf("\n");
		}
		else
		{
			perror("sqlite3_step:");
			return -1;
		}
	}
	printf("************************\n");
	
					
	// sqlite3_finalize :用来销毁一个SQL语句对象
	re = sqlite3_finalize(ppStmt);
	if(re != 0)
	{
		perror("sqlite3_finalize error:");
		return -1;
	}	
	
	//关闭数据库连接对象
	sqlite3_close(ppDb);
}

int chakan()
{
	sqlite3 * ppDb = NULL;
	int re = sqlite3_open("publicstore.db",&ppDb);
	if(re != 0)
	{
		printf("error:%s\n",sqlite3_errmsg(ppDb));
		return -1;
	}				
	sqlite3_stmt *ppStmt = NULL;
	const char*pzTail = NULL;	
	char *sql = "select *from publicstore;"; 	
	re = sqlite3_prepare_v2(ppDb,sql,-1,&ppStmt,&pzTail);
	if(re != 0)
	{
		perror("sqlite3_prepare_v2 error:");
		return -1;
	}	
	
	 int flag  = 1;
	//运行
	 printf("************************\n");
	while(1)
	{
		int re = sqlite3_step(ppStmt);
		if(re == SQLITE_DONE)
		{
			break;
		}
		else if(re == SQLITE_ROW)
		{
			//获取多少列
			int n = sqlite3_column_count(ppStmt);
			
			int i;
			//输出列名
			if(flag == 1)
			{	
				for(i = 0;i < n;i++)
				{
					printf("%s\t",sqlite3_column_name(ppStmt,i));
				}
				printf("\n");
				flag = 0;
			}			
			//获取每列的类型，才输出列值
			for(i = 0;i < n;i++)
			{
				//现获取类型//根据类型取获取值
				int type = sqlite3_column_type(ppStmt,i);	
				if(type == SQLITE_INTEGER)
				{
					printf("%d\t",sqlite3_column_int(ppStmt,i));
				}
				else if(type ==  SQLITE_FLOAT)
				{
					printf("%f\t",sqlite3_column_double(ppStmt,i));
				}
				else if(type ==  SQLITE_TEXT)
				{
					printf("%s\t",sqlite3_column_text(ppStmt,i));
				}
				else 
				{
					printf("\t");
				}
				
				
			}
			printf("\n");
		}
		else
		{
			perror("sqlite3_step:");
			return -1;
		}
	}
	printf("************************\n");
	
					
	// sqlite3_finalize :用来销毁一个SQL语句对象
	re = sqlite3_finalize(ppStmt);
	if(re != 0)
	{
		perror("sqlite3_finalize error:");
		return -1;
	}	
	
	//关闭数据库连接对象
	sqlite3_close(ppDb);
}

int chakanyonghu()
{
	sqlite3 * ppDb = NULL;
	int re = sqlite3_open("publicstore.db",&ppDb);
	if(re != 0)
	{
		printf("error:%s\n",sqlite3_errmsg(ppDb));
		return -1;
	}				
	sqlite3_stmt *ppStmt = NULL;
	const char*pzTail = NULL;	
	char *sql = "select *from account;"; 	
	re = sqlite3_prepare_v2(ppDb,sql,-1,&ppStmt,&pzTail);
	if(re != 0)
	{
		perror("sqlite3_prepare_v2 error:");
		return -1;
	}	
	
	 int flag  = 1;
	//运行
	 printf("************************\n");
	while(1)
	{
		int re = sqlite3_step(ppStmt);
		if(re == SQLITE_DONE)
		{
			break;
		}
		else if(re == SQLITE_ROW)
		{
			//获取多少列
			int n = sqlite3_column_count(ppStmt);
			
			int i;
			//输出列名
			if(flag == 1)
			{	
				for(i = 0;i < n;i++)
				{
					printf("%s\t",sqlite3_column_name(ppStmt,i));
				}
				printf("\n");
				flag = 0;
			}			
			//获取每列的类型，才输出列值
			for(i = 0;i < n;i++)
			{
				//现获取类型//根据类型取获取值
				int type = sqlite3_column_type(ppStmt,i);	
				if(type == SQLITE_INTEGER)
				{
					printf("%d\t",sqlite3_column_int(ppStmt,i));
				}
				else if(type ==  SQLITE_FLOAT)
				{
					printf("%f\t",sqlite3_column_double(ppStmt,i));
				}
				else if(type ==  SQLITE_TEXT)
				{
					printf("%s\t",sqlite3_column_text(ppStmt,i));
				}
				else 
				{
					printf("\t");
				}
				
				
			}
			printf("\n");
		}
		else
		{
			perror("sqlite3_step:");
			return -1;
		}
	}
	printf("************************\n");
	
					
	// sqlite3_finalize :用来销毁一个SQL语句对象
	re = sqlite3_finalize(ppStmt);
	if(re != 0)
	{
		perror("sqlite3_finalize error:");
		return -1;
	}	
	
	//关闭数据库连接对象
	sqlite3_close(ppDb);
}

void chuku()
{
	sqlite3 * ppDb = NULL;
	int re = sqlite3_open("publicstore.db",&ppDb);
	if(re != 0)
	{
		perror("sqlite3_open error:");
		return ;
	}
	
		
	 const char*pzTail = NULL;	
	 sqlite3_stmt *ppStmt = NULL;
	
	char * sql = "update publicstore set num = (num-?2) where name == ?1;"; 

	re = sqlite3_prepare_v2(ppDb,sql,-1,&ppStmt,&pzTail);
	//re = sqlite3_step(ppStmt);
	if(re != 0)
	{
		printf("sqlite3_prepare_v2 error:%s\n",sqlite3_errmsg(ppDb));
		return ;
	}	
	
	
	while(1)
	{
		
		char name[32] = {0};
		int num;
		scanf("%s%d",name,&num);
		
		
		//获取sql参数的索引值
		int index[2];//保存参数的索引值
		index[0] =  sqlite3_bind_parameter_index(ppStmt, "?1");
		index[1] =  sqlite3_bind_parameter_index(ppStmt, "?2");
		//index[2] =  sqlite3_bind_parameter_index(ppStmt, "?3");
		
		//printf("%d %d %d \n",index[0],index[1],index[2]);
		//给参数绑定值
		//sqlite3_bind_int(ppStmt,index[0],nol);
		sqlite3_bind_text(ppStmt,index[0],name,strlen(name),NULL);
		sqlite3_bind_int(ppStmt,index[1],num);
		//sqlite3_bind_int(ppStmt,index[2],num);
		
		//printf("sql = %s\n",sql);
		//printf("%c\n",*pzTail);
		//sqlite3_step :用来执行编译好QL语句对象
		//printf("%s\n",sql);		
		re = sqlite3_step(ppStmt);
		//printf("1\n");
		if(re != SQLITE_DONE)
		{
			//printf("1\n");
			printf("sqlite3_step err%s\n",sqlite3_errmsg(ppDb));
			return ;
		}	


		//复位
		//printf("1\n");
		sqlite3_reset(ppStmt);
		char buf1[100]={0};
		//printf("1\n");
		sprintf(buf1,"%s %s %s %d",jiluzhe,"chuku",name,num);
		//printf("2\n");
		jilu(buf1);
		//printf("3\n");
		printf("chuku success! next:\
			1.exit\
			2.continue\n");
		int k;
		scanf("%d",&k);
		if(k==1)break;
	}
	
					
	// sqlite3_finalize :用来销毁一个语句对象
	re = sqlite3_finalize(ppStmt);
	if(re != 0)
	{
		perror("sqlite3_finalize error:");
		return ;
	}	
	
	//关闭数据库连接对象
	sqlite3_close(ppDb);
}

void ruku()
{
	sqlite3 * ppDb = NULL;
	int re = sqlite3_open("publicstore.db",&ppDb);
	if(re != 0)
	{
		perror("sqlite3_open error:");
		return ;
	}
	
		
	 const char*pzTail = NULL;	
	 sqlite3_stmt *ppStmt = NULL;

	char * sql = "update publicstore set num = (num+?2) where name == ?1;"; 

	re = sqlite3_prepare_v2(ppDb,sql,-1,&ppStmt,&pzTail);
	//re = sqlite3_step(ppStmt);
	if(re != 0)
	{
		printf("sqlite3_prepare_v2 error:%s\n",sqlite3_errmsg(ppDb));
		return ;
	}	
	
	
	while(1)
	{
		
		char name[32] = {0};
		int num;
		scanf("%s%d",name,&num);
		
		
		//获取sql参数的索引值
		int index[2];//保存参数的索引值
		index[0] =  sqlite3_bind_parameter_index(ppStmt, "?1");
		index[1] =  sqlite3_bind_parameter_index(ppStmt, "?2");
		//index[2] =  sqlite3_bind_parameter_index(ppStmt, "?3");
		
		//printf("%d %d %d \n",index[0],index[1],index[2]);
		//给参数绑定值
		//sqlite3_bind_int(ppStmt,index[0],nol);
		sqlite3_bind_text(ppStmt,index[0],name,strlen(name),NULL);
		sqlite3_bind_int(ppStmt,index[1],num);
		//sqlite3_bind_int(ppStmt,index[2],num);
		
		//printf("sql = %s\n",sql);
		//printf("%c\n",*pzTail);
		//sqlite3_step :用来执行编译好QL语句对象
		//printf("%s\n",sql);		
		re = sqlite3_step(ppStmt);
		if(re != SQLITE_DONE)
		{
			printf("sqlite3_step err%s\n",sqlite3_errmsg(ppDb));
			return ;
		}	


		//复位
		sqlite3_reset(ppStmt);
		char buf1[100]={0};
		sprintf(buf1,"%s %s %s %d",jiluzhe,"ruku",name,num);
		jilu(buf1);
		printf("ruku success! next:\
			1.exit\
			2.continue\n");
		int k;
		scanf("%d",&k);
		if(k==1)break;
	}
	
					
	// sqlite3_finalize :用来销毁一个语句对象
	re = sqlite3_finalize(ppStmt);
	if(re != 0)
	{
		perror("sqlite3_finalize error:");
		return ;
	}	
	
	//关闭数据库连接对象
	sqlite3_close(ppDb);
}

int xiugai()
{
	sqlite3 * ppDb = NULL;
	int re = sqlite3_open("publicstore.db",&ppDb);
	if(re != 0)
	{
		perror("sqlite3_open error:");
		return ;
	}
	
		
	 const char*pzTail = NULL;	
	 sqlite3_stmt *ppStmt = NULL;
	
	char * sql = "update publicstore set name = ?2 where name == ?1;"; 

	re = sqlite3_prepare_v2(ppDb,sql,-1,&ppStmt,&pzTail);
	//re = sqlite3_step(ppStmt);
	if(re != 0)
	{
		printf("sqlite3_prepare_v2 error:%s\n",sqlite3_errmsg(ppDb));
		return ;
	}	
	
	
	while(1)
	{
		
		char name[32] = {0};
		char nname[32] = {0};
		scanf("%s%s",name,nname);
		
		
		//获取sql参数的索引值
		int index[2];//保存参数的索引值
		index[0] =  sqlite3_bind_parameter_index(ppStmt, "?1");
		index[1] =  sqlite3_bind_parameter_index(ppStmt, "?2");
		//index[2] =  sqlite3_bind_parameter_index(ppStmt, "?3");
		
		//printf("%d %d %d \n",index[0],index[1],index[2]);
		//给参数绑定值
		//sqlite3_bind_int(ppStmt,index[0],nol);
		sqlite3_bind_text(ppStmt,index[0],name,strlen(name),NULL);
		sqlite3_bind_text(ppStmt,index[1],nname,strlen(nname),NULL);
		//sqlite3_bind_int(ppStmt,index[2],num);
		
		//printf("sql = %s\n",sql);
		//printf("%c\n",*pzTail);
		//sqlite3_step :用来执行编译好QL语句对象
		//printf("%s\n",sql);		
		re = sqlite3_step(ppStmt);
		if(re != SQLITE_DONE)
		{
			printf("sqlite3_step err%s\n",sqlite3_errmsg(ppDb));
			return ;
		}	


		//复位
		sqlite3_reset(ppStmt);
		printf("xiugai success! next:\
			1.exit\
			2.continue\n");
		int k;
		scanf("%d",&k);
		if(k==1)break;
	}
	
					
	// sqlite3_finalize :用来销毁一个语句对象
	re = sqlite3_finalize(ppStmt);
	if(re != 0)
	{
		perror("sqlite3_finalize error:");
		return ;
	}	
	
	//关闭数据库连接对象
	sqlite3_close(ppDb);
}

void zenjialei()
{
	sqlite3 * ppDb = NULL;
	int re = sqlite3_open("publicstore.db",&ppDb);
	if(re != 0)
	{
		perror("sqlite3_open error:");
		return ;
	}

	 const char*pzTail = NULL;	
	 sqlite3_stmt *ppStmt = NULL;

	char * sql = "insert into publicstore(nol,name,num) values(?1,?2,?3);"; 
	re = sqlite3_prepare_v2(ppDb,sql,-1,&ppStmt,&pzTail);
	//re = sqlite3_step(ppStmt);
	if(re != 0)
	{
		printf("sqlite3_prepare_v2 error:%s\n",sqlite3_errmsg(ppDb));
		return ;
	}	
	
	
	while(1)
	{
		int nol;
		char name[32] = {0};
		int num;
		scanf("%d%s%d",&nol,name,&num);
		if(nol == 0)
		{
			break;
		}
		
		//获取sql参数的索引值
		int index[3];//保存参数的索引值
		index[0] =  sqlite3_bind_parameter_index(ppStmt, "?1");
		index[1] =  sqlite3_bind_parameter_index(ppStmt, "?2");
		index[2] =  sqlite3_bind_parameter_index(ppStmt, "?3");
		
		//printf("%d %d %d \n",index[0],index[1],index[2]);
		//给参数绑定值
		sqlite3_bind_int(ppStmt,index[0],nol);
		sqlite3_bind_text(ppStmt,index[1],name,strlen(name),NULL);
		sqlite3_bind_int(ppStmt,index[2],num);
		
		//printf("sql = %s\n",sql);
		//printf("%c\n",*pzTail);
		//sqlite3_step :用来执行编译好QL语句对象
				
		re = sqlite3_step(ppStmt);
		if(re != SQLITE_DONE)
		{
			printf("sqlite3_step err%s\n",sqlite3_errmsg(ppDb));
			return ;
		}	


		//复位
		sqlite3_reset(ppStmt);
		printf("zenjialei success! next:\
			1.exit\
			2.continue\n");
		int k;
		scanf("%d",&k);
		if(k==1)break;
	}
	
					
	// sqlite3_finalize :用来销毁一个语句对象
	re = sqlite3_finalize(ppStmt);
	if(re != 0)
	{
		perror("sqlite3_finalize error:");
		return ;
	}	
	
	//关闭数据库连接对象
	sqlite3_close(ppDb);
}

void zenjiaaccount()
{
	sqlite3 * ppDb = NULL;
	int re = sqlite3_open("publicstore.db",&ppDb);

	const char*pzTail = NULL;	
	sqlite3_stmt *ppStmt = NULL;

	char * sql = "insert into account(account,password) values(?1,?2);"; 
	re = sqlite3_prepare_v2(ppDb,sql,-1,&ppStmt,&pzTail);

	
	
	while(1)
	{
		int nol;
		char account[32] = {0};
		char password[32]={0};
		scanf("%s%s",account,password);
		if(nol == 0)
		{
			break;
		}
		
		//获取sql参数的索引值
		int index[2];//保存参数的索引值
		index[0] =  sqlite3_bind_parameter_index(ppStmt, "?1");
		index[1] =  sqlite3_bind_parameter_index(ppStmt, "?2");
		//index[2] =  sqlite3_bind_parameter_index(ppStmt, "?3");
		
		//printf("%d %d %d \n",index[0],index[1],index[2]);
		//给参数绑定值
		sqlite3_bind_text(ppStmt,index[0],account,strlen(account),NULL);
		sqlite3_bind_text(ppStmt,index[1],password,strlen(password),NULL);
		//sqlite3_bind_int(ppStmt,index[2],num);
		
		//printf("sql = %s\n",sql);
		//printf("%c\n",*pzTail);
		//sqlite3_step :用来执行编译好QL语句对象
				
		re = sqlite3_step(ppStmt);
		if(re != SQLITE_DONE)
		{
			printf("sqlite3_step err%s\n",sqlite3_errmsg(ppDb));
			return ;
		}	


		//复位
		sqlite3_reset(ppStmt);
		printf("zenjiaaccount success! next:\
			1.exit\
			2.continue\n");
		int k;
		scanf("%d",&k);
		if(k==1)break;
	}
	
					
	// sqlite3_finalize :用来销毁一个语句对象
	re = sqlite3_finalize(ppStmt);
	if(re != 0)
	{
		perror("sqlite3_finalize error:");
		return ;
	}	
	
	//关闭数据库连接对象
	sqlite3_close(ppDb);
}

void denglu()
{
	sqlite3 * ppDb = NULL;
	int re = sqlite3_open("publicstore.db",&ppDb);

	const char*pzTail = NULL;	
	sqlite3_stmt *ppStmt = NULL;

	char * sql = "select *from account;"; 
	re = sqlite3_prepare_v2(ppDb,sql,-1,&ppStmt,&pzTail);
	printf("please input account:\n");
	
	
		char buf1[32]={0};
		start:gets(buf1);
		if(strcmp(buf1,"admin")!=0)
		{
			while(1)
			{
				
				int re = sqlite3_step(ppStmt);
				if(re == SQLITE_DONE)
				{
					printf("bucunzai,input again\n");
					goto start;

				}
				else if(re == SQLITE_ROW)
				{
					char acc[32]={0};
					//printf("%s\n",sqlite3_column_text(ppStmt,0));
					strcpy(acc,sqlite3_column_text(ppStmt,0));
					if(strcmp(buf1,acc)==0)
					{
						printf("please input password:\n");
						bzero(jiluzhe,32);
						strcpy(jiluzhe,acc);
						break;
					}	
				}
				
			}
			char buf2[32]={0};
			pssget:gets(buf2);
			while(1)
			{
				
				int re = sqlite3_step(ppStmt);
				if(re == SQLITE_DONE)
				{
					printf("password error,input again\n");
					goto pssget;

				}
				else if(re == SQLITE_ROW)
				{
					char pss[32]={0};
					strcpy(pss,sqlite3_column_text(ppStmt,1));
					if(strcmp(buf2,pss)==0)
					{
						break;
					}	
				}
				
			}
		}
		else
		{
			printf("please input admin password:\n");
			char buf3[32]={0};
			get:gets(buf3);
			if(strcmp(buf3,"123456")==0)
			{
				while(1)
				{
					printf("please input admin cmd:\n");
					printf("1.chakan yonghu\n");
					printf("2.zenjia account\n");
					printf("3.jingru publicstore\n");
					printf("4.chakan jilu\n");
					int cmd;
					scanf("%d",&cmd);
					if(cmd==1)
					{
						chakanyonghu();
					}
					if(cmd==2)
					{
						zenjiaaccount();
						
					}
					if(cmd==3)
					{

						bzero(jiluzhe,32);
						strcpy(jiluzhe,"admin");
						break;
					}
					if(cmd==4)
					{
						chakanjilu();
					}
				}
				

			}
			else
			{
				printf("erro,again:\n");
				goto get;
			}

		}
	re = sqlite3_finalize(ppStmt);
	if(re != 0)
	{
		perror("sqlite3_finalize error:");
		return ;
	}	
	
	//关闭数据库连接对象
	sqlite3_close(ppDb);

}

int main()
{
	denglu();
	while(1)
	{
	int cmd;
	printf("please input cmd:\n");
	printf("1.chakan;\n");
	printf("2.ruku;\n");
	printf("3.chuku;\n");
	printf("4.zengjialei;\n");
	printf("5.xiugai\n");
	printf("6.exit;\n");
	//printf("3.creat putong account;\n");
	scanf("%d",&cmd);
	if(cmd==1)
	{
	chakan();
	}
	if(cmd==2)
	{
	ruku();
	}
	if(cmd==3)
	{
	chuku();
	}
	if(cmd==4)
	{
	zenjialei();
	}
	if(cmd==5)
	{
	xiugai();
	}
	if(cmd==6)break;
	}						
}