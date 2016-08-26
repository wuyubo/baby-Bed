#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include "cmysql.h"


int main() 
{
	char server[] = "localhost";
    char user[] = "root";
    char password[] = "123456";
    char database[] = "Baby_Bed";
	int i;
	SQLResult result;
	char *data[100];
	result.data = data;
	result.record = 0;

	Cmysql_init(server, user, password, database);
	 if (!connectMysql())
    {
        fprintf(stderr, "%s\n", sql_error());
        exit(1);
    }
	
	if (Cmysql_query("show tables")) 
    {
        fprintf(stderr, "%s\n", sql_error());
        exit(1);
    }
    Cmysql_result(&result);
	for(i = 0; i<result.record; i++)
    {
        printf("%s \n", result.data[i]);
    }
	
	
    return 0;
}