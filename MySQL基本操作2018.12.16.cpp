#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "D:\MySQL\include\mysql.h" //����MySQL����Ҫ��ͷ�ļ�
#include <iostream>
#include <typeinfo>
using namespace std;
#pragma comment(lib, "libmysql.lib")

void function0(MYSQL mysql);//��ѯ���ݿ��ȫ����Ϣ
void function1(MYSQL mysql);//��ѯ���ݿ�ĵ�һ��
void function2(MYSQL mysql);//һЩAPI�����Ĳ���
void function3(MYSQL mysql);//��ѯָ����Ϣ���û���Ϣ
void function4(MYSQL mysql);//����ɾ�����ݿ��еļ�¼
void function5();//�����ݿ��в����¼

int main(void)
{
	MYSQL mysql; //����MySQL�ľ��
	const char * host = "127.0.0.1";//��Ϊ����Ϊ�������ԣ�������д���Ǳ���IP
	const char * user = "root";//�����Ϊ����û�����������MySQL���û���
	const char * passwd = "891256";//�����Ϊ����û�����
	const char * db = "mydatabase";//�����Ϊ��Ҫ���ӵ����ݿ������,һ�����ݿ����м��ű�
	unsigned int port = 3306;//����MySQL�ķ������Ķ˿ڣ������û���޸Ĺ��Ļ�����3306��
	const char * unix_socket = NULL;//unix_socket����unix�µģ�����Windows�£����ԾͰ�������ΪNULL
	unsigned long client_flag = 0;//�������һ��Ϊ0

	mysql_init(&mysql);//����֮ǰ����ʹ�������������ʼ��

	MYSQL* sock = mysql_real_connect(&mysql, host, user, passwd, db, port, unix_socket, client_flag);
	if (sock == NULL) //����MySQL 
	{
		printf("Failed to connect to Mysql!\n");
		fprintf(stderr, " %s\n", mysql_error(&mysql));
		exit(1);
	}
	else
	{
		fprintf(stderr, "Connected to Mysql successfully!\n");
	}

	//function0(mysql);//��ѯ���ݿ��ȫ����Ϣ
	function1(mysql);//��ѯ���ݿ�ĵ�һ��
	//function2(mysql);//һЩAPI�����Ĳ���
	function3(mysql);//��ѯָ����Ϣ���û���Ϣ
	//function4(mysql);//����ɾ�����ݿ�
	//function5(mysql);//�����ݿ��в����¼

	mysql_close(sock);//�ر�����
	return 0;
}

void function0(MYSQL mysql)//��ѯ���ݿ��ȫ����Ϣ
{


	MYSQL_RES * result;//����������

	if (mysql_set_character_set(&mysql, "gbk")) {	//���ַ������Ϊgbk
		fprintf(stderr, "����, %s\n", mysql_error(&mysql));
	}

	const char * i_query = "select * from info";//��ѯ��䣬���Ǹ����в�ѯ,�������û��;
	if (mysql_query(&mysql, i_query) != 0)//������ӳɹ�����ʼ��ѯ .�ɹ�����0
	{
		fprintf(stderr, "fail to query!\n");
		exit(1);
	}
	else
	{
		if ((result = mysql_store_result(&mysql)) == NULL) //�����ѯ�Ľ��
		{
			fprintf(stderr, "fail to store result!\n");
			exit(1);
		}
		else
		{
			MYSQL_ROW row;//������ǽ�����е�һ�� 
						  //my_ulonglong row;
			int n = 1;
			while ((row = mysql_fetch_row(result)) != NULL)
				//��ȡ������е����ݣ����ص�����һ�С���Ϊ��������ʱ����ǰ���α��ڵ�һ�С�֮ǰ�� 
			{
				cout << "��ţ�" << n++ << "  ";
				//cout << mysql_fetch_field(result) << endl;
				printf("������ %s\t", row[0]);//��ӡ��ǰ�еĵ�һ�е�����
										   //cout <<"name is " << (char *)row[0] << "\t";
										   //cout << typeid(row[0]).name() << endl;
				printf("�Ա� %s\t", row[1]);//��ӡ��ǰ�еĵڶ��е�����
										   //row = mysql_num_row(result);
										   //printf("%lu\n", mysql_num_row(result));
				printf("׼��֤�ţ� %s\t", row[2]);//��ӡ��ǰ�еĵ�һ�е�����
				printf("֤����ţ� %s\t", row[3]);//��ӡ��ǰ�еĵڶ��е�����
											 //cout << typeid(row[4]).name() << endl;
				printf("���壺 %s\t\n", row[4]);//��ӡ��ǰ�еĵ�һ�е����� 
											 /*cout << row[0] << "\t";
											 cout << row[1] << "\t";*/
				cout << endl;
			}
		}
	}
	mysql_free_result(result);//�ͷŽ����result
}

