#include <iostream>
#include <string>
#include <stack>
using namespace std;

class InPut
{
public:
	InPut() { int intput = 0;count = 0; }
	void Set_InPut() { cin >> input; }//����input
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
	stack<string> MyStack1;//�����͵�ÿλ������ת��Ϊ���ĵ��ַ����ڴ�ջ��
	
	
	//string::iterator it = OutPut.begin();
	for (int i = 0;i < count;++i)
	{
		switch (MyStack.top())
		{
		case 0:cout << "��";
			OutPut = OutPut + "��";
			MyStack1.push("��");
			break;
		case 1:cout << "һ";
			OutPut = OutPut + "һ";
			MyStack1.push("һ");
			break;
		case 2:cout << "��";
			OutPut = OutPut + "��";
			MyStack1.push("��");
			break;
		case 3:cout << "��";
			OutPut = OutPut + "��";
			MyStack1.push("��");
			break;
		case 4:cout << "��";
			OutPut = OutPut + "��";
			MyStack1.push("��");
			break;
		case 5:cout << "��";
			OutPut = OutPut + "��";
			MyStack1.push("��");
			break;
		case 6:cout << "��";
			OutPut = OutPut + "��";
			MyStack1.push("��");
			break;
		case 7:cout << "��";
			OutPut = OutPut + "��";
			MyStack1.push("��");
			break;
		case 8:cout << "��";
			OutPut = OutPut + "��";
			MyStack1.push("��");
			break;
		case 9:cout << "��";
			OutPut = OutPut + "��";
			MyStack1.push("��");
			break;
		}
		MyStack.pop();
	}
	cout << endl;

	stack<string> MyStack2;//��ÿλ����λ�ĺ��ִ����ջ

	if (count == 2)
	{
		string yami = MyStack1.top();
		if (yami == "��")
		{
			MyStack1.pop();
		}
		else
		{
			MyStack2.push(MyStack1.top());
			MyStack1.pop();
		}
		MyStack2.push(MyStack1.top()+"ʮ");
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
		MyStack2.push(MyStack1.top() + "ʮ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
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
		MyStack2.push(MyStack1.top() + "ʮ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "ǧ");
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
		MyStack2.push(MyStack1.top() + "ʮ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "ǧ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
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
		MyStack2.push(MyStack1.top() + "ʮ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "ǧ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "ʮ");
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
		MyStack2.push(MyStack1.top() + "ʮ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "ǧ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "ʮ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
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
		MyStack2.push(MyStack1.top() + "ʮ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "ǧ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "ʮ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "ǧ");
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
		MyStack2.push(MyStack1.top() + "ʮ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "ǧ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "ʮ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "ǧ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
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
		MyStack2.push(MyStack1.top() + "ʮ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "ǧ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "ʮ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "ǧ");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "��");
		MyStack1.pop();
		MyStack2.push(MyStack1.top() + "ʮ");
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
	case 2:printf("ʮ");break;
	case 3:printf("��");break;
	case 4:printf("ǧ");break;
	case 5:printf("��");break;
	case 6:printf("ʮ");break;
	case 7:printf("��");break;
	case 8:printf("ǧ");break;
	case 9:printf("��");break;
	}*/
}

int main()
{
	InPut test;
	test.Set_InPut();
	test.function();
	return 0;
}