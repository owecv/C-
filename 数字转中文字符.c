#include <iostream>
#include <string>
#include <stack>
using namespace std;

class InPut
{
public:
	InPut() { int intput = 0;count = 0; }
	void Set_InPut() { cin >> input; }//输入input
	void function();
private:
	int input;
	stack<int> MyStack;
	string OutPut;
	int count;
};

void InPut::function()
{
	int temp = input;
	while (temp != 0)
	{
		int a = temp % 10;
		MyStack.push(a);
		temp = temp / 10;
		count = MyStack.size();
	}
	stack<string> MyStack1;//将整型的每位的数字转换为中文的字符存在此栈中
	
	
	//string::iterator it = OutPut.begin();
	for (int i = 0;i < count;++i)
	{
		switch (MyStack.top())
		{
		case 0:cout << "零";
			OutPut = OutPut + "零";
			MyStack1.push("零");
			break;
		case 1:cout << "一";
			OutPut = OutPut + "一";
			MyStack1.push("一");
			break;
		case 2:cout << "二";
			OutPut = OutPut + "二";
			MyStack1.push("二");
			break;
		case 3:cout << "三";
			OutPut = OutPut + "三";
			MyStack1.push("三");
			break;
		case 4:cout << "四";
			OutPut = OutPut + "四";
			MyStack1.push("四");
			break;
		case 5:cout << "五";
			OutPut = OutPut + "五";
			MyStack1.push("五");
			break;
		case 6:cout << "六";
			OutPut = OutPut + "六";
			MyStack1.push("六");
			break;
		case 7:cout << "七";
			OutPut = OutPut + "七";
			MyStack1.push("七");
			break;
		case 8:cout << "八";
			OutPut = OutPut + "八";
			MyStack1.push("八");
			break;
		case 9:cout << "九";
			OutPut = OutPut + "九";
			MyStack1.push("九");
			break;
		}
		MyStack.pop();
	}
	cout << endl;

	stack<string> MyStack2;//将每位带单位的汉字存入此栈

	if (count == 2)
	{
		string yami = MyStack1.top();
		if (yami == "零")
		{
			MyStack1.pop();
		}
		else
		{
			MyStack2.push(MyStack1.top());
			MyStack1.pop();
		}
		MyStack2.push(MyStack1.top()+"十");
		MyStack1.pop();
		/**************************/
		OutPut = MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
	}
	if (count == 3)
	{
		MyStack2.push(MyStack1.top());
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "十");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "百");
		MyStack1.pop();
		/*****************************/
		OutPut = MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
	}
	if (count == 4)
	{
		MyStack2.push(MyStack1.top());
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "十");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "百");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "千");
		MyStack1.pop();
		/*****************************/
		OutPut = MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
	}
	if (count == 5)
	{
		MyStack2.push(MyStack1.top());
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "十");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "百");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "千");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "万");
		MyStack1.pop();
		/*****************************/
		OutPut = MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
	}
	if (count == 6)
	{
		MyStack2.push(MyStack1.top());
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "十");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "百");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "千");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "万");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "十");
		MyStack1.pop();
		/*****************************/
		OutPut = MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
	}
	if (count == 7)
	{
		MyStack2.push(MyStack1.top());
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "十");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "百");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "千");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "万");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "十");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "百");
		MyStack1.pop();
		/*****************************/
		OutPut = MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
	}
	if (count == 8)
	{
		MyStack2.push(MyStack1.top());
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "十");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "百");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "千");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "万");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "十");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "百");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "千");
		MyStack1.pop();
		/*****************************/
		OutPut = MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
	}
	if (count == 9)
	{
		MyStack2.push(MyStack1.top());
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "十");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "百");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "千");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "万");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "十");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "百");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "千");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "亿");
		MyStack1.pop();
		/*****************************/
		OutPut = MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
	}
	if (count == 10)
	{
		MyStack2.push(MyStack1.top());
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "十");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "百");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "千");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "万");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "十");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "百");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "千");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "亿");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "十");
		MyStack1.pop();
		/*****************************/
		OutPut = MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
		OutPut = OutPut + MyStack2.top();
		MyStack2.pop();
	}
	cout << "OutPut:" << OutPut.c_str() << endl;
	//cout << "char *:" << p << endl;
	//cout << "OutPut:" << OutPut.c_str() << endl;
	/*switch (count)
	{
	case 2:printf("十");break;
	case 3:printf("百");break;
	case 4:printf("千");break;
	case 5:printf("万");break;
	case 6:printf("十");break;
	case 7:printf("百");break;
	case 8:printf("千");break;
	case 9:printf("亿");break;
	}*/
}

int main()
{
	InPut test;
	test.Set_InPut();
	test.function();
	return 0;
}