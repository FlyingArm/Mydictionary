#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mysql.h"

char wordbuf[100]={0};
char  tmpbuf[100]={0};
char wordexp[400]={0};

static MYSQL 		*con_ptr;
static MYSQL_RES	*res_ptr;
static MYSQL_ROW	sqlrow;
static int 			not_connect=0 ;
static int 			ret=0 ;

char writebuf[500]={0};

int  database_start(char *name,char *pwd);
int  add_dict(char *wordname,char *wordexp);
int  database_end(void );
void display_row(void );
int  init_database();

int  search(char *wordname);
int  add_dic();
int  search_dic();
int  del_dic();
int  update_dic();
void menu(void );


void menu(void )
{
	printf("\n");
	printf("\t---------------------------------------------------------\n");
	printf("\t|\033[40;33m 0\033[0m.exit the program .                                  |\n");
	printf("\t|\033[40;33m 1\033[0m.search word .                                       |\n");
	printf("\t|\033[40;33m 2\033[0m.add word .                                          |\n");
	printf("\t|\033[40;33m 3\033[0m.delete word .                                       |\n");
	printf("\t|\033[40;33m 4\033[0m.update word .                                       |\n");
	printf("\t---------------------------------------------------------\n");
	printf("\tplease input your selection\033[40;32m(0-4)\033[0m:");
}
/*	database start */
int database_start(char *name,char *pwd)
{
	if(not_connect)
		return 1;
	con_ptr=mysql_init(NULL);
	if(! con_ptr)
	{
		fprintf(stderr,"mysql_init failed..\n");
		return EXIT_FAILURE;
	}
	con_ptr=mysql_real_connect(con_ptr,"localhost",name,pwd,"xf",0,NULL,0);
	if(!con_ptr)
	{
		fprintf(stderr,"\033[40;31mDatabase connection failure:%d,%s\033[0m\n",mysql_errno(con_ptr),mysql_error(con_ptr) );
		return -1;
	}
	else
	{
		not_connect=1;
		printf("\033[40;32mDatabase connection success !\033[0m \n");
		return 0;
	}
}
/*  end database */
int  database_end(void)
{
	if(not_connect )
		mysql_close(con_ptr);
	not_connect = 0 ;
	return 0;
}

/*  if can't open file return -1 or return you insert num */
int init_database()
{
	unsigned int i=0;
	FILE *fp;
	char wordname[100]={0};
	char wordexp[400]={0};
	int  buflen;
	fp=fopen("./dict.txt","r");
	if(fp == NULL)
	{
		fprintf(stderr,"\033[40;31mopen file dict.txt failure!\033[0m\n");
		return -1;
	}
	while(! feof(fp))
	{
		memset(wordname,0,sizeof(wordname) );
		fgets(wordname,sizeof(wordname),fp);
		buflen=strlen(wordname);
		wordname[buflen-1]='\0';

		memset(wordexp, 0,sizeof(wordexp) );
		fgets(wordexp,sizeof(wordexp),fp );
		buflen=strlen(wordexp);
		wordexp[buflen-1]='\0';

		add_dict(wordname,wordexp);
		i++;
	}
	return i;
}

/* add a word to table */
int add_dict(char *wordname,char *wordexp)
{
	MYSQL_RES	*res_ptr;
	MYSQL_ROW	mysqlrow;
	int 		ret;
	memset(writebuf,0,sizeof(writebuf) );
	sprintf(writebuf,"INSERT INTO word(wordname,wordexp) VALUES(\"%s\",\"%s\")",wordname,wordexp);
	ret=mysql_query(con_ptr,writebuf);
	if(ret)
	{
		fprintf(stderr,"\033[40;31mInsert error:%d,%s\033[0m\n",mysql_errno(con_ptr),mysql_error(con_ptr) );
		return -1;
	}
	return 0;
}

int search(char *wordname)
{
	unsigned char ret ;
	static char mybuf[100]={0} ;
	memset(mybuf,0,sizeof(mybuf) );
	sprintf(mybuf,"SELECT wordno,wordname,wordexp FROM word WHERE wordname=\"#%s\"",wordname);
	ret=mysql_query(con_ptr,mybuf);
	return ret;
}

