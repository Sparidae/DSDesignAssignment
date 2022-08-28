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
	string sta, stb;//两站点的名称
	string staName;
	int dist;//站点之间的距离 单位是m
	double speed;
	ALGraph<string> disGraph;
	ifstream fin;
	fin.open("data.txt", ios::in);
	if (!fin) {
		cout << "没有找到数据文件data.txt,请检查是否与程序放于同一目录中" << endl;
		throw"没有找到数据文件data.txt,请检查是否与程序放于同一目录中";
	}
	while (fin >> staName >> speed)
	{
		//cout << staName << " " << speed << endl;
		while ((fin >> sta) && sta != "-")
		{//当文件还没结束并且没有读取到终止符 说明还是本条线路的信息
			fin >> stb >> dist;
			disGraph.build(sta, stb, dist, speed, staName);
		}
	}
	int instruct;
	string from, to;
	while (true)
	{
		system("cls");
		cout << "--------北京地铁路线查询系统--------" << endl;
		cout << "--------1.查询两站点之间的长度最短的路径" << endl;
		cout << "--------2.查询两站点之间耗费时间最短的路径" << endl;
		cout << "--------3.查询两站点之间换乘次数最少的站点" << endl;
		cout << "--------4.插入线路" << endl;
		cout << "--------5.删除线路" << endl;
		cout << "--------6.退出系统" << endl;
		cout << "请输入对应的数字指令:";
		cin >> instruct;
		system("cls");
		switch (instruct)
		{
		case 1:
			cout << "请依次输入起点站和终点站" << endl;
			cin >> from >> to;
			disGraph.searchPath(from, to, true);
			system("pause");
			break;
		case 2:
			cout << "请依次输入起点站和终点站" << endl;
			cin >> from >> to;
			disGraph.searchPath(from, to, false);
			system("pause");
			break;
		case 3:
			cout << "请依次输入起点站和终点站" << endl;
			cin >> from >> to;
			disGraph.searchPath(from, to, false, true);
			system("pause");
			break;
		case 4:
			cout << "请按以下格式输入您想创建的线路,中间以空格分隔" << endl;
			cout << "地铁站1 地铁站2 所属线路 两站之间的距离(m) 最高运行时速(km/h)" << endl;
			cin >> sta >> stb >> staName >> dist >> speed;
			disGraph.build(sta, stb, dist, speed, staName);
			system("pause");
			break;
		case 5:
			cout << "请按照以下格式输入您想删除的线路，仅支持删除线路" << endl;
			cin >> sta >> stb;
			disGraph.edgeDel(sta,stb);
			system("pause");
			break;
		case 6:
			cout << "感谢您的使用!" << endl;
			system("pause");
			fin.close();
			return 0;
		default:
			cout << "指令输入错误，请重新输入!" << endl;
			Sleep(5000);system("cls");
			break;
		}
	}
	fin.close();
	return 0;
}


