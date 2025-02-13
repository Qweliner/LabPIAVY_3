// Lab_prog_header_3.h

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
    const int MAX_MENU_CHOICE = 3;       // Максимальный выбор в меню
} // namespace Constants

// Структуры данных

/* @brief Структура для считывания блока данных, данные даны массивами. 
@param organization, address, contactPerson, type, date
*/
struct Block_data {
    string organization; // Название организации
    string address;      // Адрес организации
    string contactPerson;  // Контактное лицо
    string type;       // Тип корреспонденции
    string date;       // Дата корреспонденции
};

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

/* @brief Запрашивает имена файлов у пользователя. */
pair<string, string> getFilenamesFromUser(const string& folderPath);

/* @brief Запускает основной процесс программы. */
void runProgram(string& folderPath, string& correspondenceFilename,
    string& addressesFilename, string& outputFilename);

/* @brief Отображает главное меню. */
void menu();