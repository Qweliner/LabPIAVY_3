//Lab_prog_header_3.cpp

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

/** @brief ������������ ���������� �� �����������.
 *  ������� � ������� ������ ����������� �� ������ ��������������� � �������.
 */
void processOrganization(const string& orgName, const string& corrFilename, const string& addrFilename, bool selectiveOutput, ofstream* outfile, set<string>& printedOrganizations, vector<string>& outputBuffer) {
    ifstream corrFile(corrFilename);
    ifstream addrFile(addrFilename);
    vector<pair<string, string>> addresses;
    bool orgFoundInAddress = false;
    int lineNumber = 1;

    auto writeToBuffer = [&](const string& str) {
        outputBuffer.push_back(str); // ���������� ������ � �����
        };

    if (addrFile.is_open()) {
        string line;
        getline(addrFile, line); // ���������� ���������
        lineNumber++;

        while (getline(addrFile, line)) {
            size_t pos1 = line.find('\t');
            size_t pos2 = line.find('\t', pos1 + 1);

            if (pos1 != string::npos && pos2 != string::npos) {
                string currentOrg = line.substr(0, pos1);
                if (currentOrg == orgName) {
                    addresses.push_back({ line.substr(pos1 + 1, pos2 - pos1 - 1),
                                         line.substr(pos2 + 1) });
                    orgFoundInAddress = true;
                    string contactFio = line.substr(pos2 + 1);
                    if (!isValidFio(contactFio)) {
                        writeToBuffer("������ � �����: " + addrFilename + ", ������: " + to_string(lineNumber) +
                            ", ������������ �������� ���� ���: " + contactFio);
                    }
                }
            }
            if (!selectiveOutput && outfile &&
                printedOrganizations.find(orgName) != printedOrganizations.end())
                break; // ���������, ���� �� ����������� � ��� ���������
            lineNumber++;
        }
        addrFile.close();
    }
    else {
        writeToBuffer("�� ������� ������� ���� �������: " + addrFilename);
        return;
    }

    if (!orgFoundInAddress) {
        if (selectiveOutput)
            writeToBuffer("����������� " + orgName + " �� ������� � ����� �������.");
        return;
    }

    if (selectiveOutput) {
        writeToBuffer("�����������: " + orgName);
        if (addresses.size() > 0) {
            for (size_t i = 0; i < addresses.size(); ++i) {
                if (i == 0) {
                    writeToBuffer("�����: " + addresses[i].first);
                    writeToBuffer("���������� ����: " + addresses[i].second);
                }
                else {
                    if (i == 1)
                        writeToBuffer("\n������ ��������� ������ ��������:\n");
                    writeToBuffer("\n�����: " + addresses[i].first);
                    writeToBuffer("���������� ����: " + addresses[i].second);
                }
            }
        }
    }
    else if (outfile) {
        if (printedOrganizations.find(orgName) == printedOrganizations.end()) {
            printedOrganizations.insert(orgName);
            *outfile << "�����������: " << orgName << endl;
            if (addresses.size() > 0) {
                for (size_t i = 0; i < addresses.size(); ++i) {
                    if (i == 0) {
                        *outfile << "�����: " << addresses[i].first << endl;
                        *outfile << "���������� ����: " << addresses[i].second << endl;
                    }
                    else {
                        if (i == 1)
                            *outfile << "\n������ ��������� ������ ��������:\n";
                        *outfile << "\n�����: " << addresses[i].first << endl;
                        *outfile << "���������� ����: " << addresses[i].second << endl;
                    }
                }
            }
        }
    }

    if (orgFoundInAddress) {
        if (selectiveOutput ||
            (outfile && printedOrganizations.find(orgName) !=
                printedOrganizations.end())) {
            if (corrFile.is_open()) {
                string line;
                getline(corrFile, line); // ���������� ���������
                lineNumber = 1;
                lineNumber++;
                bool foundCorrespondence = false;
                while (getline(corrFile, line)) {
                    size_t pos1 = line.find('\t');
                    size_t pos2 = line.find('\t', pos1 + 1);

                    if (pos1 != string::npos && pos2 != string::npos) {
                        string currentOrg = line.substr(pos2 + 1);
                        if (currentOrg == orgName) {
                            foundCorrespondence = true;
                            string type = line.substr(0, pos1);
                            string date = line.substr(pos1 + 1, pos2 - pos1 - 1);
                            bool dateError = !isValidDate(date);

                            if (dateError)
                                writeToBuffer("������ � �����: " + corrFilename + ", ������: " + to_string(lineNumber)
                                    + ", ������������ �������� ���� ����: " + date);

                            if (selectiveOutput) {
                                writeToBuffer("\t��� ���������������: " + type);
                                writeToBuffer("\t����: " + date);
                            }
                            else if (outfile) {
                                *outfile << "\t��� ���������������: " << type << endl;
                                *outfile << "\t����: " << date << endl;
                            }
                        }
                    }
                    lineNumber++;
                }
                if (!foundCorrespondence && selectiveOutput)
                    writeToBuffer("��������������� ��� ����������� " + orgName + " �� �������.");
                corrFile.close();
            }
            else
                writeToBuffer("�� ������� ������� ���� ���������������: " + corrFilename);
        }
    }
    if (selectiveOutput)
        writeToBuffer("--------------------");
    else if (outfile)
        *outfile << "--------------------" << endl;
}

