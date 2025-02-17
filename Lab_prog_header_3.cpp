#include "Lab_prog_header_3.h"

using namespace std;

// ������������� ���������� ����������
string currentFolderPath = "";

/** @brief ������� ���������. */
void tabul(int x) {
    for (int i = x; i != 0; i--)
        printf("   "); // ����� ���� ��������
}

/** @brief ����������, �������� �� ��� ����������. */
bool is_leap(int year) {
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

/** @brief ��������� ������������ ���. */
bool isValidFio(const string& fioStr) {
    bool valid_char = true;
    for (char c : fioStr) {
        if (not((c >= '�' && c <= '�') || (c >= '�' && c <= '�') ||
            (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ' ||
            c == '-' || c == '\x2D'))
            valid_char = false;
    }
    return valid_char;
}

/** @brief ��������� ������������ ������� ����. */
bool isValidDate(const string& dateStr) {
    if (dateStr.length() != 10)
        return false;
    int day = 0, month = 0, year = 0;
    int count = sscanf_s(dateStr.c_str(), "%2d.%2d.%4d", &day, &month, &year);
    if (count != 3)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;
    if (month == 2) {
        int daysInFeb = 28 + (is_leap(year) ? 1 : 0);
        if (day > daysInFeb)
            return false;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30)
            return false;
    }
    return true;
}

/** @brief ��������� ������������ ����� �����. */
bool isValidFileName(const string& fileName) {
    const string invalidChars = "\\/:*?\"<>|";
    return fileName.find_first_of(invalidChars) == string::npos;
}

/** @brief ������ ���������� �� �����. */
void readInstructionsFromFile(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line))
            cout << line << endl;
        file.close();
    }
    else
        cerr << "�� ������� ������� ���� ����������: " << filename << endl;
    cout << "\n������� ����� ������� ��� �����������...";
    _getch();
}

/** @brief ����������� ��� �������� ����� � ������������. */
string getInputFilenameFromUser(const string& folderPath) {
    string filename;
    cout << "������� ��� �������� ����� (output.txt - Enter ��� ������������� �� ���������): ";
    getline(cin, filename);

    // ���� ������������ ����� Enter, ���������� ��� ����� �� ���������
    if (filename.empty()) {
        filename = "output.txt";
    }

    return folderPath + filename;
}

