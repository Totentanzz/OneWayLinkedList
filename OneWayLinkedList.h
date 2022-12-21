#pragma once
#include "Node.h"
#include <algorithm>
#include <fstream>

template<class T,int ppSize>
class OneWayLinkedList {
public:

	OneWayLinkedList() {
		init();
	}

	OneWayLinkedList(std::initializer_list<T> initList) {
		this->listSize = 0;
		const T* beginP = initList.begin();
		T* endP = const_cast<T*>(initList.end())-1;
		T* beginPCpy = const_cast<T*>(beginP);
		int elemNum = initList.size(), curElemNum=0;
		Node<T, ppSize>* tmpNode1, *tmpNode2;
		tmpNode1 = new Node<T,ppSize>(beginPCpy,endP);
		this->pHead = tmpNode1;
		curElemNum += tmpNode1->getDataNum();
		beginPCpy += tmpNode1->getDataNum();
		this->listSize++;
		while (curElemNum < elemNum) {
			tmpNode2 = new Node < T, ppSize>(beginPCpy, endP,NULL);
			tmpNode1->getNext() = tmpNode2;
			tmpNode1 = tmpNode2;
			curElemNum += tmpNode1->getDataNum();
			beginPCpy += tmpNode1->getDataNum();
			this->listSize++;
		}
		this->pTail = tmpNode1;
	}

	OneWayLinkedList(const OneWayLinkedList& other) {
		if (other.listSize == 0) init();
		else {
			this->listSize = other.listSize;
			Node<T, ppSize>* othNode, * tmpNode1, * tmpNode2;
			othNode = other.pHead;
			tmpNode1 = new Node<T, ppSize>(*othNode);
			this->pHead = tmpNode1;
			for (othNode = othNode->getNext(); othNode != NULL; othNode = othNode->getNext()) {
				tmpNode2 = new Node<T, ppSize>(*othNode);
				tmpNode1->getNext() = tmpNode2;
				tmpNode1 = tmpNode2;
			}
			this->pTail = tmpNode1;
		}
	}

	OneWayLinkedList(OneWayLinkedList&& other) {
		this->listSize = other.listSize;
		other.listSize = 0;
		this->pHead = other.pHead;
		other.pHead = NULL;
		this->pTail = other.pTail;
		other.pTail = NULL;
	}

	~OneWayLinkedList() {
		Node<T, ppSize>* curNode;
		while (this->listSize != 0) {
			curNode = this->pHead;
			this->pHead = curNode->getNext();
			delete curNode;
			this->listSize--;
		}
	}

	void push(T& elem) {
		if (this->pHead == NULL)
			Node <T, ppSize>* newNode = this->insertNewNode();
		if (this->pTail->getDataNum() < ppSize) this->pTail->pushElem(elem);
		else {
			Node<T, ppSize>* oldTail = this->pTail;
			Node<T, ppSize>* newNode = this->insertNewNode(oldTail);
			int rest = ppSize % 2;
			int cpyIndex = (ppSize % 2 == 0) ? (ppSize / 2) : (ppSize / 2 + rest);
			int cpyNum = ppSize - cpyIndex;
			int newNodeIndex = 0;
			newNode->cpyData(oldTail,cpyIndex,cpyNum,newNodeIndex);
			newNode->pushElem(elem);
		}
	}

	T* erase(const int& cPpIndex) {
		if (cPpIndex < 0)
			throw out_of_range("erase: listIndex or ppIndex is out of range");
		bool state = false;
		int ppIndex = cPpIndex;
		Node<T, ppSize>* curNode = this->findByIndex(ppIndex);
		T* elem = NULL;
		if (curNode != NULL) {
			elem = curNode->eraseElem(ppIndex);
			if (curNode->getDataNum() == 0) {
				this->deleteNode(curNode);
			}
		}
		return elem;
	}

