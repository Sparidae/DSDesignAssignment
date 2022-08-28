#pragma once
#include<iostream>
#include<stack>
#include<string>
using namespace std;
const int MAXSIZE = 600;//��󶥵���
//�߱���
struct EdgeNode
{
	int Index;//��վ��Ķ�ӦVertexNode�±�
	int dist;//����վ�ľ��� Ȩ��1 ��λm
	int costTime;//����վ��ʱ�� Ȩ��2 ��λs
	EdgeNode* next;
};
//�������
template<class T>
struct VertexNode
{
	T vertexName;//string�洢վ������ �����±��վ����б��
	EdgeNode* firstNode;
};
//������dijkstra�㷨
struct dijkstra
{
	bool isvisited;//��̱�� true��ýڵ��������ҵ����·���ļ���S ����Ϊ�����ʼ���Q
	int d;//���·����ֵ ·��Ϊmʱ��Ϊs
	int preIndex;//���·����һ���ڵ�
};

template<class T>
class ALGraph
{//����ʱ���޸ĵ�����ͼ ��Ȩ��
private:
	VertexNode<T> Graph[MAXSIZE];//�ڽӱ��ʾ��ͼ
	bool isvisited[MAXSIZE];//�������ı�־����dfs����
	int vertexNum=0, edgeNum=0;//�������ͱ���
	map<string, set<string>> staToLine;
	bool isLineGraph=false;//ͼ���� false��վ����ͼ true����·���ͼ
	ALGraph<T>* LineGraph();//����һ�����ڸ�ͼ���е�����·��ͼ
	stack<string>* searchPathForLines(string LineA,string LineB);
public:
	ALGraph();
	~ALGraph();
	void build(T sta1, T sta2, int dist=1, double speed=1.0,string staName="");//������ұ����Ƿ���ڶ�Ӧ�����ڵ㣬�����������ڵ�Ȼ�����һ������
	void edgeDel(T sta1, T sta2);
	void searchPath(T from, T to, bool isDist = true, bool lessTransfer = false);//Ѱ�����·�� ����lessTransfer��ǰ���ֲ��Խ���ʧЧ
	int VertexLoc(T vertexName);//��λ���������ֵ ���򷵻�-1;
	bool edgeExist(T v1, T v2);//���ұ��Ƿ����
};

template<class T>
inline ALGraph<T>* ALGraph<T>::LineGraph()
{//������·��ͼ�����õ���ͼ��ָ�봫��ȥ
	if (isLineGraph)
	{
		cout << "������·ͼ�����Խ���·ͼ" << endl; return nullptr;
	}
	ALGraph<T>* statGraph = new ALGraph<T>;
	statGraph->isLineGraph = true;//��ͼ���Ա��Ϊtrue ����·���ͼ
	for (auto it = staToLine.begin(); it != staToLine.end(); it++)
	{
		if (it->second.size() > 1)
		{//����վ
			for (auto t1 = it->second.begin(); t1 != it->second.end(); t1++)
				for (auto t2 = it->second.begin(); t2 != it->second.end(); t2++)
				{
					statGraph->build(*t1, *t2);
				}
		}
	}
	return statGraph;
}

