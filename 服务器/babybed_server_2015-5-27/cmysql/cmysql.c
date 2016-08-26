#include "cmysql.h"
#include "mutex_lock.h"

static char *server;
static char *user;
static char *password;
static char *database;
static MYSQL *conn;
static MYSQL_RES *res;
static MYSQL_ROW row;

void Cmysql_init(char *_server, char *_use,char *_password, char *_database)
{
	server = (char *)malloc(strlen(_server)+1);
	user = (char *)malloc(strlen(_use)+1);
	password = (char *)malloc(strlen(_password)+1);
	database = (char *)malloc(strlen(_database)+1);
	strcpy(server, _server);
	strcpy(user, _use);
	strcpy(password, _password);
	strcpy(database, _database);
	mysql_lock();
	conn = mysql_init(NULL);
}

void Cmysql_release()
{
	free(server);
	free(user);
	free(password);
	free(database);
    mysql_close(conn);
	mysql_unlock();
}

MYSQL* connectMysql()
{
	 return mysql_real_connect(conn, server, user, password, database, 0, NULL, 0);
}

int Cmysql_query(const char *_query)
{
	return mysql_query(conn, _query);
}

SQLResultRow Cmysql_result_next(SQLResult *result)
{
	return (SQLResultRow)mysql_fetch_row(res);
}
int Cmysql_result(SQLResult *result)
{
	res = mysql_use_result(conn);
	result->record = 0;
	int i = 0;
	result->record = res->row_count;
	result->field_count = res->field_count;
	result->data = res;
	
	return 0;
}


void Cmysql_free_result(SQLResult *result)
{
	mysql_free_result(result->data);
}

const char * sql_error()
{
	return mysql_error(conn);
}

int Cmysql_row_to_int(char *obj)
{
	int result=0;
	int len = strlen(obj);
	int i;
	for(i = 0; i<len; i++)
	{
		if(i)
			result *= 10;	
		result += (obj[i]-0x30);
		
	}

	return result;
}
