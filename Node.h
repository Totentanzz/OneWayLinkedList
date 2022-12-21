#pragma once
#include <iostream>

using namespace std;

template <class T,int ppSize>
class Node {
public:

	Node() {
		this->dataNum = 0;
		this->next = NULL;
		memset(this->data, NULL, ppSize * sizeof(T*));
	}

	Node(T& data, Node* pNext = NULL) {
		this->dataNum = 1;
		this->data[0] = new T(data);
		this->next = pNext;
	}

	Node(initializer_list<T> elemList, Node* pNext = NULL) {
		if (elemList.size() > ppSize)
			throw std::length_error("Number of elements in initializer list exceeds ppSize");
		this->dataNum = elemList.size();
		const T* beginP = elemList.begin();
		for (size_t i = 0; i < this->dataNum; ++i, ++beginP) {
			this->data[i] = new T(*beginP);
		}
		this->next = pNext;
	}

	Node(T* beginP, T* endP, Node* pNext = NULL) {
		this->dataNum = 0;
		memset(this->data, NULL, ppSize * sizeof(T*));
		T* beginPCpy = beginP;
		for (size_t i = 0; (i < ppSize) && (beginPCpy<=endP); ++i,++beginPCpy) {
			this->data[i] = new T(beginP[i]);
			this->dataNum++;
		}
		this->next = pNext;
	}

	Node(const Node& other) {
		this->dataNum = other.dataNum;
		for (size_t i = 0; i < ppSize; ++i) {
			this->data[i] = new T(*other.data[i]);
		}
		this->next = other.next;
	}

	Node(Node&& other) noexcept {
		this->dataNum = other.dataNum;
		other.dataNum = 0;
		for (size_t i = 0; i < ppSize; ++i) {
			this->data[i] = other.data[i];
			other.data[i] = NULL;
		}
		this->next = other.next;
		other.next = NULL;
	}

	~Node() {
		this->clear<T>(0, ppSize);
	}

	int& getDataNum() {
		return this->dataNum;
	}

	T** getData() {
		return this->data;
	}

	Node*& getNext() {
		return this->next;
	}

	void setNULL() {
		int ppIndex = 0, curDataNum = this->dataNum;
		this->clear<T>(ppIndex, curDataNum);
	}

	void cpyData(Node* node, const int& cpyIndex, const int& cpyNum, const int& curIndex) {
		if (cpyIndex < 0 || curIndex < 0 || cpyIndex >= ppSize || curIndex >= ppSize)
			throw out_of_range("cpyData: indexes are out of bounds");
		int nodeDataNum = (node!=NULL) ? node->dataNum : 0;
		int thisDataNum = this->dataNum;
		int thisFreeNum = ppSize - thisDataNum;
		if ((node != NULL) && (cpyNum>0) && (cpyNum<=thisFreeNum)) {
			T** thisData = this->data + curIndex, **nodeData = node->data + cpyIndex;
			this->shiftRight(curIndex,cpyNum);
			memcpy(thisData, nodeData, cpyNum * sizeof(T*));
			memset(nodeData, NULL, cpyNum * sizeof(T*));
			node->shiftLeft(cpyIndex,cpyNum);
			this->dataNum += cpyNum;
			node->dataNum -= cpyNum;
		}
	}

	bool pushElem(T& elem) {
		bool state = false;
		for (size_t i = 0; (i < ppSize) && (state == false); ++i) {
			if (this->data[i] == NULL) {
				this->data[i] = new T(elem);
				this->dataNum++;
				state = true;
			}
		}
		return state;
	}

	T* eraseElem(const int& index) {
		if (index < 0)
			throw out_of_range("eraseElem: index is negative");
		T* elem = NULL;
		if ((index < ppSize) && (this->data[index]!=NULL)) {
			elem = this->data[index];
			this->shiftLeft(index);
			this->dataNum--;
		}
		return elem;
	}

	bool insertElem(T& elem, const int& index) {
		if (index < 0)
			throw out_of_range("pushElem: index is negative");
		bool state = false;
		if ((index < ppSize) && (this->dataNum < ppSize)) {
			if (this->data[index] != NULL) this->shiftRight(index);
			this->data[index] = new T(elem);
			this->dataNum++;
			state = true;
		}
		return state;
	}

	T* getElem(const int& index) {
		if (index < 0)
			throw out_of_range("getElem: index is negative");
		T* elem = index < ppSize ? this->data[index] : NULL;
		return elem;
	}

	bool& firstBigger(T& elem, int& ppIndex) {
		int index = 0, nullNum=0;
		bool state = false;
		T* curElem = NULL;
		while ((index < ppSize) && (state == false)) {
			curElem = this->data[index];
			if ((curElem!=NULL) && (elem < *curElem))
				state = true;
			else {
				nullNum += (curElem == NULL) ? 1 : 0;
				++index;
			}
		}
		ppIndex += (index - nullNum);
		return state;
	}

	int firstFree(int ppIndex) {
		for (; (ppIndex < ppSize) && (this->data[ppIndex] != NULL); ++ppIndex);
		return ppIndex;
	}

