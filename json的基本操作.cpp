#include <iostream>
#include <json/json.h>
using namespace std;
using namespace Json;

int main()
{
	//创建json包
#if 0
	Json::Value val;
	val["name"] = "zhangsan";
	val["pw"] = "123456";
#endif
	//枚举出所有的操作
	enum _TYPE
	{
    		TYPE_LOGIN,	//登陆			0
    		TYPE_REG,	//注册			1
    		TYPE_ONE,	//一对一聊天		2
    		TYPE_GROUD,	//群聊			3
    		TYPE_LIST,	//列出所有在线的用户	4
    		TYPE_GO 	//下线			5
	}TYPE;

	char name[20]="";
    	cout<<"please cin name:";
    	cin>>name;

    	char pw[20]="";
    	cout<<"please cin passwd:";
    	cin>>pw;

    	TYPE=TYPE_LOGIN;
    	Json::Value val;
    	val["type"]=TYPE;
    	val["name"]=name;
    	val["passwd"]=pw;

	cout<<val.toStyledString().c_str()<<endl;

#if 0
	Json::Reader read;
	char buff[100] = {0};
	if(-1 == read.parse(buff,val))
	{
		cerr<<"read fail;errno:"<<errno<<endl;
		return 0;
	}

	else
	{
		cout<<"name:"<<val["name"].asString()<<endl;
		cout<<"pw:"<<val["pw"].asString()<<endl;
		cout<<"type:"<<val["type"].asString()<<endl;
	}
#endif

	cout<<"name:"<<val["name"].asString()<<endl;
        cout<<"pw:"<<val["passwd"].asString()<<endl;
       //cout<<"type:"<<val["type"].asString()<<endl;


	return 0;
}
