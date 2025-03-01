#pragma once
#ifndef LAB_PROG_HEADER_3_H
#define LAB_PROG_HEADER_3_H

#include <algorithm>
#include <cctype>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <limits>
#include <locale>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <Windows.h>

#ifdef max
#undef max
#endif

using namespace std;

// Глобальная переменная для хранения текущего пути к папке с данными
extern string currentFolderPath;

// Объявление констант
namespace Constants {
    const string INSTRUCTIONS_FILE = "instructions.txt"; // Имя файла с инструкциями
    const int MAX_MENU_CHOICE = 4;       // Максимальный выбор в меню
    const string DEFAULT_EMPTY_VALUE = "Не указано"; // Значение по умолчанию для пустых полей
    const string INVALID_DATE_GROUP = "-ДАТА: Некорректная дата-";  //Заголовок для вывода
    const string INVALID_FIO_GROUP = "-КОНТАКТНОЕ ЛИЦО: Некорректное ФИО-"; //Заголовок для вывода
} // namespace Constants

// Структуры данных

/* @brief Структура для хранения информации о корреспонденции.
*/
struct Correspondence {
    string type;
    string date;
};

/* @brief Структура для хранения информации об организации.
*/
struct Organization {
    string name;
    vector<string> addresses; // Все адреса организации
    vector<string> contacts;   // Все ФИО организации
    vector<Correspondence> correspondences;
};

// Вспомогательная структура для сортировки корреспонденции
struct TempCorrespondence {
    Organization* organization; // Указатель на организацию
    int correspondenceIndex;   // Индекс корреспонденции в векторе correspondences
    string value;          // Значение поля сортировки (дата или тип)
};

// Объявление функций

bool is_leap(int year);
bool isValidFio(const string& fio);
bool isValidDate(const string& dateStr);
bool isValidFileName(const string& fileName);

void readInstructionsFromFile(const string& filename);
string getInputFilenameFromUser(const string& folderPath);
vector<Organization> readOrganizationsFromFile(const string& filename);
void writeOrganizationsToFile(const string& filename, const vector<Organization>& organizations, const string& sortField, bool ascending, const vector<TempCorrespondence>& sortedCorrespondences = {}); //Добавил
void runSorting(string& folderPath, string& inputFilename);
void menu();
void sortOrganizations(vector<Organization>& organizations, const string& sortField, bool ascending, vector<TempCorrespondence>& sortedCorrespondences); //Добавил
int compareValues(const string& value1, const string& value2, bool ascending);
bool compareDates(const string& date1, const string& date2, bool ascending);

#endif // !LAB_PROG_HEADER_3_H