template<class T>
inline stack<string>* ALGraph<T>::searchPathForLines(string LineA, string LineB)
{
	if (isLineGraph)
	{//����һ����·ͼ����ִ���������
		int pfrom = VertexLoc(LineA);
		int pto = VertexLoc(LineB);
		stack<string>* lineStack = new stack<string>;
		dijkstra* vertex = new dijkstra[vertexNum + 1];
		//dijkstra vertex[MAXSIZE];
		const int Inf = 2000000000;
		for (int i = 0; i < vertexNum; i++)
		{
			vertex[i].isvisited = false;//ȫ���������Q��
			vertex[i].d = Inf;
			vertex[i].preIndex = -1;
		}
		vertex[pfrom].d = 0;
		int mind;//��С�Ĺ���·������
		int minIndex;//��С�Ĺ���·�����ȶ�Ӧ��������
		for (int i = 0; i < vertexNum; i++)
		{//ÿ��ѭ����һ���ڵ����S��
			mind = Inf;
			minIndex = -1;//��С��������
			//EXTRACT_MIN(Q)�ҵ�Q������С����·���Ľ��
			for (int i = 0; i < vertexNum; i++)
				if (!vertex[i].isvisited && vertex[i].d <= mind)//������S���Ҷ�Ӧ��С����·��d��С
				{
					mind = vertex[i].d; minIndex = i;
				}
			vertex[minIndex].isvisited = true;//����S��
			//RELAX(u,v,w)�Ե�ǰ��С�ڵ�u���������ڽڵ�v����һ��d�ĸ���
			EdgeNode* tem = Graph[minIndex].firstNode;
			int weight;//�ߵ�Ȩ��
			while (tem != nullptr)
			{//����u(minIndex)���������ڽ��v�������������㲻����S���ҿ��ɳ�(v.d>u.d+w(u,v))
				weight = tem->dist;
				if (!vertex[tem->Index].isvisited && (vertex[tem->Index].d > vertex[minIndex].d + weight))
				{
					vertex[tem->Index].d = vertex[minIndex].d + weight;
					vertex[tem->Index].preIndex = minIndex;
				}
				tem = tem->next;//!!��Ҫ����tem
			}
		}
		//�Ѿ������·�� �洢��
		int id = pto;
		while (-1 != id)
		{
			lineStack->push(Graph[id].vertexName);
			id = vertex[id].preIndex;
		}
		return lineStack;
	}
	return nullptr;
}

template<class T>
inline ALGraph<T>::ALGraph()
{
	for (int i = 0; i < MAXSIZE; i++)
	{
		Graph[i].firstNode = nullptr;

	}
}

template<class T>
inline ALGraph<T>::~ALGraph()
{
	EdgeNode* p, *tem;
	for (int i = 0; i < vertexNum; i++)
	{
		p = Graph[i].firstNode;
		Graph[i].firstNode = nullptr;
		while (p != nullptr)
		{
			tem = p;
			p = p->next;
			delete tem;
		}
	}
}

template<class T>
inline void ALGraph<T>::build(T sta1, T sta2, int dist,double speed,string staName)
{//dist��λ��m speed��λ��km/h
	if (sta1 == sta2)return;//��ͬ��
	if (fabs(speed) < 1e-6)
		throw"invalid speed 0";
	//����վ���Ӧ��·���ݵ�ӳ��map
	if (!isLineGraph)
	{//������ͼ ��վ��ͼ ������·ͼ
		staToLine[sta2].insert(staName);
		staToLine[sta1].insert(staName);
	}
	//��ʼ������·
	speed = speed / 3.6;//�����m/s
	edgeNum++;
	int p1 = VertexLoc(sta1);
    int p2 = VertexLoc(sta2);
	if (p1 == -1)
	{//��ǰ�ڵ��б���û�и�վ
		Graph[vertexNum].vertexName = sta1;
		p1 = vertexNum;
		vertexNum++;
	}
	if (p2 == -1)
	{
		Graph[vertexNum].vertexName = sta2;
		p2 = vertexNum;
		vertexNum++;
	}
	EdgeNode* tem;
	//����p1 ����Ƿ��Ѿ�����������
	if (!edgeExist(sta1,sta2))
	{
		tem = new EdgeNode;
		tem->dist = dist;
		tem->costTime = (int)dist / speed * 2;
		tem->Index = p2;
		tem->next = Graph[p1].firstNode;
		Graph[p1].firstNode = tem;
	}
	//����p2��ѯp1
	if (!edgeExist(sta2,sta1))
	{
		tem = new EdgeNode;
		tem->dist = dist;
		tem->costTime = (int)dist / speed * 2;
		tem->Index = p1;
		tem->next = Graph[p2].firstNode;
		Graph[p2].firstNode = tem;
	}
}

