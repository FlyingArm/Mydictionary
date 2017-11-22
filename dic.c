#include <dic.h>

void exit_pro(void)
{
	unsigned char ret;
	ret=database_end();
	if(!ret)
		printf("close database and exit succeed! \n\n");
	exit(EXIT_SUCCESS);
}

unsigned int key()
{
	unsigned int ret;
	char c;
	c=getchar();
	while( c < 48 || c > 52 )
	{
		printf("please input a num again\033[40;32m(0-4)\033[0m:");
		c=getchar();
	}
	return c-48;
}

int main(void )
{
	unsigned char haveexist=0;
	unsigned int  switchnum=0 ;
	unsigned int ret =0 ;
	ret=database_start("root","xf");
	if(ret)
	{
		printf("database_start failure \n");
	}
	if(haveexist==1)
	{
		ret=init_database();
		if(ret)
		{
			printf("init_database success all have %d words\n",ret);	
		}
		else
		{
			printf("have some error please to check it! \n");
		}
	}
	else
	{
		printf("the data had insert it!\n");	
	}
	menu();
	switchnum=key();
	while(1)
	{
		switch(switchnum)
		{
			case 0:exit_pro();break;
			case 1:ret=search_dic();break;
   			case 2:ret=add_dic();break;
			case 3:ret=del_dic();break;
			case 4:ret=update_dic();break;
			default : break;
		}
		if(ret == -1)
		{
			printf("\033[40;31mthere have some error please to check it !\033[0m\n");
			printf("please input a num to switch(0-4):");
	//		system("clear");
			menu();
			switchnum=key();
		}
		if(ret == -2)
		{
			printf("the argv is error please check it!\n");
			switchnum=key();
		}
		if(ret == 3)
		{
			system("clear");
			printf("\n\t\033[40;32mend last opertion and go a new selection !\033[0m");
			menu();
			switchnum=key();
		}
	}  
	return 0;
}











