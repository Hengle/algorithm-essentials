#include <iostream>
#include <vector>
using namespace std;

#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)
#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)

typedef struct ListElmt_ {
	void *data;
	struct ListElmt_ *representative; // ָ����е�һ�����󣨼�������ָ��
	struct ListElmt_ *next; // ָ�������һ�����ָ��
}ListElmt;

typedef struct List_ {
	int size;
	void(*destroy)(void *data);
	ListElmt *head;
	ListElmt *tail;
}List;

void list_init(List *list, void(*destroy)(void *data)) {	// ��ʼ������
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;
}

int list_ins_next(List *list, ListElmt *element, const void *data) {
	// ��listָ����������element�������һ����Ԫ��
	ListElmt *new_element;
	if ((new_element = (ListElmt *)malloc(sizeof(ListElmt))) == NULL)
		return -1;
	new_element->data = (void *)data; // ǿ������ת��

	// ���element����ΪNULL������Ԫ�ز�������ͷ��
	if (element == NULL) {
		if (list_size(list) == 0)
			list->tail = new_element;

		new_element->next = list->head;
		list->head = new_element;

		/* ��֤ÿ�����еĵ�һ�����������ڼ��ϵĴ��� */
		ListElmt *ptr = list->head;
		while (ptr != NULL)
		{
			ptr->representative = list->head;
			ptr = ptr->next;
		}
	}
	else {
		// �������Ԫ��λ������ĩβʱ������tailʹ��ָ���µĽ��
		if (element->next == NULL)
			list->tail = new_element;

		new_element->next = element->next;
		element->next = new_element;

		/* ��Ԫ�صĴ���ָ��ָ�������еĵ�һ������ */
		new_element->representative = list->head;
	}

	list->size++;

	return 0;
}

// ���÷��غ�dataָ�����Ƴ�Ԫ���д洢������
// �ɵ����߸������data�����õĴ洢�ռ�
int list_rem_next(List *list, ListElmt *element, void **data) {
	// ���������Ƴ���element��ָ����Ԫ��֮����Ǹ����
	ListElmt *old_element;
	if (list_size(list) == 0)
		return -1;
	// �����ͷ�����Ҫ�Ƴ�
	if (element == NULL) {
		*data = list->head->data;
		old_element = list->head;
		list->head = list->head->next;
		// �Ƴ�����ʹ�����������Ϊ������
		if (list_size(list) == 1)			list->tail = NULL;

		/* ��֤ÿ�����еĵ�һ�����������ڼ��ϵĴ��� */
		ListElmt *ptr = list->head;
		while (ptr != NULL)
		{
			ptr->representative = list->head;
			ptr = ptr->next;
		}
	}
	// �Ƴ�����λ���ϵĽ��
	else {
		if (element->next == NULL)
			return -1;
		*data = element->next->data;
		old_element = element->next;
		element->next = element->next->next;
		// �Ƴ���Ŀ�����������β���
		if (element->next == NULL)
			list->tail = element;
	}
	free(old_element);
	list->size--;
	return 0;
}

void list_destroy(List *list) {
	void  *data;
	while (list_size(list) > 0) {
		if (list_rem_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL) {
			// �������list_initʱdestroy������ΪNULL
			// ���Ƴ�������ÿ��Ԫ��ʱ�����øú���һ��
			list->destroy(data);
		}
	}
	memset(list, 0, sizeof(List));
}

vector<List *> DisjointSet; // collection of disjoint dynamic sets

// ����һ���µļ��ϣ���Ψһ��Ա��������������x
List *makeSet(void *x) {
	List *new_list = NULL;
	new_list = (List *)malloc(sizeof(List));
	list_init(new_list, NULL); // �����п�
	list_ins_next(new_list, NULL, x);
	DisjointSet.push_back(new_list); // adds to collection of sets
	return new_list;
}

// ����һ��ָ�룬ָ�����x�ģ�Ψһ�����ϵĴ���
template <typename DataType>
ListElmt *findSet(DataType *x) {
	for (int i = 0; i < DisjointSet.size(); i++) {
		ListElmt *ptr = DisjointSet[i]->head;
		while (ptr != NULL)
		{
			if (*(DataType *)(ptr->data) == *x) {
				return ptr->representative;
			}
			else {
				ptr = ptr->next;
			}
		}
	}

	return NULL;
}

// ������x��y�Ķ�̬���ϣ��ϲ�Ϊһ���µļ���
template <typename DataType>
List *unionSet(DataType *x, DataType *y) {
	ListElmt *rep_a = findSet(x);
	ListElmt *rep_b = findSet(y);
	List *a = NULL;
	List *b = NULL;
	vector<List *>::iterator it;
	for (it = DisjointSet.begin(); it != DisjointSet.end(); it++) {
		if ((*it)->head != NULL && (*it)->head->representative == rep_a) {
			a = *it;
		}
		if ((*it)->head != NULL && (*it)->head->representative == rep_b) {
			b = *it;
		}
	}

	if (a->size > b->size) {
		swap(a, b); // ��������a�ĳ��ȱ�b��
	}

	// ������aƴ�ӵ�b�ı�β
	b->tail->next = a->head;
	b->tail = a->tail;
	// ��������a�е�ÿһ�����󣬶��������ָ������ָ��
	ListElmt *ptr = a->head;
	while (ptr != NULL) {
		ptr->representative = b->head->representative;
		ptr = ptr->next;
	}

	for (it = DisjointSet.begin(); it != DisjointSet.end(); ) {
		if ((*it) == a)
			it = DisjointSet.erase(it);
		else
			it++;
	}

	free(a);

	return b;
}

int destroySet() {
	for (int i = 0; i < DisjointSet.size(); i++) {
		List *ptr = DisjointSet[i];
		list_destroy(ptr);
	}
	return 0;
}

template <typename DataType>
void printSet() {
	cout << "��ʼ��ӡ��" << endl;
	for (int i = 0; i < DisjointSet.size(); i++) {
		ListElmt *ptr = DisjointSet[i]->head;
		cout << "���ڴ�ӡ����Ϊ" << *(DataType *)ptr->representative->data << "�ļ��ϣ�";
		while (ptr != NULL)
		{
			cout << *(DataType *)ptr->data << " ";
			ptr = ptr->next;
		}
		cout << endl;
	}
	cout << "��ӡ������" << endl << endl;
}

int main() {
	int a = 1;
	makeSet((void *)&a);
	printSet<int>();
	int b = 2;
	makeSet((void *)&b);
	printSet<int>();
	int c = 3;
	makeSet((void *)&c);
	printSet<int>();
	int d = 4;
	makeSet((void *)&d);
	printSet<int>();

	unionSet<int>(&a, &b);
	printSet<int>();
	unionSet<int>(&c, &d);
	printSet<int>();
	unionSet<int>(&a, &c);
	printSet<int>();

	destroySet();

	system("pause");
	return 0;
}