	void replaceElem(T** arrP,const int& arrPSize, const int& elemNum, int& arrPIndex) {
		if ((elemNum > ppSize) || (arrPIndex > arrPSize))
			throw out_of_range("replaceElem: elemNum is bigger than ppSize");
		T** thisPP = this->data;
		T** arrPPos = arrP + arrPIndex;
		memcpy(thisPP, arrPPos, elemNum * sizeof(T*));
		memset(thisPP + elemNum, 0, (ppSize - elemNum) * sizeof(T*));
		this->dataNum = elemNum;
		arrPIndex += elemNum;
	}

	void writeToFile(const char* file) {
		ofstream ofstrm(file, ios::app);
		if (!ofstrm)
			throw ofstream::failure("Can't open file");
		for (size_t i = 0; i < this->dataNum; ++i) {
			ofstrm << *this->data[i] << endl;
		}
		ofstrm.close();
	}

	void readFromFile(const char* file, int& startPos) {
		ifstream ifstrm(file, ios::in);
		if (!ifstrm)
			throw ifstream::failure("Can't open file");
		T elem;
		int ppIndex = 0;
		this->clear<T>(ppIndex, this->dataNum);
		ifstrm.seekg(startPos);
		for (size_t i = 0; (i < ppSize) && (ifstrm >> elem); ++i) {
				this->data[i] = new T(elem);
				this->dataNum++;
		}
		startPos = ifstrm.tellg();
		ifstrm.close();
	}

	void writeToBinFile(const char* file) {
		ofstream ofstrm(file, ios::binary | ios::app);
		if (!ofstrm)
			throw ofstream::failure("Can't open file");
		T* elem = NULL;
		char* str = NULL;
		int strLen = 0;
		for (size_t i = 0; i < this->dataNum; ++i) {
			elem = this->data[i];
			str = &(*elem)[0];
			strLen = strlen(str) + 1;
			ofstrm.write((char*)&strLen, sizeof(int));
			ofstrm.write(str, strLen);
		}
		ofstrm.close();
	}

	void readFromBinFile(const char* file, int& startPos) {
		ifstream ifstrm(file, ios::binary | ios::in);
		if (!ifstrm)
			throw ifstream::failure("Can't open file");
		char* str = NULL;
		int strLen = 0, ppIndex = 0;
		this->clear<T>(ppIndex, this->dataNum);
		ifstrm.seekg(startPos);
		for (size_t i = 0; (i < ppSize) && (ifstrm.peek()!=EOF); ++i) {
			ifstrm.read((char*)&strLen, sizeof(int));
			str = new char[strLen];
			ifstrm.read(str, strLen);
			this->data[i] = new T(str);
			this->dataNum++;
			delete[] str;
		}
		startPos = ifstrm.tellg();
		ifstrm.close();
	}

	friend istream& operator>>(istream& in, Node* node) {
		T curElem;
		int ppIndex = 0;
		node->clear<T>(ppIndex, node->dataNum);
		for (size_t i = 0; i < ppSize; ++i) {
			cout << "Введите данные: ";
			in >> curElem;
			while (!in) {
				in.clear();
				in.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Некорректные данные, повторите ввод: ";
				in >> curElem;
			}
			node->data[i] = new T(curElem);
			node->dataNum++;
		}
		return in;
	}

	friend ostream& operator<<(ostream& out, Node* node) {
		T* curElem = NULL;
		for (size_t i = 0; i < ppSize; ++i) {
			curElem = node->data[i];
			out << "[" << i << "]:\t";
			if (curElem == NULL) out <<"NULL";
			else out << (*curElem);
			out << "\n";
		}
		return out;
	}

protected:
private:
	int dataNum;
	T* data[ppSize>0 ? ppSize : -ppSize];
	Node* next=NULL;

	void shiftRight(const int& ppIndex, const int& shiftPos=1) {
		if ((ppIndex < 0) || (shiftPos < 0))
			throw out_of_range("shiftRight: index is negative");
		T** pos = this->data + ppIndex;
		int elemNum = (this->dataNum - ppIndex >0) ? (this->dataNum - ppIndex) : 0;
		memmove(pos + shiftPos, pos, elemNum * sizeof(T*));
		memset(pos, NULL, shiftPos * sizeof(T*));
		//this->data[ppIndex] = NULL;
	}

	void shiftLeft(const int& ppIndex, const int& shiftPos=1) {
		if ((ppIndex < 0) || (shiftPos < 0))
			throw out_of_range("shiftLeft: index is negative");
		T** pos = this->data + ppIndex;
		int elemNum = ppSize - ppIndex - shiftPos;
		memmove(pos, pos + shiftPos, elemNum * sizeof(T*));
		memset(pos + elemNum, NULL, shiftPos * sizeof(T*));
		//this->data[ppIndex + elemNum] = NULL;
	}

	template<class T>
	void clear(const int& ppIndex, const int& elemNum) {
		for (size_t i = ppIndex; (i < ppIndex + elemNum) && (i < ppSize); ++i) {
			delete[] this->data[i];
		}
		memset(this->data+ppIndex, NULL, elemNum * sizeof(T*));
		this->dataNum = 0;
	}

	template<>
	void clear<string>(const int& ppIndex, const int& elemNum) {
		for (size_t i = ppIndex; (i < ppIndex + elemNum) && (i < ppSize); i++) {
			delete this->data[i];
		}
		memset(this->data + ppIndex, NULL, elemNum * sizeof(T*));
		this->dataNum = 0;
	}

};
