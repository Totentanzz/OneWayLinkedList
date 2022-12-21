#include <iostream>
#include <chrono>
#include "OneWayLinkedList.h"
#include "createdString.h"

using namespace std;

void printMethods() {
	system("cls");
	cout << "\n";
	cout << " [1] - ���������� � ����� (push)\n";
	cout << " [2] - �������� �� �� ����������� ������ (erase)\n";
	cout << " [3] - ��������� �� ����������� ������ (insert)\n";
	cout << " [4] - ���������� �� ����������� ������ (get)\n";
	cout << " [5] - ���������� (sort)\n";
	cout << " [6] - ��������� � ����������� ������� (insertKeepOrd)\n";
	cout << " [7] - ���������� � ������� ���� (writeToFile)\n";
	cout << " [8] - �������� �� �������� ����� (readFromFile)\n";
	cout << " [9] - ���������� � �������� ���� (writeToFile)\n";
	cout << "[10] - �������� �� ��������� ����� (readFromFile)\n";
	cout << "[11] - ������������ (balance)\n";
	cout << "[12] - ����� ������ �� �����\n";
	cout << "[13] - ������ ���� �������\n";
	cout << " [0] - �����\n\n";
}

void menu() {
	OneWayLinkedList<string, 5> list;
	int num;
	printMethods();
	while (1) {
		cout << "\n������� ����� �������: ";
		cin >> num;
		while (!cin || num<0 || num>13) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "������� ���������� ����� (13 - help): ";
			cin >> num;
		}
		cin.get();
		switch (num) {
		case 0:
			return;
		case 1: {
			string inputStr;
			cout << "������� ������: ";
			getline(cin, inputStr);
			list.push(inputStr);
		} break;

		case 2: {
			int index;
			cout << "������� ���������� ����� (������) ��� ��������: ";
			cin >> index;
			while (!cin || index<0) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "������������ ������, ��������� ����: ";
				cin >> index;
			}
			string* gottenStr = list.erase(index);
			if (gottenStr != NULL) cout << "��������� �������: " << *gottenStr << endl;
			else cout << "��������� �������: NULL" << endl;
			delete gottenStr;
		} break;

		case 3: {
			string inputStr;
			int index;
			cout << "������� ������: ";
			getline(cin, inputStr);
			cout << "������� ���������� ����� (������) ��� �������: ";
			cin >> index;
			while (!cin || index<0) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "������������ ������, ��������� ����: ";
				cin >> index;
			}
			list.insert(inputStr, index);
		} break;

		case 4: {
			int index;
			cout << "������� ���������� ����� (������) ��� ��������� ��������: ";
			cin >> index;
			while (!cin || index<0) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "������������ ������, ��������� ����: ";
				cin >> index;
			}
			string* gottenStr = list.get(index);
			if (gottenStr!=NULL) cout << "���������� �������: " << *gottenStr << endl;
			else cout << "���������� �������: NULL"  << endl;
		} break;

		case 5: {
			auto t1 = chrono::high_resolution_clock::now();
			list.sort();
			auto t2 = chrono::high_resolution_clock::now();
			auto time = chrono::duration<float,ratio<1>>(t2 - t1).count();
			cout << "����� ���������� (� ��������): " << time << endl;
		} break;

		case 6: {
			string inputStr;
			cout << "������� ������: ";
			getline(cin, inputStr);
			list.insertKeepOrd(inputStr);
		} break;

		case 7: {
			list.writeToFile("1.txt");
		} break;

		case 8: {
			list.readFromFile("1.txt");
		} break;

		case 9: {
			list.writeToFile("2.txt", ios::binary);
		} break;

		case 10: {
			list.readFromFile("2.txt", ios::binary);
		} break;

		case 11: {
			auto t1 = chrono::high_resolution_clock::now();
			list.balance();
			auto t2 = chrono::high_resolution_clock::now();
			auto time = chrono::duration<float,ratio<1>>(t2 - t1).count();
			cout << "����� ������������ (� ��������): " << time << endl;
		} break;

		case 12: {
			cout << list;
		} break;

		case 13: {
			printMethods();
		} break;

		}
	}
}

void main() {
	system("chcp 1251");
	menu();
}