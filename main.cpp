#include <iostream>
#include <chrono>
#include "OneWayLinkedList.h"
#include "createdString.h"

using namespace std;

void printMethods() {
	system("cls");
	cout << "\n";
	cout << " [1] - Добавление в конец (push)\n";
	cout << " [2] - Удаление по по логическому номеру (erase)\n";
	cout << " [3] - Включение по логическому номеру (insert)\n";
	cout << " [4] - Извличение по логическому номеру (get)\n";
	cout << " [5] - Сортировка (sort)\n";
	cout << " [6] - Включение с сохранением порядка (insertKeepOrd)\n";
	cout << " [7] - Сохранение в обычный файл (writeToFile)\n";
	cout << " [8] - Загрузка из обычного файла (readFromFile)\n";
	cout << " [9] - Сохранение в бинарный файл (writeToFile)\n";
	cout << "[10] - Загрузка из бинарного файла (readFromFile)\n";
	cout << "[11] - Балансировка (balance)\n";
	cout << "[12] - Вывод списка на экран\n";
	cout << "[13] - Печать всех комманд\n";
	cout << " [0] - Выход\n\n";
}

void menu() {
	OneWayLinkedList<string, 5> list;
	int num;
	printMethods();
	while (1) {
		cout << "\nВведите номер команды: ";
		cin >> num;
		while (!cin || num<0 || num>13) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Введите корректный номер (13 - help): ";
			cin >> num;
		}
		cin.get();
		switch (num) {
		case 0:
			return;
		case 1: {
			string inputStr;
			cout << "Введите строку: ";
			getline(cin, inputStr);
			list.push(inputStr);
		} break;

		case 2: {
			int index;
			cout << "Введите логический номер (индекс) для удаления: ";
			cin >> index;
			while (!cin || index<0) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Некорректные данные, повторите ввод: ";
				cin >> index;
			}
			string* gottenStr = list.erase(index);
			if (gottenStr != NULL) cout << "Удаляемый элемент: " << *gottenStr << endl;
			else cout << "Удаляемый элемент: NULL" << endl;
			delete gottenStr;
		} break;

		case 3: {
			string inputStr;
			int index;
			cout << "Введите строку: ";
			getline(cin, inputStr);
			cout << "Введите логический номер (индекс) для вставки: ";
			cin >> index;
			while (!cin || index<0) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Некорректные данные, повторите ввод: ";
				cin >> index;
			}
			list.insert(inputStr, index);
		} break;

		case 4: {
			int index;
			cout << "Введите логический номер (индекс) для получения элемента: ";
			cin >> index;
			while (!cin || index<0) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Некорректные данные, повторите ввод: ";
				cin >> index;
			}
			string* gottenStr = list.get(index);
			if (gottenStr!=NULL) cout << "Полученный элемент: " << *gottenStr << endl;
			else cout << "Полученный элемент: NULL"  << endl;
		} break;

		case 5: {
			auto t1 = chrono::high_resolution_clock::now();
			list.sort();
			auto t2 = chrono::high_resolution_clock::now();
			auto time = chrono::duration<float,ratio<1>>(t2 - t1).count();
			cout << "Время сортировки (в секундах): " << time << endl;
		} break;

		case 6: {
			string inputStr;
			cout << "Введите строку: ";
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
			cout << "Время балансировки (в секундах): " << time << endl;
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