	bool insert(T& elem, const int& cPpIndex) {
		if (cPpIndex < 0)
			throw out_of_range("insert: index is out of range");
		bool state = false;
		int ppIndex = cPpIndex;
		Node<T, ppSize>* curNode = this->findByIndex(ppIndex);
		if (curNode != NULL) {
			state = curNode->insertElem(elem, ppIndex);
			if ((state == false) && (curNode->getDataNum() == ppSize)) {
				Node<T, ppSize>* nextNode = curNode->getNext();
				Node<T, ppSize>* newNode = this->insertNewNode(curNode, nextNode);
				int rest = ppSize % 2;
				int cpyIndex = (ppSize % 2 == 0) ? (ppSize / 2) : (ppSize / 2 + rest);
				int cpyNum = ppSize - cpyIndex;
				int newNodeIndex = 0;
				newNode->cpyData(curNode, cpyIndex, cpyNum, newNodeIndex);
				curNode = (ppIndex < cpyIndex) ? curNode : newNode;
				int curDataNum = curNode->getDataNum();
				ppIndex = (curNode != newNode) ? ppIndex : (ppIndex - (curDataNum + rest));
				state = curNode->insertElem(elem, ppIndex);
			}
		}
		else if (ppIndex == 0) {
			Node<T, ppSize>* newNode = this->insertNewNode(this->pTail);
			state = newNode->insertElem(elem, ppIndex);
		}
		return state;
	}

	T* get(const int& cPpIndex) {
		if (cPpIndex < 0)
			throw out_of_range("insert: index is out of range");
		int ppIndex = cPpIndex;
		T* elem = NULL;
		Node<T, ppSize>* curNode = this->findByIndex(ppIndex);
		if (curNode != NULL) elem = curNode->getElem(ppIndex);
		return elem;
	}

	void sort() {
		auto cmp = [](const T* a, const T* b) -> int
		{
			int state;
			if (a == NULL && b == NULL) state = 0;
			else if (a == NULL) state = 0;
			else if (b == NULL) state = 1;
			else state = ( *a < *b);
			return state;
		};
		bool state1 = false, state2 = (this->pTail==NULL) ? true : false;
		int arrSize = this->listSize * ppSize;
		T** arr = this->getAllPointers(state1);
		std::sort(arr, arr + arrSize, cmp);
		this->replaceAllPointers(arr, arrSize);
		delete[] arr;
		while ((state2 == false) && (this->pTail->getDataNum() == 0)) {
			this->deleteNode(this->pTail);
			state2 = (this->pTail == NULL) ? true : false;
		}
	}

	void insertKeepOrd(T& elem) {
		bool state = false;
		int ppIndex = this->findBiggerElem(elem);
		int totalElem = this->totalElemNum();
		if (ppIndex == totalElem) this->push(elem);
		else this->insert(elem, ppIndex);
	}

	void balance() {
		bool state1 = false, state2 = false;
		int totalElem = this->totalElemNum();
		//int curListSize = this->listSize;
		int curListSize = this->totalNodes();
		int averageSize = (curListSize > 0) ? (totalElem / curListSize) : 0;
		Node<T, ppSize>* curNode = this->pHead;
		Node<T,ppSize>* nextNode = (curNode!=NULL) ? curNode->getNext() : NULL;
		if (averageSize * (this->listSize) < totalElem) this->insertNewNode(this->pTail);
		while ((curNode != NULL) && (nextNode != NULL)) {
			state1 = this->balanceTwoNodes(curNode, nextNode, averageSize);
			curNode = nextNode;
			nextNode = nextNode->getNext();
			if (state1 == false) state2 = true;
		}
		if (state2 == true) this->balance();
	}

	void writeToFile(const char* file, ios::_Openmode mode=ios::out) {
		ofstream ofstrm(file, ios::trunc);
		if (!ofstrm)
			throw ofstream::failure("Can't open file");
		Node<T, ppSize>* curNode = this->pHead;
		void (Node<T, ppSize>:: * saveToFile) (const char*);
		if (mode == ios::binary) saveToFile = &Node<T, ppSize>::writeToBinFile;
		else saveToFile = &Node<T, ppSize>::writeToFile;
		for (; curNode != NULL; curNode = curNode->getNext()) {
			(curNode->*saveToFile)(file);
		}
		ofstrm.close();
	}

