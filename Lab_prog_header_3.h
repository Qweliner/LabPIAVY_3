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
} // namespace Constants

// ��������� ������

/* @brief ��������� ��� �������� ���������� � ���������������.
@param type ��� ���������������.
@param date ���� ���������������.
*/
struct Correspondence {
    string type;  // ��� ���������������
    string date;  // ���� ���������������
};

/* @brief ��������� ��� �������� ���������� �� �����������.
@param name �������� �����������.
@param address ����� �����������.
@param contactPerson ���������� ����.
@param correspondences ������ ��������������� �����������.
@param additionalContacts ������ ���(�����, ���������� ����) �������������� ���������.
*/
struct Organization {
    string name;  // �������� �����������
    string address;  // ����� �����������
    string contactPerson;  // ���������� ����
    vector<Correspondence> correspondences; // ������ ���������������
    vector<pair<string, string>> additionalContacts; // ������ ���(�����, ���������� ����)
};

// ���������� �������

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

/* @brief ����������� ��� �������� ����� � ������������. */
string getInputFilenameFromUser(const string& folderPath);

/* @brief ��������� ������ �� ������������ �� �����. */
vector<Organization> readOrganizationsFromFile(const string& filename);

/* @brief ������� ������ �� ������������ � ����. */
void writeOrganizationsToFile(const string& filename, const vector<Organization>& organizations, const string& sortField, bool ascending, const vector<int>& missingValuesIndices);

/* @brief ��������� ������� ���������� ������. */
void runSorting(string& folderPath, string& inputFilename);

/* @brief ���������� ������� ����. */
void menu();

/* @brief ��������� ���������� ������� ����������� ������� ������.
   @param organizations ������ ����������� ��� ����������.
   @param sortField ����, �� �������� ����������� ����������.
   @param ascending ����������� ���������� (true - �� �����������, false - �� ��������).
   @param missingValuesIndices ������ �������� �����������, � ������� ����������� �������� � ���� ����������.
*/
void sortOrganizations(vector<Organization>& organizations, const string& sortField, bool ascending, vector<int>& missingValuesIndices);

/* @brief ������� ��������� ��� ��� ����������. */
bool compareDates(const string& date1, const string& date2, bool ascending);

#endif // !LAB_PROG_HEADER_3_H