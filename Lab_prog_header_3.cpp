#include "Lab_prog_header_3.h"

using namespace std;

// ������������� ���������� ����������
string currentFolderPath = "";

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
        return organizations;
    }

    string line;
    Organization currentOrg;
    bool inAdditionalContacts = false;

    while (getline(file, line)) {
        if (line.rfind("�����������:", 0) == 0) {
            if (!currentOrg.name.empty()) {
                organizations.push_back(currentOrg);
            }
            currentOrg = {}; // ������� ����� ������ ������ Organization
            currentOrg.name = line.substr(string("�����������:").length() + 1);
            if (currentOrg.name.empty()) currentOrg.name = Constants::DEFAULT_EMPTY_VALUE;
            inAdditionalContacts = false;
        }
        else if (line.rfind("�����:", 0) == 0) {
            string address = line.substr(string("�����:").length() + 1);
            if (address.empty()) address = Constants::DEFAULT_EMPTY_VALUE;
            currentOrg.addresses.push_back(address);
            inAdditionalContacts = false;
        }
        else if (line.rfind("���������� ����:", 0) == 0) {
            string contact = line.substr(string("���������� ����:").length() + 1);
            if (contact.empty()) contact = Constants::DEFAULT_EMPTY_VALUE;
            if (!isValidFio(contact) && contact != Constants::DEFAULT_EMPTY_VALUE) {
                cerr << "������������ ������ ���: " << contact << endl;
            }
            currentOrg.contacts.push_back(contact);
            inAdditionalContacts = false;
        }
        else if (line == "������ ��������� ������ ��������:") {
            inAdditionalContacts = true;
        }
        else if (inAdditionalContacts && line.rfind("�����:", 0) == 0) {
            string address = line.substr(string("�����:").length() + 1);
            if (address.empty()) address = Constants::DEFAULT_EMPTY_VALUE;
            currentOrg.addresses.push_back(address);

            getline(file, line); // ��������� ������ � ���������� �����
            if (line.rfind("���������� ����:", 0) == 0) {
                string contact = line.substr(string("���������� ����:").length() + 1);
                if (contact.empty()) contact = Constants::DEFAULT_EMPTY_VALUE;
                if (!isValidFio(contact) && contact != Constants::DEFAULT_EMPTY_VALUE) {
                    cerr << "������������ ������ ���: " << contact << endl;
                }
                currentOrg.contacts.push_back(contact);
            }
        }
        else if (line.rfind("\t��� ���������������:", 0) == 0) {
            Correspondence corr;
            corr.type = line.substr(string("\t��� ���������������:").length() + 1);
            if (corr.type.empty()) corr.type = Constants::DEFAULT_EMPTY_VALUE;

            getline(file, line);
            if (line.rfind("\t����:", 0) == 0) {
                corr.date = line.substr(string("\t����:").length() + 1);
                if (corr.date.empty()) corr.date = Constants::DEFAULT_EMPTY_VALUE;
                if (!isValidDate(corr.date) && corr.date != Constants::DEFAULT_EMPTY_VALUE) {
                    cerr << "������������ ������ ����: " << corr.date << endl;
                }
            }
            currentOrg.correspondences.push_back(corr);
            inAdditionalContacts = false;
        }
        else if (line == "--------------------") {
            organizations.push_back(currentOrg);
            currentOrg = {};
            inAdditionalContacts = false;
        }
    }

    if (!currentOrg.name.empty()) {
        organizations.push_back(currentOrg);
    }

    file.close();
    return organizations;
}