/** @brief ��������� ������ �� ������������ �� �����. */
vector<Organization> readOrganizationsFromFile(const string& filename) {
    vector<Organization> organizations;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "�� ������� ������� ����: " << filename << endl;
        return organizations; // ���������� ������ ������
    }

    string line;
    Organization currentOrg;
    bool inAdditionalContacts = false;

    while (getline(file, line)) {
        if (line.rfind("�����������:", 0) == 0) {
            // ������ ������ ����� �����������
            if (!currentOrg.name.empty()) {
                // ��������� ���������� �����������, ���� ��� ���� ���������
                organizations.push_back(currentOrg);
            }
            currentOrg = {}; // ������� ����� ������ ������ Organization
            currentOrg.name = line.substr(string("�����������:").length() + 1);
            if (currentOrg.name.empty()) currentOrg.name = Constants::DEFAULT_EMPTY_VALUE; // ��������� ������� ��������
            inAdditionalContacts = false;
        }
        else if (line.rfind("�����:", 0) == 0) {
            currentOrg.address = line.substr(string("�����:").length() + 1);
            if (currentOrg.address.empty()) currentOrg.address = Constants::DEFAULT_EMPTY_VALUE; // ��������� ������� ��������
            inAdditionalContacts = false;
        }
        else if (line.rfind("���������� ����:", 0) == 0) {
            currentOrg.contactPerson = line.substr(string("���������� ����:").length() + 1);
            if (currentOrg.contactPerson.empty()) currentOrg.contactPerson = Constants::DEFAULT_EMPTY_VALUE; // ��������� ������� ��������
            inAdditionalContacts = false;
        }
        else if (line == "������ ��������� ������ ��������:") {
            inAdditionalContacts = true;
        }
        else if (inAdditionalContacts && line.rfind("�����:", 0) == 0) {
            string address = line.substr(string("�����:").length() + 1);
            if (address.empty()) address = Constants::DEFAULT_EMPTY_VALUE; // ��������� ������� ��������
            getline(file, line); // ��������� ������ � ���������� �����
            if (line.rfind("���������� ����:", 0) == 0) {
                string contactPerson = line.substr(string("���������� ����:").length() + 1);
                if (contactPerson.empty()) contactPerson = Constants::DEFAULT_EMPTY_VALUE; // ��������� ������� ��������
                currentOrg.additionalContacts.push_back({ address, contactPerson });
            }
        }
        else if (line.rfind("\t��� ���������������:", 0) == 0) {
            // ������ ������ � ���������������
            Correspondence corr;
            corr.type = line.substr(string("\t��� ���������������:").length() + 1);
            if (corr.type.empty()) corr.type = Constants::DEFAULT_EMPTY_VALUE; // ��������� ������� ��������

            getline(file, line); // ��������� ������ � �����
            if (line.rfind("\t����:", 0) == 0) {
                corr.date = line.substr(string("\t����:").length() + 1);
                if (corr.date.empty()) corr.date = Constants::DEFAULT_EMPTY_VALUE; // ��������� ������� ��������
            }
            currentOrg.correspondences.push_back(corr);
            inAdditionalContacts = false;
        }
        else if (line == "--------------------") {
            // ����� ����� �����������
            organizations.push_back(currentOrg);
            currentOrg = {};
            inAdditionalContacts = false;
        }
    }

    // ��������� ��������� �����������, ���� ���� �� ������������� ������������
    if (!currentOrg.name.empty()) {
        organizations.push_back(currentOrg);
    }

    file.close();
    return organizations;
}

/** @brief ������� ������ �� ������������ � ����. */
void writeOrganizationsToFile(const string& filename, const vector<Organization>& organizations, const string& sortField, bool ascending, const vector<int>& missingValuesIndices) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "�� ������� ������� ���� ��� ������: " << filename << endl;
        return;
    }

    // ���������� ���������� � ���������� � ����
    file << "���������� �� ����: " << sortField << endl;
    file << "����������� ����������: " << (ascending ? "�� �����������" : "�� ��������") << endl;
    file << "--------------------\n";

    for (size_t i = 0; i < organizations.size(); ++i) {
        const auto& org = organizations[i];

        file << "�����������: " << org.name << endl;
        file << "�����: " << org.address << endl;
        file << "���������� ����: " << org.contactPerson << endl;

        if (!org.additionalContacts.empty()) {
            file << "������ ��������� ������ ��������:\n";
            for (const auto& contact : org.additionalContacts) {
                file << "�����: " << contact.first << endl;
                file << "���������� ����: " << contact.second << endl;
            }
        }

        if (org.correspondences.empty()) {
            file << "\t��������������� �����������\n";
        }
        else {
            for (const auto& corr : org.correspondences) {
                file << "\t��� ���������������: " << corr.type << endl;
                file << "\t����: " << corr.date << endl;
            }
        }

        // ������� ��������� �� ���������� ��������, ���� ����������� ���� ���������� � ����� ������ ��-�� �����
        bool missingValue = false;
        for (int index : missingValuesIndices) {
            if (index == i) {
                missingValue = true;
                break;
            }
        }
        if (missingValue) {
            file << "\t" << sortField << " �����������\n";
        }

        file << "--------------------\n";
    }

    file.close();
    cout << "������ ������� �������� � ����: " << filename << endl;
}

