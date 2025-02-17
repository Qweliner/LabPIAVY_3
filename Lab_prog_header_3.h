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
} // namespace Constants

// Структуры данных

/* @brief Структура для хранения информации о корреспонденции.
@param type Тип корреспонденции.
@param date Дата корреспонденции.
*/
struct Correspondence {
    string type;  // Тип корреспонденции
    string date;  // Дата корреспонденции
};

/* @brief Структура для хранения информации об организации.
@param name Название организации.
@param address Адрес организации.
@param contactPerson Контактное лицо.
@param correspondences Вектор корреспонденции организации.
@param additionalContacts Вектор пар(адрес, контактное лицо) дополнительных контактов.
*/
struct Organization {
    string name;  // Название организации
    string address;  // Адрес организации
    string contactPerson;  // Контактное лицо
    vector<Correspondence> correspondences; // Вектор корреспонденции
    vector<pair<string, string>> additionalContacts; // Вектор пар(адрес, контактное лицо)
};

// Объявление функций

/* @brief Выводит табуляцию. */
void tabul(int x);

/* @brief Определяет, является ли год високосным. */
bool is_leap(int year);

/* @brief Проверяет допустимость ФИО. */
bool isValidFio(const string& fio);

/* @brief Проверяет допустимость формата даты. */
bool isValidDate(const string& dateStr);

/* @brief Проверяет допустимость имени файла. */
bool isValidFileName(const string& fileName);

/* @brief Читает инструкции из файла. */
void readInstructionsFromFile(const string& filename);

/* @brief Запрашивает имя входного файла у пользователя. */
string getInputFilenameFromUser(const string& folderPath);

/* @brief Считывает данные об организациях из файла. */
vector<Organization> readOrganizationsFromFile(const string& filename);

/* @brief Выводит данные об организациях в файл. */
void writeOrganizationsToFile(const string& filename, const vector<Organization>& organizations, const string& sortField, bool ascending, const vector<int>& missingValuesIndices);

/* @brief Запускает процесс сортировки данных. */
void runSorting(string& folderPath, string& inputFilename);

/* @brief Отображает главное меню. */
void menu();

/* @brief Выполняет сортировку вектора организаций методом выбора.
   @param organizations Вектор организаций для сортировки.
   @param sortField Поле, по которому выполняется сортировка.
   @param ascending Направление сортировки (true - по возрастанию, false - по убыванию).
   @param missingValuesIndices Вектор индексов организаций, у которых отсутствуют значения в поле сортировки.
*/
void sortOrganizations(vector<Organization>& organizations, const string& sortField, bool ascending, vector<int>& missingValuesIndices);

/* @brief Функция сравнения дат для сортировки. */
bool compareDates(const string& date1, const string& date2, bool ascending);

#endif // !LAB_PROG_HEADER_3_H