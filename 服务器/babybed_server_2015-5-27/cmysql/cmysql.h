#ifndef _CMYSQL_H_
#define _CMYSQL_H_

#include <mysql/mysql.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef  MYSQL_ROW SQLResultRow;

typedef struct _SQLResult
{
	MYSQL_RES *data;
	SQLResultRow row;
	int record;
	int field_count;
}SQLResult;

void Cmysql_init(char *_server, char *_use,char *_password, char *_database);
void Cmysql_release();
MYSQL* connectMysql();
int Cmysql_query(const char *_query);
int Cmysql_result(SQLResult *result);
const char *sql_error();
void Cmysql_free_result(SQLResult *result);
SQLResultRow Cmysql_result_next(SQLResult *result);
int Cmysql_row_to_int(char *obj);


#endif