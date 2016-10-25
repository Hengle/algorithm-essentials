#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

#define MAX_N 100

int p[MAX_N]; // p[x]��ʾx�ĸ����
int rank_[MAX_N]; // ���ȱ�ʾ���x�߶ȵ�һ���Ͻ�

void makeSet(int x) {
	p[x] = x; // ÿ�����ĸ������Լ��ĸ����
	rank_[x] = 0; // ����Ψһ���ĳ�ʼ��Ϊ0
}

// ��ѯ���ĸ�
int findSet(int x) {
	// ·��ѹ��ʹ����·���ϵ�ÿ����㶼ֱ��ָ����ڵ�
	// ·��ѹ�����ı������
	if (x != p[x])
		p[x] = findSet(p[x]);
	return p[x];
}

// �ϲ�x��y�����ļ��ϣ����Ⱥϲ�
void unionSet(int x, int y) {
	x = findSet(x);
	y = findSet(y);

	if (x == y) {
		return;
	}

	if (rank_[x] < rank_[y]) {
		// ���нϸ��ȵĸ���Ϊ���нϵ��ȵĸ��ĸ����
		p[x] = y;
	}
	else if (rank_[x] > rank_[y]) {
		p[y] = x;
	}
	else {
		p[y] = x; // ��ѡһ������Ϊ����㣬���������ȵ�ֵ
		rank_[x]++; // ע������x�ĸ߶ȶ�����y
	}
}

#define MAX_E 1000

int V; // ͼ�ж�����
int E; // ͼ�б���

// �Ӷ���uָ�򶥵�v��ȨֵΪcost�ı�
struct edge
{
	int u;
	int v;
	int cost;
};

edge es[MAX_E]; // ��

vector<edge> A; // ����A��һ��ɭ�֣���ĳ����С��������һ���Ӽ�
// ���뼯��A�еİ�ȫ������ͼ������������ͬ��ͨ��֧����СȨ��

bool cmp(const edge& e1, const edge& e2) {
	return e1.cost < e2.cost;
}

int InitGraph() {
	V = 9;
	E = 13;
	es[0] = { 0, 1, 4 };
	es[1] = { 1, 2, 8 };
	es[2] = { 2, 3, 7 };
	es[3] = { 3, 4, 9 };
	es[4] = { 4, 5, 10 };
	es[5] = { 5, 6, 2 };
	es[6] = { 6, 7, 1 };
	es[7] = { 7, 0, 8 };
	es[8] = { 7, 8, 7 };
	es[9] = { 6, 8, 6 };
	es[10] = { 8, 2, 2 };
	es[11] = { 7, 1, 11 };
	es[12] = { 2, 5, 4 };
	return 0;
}

int kruskal() {
	A.clear(); // ������A��ʼ��Ϊ�ռ�

	// ����|V|������ÿ������������ͼ��һ������
	for (int i = 0; i < V; i++) {
		makeSet(i);
	}

	// ����Ȩֵ�ķǵݼ�˳�򣬶�es�еı߽�������
	sort(es, es + E, cmp);

	for (int i = 0; i < E; i++) {
		// ���ÿ���ߵĶ˵�u��v�Ƿ�����ͬһ����������Ǿͻ��γ�һ����·
		if (findSet(es[i].u) != findSet(es[i].v)) { // ������������ڲ�ͬ����
			A.push_back(es[i]); // �ѱ߼��뼯��A��
			unionSet(es[i].u, es[i].v); // ���������еĶ�����кϲ�
		}
	}
	return 0;
}

int main() {
	InitGraph();
	kruskal();
	cout << "��ӡĳһ��С�������ıߣ�" << endl;
	for (int i = 0; i < A.size(); i++) {
		cout << "���Ӷ���" << A[i].u << "�Ͷ���" << A[i].v << "�ı�" << endl;
	}
	system("pause");
	return 0;
}