/** @brief ��������� ������� ���������� ������. */
void runSorting(string& folderPath, string& inputFilename) {
    // �������� ��� �������� ����� �� ������������
    inputFilename = getInputFilenameFromUser(folderPath);

    // ��������� ������ �� ������������ �� �����
    vector<Organization> organizations = readOrganizationsFromFile(inputFilename);

    // ������� ���������, ���� �� ������� ��������� �����������
    if (organizations.empty()) {
        cout << "��� ������ ��� ����������." << endl;
        cout << "������� ����� ������� ��� �������� � ����...";
        _getch();
        return;
    }

    // ����� ���� ��� ����������
    cout << "\n�������� ���� ��� ����������:\n";
    cout << "1. �����������\n";
    cout << "2. �����\n";
    cout << "3. ���������� ����\n";
    cout << "Esc - ��������� � ������� ����\n";
    cout << "\n��� �����: ";

    char choice = _getch();
    cout << endl;

    string sortField;
    switch (choice) {
    case '1':
        sortField = "�����������";
        break;
    case '2':
        sortField = "�����";
        break;
    case '3':
        sortField = "���������� ����";
        break;
    case 27:
        return;
    default:
        cerr << "�������� �����.\n";
        cout << "������� ����� ������� ��� �������� � ����...";
        _getch();
        return;
    }

    // ����� ����������� ����������
    cout << "\n�������� ����������� ����������:\n";
    cout << "1. �� ����������� (up)\n";
    cout << "2. �� �������� (down)\n";
    cout << "Esc - ��������� � ������� ����\n";
    cout << "\n��� �����: ";

    choice = _getch();
    cout << endl;

    bool ascending = true;
    switch (choice) {
    case '1':
        ascending = true;
        break;
    case '2':
        ascending = false;
        break;
    case 27:
        return;
    default:
        cerr << "�������� �����.\n";
        cout << "������� ����� ������� ��� �������� � ����...";
        _getch();
        return;
    }

    // ������ ��� �������� �������� �����������, � ������� ����������� �������� � ���� ����������
    vector<int> missingValuesIndices;

    // ��������� ����������
    sortOrganizations(organizations, sortField, ascending, missingValuesIndices);

    // ��������� ��� ����� ��� ���������� �����������
    string outputFilename = folderPath + "sorted_by_" + sortField + "_" + (ascending ? "up" : "down") + ".txt";

    // ���������� ��������������� ������ � ����
    writeOrganizationsToFile(outputFilename, organizations, sortField, ascending, missingValuesIndices);

    cout << "������� ����� ������� ��� �������� � ����...";
    _getch();
}

/** @brief ���������� ������� ����. */
void menu() {
    bool folderPathSet = false;
    string inputFilename;

    while (true) {
        system("cls");
        cout << "������� ����:\n";
        cout << "1. ���������� ������ ";
        if (!folderPathSet)
            cout << "(������� ������� ���� � �����)\n";
        else
            cout << '\n';
        cout << "2. ���� � �����\n";
        cout << "3. ����������\n";
        cout << "4. �����\n\n";
        cout << "������� ����: " << currentFolderPath << "\n\n";
        cout << "> ";

        char mainChoice = _getch();

        switch (mainChoice) {
        case '1':
            if (!folderPathSet) {
                cerr << "������: ������� ������� ���� � �����.\n";
                _getch();
            }
            else
                runSorting(currentFolderPath, inputFilename);
            break;
        case '2': {
            cout << "������� ���� � �����: ";
            getline(cin, currentFolderPath);

            struct stat buffer;
            if (stat(currentFolderPath.c_str(), &buffer) != 0) {
                if (errno == ENOENT)
                    cerr << "������: ��������� ���� �� ����������.\n";
                else
                    cerr << "������: �� ������� ��������� ���� � �����. ��� ������: "
                    << errno << "\n";
                _getch();
                continue; // ������� � ������ �����, ���� ���� �����������
            }

            if (currentFolderPath.back() != '\\')
                currentFolderPath += '\\';

            folderPathSet = true;
            cout << "\n������� ���� � �����: " << currentFolderPath << endl;
            cout << "������� ����� ������� ��� �����������...";
            _getch();
            break;
        }
        case '3':
            readInstructionsFromFile(Constants::INSTRUCTIONS_FILE);
            break;
        case '4':
            return; // ����� �� ���������
        default:
            cerr << "�������� �����.\n";
        }
    }
}