/* if successed return ret */
int search_dic()
{
	unsigned char ret ;
	if(!not_connect)
		return -2 ;
	printf("please input you want to search words (enter \033[40;32m'searchend'\033[0m to be end !):");
	memset(tmpbuf,0,sizeof(tmpbuf) );
	scanf("%s",tmpbuf);
	if( (strncmp(tmpbuf,"searchend",9) == 0 ))
		return 3;
	ret=search(tmpbuf);
	if(!ret)
	{
		res_ptr=mysql_use_result(con_ptr);
		if(res_ptr)
		{
			while( (sqlrow=mysql_fetch_row(res_ptr)) )
			{
				display_row();	
			}
			if( mysql_errno(con_ptr) )
				fprintf(stderr,"\033[40;31mRetrieve error:%s\033[0m\n",mysql_error(con_ptr) );
			mysql_free_result(res_ptr);
		}
	}
	else 
		printf("\033[40;31mmysql_query() returned a error !\033[0m\n");
	return ret;
}

void display_row()
{
	unsigned int field_count=0;
	while(field_count< mysql_field_count(con_ptr) )
	{
		printf("%s\t",sqlrow[field_count]);
		field_count ++ ;
	}
	printf("\n");
}

int add_dic()
{
	unsigned char ret ;
	if(!not_connect)
		return -2;
	printf("please input you want to add words (enter \033[40;32m'addend'\033[0m to be end !):");
	memset(tmpbuf,0,sizeof(tmpbuf) );
	memset(wordbuf,0,sizeof(wordbuf) );
	wordbuf[0]='#';
	scanf("%s",tmpbuf );
	if( (strncmp(tmpbuf,"addend",6)) == 0)
		return 3;
	strcat(wordbuf,tmpbuf);
	printf("please write it mean:");
	memset(wordexp,0,sizeof(wordexp) );
	scanf("%s",wordexp);
	ret=add_dict(wordbuf,wordexp);
	if(ret==0)
		printf("\033[40;32myou have succeed insert a word !\033[0m\n");
	return ret;  
}

int del_dic()
{
	unsigned char ret ;
	if(!not_connect)
		return -2;
	printf("please input you want to delete words (enter \033[40;32m'delend'\033[0m to be end !):");
	memset(tmpbuf,0,sizeof(tmpbuf) );
	memset(wordbuf,0,sizeof(wordbuf) );
	scanf("%s",tmpbuf);
	if( (strncmp(tmpbuf,"delend",6)) ==0)
		return 3;
	sprintf(wordbuf,"DELETE FROM word WHERE wordname=\"#%s\"",tmpbuf);
	ret=mysql_query(con_ptr,wordbuf);
	if(ret)
	{
		fprintf(stderr,"\033[40;31mDelete word have some error:%d,%s\033[0m\n",mysql_errno(con_ptr),mysql_error(con_ptr) );
		ret=-1;
	}
	return ret;
}

int update_dic()
{
	unsigned char ret;
	if(!not_connect)
		return -2;
	printf("please input you want to update words (enter \033[40;32m'updateend'\033[0m to be end !):");
	memset(tmpbuf,0,sizeof(tmpbuf) );
	memset(wordbuf,0,sizeof(wordbuf) );
	scanf("%s",tmpbuf);
	if( (strncmp(tmpbuf,"updateend",9)) == 0)
		return 3;
	sprintf(wordbuf,"DELETE FROM word WHERE wordname=\"#%s\"",tmpbuf);
	ret=mysql_query(con_ptr,wordbuf);
	if(ret==0)
	{
		printf("please input the word's mean:");
		memset(wordexp,0,sizeof(wordexp) );
		scanf("%s",wordexp);
		memset(wordbuf,0,sizeof(wordbuf) );
		wordbuf[0]='#';
		strcat(wordbuf,tmpbuf);
		ret=add_dict(wordbuf,wordexp);
		if(ret==0)
			printf("update %s success !\n",tmpbuf);
	}
	return ret ;
}


