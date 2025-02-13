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

// ���������� ���������� ��� �������� �������� ���� � ����� � �������
extern string currentFolderPath;

// ���������� ��������
namespace Constants {
    const string INSTRUCTIONS_FILE = "instructions.txt"; // ��� ����� � ������������
    const int MAX_MENU_CHOICE = 3;       // ������������ ����� � ����
} // namespace Constants

// ��������� ������

/* @brief ��������� ��� ���������� ����� ������, ������ ���� ���������. 
@param organization, address, contactPerson, type, date
*/
struct Block_data {
    string organization; // �������� �����������
    string address;      // ����� �����������
    string contactPerson;  // ���������� ����
    string type;       // ��� ���������������
    string date;       // ���� ���������������
};

/* @brief ������� ���������. */
void tabul(int x);

/* @brief ����������, �������� �� ��� ����������. */
bool is_leap(int year);

/* @brief ��������� ������������ ���. */
bool isValidFio(const string& fio);

/* @brief ��������� ������������ ������� ����. */
bool isValidDate(const string& dateStr);

/* @brief ��������� ������������ ����� �����. */
bool isValidFileName(const string& fileName);

/* @brief ������ ���������� �� �����. */
void readInstructionsFromFile(const string& filename);

/* @brief ����������� ����� ������ � ������������. */
pair<string, string> getFilenamesFromUser(const string& folderPath);

/* @brief ��������� �������� ������� ���������. */
void runProgram(string& folderPath, string& correspondenceFilename,
    string& addressesFilename, string& outputFilename);

/* @brief ���������� ������� ����. */
void menu();