void function1(MYSQL mysql)//��ѯ���ݿ�ĵ�һ��
{
	MYSQL_RES * result;//����������
	if (mysql_set_character_set(&mysql, "gbk")) {	//���ַ������Ϊgbk
		fprintf(stderr, "����, %s\n", mysql_error(&mysql));
	}

	const char * i_query0 = "select ���� from info";//��ѯ��䣬���Ǹ����в�ѯ,�������û��;

	if (mysql_query(&mysql, i_query0) != 0)//������ӳɹ�����ʼ��ѯ .�ɹ�����0
	{
		fprintf(stderr, "fail to query!\n");
		exit(1);
	}
	else
	{
		if ((result = mysql_store_result(&mysql)) == NULL) //�����ѯ�Ľ��
		{
			fprintf(stderr, "fail to store result!\n");
			exit(1);
		}
		else
		{
			MYSQL_ROW row;//������ǽ�����е�һ��
			while ((row = mysql_fetch_row(result)) != NULL)
				//��ȡ������е����ݣ����ص�����һ�С���Ϊ��������ʱ����ǰ���α��ڵ�һ�С�֮ǰ�� 
			{
				printf("������ %s\n", row[0]);//��ӡ��ǰ�еĵ�һ�е�����
			}
		}
	}
	mysql_free_result(result);//�ͷŽ����result
}

