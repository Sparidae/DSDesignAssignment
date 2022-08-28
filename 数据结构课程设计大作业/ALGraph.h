#pragma once
#include<iostream>
#include<stack>
#include<string>
using namespace std;
const int MAXSIZE = 600;//最大顶点数
//边表结点
struct EdgeNode
{
	int Index;//该站点的对应VertexNode下标
	int dist;//到该站的距离 权重1 单位m
	int costTime;//到该站的时间 权重2 单位s
	EdgeNode* next;
};
//顶点表结点
template<class T>
struct VertexNode
{
	T vertexName;//string存储站点名称 数组下标对站点进行标号
	EdgeNode* firstNode;
};
//服务于dijkstra算法
struct dijkstra
{
	bool isvisited;//最短标记 true则该节点属于已找到最短路径的集合S 否则为待访问集合Q
	int d;//最短路估计值 路程为m时间为s
	int preIndex;//最短路的上一个节点
};

template<class T>
class ALGraph
{//运行时可修改的无向图 带权重
private:
	VertexNode<T> Graph[MAXSIZE];//邻接表表示的图
	bool isvisited[MAXSIZE];//遍历到的标志用于dfs搜索
	int vertexNum=0, edgeNum=0;//顶点数和边数
	map<string, set<string>> staToLine;
	bool isLineGraph=false;//图属性 false是站点结点图 true是线路结点图
	ALGraph<T>* LineGraph();//建立一个关于该图所有地铁线路的图
	stack<string>* searchPathForLines(string LineA,string LineB);
public:
	ALGraph();
	~ALGraph();
	void build(T sta1, T sta2, int dist=1, double speed=1.0,string staName="");//负责查找表中是否存在对应两个节点，不存在则建立节点然后进行一个建边
	void edgeDel(T sta1, T sta2);
	void searchPath(T from, T to, bool isDist = true, bool lessTransfer = false);//寻找最短路径 传入lessTransfer后前两种策略将会失效
	int VertexLoc(T vertexName);//定位顶点的索引值 无则返回-1;
	bool edgeExist(T v1, T v2);//查找边是否存在
};