/** @brief ������� ������ �� ������������ � ����. */
void writeOrganizationsToFile(const string& filename, const vector<Organization>& organizations, const string& sortField, bool ascending, const vector<TempCorrespondence>& sortedCorrespondences) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "�� ������� ������� ���� ��� ������: " << filename << endl;
        return;
    }

    file << "���������� �� ����: " << sortField << endl;
    file << "����������� ����������: " << (ascending ? "�� �����������" : "�� ��������") << endl;
    file << "--------------------\n";

    string previousGroupValue = ""; // �������� ���� ���������� ��� ���������� ������

    if (sortField == "����" || sortField == "��� ���������������") {
        for (const auto& tempCorr : sortedCorrespondences) {
            string currentGroupValue;

            if (sortField == "����") {
                currentGroupValue = tempCorr.value;
            }
            else {
                currentGroupValue = tempCorr.organization->correspondences[tempCorr.correspondenceIndex].type; //�������� ���
            }

            // ������� ��������� ������, ���� �������� ����������
            if (currentGroupValue != previousGroupValue) {
                if (currentGroupValue == Constants::DEFAULT_EMPTY_VALUE) {
                    file << "-" << sortField << ": " << currentGroupValue << "-\n";
                }
                else if (sortField == "����" && !isValidDate(currentGroupValue)) {
                    file << Constants::INVALID_DATE_GROUP << "\n";
                }
                else {
                    file << "-" << sortField << ": " << currentGroupValue << "-\n";
                }
                previousGroupValue = currentGroupValue;
            }

            // ������� ������ �����������
            file << "�����������: " << tempCorr.organization->name << endl;

            file << "�����: ";
            if (!tempCorr.organization->addresses.empty()) {
                file << tempCorr.organization->addresses[0];
                for (size_t i = 1; i < tempCorr.organization->addresses.size(); ++i) {
                    file << "; " << tempCorr.organization->addresses[i];
                }
                file << endl;
            }
            else {
                file << Constants::DEFAULT_EMPTY_VALUE << endl;
            }

            file << "���������� ����: ";
            if (!tempCorr.organization->contacts.empty()) {
                file << tempCorr.organization->contacts[0];
                for (size_t i = 1; i < tempCorr.organization->contacts.size(); ++i) {
                    file << "; " << tempCorr.organization->contacts[i];
                }
                file << endl;
            }
            else {
                file << Constants::DEFAULT_EMPTY_VALUE << endl;
            }
            //E��� ���
            if (sortField == "��� ���������������") {
                file << "\t��� ���������������: " << tempCorr.organization->correspondences[tempCorr.correspondenceIndex].type << endl;
                //������� ��� ���� ����� �������
                file << "\t����: " << tempCorr.value << endl; // ������� value, ������� ������ �������� ������������ ����

            }
            else {
                file << "\t��� ���������������: " << tempCorr.organization->correspondences[tempCorr.correspondenceIndex].type << endl;
                file << "\t����: " << tempCorr.organization->correspondences[tempCorr.correspondenceIndex].date << endl;
            }
            file << "--------------------\n";
        }
    }
    else {
        for (const auto& org : organizations) {
            //���������� ������� ��������
            string currentGroupValue = "";
            if (sortField == "�����������") {
                currentGroupValue = org.name;
            }
            else if (sortField == "�����") {
                currentGroupValue = org.addresses.empty() ? Constants::DEFAULT_EMPTY_VALUE : org.addresses[0];
                for (size_t i = 1; i < org.addresses.size(); ++i) {
                    currentGroupValue += "; " + org.addresses[i];
                }
            }
            else if (sortField == "���������� ����") {
                currentGroupValue = org.contacts.empty() ? Constants::DEFAULT_EMPTY_VALUE : org.contacts[0];
                bool hasInvalid = false;
                for (const string& contact : org.contacts) {
                    if (!isValidFio(contact) && contact != Constants::DEFAULT_EMPTY_VALUE) {
                        hasInvalid = true;
                        break;
                    }
                }
                if (hasInvalid)
                {
                    currentGroupValue = Constants::INVALID_FIO_GROUP;
                }
                else {
                    for (size_t i = 1; i < org.contacts.size(); ++i) {
                        currentGroupValue += "; " + org.contacts[i];
                    }
                }
            }

            // ������� ��������� ������, ���� �������� ����������

            if (currentGroupValue != previousGroupValue) {
                if (currentGroupValue == Constants::DEFAULT_EMPTY_VALUE) {
                    file << "-" << sortField << ": " << currentGroupValue << "-\n";
                }
                else if (sortField == "���������� ����" && currentGroupValue == Constants::INVALID_FIO_GROUP) {
                    file << Constants::INVALID_FIO_GROUP << "\n";
                }
                else {
                    file << "-" << sortField << ": " << currentGroupValue << "-\n";
                }
                previousGroupValue = currentGroupValue;
            }
            // ������� ������ �����������
            file << "�����������: " << org.name << endl;

            file << "�����: ";
            if (!org.addresses.empty()) {
                file << org.addresses[0];
                for (size_t i = 1; i < org.addresses.size(); ++i) {
                    file << "; " << org.addresses[i];
                }
                file << endl;
            }
            else {
                file << Constants::DEFAULT_EMPTY_VALUE << endl;
            }

            file << "���������� ����: ";
            if (!org.contacts.empty()) {
                file << org.contacts[0];
                for (size_t i = 1; i < org.contacts.size(); ++i) {
                    file << "; " << org.contacts[i];
                }
                file << endl;
            }
            else {
                file << Constants::DEFAULT_EMPTY_VALUE << endl;
            }

            if (org.correspondences.empty())
            {
                file << "\t��������������� �����������\n";
            }
            else {
                for (const auto& corr : org.correspondences) {
                    file << "\t��� ���������������: " << corr.type << endl;
                    file << "\t����: " << corr.date << endl;
                }
            }
            file << "--------------------\n";
        }
    }
    file.close();
    cout << "������ ������� �������� � ����: " << filename << endl;
}

