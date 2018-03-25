#include <stdio.h>
#include <mysql.h>


 MYSQL_ROW mysql_row;  
 char execsql[256];  
 int f1,f2,num_row,num_col;  
 MYSQL_RES *mysql_result;  
int main(int argc,char *argv[])
{
	MYSQL conn;
	int res;
	mysql_init(&conn);
	if(mysql_real_connect(&conn,"localhost","root","106699","test",0,NULL,CLIENT_FOUND_ROWS)) //"root":数据库管理员 "":root密码 "test":数据库的名
	{
		printf("connect success!\n");
		res=mysql_query(&conn,"select * from test");
		if(res)
		{
			printf("error\n");
		}
		else
		{
			printf("OK\n");
		}

		mysql_result=mysql_store_result(&conn);  
	    num_row=mysql_num_rows(mysql_result);  
	    num_col=mysql_num_fields(mysql_result); 
		 for(f1=0;f1<num_row;f1++)
		 {
			 mysql_row=mysql_fetch_row(mysql_result);
			 for(f2=0;f2<num_col;f2++)
			 {
				printf("[Row %d,Col %d]==>[%s]\n",f1,f2,mysql_row[f2]);  
			 }
		 }
		 mysql_free_result(mysql_result);  
		mysql_close(&conn);
	}
	return 0;
}
