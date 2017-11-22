dic:dic.c app_mysql.c
	gcc -I/usr/include/mysql -I./  dic.c -L/usr/lib/mysql -lmysqlclient -c -o dic.o 
	gcc -I/usr/include/mysql -I./  app_mysql.c -L/usr/lib/mysql -lmysqlclient -c -o  app_mysql.o
	gcc -I/usr/include/mysql -I./  dic.o app_mysql.o  -o dic -L/usr/lib/mysql -lmysqlclient 

clean :
	rm -f *.o dic