/** @brief ��������� ������� ���������� ������. */
void runSorting(string& folderPath, string& inputFilename) {
    inputFilename = getInputFilenameFromUser(folderPath);
    vector<Organization> organizations = readOrganizationsFromFile(inputFilename);

    if (organizations.empty()) {
        cout << "��� ������ ��� ����������." << endl;
        cout << "������� ����� ������� ��� �������� � ����...";
        _getch();
        return;
    }

    cout << "\n�������� ���� ��� ����������:\n";
    cout << "1. �����������\n";
    cout << "2. �����\n";
    cout << "3. ���������� ����\n";
    cout << "4. ��� ���������������\n";
    cout << "5. ����\n";
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
    case '4':
        sortField = "��� ���������������";
        break;
    case '5':
        sortField = "����";
        break;
    case 27: // ��� ������� Esc
        return;
    default:
        cerr << "�������� �����.\n";
        cout << "������� ����� ������� ��� �������� � ����...";
        _getch();
        return;
    }

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

    vector<TempCorrespondence> sortedCorrespondences;
    sortOrganizations(organizations, sortField, ascending, sortedCorrespondences);

    string outputFilename = folderPath + "sorted_by_" + sortField + "_" + (ascending ? "up" : "down") + ".txt";
    writeOrganizationsToFile(outputFilename, organizations, sortField, ascending, sortedCorrespondences);

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
                continue;
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
            return;
        default:
            cerr << "�������� �����.\n";
        }
    }
}

/** @brief ��������� ���������� ������� ����������� ������� ������. */
void sortOrganizations(vector<Organization>& organizations, const string& sortField, bool ascending, vector<TempCorrespondence>& sortedCorrespondences) {
    int n = organizations.size();

    if (sortField == "����" || sortField == "��� ���������������") {
        // 1. ������� ��������� ������
        vector<TempCorrespondence> tempCorrespondences;
        for (int i = 0; i < n; ++i) {
            for (size_t j = 0; j < organizations[i].correspondences.size(); ++j) {
                TempCorrespondence tempCorr;
                tempCorr.organization = &organizations[i];
                tempCorr.correspondenceIndex = j;
                tempCorr.value = (sortField == "����") ? organizations[i].correspondences[j].date : organizations[i].correspondences[j].type;
                tempCorrespondences.push_back(tempCorr);
            }
        }

        // 2. ��������� ��������� ������ ������� ������
        for (size_t i = 0; i < tempCorrespondences.size() - 1; ++i) {
            size_t minMaxIndex = i;
            for (size_t j = i + 1; j < tempCorrespondences.size(); ++j) {
                bool swap = false;
                if (sortField == "����") {
                    if (compareDates(tempCorrespondences[j].value, tempCorrespondences[minMaxIndex].value, ascending)) {
                        minMaxIndex = j;
                    }
                }
                else { // sortField == "��� ���������������"
                    if (compareValues(tempCorrespondences[j].value, tempCorrespondences[minMaxIndex].value, ascending) < 0) {
                        minMaxIndex = j;
                    }
                }
            }
            if (minMaxIndex != i) {
                swap(tempCorrespondences[i], tempCorrespondences[minMaxIndex]);
            }
        }
        //���� ���
        if (sortField == "��� ���������������") {
            vector<TempCorrespondence> mergedCorrespondences;
            for (const auto& tempCorr : tempCorrespondences) {
                bool merged = false;
                for (auto& mergedCorr : mergedCorrespondences) {
                    // ���� ��� ��������� � ��� ���������
                    if (tempCorr.organization == mergedCorr.organization &&
                        tempCorr.organization->correspondences[tempCorr.correspondenceIndex].type ==
                        mergedCorr.organization->correspondences[mergedCorr.correspondenceIndex].type) {
                        // ���������� ����
                        mergedCorr.value += ", " + tempCorr.organization->correspondences[tempCorr.correspondenceIndex].date;
                        merged = true;
                        break;
                    }
                }
                if (!merged) {
                    // ������� ����� ������ � mergedCorrespondences
                    TempCorrespondence newCorr;
                    newCorr.organization = tempCorr.organization;
                    newCorr.correspondenceIndex = tempCorr.correspondenceIndex;
                    newCorr.value = tempCorr.organization->correspondences[tempCorr.correspondenceIndex].date;  // value - ��� ������ � ������!
                    mergedCorrespondences.push_back(newCorr);
                }
            }
            sortedCorrespondences = mergedCorrespondences;
        }
        else {
            sortedCorrespondences = tempCorrespondences;
        }


    }
    else {
        for (int i = 0; i < n - 1; ++i) {
            int minMaxIndex = i;
            for (int j = i + 1; j < n; ++j) {
                bool swap = false;

                //���������� �������� ��� ������ �����������
                string value1;
                if (sortField == "�����������") {
                    value1 = organizations[minMaxIndex].name;
                }
                else if (sortField == "�����") {
                    value1 = organizations[minMaxIndex].addresses.empty() ? Constants::DEFAULT_EMPTY_VALUE : organizations[minMaxIndex].addresses[0]; // ����� ������ �����
                }
                else if (sortField == "���������� ����") {
                    //����������
                    if (organizations[minMaxIndex].contacts.empty()) {
                        value1 = Constants::DEFAULT_EMPTY_VALUE;
                    }
                    else {
                        value1 = organizations[minMaxIndex].contacts[0];
                        for (size_t k = 1; k < organizations[minMaxIndex].contacts.size(); ++k) {
                            value1 += "; " + organizations[minMaxIndex].contacts[k];
                        }
                    }
                }

                //���������� �������� ��� ������ �����������
                string value2;
                if (sortField == "�����������") {
                    value2 = organizations[j].name;
                }
                else if (sortField == "�����") {
                    value2 = organizations[j].addresses.empty() ? Constants::DEFAULT_EMPTY_VALUE : organizations[j].addresses[0]; // ����� ������ �����
                }
                else if (sortField == "���������� ����") {
                    //����������
                    if (organizations[j].contacts.empty()) {
                        value2 = Constants::DEFAULT_EMPTY_VALUE;
                    }
                    else {
                        value2 = organizations[j].contacts[0];
                        for (size_t k = 1; k < organizations[j].contacts.size(); ++k) {
                            value2 += "; " + organizations[j].contacts[k];
                        }
                    }
                }
                if (compareValues(value1, value2, ascending) < 0)
                {
                    minMaxIndex = j;
                }
            }
            if (minMaxIndex != i) {
                swap(organizations[i], organizations[minMaxIndex]);
            }
        }
    }
}

