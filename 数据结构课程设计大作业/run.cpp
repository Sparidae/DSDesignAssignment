#include<iostream>
#include<cstdio>
#include<fstream>
#include<map>
#include<vector>
#include<string>
#include<set>
#include<Windows.h>
#include"ALGraph.h"

using namespace std;
map<string, set<string>> staToLine;
set<string> s;

int main()
{
	string sta, stb;//��վ�������
	string staName;
	int dist;//վ��֮��ľ��� ��λ��m
	double speed;
	ALGraph<string> disGraph;
	ifstream fin;
	fin.open("data.txt", ios::in);
	if (!fin) {
		cout << "û���ҵ������ļ�data.txt,�����Ƿ���������ͬһĿ¼��" << endl;
		throw"û���ҵ������ļ�data.txt,�����Ƿ���������ͬһĿ¼��";
	}
	while (fin >> staName >> speed)
	{
		//cout << staName << " " << speed << endl;
		while ((fin >> sta) && sta != "-")
		{//���ļ���û��������û�ж�ȡ����ֹ�� ˵�����Ǳ�����·����Ϣ
			fin >> stb >> dist;
			disGraph.build(sta, stb, dist, speed, staName);
		}
	}
	int instruct;
	string from, to;
	while (true)
	{
		system("cls");
		cout << "--------��������·�߲�ѯϵͳ--------" << endl;
		cout << "--------1.��ѯ��վ��֮��ĳ�����̵�·��" << endl;
		cout << "--------2.��ѯ��վ��֮��ķ�ʱ����̵�·��" << endl;
		cout << "--------3.��ѯ��վ��֮�任�˴������ٵ�վ��" << endl;
		cout << "--------4.������·" << endl;
		cout << "--------5.ɾ����·" << endl;
		cout << "--------6.�˳�ϵͳ" << endl;
		cout << "�������Ӧ������ָ��:";
		cin >> instruct;
		system("cls");
		switch (instruct)
		{
		case 1:
			cout << "�������������վ���յ�վ" << endl;
			cin >> from >> to;
			disGraph.searchPath(from, to, true);
			system("pause");
			break;
		case 2:
			cout << "�������������վ���յ�վ" << endl;
			cin >> from >> to;
			disGraph.searchPath(from, to, false);
			system("pause");
			break;
		case 3:
			cout << "�������������վ���յ�վ" << endl;
			cin >> from >> to;
			disGraph.searchPath(from, to, false, true);
			system("pause");
			break;
		case 4:
			cout << "�밴���¸�ʽ�������봴������·,�м��Կո�ָ�" << endl;
			cout << "����վ1 ����վ2 ������· ��վ֮��ľ���(m) �������ʱ��(km/h)" << endl;
			cin >> sta >> stb >> staName >> dist >> speed;
			disGraph.build(sta, stb, dist, speed, staName);
			system("pause");
			break;
		case 5:
			cout << "�밴�����¸�ʽ��������ɾ������·����֧��ɾ����·" << endl;
			cin >> sta >> stb;
			disGraph.edgeDel(sta,stb);
			system("pause");
			break;
		case 6:
			cout << "��л����ʹ��!" << endl;
			system("pause");
			fin.close();
			return 0;
		default:
			cout << "ָ�������������������!" << endl;
			Sleep(5000);system("cls");
			break;
		}
	}
	fin.close();
	return 0;
}


