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

// ���������� ���������� ��� �������� �������� ���� � ����� � �������
extern string currentFolderPath;

// ���������� ��������
namespace Constants {
    const string INSTRUCTIONS_FILE = "instructions.txt"; // ��� ����� � ������������
    const int MAX_MENU_CHOICE = 4;       // ������������ ����� � ����
    const string DEFAULT_EMPTY_VALUE = "�� �������"; // �������� �� ��������� ��� ������ �����
    const string INVALID_DATE_GROUP = "-����: ������������ ����-";  //��������� ��� ������
    const string INVALID_FIO_GROUP = "-���������� ����: ������������ ���-"; //��������� ��� ������
} // namespace Constants

// ��������� ������

/* @brief ��������� ��� �������� ���������� � ���������������.
*/
struct Correspondence {
    string type;
    string date;
};

/* @brief ��������� ��� �������� ���������� �� �����������.
*/
struct Organization {
    string name;
    vector<string> addresses; // ��� ������ �����������
    vector<string> contacts;   // ��� ��� �����������
    vector<Correspondence> correspondences;
};

// ��������������� ��������� ��� ���������� ���������������
struct TempCorrespondence {
    Organization* organization; // ��������� �� �����������
    int correspondenceIndex;   // ������ ��������������� � ������� correspondences
    string value;          // �������� ���� ���������� (���� ��� ���)
};

// ���������� �������

bool is_leap(int year);
bool isValidFio(const string& fio);
bool isValidDate(const string& dateStr);
bool isValidFileName(const string& fileName);

void readInstructionsFromFile(const string& filename);
string getInputFilenameFromUser(const string& folderPath);
vector<Organization> readOrganizationsFromFile(const string& filename);
void writeOrganizationsToFile(const string& filename, const vector<Organization>& organizations, const string& sortField, bool ascending, const vector<TempCorrespondence>& sortedCorrespondences = {}); //�������
void runSorting(string& folderPath, string& inputFilename);
void menu();
void sortOrganizations(vector<Organization>& organizations, const string& sortField, bool ascending, vector<TempCorrespondence>& sortedCorrespondences); //�������
int compareValues(const string& value1, const string& value2, bool ascending);
bool compareDates(const string& date1, const string& date2, bool ascending);

#endif // !LAB_PROG_HEADER_3_H