/** @brief ��������� ���������� ������� ����������� ������� ������.
   @param organizations ������ ����������� ��� ����������.
   @param sortField ����, �� �������� ����������� ����������.
   @param ascending ����������� ���������� (true - �� �����������, false - �� ��������).
   @param missingValuesIndices ������ �������� �����������, � ������� ����������� �������� � ���� ����������.
*/
void sortOrganizations(vector<Organization>& organizations, const string& sortField, bool ascending, vector<int>& missingValuesIndices) {
    int n = organizations.size();

    // ������� �������� ������� ����������� � �������������� ����������
    for (int i = 0; i < n; ++i) {
        bool hasValue = true;
        if (sortField == "�����������") {
            if (organizations[i].name == Constants::DEFAULT_EMPTY_VALUE) hasValue = false;
        }
        else if (sortField == "�����") {
            if (organizations[i].address == Constants::DEFAULT_EMPTY_VALUE) hasValue = false;
        }
        else if (sortField == "���������� ����") {
            if (organizations[i].contactPerson == Constants::DEFAULT_EMPTY_VALUE) hasValue = false;
        }

        if (!hasValue) {
            missingValuesIndices.push_back(i);
        }
    }

    // ��������� ������ ����������� � ���������� ����������
    for (int i = 0; i < n - 1; ++i) {
        // ���������� �����������, ������� ����� ���������� � ����� ������
        bool skipI = false;
        for (int index : missingValuesIndices) {
            if (index == i) {
                skipI = true;
                break;
            }
        }
        if (skipI) continue;

        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            // ���������� �����������, ������� ����� ���������� � ����� ������
            bool skipJ = false;
            for (int index : missingValuesIndices) {
                if (index == j) {
                    skipJ = true;
                    break;
                }
            }
            if (skipJ) continue;

            bool swap = false;
            if (sortField == "�����������") {
                if (ascending) {
                    swap = organizations[j].name < organizations[minIndex].name;
                }
                else {
                    swap = organizations[j].name > organizations[minIndex].name;
                }
            }
            else if (sortField == "�����") {
                if (ascending) {
                    swap = organizations[j].address < organizations[minIndex].address;
                }
                else {
                    swap = organizations[j].address > organizations[minIndex].address;
                }
            }
            else if (sortField == "���������� ����") {
                if (ascending) {
                    swap = organizations[j].contactPerson < organizations[minIndex].contactPerson;
                }
                else {
                    swap = organizations[j].contactPerson > organizations[minIndex].contactPerson;
                }
            }

            if (swap) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            swap(organizations[i], organizations[minIndex]);
        }
    }

    // ���������� ����������� � �������������� ���������� � ����� ������
    int missingCount = missingValuesIndices.size();
    for (int i = 0; i < missingCount; ++i) {
        int indexToMove = missingValuesIndices[i] - i; // ������������ ������, ��� ��� �������� ����������
        if (indexToMove >= 0 && indexToMove < organizations.size()) {
            Organization temp = organizations[indexToMove];
            organizations.erase(organizations.begin() + indexToMove);
            organizations.push_back(temp);
            // ��������� ������� � missingValuesIndices, ����� ��� ��������� �� ���������� �������
            for (int j = i + 1; j < missingCount; ++j) {
                if (missingValuesIndices[j] > indexToMove) {
                    missingValuesIndices[j]--;
                }
            }
        }
    }
}

/** @brief ������� ��������� ��� ��� ����������. */
bool compareDates(const string& date1, const string& date2, bool ascending) {
    // TODO: ����������� ������� ��������� ��� � ������ �������� ��������.
    // �������� ���� ������� �����������/����������� � ����������� �� ����������� ����������.
    return false; // ��������
}