/** @brief ����������� ����� ������ � ������������. */
pair<string, string> getFilenamesFromUser(const string& folderPath) {
    string correspondenceFilename, addressesFilename;

    cout << "������� ��� ����� � ��������� ���������������� (��� ����������): ";
    cin >> correspondenceFilename;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!isValidFileName(correspondenceFilename)) {
        tabul(11);
        printf("������������ ������� � ����� �����. ����������, ����������� ������ "
            "���.\n");
        printf("��� ����������� ������� ENTER.");
        system("PAUSE>nul");
    }

    cout << "������� ��� ����� � �������� ����������� (��� ����������): ";
    cin >> addressesFilename;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!isValidFileName(addressesFilename)) {
        tabul(11);
        printf("������������ ������� � ����� �����. ����������, ����������� ������ "
            "���.\n");
        printf("��� ����������� ������� ENTER.");
        system("PAUSE>nul");
    }

    return { folderPath + "IC_" + correspondenceFilename + ".txt",
            folderPath + "AO_" + addressesFilename + ".txt" };
}

/** @brief ��������� �������� ������� ���������. */
void runProgram(string& folderPath, string& correspondenceFilename, string& addressesFilename, string& outputFilename) {
    pair<string, string> filenames = getFilenamesFromUser(folderPath);
    correspondenceFilename = filenames.first;
    addressesFilename = filenames.second;

    if (folderPath.back() != '\\' && folderPath.back() != '/')
        folderPath += '\\';

    outputFilename = folderPath + "output.txt";

    char choice;
    cout << "\n�������� ����� ������:\n";
    cout << "1. ������������� ����� �� �����\n";
    cout << "2. ������ ����� � ����\n";
    cout << "Esc - ��������� � ������� ����\n";
    cout << "\n��� �����: ";

    choice = _getch();
    cout << endl;

    switch (choice) {
    case '1': {
        string orgName;
        cout << "������� �������� ����������� ��� ������: ";
        getline(cin, orgName);
        ofstream* outfile = nullptr;
        set<string> printedOrganizations;
        vector<string> outputBuffer; // ������� ����� ��� �������� ������
        processOrganization(orgName, correspondenceFilename, addressesFilename,
            true, outfile, printedOrganizations, outputBuffer);

        if (outputBuffer.size() > 28) { // ���� ����� ������, ��� 28
            string currentFolderPathselectiveoutput = folderPath + "selective_output.txt";
            ofstream selectiveOutfile(currentFolderPathselectiveoutput);
            if (selectiveOutfile.is_open()) { // ���������, ��� ���� ������
                for (const string& line : outputBuffer)
                    selectiveOutfile << line << endl;
                selectiveOutfile.close();
                cout << "\n��������� �������� � ����� selective_output.txt, ��� ��� �������� 28 �����.\n";
                cout << "���� � ����������: " << currentFolderPathselectiveoutput << "\n";
            }
            else
                cerr << "������: �� ������� ������� ���� ��� ������ �������������� ������.\n";
        }
        else { // �����, ���� ����� �� ������ 28, �� ������� � �������
            for (const string& line : outputBuffer)
                cout << line << endl;
        }
        break;
    }
    case '2': {
        ofstream outfile(outputFilename);
        if (!outfile.is_open()) {
            cerr << "������: �� ������� ������� ���� ��� ������: " << outputFilename
                << endl;
            return;
        }
        set<string> printedOrganizations;
        vector<string> outputBuffer; // ���� ����� ������ �� �����
        ifstream addrFile(addressesFilename);
        if (addrFile.is_open()) {
            string line;
            getline(addrFile, line); // ���������� ���������
            while (getline(addrFile, line)) {
                size_t pos1 = line.find('\t');
                if (pos1 != string::npos) {
                    string orgName = line.substr(0, pos1);
                    if (printedOrganizations.find(orgName) == printedOrganizations.end())
                        processOrganization(orgName, correspondenceFilename,
                            addressesFilename, false, &outfile,
                            printedOrganizations, outputBuffer); // ����������� ����� ��� �������������
                }
            }
            addrFile.close();
        }
        outfile.close();
        cout << "\n���������� ������� �������� � ����: " << outputFilename << endl;
        break;
    }
    case 27:
        return;
    default:
        cerr << "�������� �����.\n";
    }

    cout << "������� ����� ������� ��� �������� � ����...";
    _getch();
}

/** @brief ���������� ������� ����. */
void menu() {
    bool folderPathSet = false;
    string correspondenceFilename, addressesFilename, outputFilename;
    string currentFolderPath = "";
    const string instructionsFile = "instructions.txt";

    while (true) {
        system("cls");
        cout << "������� ����:\n";
        cout << "1. ������ ������ ";
        if (!folderPathSet)
            cout << "(������� ������� ���� � �����)\n";
        else
            cout << '\n';
        cout << "2. ���� � �����\n";
        cout << "3. ����������\n\n";
        cout << "������� ����: " << currentFolderPath << "\n\n";
        cout << "Esc - �����\n\n";
        cout << "> ";

        char mainChoice = _getch();

        switch (mainChoice) {
        case '1':
            if (!folderPathSet) {
                cerr << "������: ������� ������� ���� � �����.\n";
                _getch();
            }
            else
                runProgram(currentFolderPath, correspondenceFilename, addressesFilename,
                    outputFilename);
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
            readInstructionsFromFile(instructionsFile);
            break;
        case 27:
            return;
        default:
            cerr << "�������� �����.\n";
        }
    }
}