/** @brief ���������� ��� �������� value1 � value2 � ����������� �� ����������� ascending. */
int compareValues(const string& value1, const string& value2, bool ascending) {
    // ��������� �������, ����� ���� �� �������� �������� DEFAULT_EMPTY_VALUE
    if (value1 == Constants::DEFAULT_EMPTY_VALUE && value2 == Constants::DEFAULT_EMPTY_VALUE) {
        return 0; // ��� �������� �����, ������� �� �������
    }
    else if (value1 == Constants::DEFAULT_EMPTY_VALUE) {
        return ascending ? 1 : -1; // value1 ������, �������� ��� � ����� ��� ����� �����������
    }
    else if (value2 == Constants::DEFAULT_EMPTY_VALUE) {
        return ascending ? -1 : 1; // value2 ������, �������� ��� � ����� ��� ����� �����������
    }

    //������� ���������
    if (ascending) {
        return value1.compare(value2);
    }
    else {
        return value2.compare(value1);
    }
}

/** @brief ������� ��������� ��� ��� ����������. */
bool compareDates(const string& date1, const string& date2, bool ascending) {

    // ��������� �������, ����� ���� �� �������� �������� DEFAULT_EMPTY_VALUE
    if (date1 == Constants::DEFAULT_EMPTY_VALUE && date2 == Constants::DEFAULT_EMPTY_VALUE) {
        return false; // ��� �������� �����, ������� �� �������
    }
    else if (date1 == Constants::DEFAULT_EMPTY_VALUE) {
        return ascending ? true : false; // date1 ������, �������� ��� � ����� ��� ����� �����������
    }
    else if (date2 == Constants::DEFAULT_EMPTY_VALUE) {
        return ascending ? false : true; // date2 ������, �������� ��� � ����� ��� ����� �����������
    }

    if (!isValidDate(date1) && !isValidDate(date2)) {
        return false; // ��� ���� �����������
    }
    else if (!isValidDate(date1)) {
        return ascending; // ������ ���� �����������
    }
    else if (!isValidDate(date2)) {
        return !ascending; // ������ ���� �����������
    }

    int day1, month1, year1, day2, month2, year2;
    sscanf_s(date1.c_str(), "%2d.%2d.%4d", &day1, &month1, &year1);
    sscanf_s(date2.c_str(), "%2d.%2d.%4d", &day2, &month2, &year2);

    if (year1 != year2) {
        return (year1 < year2) ^ !ascending;
    }
    else if (month1 != month2) {
        return (month1 < month2) ^ !ascending;
    }
    else {
        return (day1 < day2) ^ !ascending;
    }
}