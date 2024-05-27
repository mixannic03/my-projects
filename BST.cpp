// bst.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <stack>
#include <queue>
template <typename K, typename D> class BST
{
	struct Node // это узел дерева
	{
		Node* pLeft, * pRight;
		K key;
		D data;
		Node(const K& k, const D& dat) // конструктор узла дерева
		{
			key = k;
			data = dat;
			pLeft = NULL;
			pRight = NULL;
		}
	};
	Node* m_pRoot; // корень дерева
	size_t m_nSize; // сколько элементов в дереве
	std::stack<Node*> S;
private:
	bool BSTInsert(Node* pRoot, const K k, const D dat) // итеративная функция вставки
	{
		if (pRoot == NULL)
		{
			pRoot = new Node(k, dat);
			m_pRoot = pRoot;
			m_nSize++;
			return true;
		}
		else
		{
			Node* pCurrent = pRoot;
			Node* pParent = NULL;
			while (pCurrent) {
				pParent = pCurrent;
				if (k == pCurrent->key)
					return false;
				if (k < pCurrent->key)
					pCurrent = pCurrent->pLeft;
				else
					pCurrent = pCurrent->pRight;
			}
			if (k < pParent->key)
				pParent->pLeft = new Node(k, dat);
			else
				pParent->pRight = new Node(k, dat);
		}
		m_nSize++;
		return true;
	}

	Node* RootInsert(Node* pRoot, K& key, D& data, bool& inserted)// Вставка в корень
	{
		// pRoot - Указатель Ha корень дерева (поддерева).
		// key, data - Добавляемые ключ и данные.
		// inserted - Ссылка на флаг успешного добавления.
		if (pRoot == NULL) // Нужный лист нейден
		{
			inserted = true;
			return new Node(key, data);
		}
		if (key == pRoot->key)// Такой ключ существует
		{
			inserted = false;
			return pRoot;
		}
		//Спуск до нужного листа
		if (key < pRoot->key)
		{
			pRoot->pLeft = RootInsert(pRoot->pLeft, key, data, inserted);
			if (inserted) // Элемент был добавлен, нужно вращение
				return R(pRoot);
			else
				return pRoot;
		}
		else
		{
			pRoot->pRight = RootInsert(pRoot->pRight, key, data, inserted);
			if (inserted) // Элемент был добавлен, нужно вращение
				return L(pRoot);
			else
				return pRoot;
		}
	}

	K& tLR_Iterative(Node* pRoot, D& dat)// Итеративный tLR-обход
	{
		if (pRoot == NULL)
			throw "Дерево пустое";
		Node* pTemp = NULL;
		S.push(pRoot);
		while (!S.empty())
		{
			pTemp = S.top();
			S.pop();
			if (pTemp->data == dat)
			{
				return pTemp->key;
			}
			if (pTemp->pRight)
				S.push(pTemp->pRight);
			if (pTemp->pLeft)
				S.push(pTemp->pLeft);
		}
		throw "Элемента с такими данными не сущетсвует";
	}

	Node* BSTDelete(Node* t, K k, bool& deleted)//Удаление элемента
	{
		if (t == NULL)
		{
			deleted = false;
			return t;
		}
		bool del;
		Node* temp;
		if (k < t->key) {
			t->pLeft = BSTDelete(t->pLeft, k, del);
			deleted = del;
			return t;
		}
		if (k > t->key) {
			t->pRight = BSTDelete(t->pRight, k, del);
			deleted = del;
			return t;
		}
		deleted = true;
		m_nSize--;
		if (t->pRight == NULL && t->pLeft == NULL) {
			delete t;
			return NULL;
		}
		if (t->pLeft == NULL) {
			temp = t->pRight;
			delete t;
			return temp;
		}
		if (t->pRight == NULL) {
			temp = t->pLeft;
			delete t;
			return temp;
		}
		t->pRight = Del(t->pRight, t);
		return t;
	}
public:

	BST() // конструктор дерева
	{
		m_pRoot = NULL;
		m_nSize = 0;
	}

	BST(const BST& source) // конструктор копии
	{
		m_pRoot = CopyTree(source.m_pRoot);
		m_nSize = source.m_nSize;
	}

	Node* CopyTree(Node* temp) {
		if (temp == nullptr) {
			return nullptr;
		}
		Node* newNode = new Node(temp->key, temp->data);
		newNode->pLeft = copyTree(temp->pLeft);
		newNode->pRight = copyTree(temp->pRight);
		return newNode;
	}

	~BST() // Деструктор дерева
	{
		deleteTree(m_pRoot);
	}

	void deleteTree(Node* temp) {
		if (temp == nullptr) {
			return;
		}
		deleteTree(temp->pLeft);
		deleteTree(temp->pRight);
		delete temp;
	}

	size_t Size()
	{
		return m_nSize;
	}

	void clear()
	{
		deleteTree(m_pRoot);
	}

	bool Insert(const K& k, const D& dat)// функция обертка для вставки. Нужна потому что при вставке нужен корень,
		//а давать к нему доступ снаружи нельзя, по этому вызываем отсюда приватную функцию вставки, в которую
		// переадим корень
	{
		return BSTInsert(m_pRoot, k, dat);
	}

	Node* L(Node* t)// Левый поворот
	{
		Node* temp = t->pRight;
		t->pRight = temp->pLeft;
		temp->pLeft = t;
		return temp;
	}

	Node* R(Node* t)//Правый поворот
	{
		Node* temp = t->pLeft;
		t->pLeft = temp->pRight;
		temp->pRight = t;
		return temp;
	}


	bool RootIns(K key, D dat)// Обертка для вставки в корень
	{
		bool ins;
		m_pRoot = RootInsert(m_pRoot, key, dat, ins);
		return ins;
	}

	K& TLR(D dat)// Обертка для tLR-обхода
	{
		K ky = tLR_Iterative(m_pRoot, dat);
		if (ky)
			return ky;
		else
			throw"Элемента с такими данными не существует";
	}

	typedef struct Iterator {}*BSTPOSITION;// Итератор
	BSTPOSITION oIterator;
	std::queue<Node*> queue;
	void prepareQueue(Node* node)
	{
		if (node == NULL) {
			return;
		}
		prepareQueue(node->pLeft);
		prepareQueue(node->pRight);
		queue.push(node);
	}
	BSTPOSITION getHead()
	{
		if (m_pRoot == NULL) {
			return NULL;
		}
		prepareQueue(m_pRoot);
		return (BSTPOSITION)&oIterator;
	}
	K getNext(BSTPOSITION& pos)
	{
		if (queue.empty()) {
			throw;
		}
		K tempData;
		Node* curNode = queue.front();
		queue.pop();
		tempData = curNode->key;
		if (queue.empty()) {
			pos = NULL;
		}
		return tempData;
	}

	Node* Del(Node* t, Node* t0) // доп. метод удаления для случая, когда есть и правый и левый потомки
	{
		if (t->pLeft != NULL) {
			t->pLeft = Del(t->pLeft, t0);
			return t;
		}
		t0->key = t->key;
		t0->data = t->data;
		Node* temp;
		temp = t->pRight;
		delete t;
		return temp;
	}

	bool bstDel(K key)// Обертка для удаления
	{
		bool del;
		m_pRoot = BSTDelete(m_pRoot, key, del);
		return del;
	}
};


