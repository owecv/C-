#include <iostream>
#include <string>
using namespace std;

class Student
{
public:
	void get_value() { cin >> num >> name >> sex; }
	void display()
	{
		cout << "num:" << num << endl;
		cout << "name:" << name << endl;
		cout << "sex:" << sex << endl;
	}
private:
	int num;
	string name;
	char sex;
};

class Student1 :public Student
{
public:
	void get_value_1()
	{
		cin >> age >> addr;
	}
	void display_1()
	{
		cout << "age:" << age << endl;
		cout << "address:" << addr << endl;
	}
private:
	int age;
	string addr;
};

int main()
{
	Student1 MyStudent;
	MyStudent.get_value();
	MyStudent.get_value_1();
	MyStudent.display();
	MyStudent.display_1();
	return 0;
}