	void readFromFile(const char* file, ios::_Openmode mode = ios::in) {
		if (this->pHead == NULL)
			this->insertNewNode();
		int startPos = 0;
		bool state = false;
		Node<T, ppSize>* curNode = this->pHead;
		void (Node<T, ppSize>:: * loadFromFile) (const char*, int&);
		if (mode == ios::binary) loadFromFile = &Node<T, ppSize>::readFromBinFile;
		else loadFromFile = &Node<T, ppSize>::readFromFile;
		for (; curNode != NULL; curNode = curNode->getNext()) {
			if ((startPos != -1) && (curNode->getNext() == NULL)) {
				this->insertNewNode(curNode);
				(curNode->*loadFromFile)(file, startPos);
			}
			else if (startPos!=-1) (curNode->*loadFromFile)(file, startPos);
			else curNode->setNULL();
		}
		while ((state == false) && (this->pTail->getDataNum() == 0)) {
			this->deleteNode(this->pTail);
			state = (this->pTail == NULL) ? true : false;
		}
	}

	friend istream& operator>>(istream& in, OneWayLinkedList& list) {
		if (list.pHead == NULL)
			list.insertNewNode(list.pTail);
		int n = 0;
		Node<T, ppSize>* curNode = list.pHead;
		cout << " ол-во заполненных узлов, размерностью " << ppSize <<": ";
		in >> n;
		for (size_t i = 0; (n>0) && (curNode != NULL); curNode = curNode->getNext(), ++i,--n) {
			in >> curNode;
			if ((n-1 > 0) && (curNode->getNext() == NULL)) list.insertNewNode(list.pTail);
		}
		return in;
	}

	friend ostream& operator<<(ostream& out, OneWayLinkedList& list) {
		Node<T, ppSize>* curNode = list.pHead;
		int totalElem = list.totalElemNum();
		char div[] = "--------------------------------------------------------";
		for (size_t i = 0; curNode != NULL; curNode = curNode->getNext(), ++i) {
			out << "[" << i << "]:" << div << div << "\n\n\n";
			out << curNode;
			out << "\n\n";
		}
		cout << " оличество элементов: " << totalElem;
		cout << ", количество узлов: " << list.listSize;
		cout << ", размерность массива указателей: " << ppSize << endl;
		return out;
	}

protected:
private:
	int listSize;
	Node<T, ppSize>* pHead;
	Node<T, ppSize>* pTail;

	void init() {
		this->listSize = 0;
		this->pHead = this->pTail = NULL;
	}

	Node<T, ppSize>* findFreeP() {
		Node<T, ppSize>* curNode = this->pHead;
		while ((curNode != NULL) && (curNode->getDataNum() == ppSize))
			curNode = curNode->getNext();
		return curNode;
	}

	Node<T, ppSize>* findByIndex(int& ppIndex) {
		Node<T, ppSize>* curNode = this->pHead, *nextNode;
		bool state = false;
		int realNum = ppIndex + 1, curDataNum = 0, nextDataNum = 0;
		if (ppIndex >= 0) {
			while ((state == false) && (curNode!=NULL)) {
				curDataNum = curNode->getDataNum();
				nextNode = curNode->getNext();
				nextDataNum = (nextNode == NULL) ? 0 : (nextNode->getDataNum());
				if ((realNum <= curDataNum) || ((curDataNum + 1 == realNum) && 
					(nextDataNum == 0))) state = true;
				else {
					realNum -= curDataNum;
					curNode = nextNode;
				}
			}
		}
		ppIndex = (ppSize>1) ? realNum - 1 : 1;
		return curNode;
	}

	int& findBiggerElem(T& elem) {
		bool state = false;
		int totalElem = this->totalElemNum(), ppIndex=0;
		Node<T, ppSize>* curNode = this->pHead;
		while ((curNode != NULL) && (state == false)) {
			state = curNode->firstBigger(elem,ppIndex);
			curNode = (state == false) ? curNode->getNext() : curNode;
		}
		ppIndex = (ppIndex > totalElem) ? totalElem : ppIndex;
		return ppIndex;
	}

	Node<T, ppSize>* previousNode(Node<T, ppSize>*node) {
		if (node == NULL)
			throw out_of_range("previousNode: node pointer is null");
		Node<T, ppSize>* prevNode = this->pHead;
		if (node != this->pHead) {
			while (prevNode->getNext() != node)
				prevNode = prevNode->getNext();
		}
		return prevNode;
	}

