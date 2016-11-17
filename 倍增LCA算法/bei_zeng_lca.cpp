#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <string>
using namespace std;
// �����������LCA_���ڶ����������㷨������LCA��
// ����ͼ���������ڽӱ��ʾ
#define MAX_V 200000	// ͼ�ж������������ֵ
#define MAX_LOG_V 17	// MAX_LOG_V = floor[log2(MAX_V)]
vector<int> G[MAX_V];
int rootID;	// ���и��ڵ�ı��
int parentID[MAX_LOG_V][MAX_V];	// ����v������2^k�������Ķ���
int depth[MAX_V]; // �ڵ�����

map<string, int> name2id;
map<int, string> id2name;

int cnt = 0;

int InitTree() {
	rootID = 0;

	int total = 0;

	cin >> total;

	for (int i = 0; i < total; i++) {
		string father, son;
		cin >> father >> son;

		if (name2id.find(father) == name2id.end()) {
			name2id[father] = cnt;
			id2name[cnt] = father;
			cnt++;
		}
		if (name2id.find(son) == name2id.end()) {
			name2id[son] = cnt;
			id2name[cnt] = son;
			cnt++;
		}

		G[name2id[father]].push_back(name2id[son]);
		G[name2id[son]].push_back(name2id[father]);
	}

	return 0;
}

void dfs(int v, int p, int d) {
	// ͨ��������ȱ�����ʼ��
	parentID[0][v] = p;
	depth[v] = d;

	for (int i = 0; i < G[v].size(); i++) {
		if (G[v][i] != p)
			dfs(G[v][i], v, d + 1);
	}
}

void init(int V) {
	// V��ʾ���нڵ������
	// Ԥ��������ⶥ��������һ�������Ķ����depth
	dfs(rootID, -1, 0);

	// Ԥ�����parentID[k]��k<=floor(MAX_LOG_V)��������ȡ��
	// ���Ե����ϵķ�ʽ�����Ž⣬����������ص�������Ľ��
	for (int k = 0; k + 1 < MAX_LOG_V; k++)
		for (int v = 0; v < V; v++) {
			if (parentID[k][v] < 0) {
				parentID[k + 1][v] = -1;
			}
			else {
				// parent2[v]=parent[parent[v]]���õ����������������Ķ���
				parentID[k + 1][v] = parentID[k][(parentID[k][v])];
			}
		}
}

int LCA(int u, int v) {
	// ����u��v��LCA������������ȣ�
	// ��֤u�����С��v
	if (depth[u] > depth[v]) swap(u, v);
	// ��u��v�����ߵ�ͬһ���
	int log_v = (int)(log(depth[v] - depth[rootID]) / log(2));
	for (int i = log_v; i >= 0; i--) {
		if (depth[v] - (1 << i) >= depth[u]) {	// depth[v]-2^i >= depth[u]
			v = parentID[i][v];	// �����ƶ���depth[v]+2^i
		}
	}

	if (u == v)	return u;

	// ���ö�����������LCA
	for (int k = log_v; k >= 0; k--) {
		if (parentID[k][u] != parentID[k][v]) {
			u = parentID[k][u];
			v = parentID[k][v];
		}
	}
	return parentID[0][u];

	// ����һλ��ż����Ϊԭ����һ�룬������Ϊԭ����1��һ��
}

int main(int argc, char** argv) {
	InitTree();
	init(cnt);
	int total = 0;
	cin >> total;
	for (int i = 0; i < total; i++) {
		string a, b;
		cin >> a >> b;
		cout << id2name[LCA(name2id[a], name2id[b])] << endl;
	}
	return 0;
}