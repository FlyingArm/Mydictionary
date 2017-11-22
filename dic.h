#ifndef		_DIC_H_
#define 	_DIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menu(void );
int  databse_start(char *name,char *pwd);
void databse_end(void);
int  add_dict(char *wordname,char *wordexp);
void display_row(void);

int search_dic();
int add_dic();
int del_dic();
int update_dic();

#endif


