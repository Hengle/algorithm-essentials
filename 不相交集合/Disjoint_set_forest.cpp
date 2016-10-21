#include <iostream>
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

int main() {
	makeSet(1);
	makeSet(2);
	makeSet(3);
	makeSet(4);
	makeSet(5);
	unionSet(1, 2);
	unionSet(3, 4);
	unionSet(3, 5);
	unionSet(1, 3);
	for (int i = 1; i <= 5; i++) {
		cout << findSet(i) << " ";
	}
	cout << endl;
	system("pause");
	return 0;
}