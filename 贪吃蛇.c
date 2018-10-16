#include <iostream>
#include <time.h>
#include <windows.h>
#include <list>
using namespace std;

const int MapSize = 30;

class Position
{
public:
	Position(int a, int b) { x = a;y = b; }
	int x;
	int y;
};

Position temp(1, 1);

class Map
{
public:
	void InitMap();//��ʼ����ͼ
	void ShowMap();//��ʾ��ͼ
	void SetFood();//�ڵ�ͼ�Ϸ���ʳ��
	void Game();//ˢ�µ�ͼ
	void EatSnack();//����
	list<Position> Snack;
private:
	int map[MapSize][MapSize];
	int SnackLength;
};
void Map::InitMap()
{
	memset(map, 0, MapSize*MapSize * 4);
	for (int i = 0;i < MapSize;++i)
	{
		map[0][i] = 1;
		map[MapSize - 1][i] = 1;
	}
	for (int i = 0;i < MapSize;++i)
	{
		map[i][0] = 1;
		map[i][MapSize - 1] = 1;
	}
	//SnackLength = Snack.size();
}
void Map::ShowMap()
{
	//if()
	for (int i = 0;i < Snack.size();++i)
	{
		list<Position>::iterator it = Snack.begin();
		map[it->x][it->y] = 3;
		++it;
	}
	for (int i = 0;i < MapSize;++i)
	{
		for (int j = 0;j < MapSize;++j)
		{
			if (map[i][j] == 1)
			{
				cout << "* ";
			}
			if (map[i][j] == 0)
			{
				cout << "  ";
			}
			if (map[i][j] == 3)
			{
				cout << "# ";
			}
		}
		cout << endl;
	}
}
void Map::SetFood()
{
	int food = rand() % MapSize;
	if (food == 0)
	{
		food= rand() % MapSize;
	}
	map[food][food] = 3;
}
void Map::Game()
{
	SetFood();
	while (1)
	{
		InitMap();
		//SetFood();
		ShowMap();
		EatSnack();
		Sleep(1000);
		system("cls");
	}
}
void Map::EatSnack()
{
	Position p = temp;
	p.y = p.y + 1;
	temp=p;
	Snack.push_front(temp);
	Snack.pop_back();
}

int main()
{
	Map MyMap;
	srand(time(0));
	MyMap.Snack.push_back(temp);
	MyMap.Game();
	return 0;
}