template<class T>
inline void ALGraph<T>::edgeDel(T sta1, T sta2)
{
	if (sta1 == sta2){
		cout << "�����ڱ�" << endl; return;
	}
	int p1 = VertexLoc(sta1);
	int p2 = VertexLoc(sta2);
	if (-1 == p1 || -1 == p2) {
		cout << "�߲�����" << endl; return;
	}
	EdgeNode* tem = Graph[p1].firstNode;
	if (tem->Index == p2){
		Graph[p1].firstNode=tem->next;
		delete tem;
	}
	else {
		while (tem->next != nullptr){
			if (tem->next->Index == p2) {
				EdgeNode* p = tem->next;
				tem->next = tem->next->next;
				delete p;
				break;
			}
		}
	}
	tem = Graph[p2].firstNode;
	if (tem->Index == p1) {
		Graph[p2].firstNode=tem->next;
		delete tem;
	}
	else {
		while (tem->next != nullptr) {
			if (tem->next->Index == p2) {
				EdgeNode* p = tem->next;
				tem->next = tem->next->next;
				delete p;
				break;
			}
		}
	}
}

template<class T>
inline void ALGraph<T>::searchPath(T from, T to, bool isDist, bool lessTransfer)
{//dijkstra����Ȩ��ͼ���·��
	//����lessTransfer��ǰ���ֲ��Խ���ʧЧ
	//��������Ƿ�Ϸ�
	int pfrom = VertexLoc(from);
	int pto = VertexLoc(to);
	if (-1 == pfrom || -1 == pto)
	{
		cout << "������󣬲�ѯվ�㲻����" << endl;
		return;
	}
	if (!lessTransfer)
	{
		//INITIAL_SINGLE_SOURCE
		dijkstra* vertex = new dijkstra[vertexNum + 1];
		//dijkstra vertex[MAXSIZE];
		const int Inf = 2000000000;
		for (int i = 0; i < vertexNum; i++)
		{
			vertex[i].isvisited = false;//ȫ���������Q��
			vertex[i].d = Inf;
			vertex[i].preIndex = -1;
		}
		vertex[pfrom].d = 0;
		int mind;//��С�Ĺ���·������
		int minIndex;//��С�Ĺ���·�����ȶ�Ӧ��������
		for (int i = 0; i < vertexNum; i++)
		{//ÿ��ѭ����һ���ڵ����S��
			mind = Inf;
			minIndex = -1;//��С��������
			//EXTRACT_MIN(Q)�ҵ�Q������С����·���Ľ��
			for (int i = 0; i < vertexNum; i++)
				if (!vertex[i].isvisited && vertex[i].d <= mind)//������S���Ҷ�Ӧ��С����·��d��С
				{
					mind = vertex[i].d; minIndex = i;
				}
			vertex[minIndex].isvisited = true;//����S��
			//RELAX(u,v,w)�Ե�ǰ��С�ڵ�u���������ڽڵ�v����һ��d�ĸ���
			EdgeNode* tem = Graph[minIndex].firstNode;
			int weight;//�ߵ�Ȩ��
			while (tem != nullptr)
			{//����u(minIndex)���������ڽ��v�������������㲻����S���ҿ��ɳ�(v.d>u.d+w(u,v))
				weight = (isDist ? tem->dist : tem->costTime);
				if (!vertex[tem->Index].isvisited && (vertex[tem->Index].d > vertex[minIndex].d + weight))
				{
					vertex[tem->Index].d = vertex[minIndex].d + weight;
					vertex[tem->Index].preIndex = minIndex;
				}
				tem = tem->next;//!!��Ҫ����tem
			}
		}
		//�Ҳ����Ľ�����˵�����ڵ�Ĳ�����·�����ӦpreIndex����Ϊ-1;
		//�ڱ�����ֻ���ڱ�edgedel����ɾ���ߺ�ſ�����������վ֮�䲻����·��
		if (-1 == vertex[pto].preIndex)
		{
			cout << "��������������·" << endl;
			delete[]vertex; return;
		}
		//��� δ���
		int id = pto;
		stack<int>route;
		while (-1 != id)
		{
			route.push(id);
			id = vertex[id].preIndex;
		}//routeջջ�������
		int size = route.size();
		int preid, nextid;
		for (int i = 0; i < size; i++)
		{//���·��
			//һ���޸�bug�İ취���������վ��¼ ��������Ҫ�ߵĵ�ǰ������·��
			//������ÿ��һ��������һվ�Ƿ���������ǰ��·��û��˵��������,������ǰ��·����һ������
			if (i == 0)
			{//���վ�յ�վ�ǻ���վ����ν
				cout << Graph[route.top()].vertexName<<"->";
				preid = route.top();
			}
			else if (i == size - 1)
			{
				cout << Graph[route.top()].vertexName;
			}
			else
			{//
				bool transferFlag = true;
				int tem1 = route.top(); route.pop(); nextid = route.top(); route.push(tem1);
				for (auto it1 : staToLine[Graph[preid].vertexName])
					for (auto it2 : staToLine[Graph[nextid].vertexName])
						if (it1 == it2)
						{
							transferFlag = false;//Ѱ���Ƿ������ͬ��· ���ھͲ���Ҫ����bug��
							break;
						}
				if (transferFlag)
				{//����
					cout << Graph[route.top()].vertexName << "(��վ����)" << "->";
				}
				else 
				{//������
					cout << Graph[route.top()].vertexName << "->";
				}
				preid = route.top();
			}
			route.pop();
		}
		cout << endl;
		cout << ((isDist ? "�ܾ���:" : "��ʱ��:"));
		if (isDist)
		{
			cout << vertex[pto].d / 1000.0 << "km" << endl;
		}
		else
		{
			int h=0, minute=0, s;
			s = vertex[pto].d;
			if (s >= 60)
			{
				minute = s / 60;
				s = s % 60;
			}
			if (minute >= 60)
			{
				h = minute / 60;
				minute = minute % 60;
			}
			cout << h << "h" << minute << "min" << s << "s" << endl;
		}
		delete[]vertex;
		return;
	}
	else
	{
		//Ѱ�����ٻ��˲��� δ���
		ALGraph<string>* LineGraph = this->LineGraph();

		//�ж���վ���ڵ���·�Ƿ���һ������
		//վ�����ƺϷ�
		
		for (auto afrom : staToLine[from])//it�Ǳ���set��Ԫ��,string���͵�վ������,�������޸�set
			for(auto ato : staToLine[to])
				if (afrom == ato)
				{//˵����վ��ͬһ����·��
					//ֻ����������ֱ�߾�Ҫ���Եõ�·����to������from
					for (auto& a : isvisited)a = false;
					stack<int> dfs;
					dfs.push(pto); isvisited[pto] = true;
					int id;
					while (dfs.top()!=pfrom&&!dfs.empty())
					{//ѭ������ û���ҵ�pfrom��������ջ�ǿգ�����˵��atoҲ�������˴�ʱ����֮�䲢������һ����·��
						id = dfs.top();
						EdgeNode* tem = Graph[id].firstNode;
						while (tem != nullptr)
						{//Ѱ������ڵ�����Ŀɷ��ʽڵ�
							if (!isvisited[tem->Index]&&staToLine[Graph[tem->Index].vertexName].contains(afrom))
							{//����ڵ�û�з��� �� ����������������·
								dfs.push(tem->Index);
								isvisited[tem->Index] = true;
								break;
							}
							tem = tem->next;//ѭ������
						}
						//û���ҵ����Է��ʵĽڵ� ��ջ
						if (tem == nullptr)dfs.pop();
					}
					//
					if (!dfs.empty() && dfs.top() == pfrom)
					{//·���ҵ��˴���һ�����
						cout << "��·��ȫ�̲���Ҫ����" << endl;
						cout << "ȫ����·Ϊ" << afrom << endl;
						cout << "��ϸ·��:";
						while (!dfs.empty())
						{
							cout << Graph[dfs.top()].vertexName << "->";
							dfs.pop();
						}
					}
					else{}//Ӧ�ô���һ����������·�ڶ�·�����������
					cout << endl;
					return;
				}
		//��վ����ͬһ����·��
		for (auto& a : isvisited)a = false;
		int minTransfer = 10;
		string minfrom, minto;
		stack<string>* linestack;
		for (auto afrom : staToLine.at(from))//it�Ǳ���set��Ԫ��,string���͵�վ������,�������޸�set
			for (auto ato : staToLine.at(to))
			{
				linestack = LineGraph->searchPathForLines(afrom, ato);
				if (linestack->size() < minTransfer)
				{
					minTransfer = linestack->size();
					minfrom = afrom; minto = ato;
				}
			}
		linestack = LineGraph->searchPathForLines(minfrom, minto);//·��ջ
		stack<string> LineCopy(*linestack);//��ջ����һ������
		//�õ���·���洢��ջ ջ������ʼ��· �������Ƕ����·���Ĵ����Ե�����·ͼ��������
		for (auto& a : isvisited)a = false;
		stack<int> vertexDfs;//վ��ջ
		vertexDfs.push(pfrom); isvisited[pfrom] = true;
		int id;
		//int mt = LineGraph->VertexLoc(minto);
		while (vertexDfs.top() != pto && !vertexDfs.empty())
		{//ѭ������ û���ҵ�minto��������ջ�ǿգ�����˵��atoҲ�������˴�ʱ����֮�䲢������һ����·��
			id = vertexDfs.top();//ȡջ�����id
			EdgeNode* tem = Graph[id].firstNode;
			//Ѱ������ڵ����ڵĿɷ��ʽڵ�
			while (tem != nullptr)
			{
				string temLine="";
				if (linestack->size() >= 2)
				{//˵������������������· ����������· ���Խ��в��� 
					temLine = linestack->top(); linestack->pop();
				}
				if (!isvisited[tem->Index] && staToLine[Graph[tem->Index].vertexName].contains(linestack->top()))
				{//����ڵ�û�з����� ����ڵ�������һ����· �������һ����·������һ����·
					vertexDfs.push(tem->Index);
					isvisited[tem->Index] = true;
					//if (temLine != "")linestack->push(temLine);
					break;
				}
				if (temLine != "")linestack->push(temLine);
				if (!isvisited[tem->Index] && staToLine[Graph[tem->Index].vertexName].contains(linestack->top()))
				{//����ڵ�û�з��� �� ����������������·
					vertexDfs.push(tem->Index);
					isvisited[tem->Index] = true;
					break;
				}
				
				tem = tem->next;//ѭ������
			}
			//û���ҵ����Է��ʵĽڵ� ��ջ
			if (tem == nullptr&&!vertexDfs.empty())vertexDfs.pop();
		}
		//�ҵ�������һ����·�洢��stack dfs��ջ�����յ㣬
		stack<int> finalResult;
		while (!vertexDfs.empty())
		{
			finalResult.push(vertexDfs.top());
			vertexDfs.pop();
		}
		//����final��ջ�����յ�
		if (!finalResult.empty() && finalResult.top() == pfrom)
		{//·���ҵ��˴���һ�����
			cout << "���ٻ��˴���Ϊ" << minTransfer-1 << endl;
			cout << "����ָ��:" << endl;;
			string sLine = LineCopy.top();
			string a,s;
			while (!finalResult.empty())
			{
				a=Graph[finalResult.top()].vertexName;//վ������

				//int t = finalResult.top(); finalResult.pop();
				if (!LineCopy.empty())
				{
					s = LineCopy.top(); LineCopy.pop();//���軻��
				}
				if (!LineCopy.empty() && staToLine[a].contains(LineCopy.top()))//����
					cout << "��" << a << "����" << LineCopy.top() << "->";
				else
				{//������
					cout << a << "->";
					LineCopy.push(s);
				}
				finalResult.pop();
			}
		}
		else
		{ }
		cout << endl;
		return;
	}
}

template<class T>
inline int ALGraph<T>::VertexLoc(T vertexName)
{
	for (int i = 0; i < vertexNum; i++)
		if (vertexName == Graph[i].vertexName)
			return i;
	return -1;
}

template<class T>
inline bool ALGraph<T>::edgeExist(T v1, T v2)
{//��ѯ����֮��ֱ�ӵı��Ƿ����
	if (v1 == v2)return false;//�������Ի�
	int p1 = VertexLoc(v1);
	int p2 = VertexLoc(v2);
	if (-1 == p1 || -1 == p2)return false;
	EdgeNode* tem = Graph[p1].firstNode;
	while (tem != nullptr)
	{
		if (tem->Index == p2)return true;
		tem = tem->next;
	}
	return false;
}