template<class T>
inline ALGraph<T>* ALGraph<T>::LineGraph()
{//按照线路建图并将得到的图的指针传出去
	if (isLineGraph)
	{
		cout << "错误：线路图不可以建线路图" << endl; return nullptr;
	}
	ALGraph<T>* statGraph = new ALGraph<T>;
	statGraph->isLineGraph = true;//将图属性标记为true 即线路结点图
	for (auto it = staToLine.begin(); it != staToLine.end(); it++)
	{
		if (it->second.size() > 1)
		{//换乘站
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
	{//这是一个线路图才能执行这个函数
		int pfrom = VertexLoc(LineA);
		int pto = VertexLoc(LineB);
		stack<string>* lineStack = new stack<string>;
		dijkstra* vertex = new dijkstra[vertexNum + 1];
		//dijkstra vertex[MAXSIZE];
		const int Inf = 2000000000;
		for (int i = 0; i < vertexNum; i++)
		{
			vertex[i].isvisited = false;//全部结点置于Q集
			vertex[i].d = Inf;
			vertex[i].preIndex = -1;
		}
		vertex[pfrom].d = 0;
		int mind;//最小的估计路径长度
		int minIndex;//最小的估计路径长度对应结点的索引
		for (int i = 0; i < vertexNum; i++)
		{//每次循环讲一个节点加入S集
			mind = Inf;
			minIndex = -1;//最小结点的索引
			//EXTRACT_MIN(Q)找到Q集中最小估计路径的结点
			for (int i = 0; i < vertexNum; i++)
				if (!vertex[i].isvisited && vertex[i].d <= mind)//不属于S集且对应最小估计路径d更小
				{
					mind = vertex[i].d; minIndex = i;
				}
			vertex[minIndex].isvisited = true;//加入S集
			//RELAX(u,v,w)对当前最小节点u的所有相邻节点v进行一个d的更新
			EdgeNode* tem = Graph[minIndex].firstNode;
			int weight;//边的权重
			while (tem != nullptr)
			{//遍历u(minIndex)的所有相邻结点v，条件是这个结点不属于S集且可松弛(v.d>u.d+w(u,v))
				weight = tem->dist;
				if (!vertex[tem->Index].isvisited && (vertex[tem->Index].d > vertex[minIndex].d + weight))
				{
					vertex[tem->Index].d = vertex[minIndex].d + weight;
					vertex[tem->Index].preIndex = minIndex;
				}
				tem = tem->next;//!!重要更新tem
			}
		}
		//已经找最短路线 存储于
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
{//dist单位是m speed单位是km/h
	if (sta1 == sta2)return;//相同点
	if (fabs(speed) < 1e-6)
		throw"invalid speed 0";
	//建立站点对应线路数据的映射map
	if (!isLineGraph)
	{//如果这个图 是站点图 则建立线路图
		staToLine[sta2].insert(staName);
		staToLine[sta1].insert(staName);
	}
	//开始建立线路
	speed = speed / 3.6;//换算成m/s
	edgeNum++;
	int p1 = VertexLoc(sta1);
    int p2 = VertexLoc(sta2);
	if (p1 == -1)
	{//当前节点列表中没有该站
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
	//设置p1 检查是否已经建立这条边
	if (!edgeExist(sta1,sta2))
	{
		tem = new EdgeNode;
		tem->dist = dist;
		tem->costTime = (int)dist / speed * 2;
		tem->Index = p2;
		tem->next = Graph[p1].firstNode;
		Graph[p1].firstNode = tem;
	}
	//设置p2查询p1
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
		cout << "不存在边" << endl; return;
	}
	int p1 = VertexLoc(sta1);
	int p2 = VertexLoc(sta2);
	if (-1 == p1 || -1 == p2) {
		cout << "边不存在" << endl; return;
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
{//dijkstra无向权重图最短路径
	//传入lessTransfer后前两种策略将会失效
	//检验参数是否合法
	int pfrom = VertexLoc(from);
	int pto = VertexLoc(to);
	if (-1 == pfrom || -1 == pto)
	{
		cout << "输入错误，查询站点不存在" << endl;
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
			vertex[i].isvisited = false;//全部结点置于Q集
			vertex[i].d = Inf;
			vertex[i].preIndex = -1;
		}
		vertex[pfrom].d = 0;
		int mind;//最小的估计路径长度
		int minIndex;//最小的估计路径长度对应结点的索引
		for (int i = 0; i < vertexNum; i++)
		{//每次循环讲一个节点加入S集
			mind = Inf;
			minIndex = -1;//最小结点的索引
			//EXTRACT_MIN(Q)找到Q集中最小估计路径的结点
			for (int i = 0; i < vertexNum; i++)
				if (!vertex[i].isvisited && vertex[i].d <= mind)//不属于S集且对应最小估计路径d更小
				{
					mind = vertex[i].d; minIndex = i;
				}
			vertex[minIndex].isvisited = true;//加入S集
			//RELAX(u,v,w)对当前最小节点u的所有相邻节点v进行一个d的更新
			EdgeNode* tem = Graph[minIndex].firstNode;
			int weight;//边的权重
			while (tem != nullptr)
			{//遍历u(minIndex)的所有相邻结点v，条件是这个结点不属于S集且可松弛(v.d>u.d+w(u,v))
				weight = (isDist ? tem->dist : tem->costTime);
				if (!vertex[tem->Index].isvisited && (vertex[tem->Index].d > vertex[minIndex].d + weight))
				{
					vertex[tem->Index].d = vertex[minIndex].d + weight;
					vertex[tem->Index].preIndex = minIndex;
				}
				tem = tem->next;//!!重要更新tem
			}
		}
		//找不到的结点或者说孤立节点的不存在路径则对应preIndex属性为-1;
		//在本题中只有在被edgedel函数删除边后才可能两个地铁站之间不存在路线
		if (-1 == vertex[pto].preIndex)
		{
			cout << "不存在这样的线路" << endl;
			delete[]vertex; return;
		}
		//输出 未完成
		int id = pto;
		stack<int>route;
		while (-1 != id)
		{
			route.push(id);
			id = vertex[id].preIndex;
		}//route栈栈顶是起点
		int size = route.size();
		int preid, nextid;
		for (int i = 0; i < size; i++)
		{//输出路线
			//一种修改bug的办法是先在起点站记录 接下来将要走的当前所在线路，
			//接下来每走一步搜索下一站是否有这条当前线路，没有说明换乘了,并将当前线路进行一个更新
			if (i == 0)
			{//起点站终点站是换乘站无所谓
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
							transferFlag = false;//寻找是否存在相同线路 存在就不需要换乘bug！
							break;
						}
				if (transferFlag)
				{//换乘
					cout << Graph[route.top()].vertexName << "(此站换乘)" << "->";
				}
				else 
				{//不换乘
					cout << Graph[route.top()].vertexName << "->";
				}
				preid = route.top();
			}
			route.pop();
		}
		cout << endl;
		cout << ((isDist ? "总距离:" : "总时间:"));
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
		//寻找最少换乘策略 未完成
		ALGraph<string>* LineGraph = this->LineGraph();

		//判断两站所在的线路是否在一条线上
		//站点名称合法
		
		for (auto afrom : staToLine[from])//it是遍历set中元素,string类型的站点名称,并不能修改set
			for(auto ato : staToLine[to])
				if (afrom == ato)
				{//说明两站在同一条线路上
					//只需搜索这条直线就要可以得到路径从to搜索到from
					for (auto& a : isvisited)a = false;
					stack<int> dfs;
					dfs.push(pto); isvisited[pto] = true;
					int id;
					while (dfs.top()!=pfrom&&!dfs.empty())
					{//循环条件 没有找到pfrom并且搜索栈非空（空了说明ato也被弹出了此时两点之间并不存在一条线路）
						id = dfs.top();
						EdgeNode* tem = Graph[id].firstNode;
						while (tem != nullptr)
						{//寻找这个节点里面的可访问节点
							if (!isvisited[tem->Index]&&staToLine[Graph[tem->Index].vertexName].contains(afrom))
							{//这个节点没有访问 且 这个结点属于这条线路
								dfs.push(tem->Index);
								isvisited[tem->Index] = true;
								break;
							}
							tem = tem->next;//循环继续
						}
						//没有找到可以访问的节点 退栈
						if (tem == nullptr)dfs.pop();
					}
					//
					if (!dfs.empty() && dfs.top() == pfrom)
					{//路径找到了处理一个输出
						cout << "该路线全程不需要换乘" << endl;
						cout << "全程线路为" << afrom << endl;
						cout << "详细路线:";
						while (!dfs.empty())
						{
							cout << Graph[dfs.top()].vertexName << "->";
							dfs.pop();
						}
					}
					else{}//应该处理一种在这条线路内断路的情况，但是
					cout << endl;
					return;
				}
		//两站不在同一条线路上
		for (auto& a : isvisited)a = false;
		int minTransfer = 10;
		string minfrom, minto;
		stack<string>* linestack;
		for (auto afrom : staToLine.at(from))//it是遍历set中元素,string类型的站点名称,并不能修改set
			for (auto ato : staToLine.at(to))
			{
				linestack = LineGraph->searchPathForLines(afrom, ato);
				if (linestack->size() < minTransfer)
				{
					minTransfer = linestack->size();
					minfrom = afrom; minto = ato;
				}
			}
		linestack = LineGraph->searchPathForLines(minfrom, minto);//路径栈
		stack<string> LineCopy(*linestack);//将栈进行一个复制
		//得到了路径存储于栈 栈顶是起始线路 接下来是对这个路径的处理，对地铁线路图进行搜索
		for (auto& a : isvisited)a = false;
		stack<int> vertexDfs;//站点栈
		vertexDfs.push(pfrom); isvisited[pfrom] = true;
		int id;
		//int mt = LineGraph->VertexLoc(minto);
		while (vertexDfs.top() != pto && !vertexDfs.empty())
		{//循环条件 没有找到minto并且搜索栈非空（空了说明ato也被弹出了此时两点之间并不存在一条线路）
			id = vertexDfs.top();//取栈顶结点id
			EdgeNode* tem = Graph[id].firstNode;
			//寻找这个节点相邻的可访问节点
			while (tem != nullptr)
			{
				string temLine="";
				if (linestack->size() >= 2)
				{//说明还有两条及以上线路 不是最终线路 可以进行操作 
					temLine = linestack->top(); linestack->pop();
				}
				if (!isvisited[tem->Index] && staToLine[Graph[tem->Index].vertexName].contains(linestack->top()))
				{//这个节点没有访问且 这个节点属于下一条线路 若是最后一条线路则还是这一条线路
					vertexDfs.push(tem->Index);
					isvisited[tem->Index] = true;
					//if (temLine != "")linestack->push(temLine);
					break;
				}
				if (temLine != "")linestack->push(temLine);
				if (!isvisited[tem->Index] && staToLine[Graph[tem->Index].vertexName].contains(linestack->top()))
				{//这个节点没有访问 且 这个结点属于这条线路
					vertexDfs.push(tem->Index);
					isvisited[tem->Index] = true;
					break;
				}
				
				tem = tem->next;//循环继续
			}
			//没有找到可以访问的节点 退栈
			if (tem == nullptr&&!vertexDfs.empty())vertexDfs.pop();
		}
		//找到了这样一条线路存储于stack dfs中栈顶是终点，
		stack<int> finalResult;
		while (!vertexDfs.empty())
		{
			finalResult.push(vertexDfs.top());
			vertexDfs.pop();
		}
		//现在final的栈底是终点
		if (!finalResult.empty() && finalResult.top() == pfrom)
		{//路径找到了处理一个输出
			cout << "最少换乘次数为" << minTransfer-1 << endl;
			cout << "乘坐指南:" << endl;;
			string sLine = LineCopy.top();
			string a,s;
			while (!finalResult.empty())
			{
				a=Graph[finalResult.top()].vertexName;//站点名称

				//int t = finalResult.top(); finalResult.pop();
				if (!LineCopy.empty())
				{
					s = LineCopy.top(); LineCopy.pop();//假设换乘
				}
				if (!LineCopy.empty() && staToLine[a].contains(LineCopy.top()))//换乘
					cout << "在" << a << "换乘" << LineCopy.top() << "->";
				else
				{//不换乘
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
{//查询两点之间直接的边是否存在
	if (v1 == v2)return false;//不存在自环
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