	Node<T, ppSize>* insertNewNode(Node<T, ppSize>*nodeL = NULL, Node<T, ppSize>*nodeR = NULL) {
		Node<T, ppSize>* newNode = new Node<T, ppSize>;
		if (nodeL != NULL) {
			nodeL->getNext() = newNode;
			newNode->getNext() = nodeR;
		}
		else if ((nodeL == NULL) && (nodeR == NULL)) this->pHead = newNode;
		if (nodeR == NULL) this->pTail = newNode;
		this->listSize++;
		return newNode;
	}

	void deleteNode(Node<T, ppSize>*delNode) {
		if (delNode == NULL)
			throw invalid_argument("deleteNode: deleted node pointer is null");
		Node<T, ppSize>* prevNode = this->previousNode(delNode);
		if (delNode == this->pHead) this->pHead = delNode->getNext();
		else if (delNode == this->pTail) {
			prevNode->getNext() = NULL;
			this->pTail = prevNode;
		}
		else prevNode->getNext() = delNode->getNext();
		if (this->pHead == NULL) this->pTail = NULL;
		delete delNode;
		this->listSize--;
	}

	T** getAllPointers(bool& state) {
		int arrSize = (this->listSize * ppSize + 1);
		T** arr = new T*[arrSize], *elem;
		size_t i = 0, j = 0;
		while (i < arrSize - 1) {
			elem = this->get(i++);
			if (state == false) arr[j++] = elem;
			else if (elem != NULL) arr[j++] = elem;
		}
		arr[j] = NULL;
		return arr;
	}

	int& findFirstElem(T * *arr, const int& arrSize) {
		int ppIndex = 0;
		for (; (ppIndex < arrSize) && (arr[ppIndex] == NULL); ++ppIndex);
		return ppIndex;
	}

	void replaceAllPointers(T * *arrP, const int& arrPSize)
	{
		int arrPIndex = 0;
		int totalElem = this->totalElemNum(), curElemNum;
		Node<T, ppSize>* curNode = this->pHead;
		for (; curNode != NULL; curNode = curNode->getNext()) {
			curElemNum = (totalElem > ppSize) ? ppSize : totalElem;
			curNode->replaceElem(arrP, arrPSize, curElemNum, arrPIndex);
			totalElem = (totalElem - ppSize > 0) ? (totalElem - ppSize) : 0;
		}
	}

	int& totalElemNum() {
		int totalElem = 0;
		Node<T, ppSize>* curNode = this->pHead;
		for (; curNode != NULL; curNode = curNode->getNext())
			totalElem += curNode->getDataNum();
		return totalElem;
	}

	int& totalNodes() {
		int totalNodes = 0;
		Node<T, ppSize>* curNode = this->pHead;
		for (; curNode != NULL; curNode = curNode->getNext())
			if (curNode->getDataNum() != 0) totalNodes++;
		return totalNodes;
	}

	bool& balanceTwoNodes(Node<T,ppSize>* curNode, Node<T,ppSize>* nextNode, int& averageNum) {
		bool state = false;
		int curDataNum = curNode->getDataNum(), nextDataNum;
		int cpyIndex, curIndex, cpyDataNum, freeNum;
		if (nextNode != NULL && curDataNum != averageNum) {
			if (curDataNum < averageNum) {
				cpyIndex = 0;
				cpyDataNum = averageNum - curDataNum;
				curIndex = curDataNum;
				freeNum = ppSize - curDataNum;
				//if (cpyDataNum<=freeNum) 
				curNode->cpyData(nextNode, cpyIndex, cpyDataNum,curIndex);
			}
			else if (curDataNum > averageNum) {
				nextDataNum = nextNode->getDataNum();
				cpyDataNum = curDataNum - averageNum;
				cpyIndex = curDataNum - cpyDataNum;
				curIndex = 0;
				freeNum = ppSize - nextDataNum;
				//if (cpyDataNum<=freeNum)
				nextNode->cpyData(curNode, cpyIndex, cpyDataNum, curIndex);
			}
			state = (curNode->getDataNum() == averageNum) ? true : false;
		}
		return state;
	}
};