void function2(MYSQL mysql)//һЩAPI�����Ĳ���
{
	MYSQL_RES * result;//����������
	if (mysql_set_character_set(&mysql, "gbk")) {	//���ַ������Ϊgbk
		fprintf(stderr, "����, %s\n", mysql_error(&mysql));
	}

	const char * i_query = "select * from info";//��ѯ��䣬���Ǹ����в�ѯ,�������û��;
	if (mysql_query(&mysql, i_query) != 0)//������ӳɹ�����ʼ��ѯ .�ɹ�����0
	{
		fprintf(stderr, "fail to query!\n");
		exit(1);
	}
	else
	{
		if ((result = mysql_store_result(&mysql)) == NULL) //�����ѯ�Ľ��
		{
			fprintf(stderr, "fail to store result!\n");
			exit(1);
		}
		else
		{
			int num = mysql_num_fields(result);//���������е�����
			int row_num = mysql_num_rows(result);//���ؽ�����е��е���Ŀ
			cout << "/******************/" << endl;
			cout << "������е�����Ϊ��" << num << endl;
			cout << "������е�����Ϊ��" << row_num << endl;
			/*
			typedef struct st_mysql_res
			{
			my_ulonglong row_count;	                     // �����������
			unsigned int	field_count, current_field;	     // ���������������ǰ��
			MYSQL_FIELD	*fields;	                     // �����������Ϣ
			MYSQL_DATA	*data;	                         // �����������
			MYSQL_ROWS	*data_cursor;	                 // ������Ĺ��
			MEM_ROOT	field_alloc;                         // �ڴ�ṹ
			MYSQL_ROW	row;	                             // �ǻ����ʱ���õ�
			MYSQL_ROW	current_row;                         // mysql_store_resultʱ���õ�����ǰ��
			unsigned long *lengths;	                     // ÿ�еĳ���
			MYSQL	*handle;	                             // mysql_use_result���á�
			my_bool	eof;	                             // �Ƿ�Ϊ��β
			} MYSQL_RES;
			*/
			cout << "/*******************/" << endl;
			cout << "�ṹ��ָ��ķ���" << endl;
			cout << "������е�������" << result->field_count << endl;
			cout << "������е�������" << result->row_count << endl;
			cout << "��ǰ������" << result->current_field << endl;

			cout << "/*******************/" << endl;
			cout << "�����ϴ�UPDATE��DELETE��INSERT��ѯ���ģ�ɾ�������������" << endl;
			cout << mysql_affected_rows(&mysql) << endl;
			cout << "������һ�����ֶε�����" << endl;
			cout << mysql_fetch_field(result) << endl;
			cout << "����ȫ���ֶνṹ�����顣" << endl;
			cout << mysql_fetch_fields(result) << endl;
			cout << "���ַ�����ʽ����client�汾����Ϣ��" << endl;
			cout << mysql_get_client_info() << endl;
			cout << "��������ʽ����client�汾����Ϣ��" << endl;
			cout << mysql_get_client_version() << endl;
			cout << "������д�������ӵ��ַ�����" << endl;
			cout << mysql_get_host_info(&mysql) << endl;
			cout << "��ȡ���ʼ��MYSQL�ṹ��" << endl;
			cout << mysql_init(&mysql) << endl;
			cout << "�����server�������Ƿ��������б�Ҫ��һ�����ӡ�" << endl;
			//����1��ʾserver��������������
			cout << mysql_ping(&mysql) << endl;
			cout << "ѡ�����ݿ⡣" << endl;
			cout << mysql_select_db(&mysql, "test0") << endl;
			cout << "���ַ�����ʽ����server״̬��" << endl;
			cout << mysql_stat(&mysql) << endl;
			cout << "���ص�ǰ�߳�ID��" << endl;
			cout << mysql_thread_id(&mysql) << endl;
			cout << "��ʼ��MySQL��" << endl;
			mysql_library_init;
			cout << "����MySQL���ʹ�á�" << endl;
			mysql_library_end();
			cout << "����ÿһ����SELECT��ѯ������INSERT��UPDATE��DELETE����ͨ������mysql_affected_rows()���ɷ����ж������ѱ��ı䣨Ӱ�죩��" << endl;
			cout << mysql_affected_rows(&mysql) << endl;
}
	}
	mysql_free_result(result);//�ͷŽ����result
}