int main()
{
	setlocale(LC_ALL, "Russian");
	BST<int, int> bst;
	bool flag;
	int c, k, d;
	BST<int, int>::BSTPOSITION position;
	while (true)
	{
		std::cout << "\tМеню\n1:Добавить данные\n2:Поиск ключа по данным\n3:Удалить данные\n4:запрос объема\n5:Вывести данные\n6:Очистить коллекцию\nДругое - Выход\n";
		std::cout << ">>";
		std::cin >> c;
		std::cout << "\n";
		switch (c)
		{
		case 1:
		{
			std::cout << "Введите ключ и данные\n";
			while (!(std::cin >> k)) {
				std::cin.clear();
				std::cout << "Неправильный ввод, введите число\n";
				char l;
				std::cin >> l;
			}
			while (!(std::cin >> d)) {
				std::cin.clear();
				std::cout << "Неправильный ввод, введите число\n";
				char l;
				std::cin >> l;
			}
			flag = bst.Insert(k, d);
			if (flag == true)
				std::cout << "Kлюч и данные успешно добавлены\n";
			else
				std::cout << "Такой ключ уже существует\n";
			break;
		}
		case 2:
		{
			std::cout << "Введите данные\n";
			while (!(std::cin >> d)) {
				std::cin.clear();
				std::cout << "Неправильный ввод, введите число\n";
				char l;
				std::cin >> l;
			}
			try {
				k = bst.TLR(d);
				std::cout << "Ключ по этим данным = " << k << "\n";
			}
			catch (const char* error_message) {
				std::cout << error_message << std::endl;
			}
			break;
		}
		case 3:
		{
			std::cout << "Введите ключ, данные по которому хотите удалить\n";
			while (!(std::cin >> k)) {
				std::cin.clear();
				std::cout << "Неправильный ввод, введите число\n";
				char l;
				std::cin >> l;
			}
			flag = bst.bstDel(k);
			if (flag == true)
				std::cout << "Данные успешно удалены!\n";
			else
				std::cout << "Данных по такому ключю не существует\n";
			break;
		}
		case 4:
		{
			k = bst.Size();
			std::cout << "объем данных = " << k << "\n";
			break;
		}
		case 5:
		{
			position = bst.getHead();
			while (position != NULL) {
				k = bst.getNext(position);
				std::cout << "key " << k << std::endl;
			}
			break;
		}
		case 6:
		{
			bst.clear();
			std::cout << "Коллекция очищена\n";
		}
		default:
			return 0;
		}
	}
}