void function3(MYSQL mysql)//��ѯָ����Ϣ���û���Ϣ
{
	MYSQL_RES * result;//����������

	if (mysql_set_character_set(&mysql, "gbk")) {	//���ַ������Ϊgbk
		fprintf(stderr, "����, %s\n", mysql_error(&mysql));
	}

	/*cout << "�������û�����";
	char *name ;
	cin >> *name;*/

	//const char * i_query = "select * from info where �Ա�='Ů'";//��ѯ��䣬���Ǹ����в�ѯ,�������û��;
	while (1) {
		string query = "select * from info where ����='";
		cout << "�������û�����";
		string name;
		cin >> name;
		query = query + name;
		string temp = "'";
		query = query + temp;
		cout << query << endl;
		const char * i_query = query.c_str();
		if (mysql_query(&mysql, i_query) != 0)//������ӳɹ�����ʼ��ѯ .�ɹ�����0
		{
			fprintf(stderr, "fail to query!\n");
			//��ӡ����ԭ��
			fprintf(stderr, " %s\n", mysql_error(&mysql));
			exit(1);
		}
		else
		{
			if ((result = mysql_store_result(&mysql)) == NULL) //�����ѯ�Ľ��
			{
				fprintf(stderr, "fail to store result!\n");
				exit(1);
			}
			else
			{
				MYSQL_ROW row;//������ǽ�����е�һ�� 
							  //my_ulonglong row;
				int n = 1;
				while ((row = mysql_fetch_row(result)) != NULL)
					//��ȡ������е����ݣ����ص�����һ�С���Ϊ��������ʱ����ǰ���α��ڵ�һ�С�֮ǰ�� 
				{
					cout << "��ţ�" << n++ << "  ";
					//cout << mysql_fetch_field(result) << endl;
					printf("������ %s\t", row[0]);//��ӡ��ǰ�еĵ�һ�е�����
											   //cout <<"name is " << (char *)row[0] << "\t";
											   //cout << typeid(row[0]).name() << endl;
					printf("�Ա� %s\t", row[1]);//��ӡ��ǰ�еĵڶ��е�����
											   //row = mysql_num_row(result);
											   //printf("%lu\n", mysql_num_row(result));
					printf("׼��֤�ţ� %s\t", row[2]);//��ӡ��ǰ�еĵ�һ�е�����
					printf("֤����ţ� %s\t", row[3]);//��ӡ��ǰ�еĵڶ��е�����
												 //cout << typeid(row[4]).name() << endl;
					printf("���壺 %s\t\n", row[4]);//��ӡ��ǰ�еĵ�һ�е����� 
												 /*cout << row[0] << "\t";
												 cout << row[1] << "\t";*/
					cout << endl;
				}
			}
		}
	}
	mysql_free_result(result);//�ͷŽ����result
}

void function4(MYSQL mysql)//ɾ�����ݿ��еļ�¼
{
	const char * query = "delete from info where ����='����ԣ'";
	/*ɾ�����ɹ��򷵻�0*/
	int flag = mysql_real_query(&mysql, query, (unsigned int)
		strlen(query));
	if (flag) {
		printf("Delete data failure!\n");
	}
	else {
		printf("Delete data success!\n");
	}
	mysql_close(&mysql);
}

void function5()//�����ݿ��в����¼
{
	MYSQL mysql; //����MySQL�ľ��
	const char * host = "127.0.0.1";//��Ϊ����Ϊ�������ԣ�������д���Ǳ���IP
	const char * user = "root";//�����Ϊ����û�����������MySQL���û���
	const char * passwd = "891256";//�����Ϊ����û�����
	const char * db = "mydatabase";//�����Ϊ��Ҫ���ӵ����ݿ������,һ�����ݿ����м��ű�
	unsigned int port = 3306;//����MySQL�ķ������Ķ˿ڣ������û���޸Ĺ��Ļ�����3306��
	const char * unix_socket = NULL;//unix_socket����unix�µģ�����Windows�£����ԾͰ�������ΪNULL
	unsigned long client_flag = 0;//�������һ��Ϊ0

	mysql_init(&mysql);//����֮ǰ����ʹ�������������ʼ��

	MYSQL* sock = mysql_real_connect(&mysql, host, user, passwd, db, port, unix_socket, client_flag);
	if (sock == NULL) //����MySQL 
	{
		printf("fail to connect mysql \n");
		fprintf(stderr, " %s\n", mysql_error(&mysql));
		exit(1);
	}
	else
	{
		fprintf(stderr, "connect ok!\n");
	}

	if (mysql_set_character_set(&mysql, "gbk")) {	//���ַ������Ϊgbk
		fprintf(stderr, "����, %s\n", mysql_error(&mysql));
	}

	const char * query = "insert into info(����,֤�����) values('������',101)";
	/*���룬�ɹ��򷵻�0*/
	int flag = mysql_real_query(&mysql, query, (unsigned int)
		strlen(query));
	if (flag) {
		printf("Insert data failure!\n");
		//��ӡ����ԭ��
		fprintf(stderr, " %s\n", mysql_error(&mysql));
	}
	else {
		printf("Insert data success!\n");
	}
	mysql_close(&mysql);
}