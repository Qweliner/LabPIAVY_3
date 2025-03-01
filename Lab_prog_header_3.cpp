#include "Lab_prog_header_3.h"

using namespace std;

// Инициализация глобальной переменной
string currentFolderPath = "";

/** @brief Определяет, является ли год високосным. */
bool is_leap(int year) {
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

/** @brief Проверяет допустимость ФИО. */
bool isValidFio(const string& fioStr) {
    bool valid_char = true;
    for (char c : fioStr) {
        if (not((c >= 'а' && c <= 'я') || (c >= 'А' && c <= 'Я') ||
            (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ' ||
            c == '-' || c == '\x2D'))
            valid_char = false;
    }
    return valid_char;
}

/** @brief Проверяет допустимость формата даты. */
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

/** @brief Проверяет допустимость имени файла. */
bool isValidFileName(const string& fileName) {
    const string invalidChars = "\\/:*?\"<>|";
    return fileName.find_first_of(invalidChars) == string::npos;
}

/** @brief Читает инструкции из файла. */
void readInstructionsFromFile(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line))
            cout << line << endl;
        file.close();
    }
    else
        cerr << "Не удалось открыть файл инструкции: " << filename << endl;
    cout << "\nНажмите любую клавишу для продолжения...";
    _getch();
}

/** @brief Запрашивает имя входного файла у пользователя. */
string getInputFilenameFromUser(const string& folderPath) {
    string filename;
    cout << "Введите имя входного файла (output.txt - Enter для использования по умолчанию): ";
    getline(cin, filename);

    if (filename.empty()) {
        filename = "output.txt";
    }

    return folderPath + filename;
}

/** @brief Считывает данные об организациях из файла. */
vector<Organization> readOrganizationsFromFile(const string& filename) {
    vector<Organization> organizations;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл: " << filename << endl;
        return organizations;
    }

    string line;
    Organization currentOrg;
    bool inAdditionalContacts = false;

    while (getline(file, line)) {
        if (line.rfind("Организация:", 0) == 0) {
            if (!currentOrg.name.empty()) {
                organizations.push_back(currentOrg);
            }
            currentOrg = {}; // Создаем новый пустой объект Organization
            currentOrg.name = line.substr(string("Организация:").length() + 1);
            if (currentOrg.name.empty()) currentOrg.name = Constants::DEFAULT_EMPTY_VALUE;
            inAdditionalContacts = false;
        }
        else if (line.rfind("Адрес:", 0) == 0) {
            string address = line.substr(string("Адрес:").length() + 1);
            if (address.empty()) address = Constants::DEFAULT_EMPTY_VALUE;
            currentOrg.addresses.push_back(address);
            inAdditionalContacts = false;
        }
        else if (line.rfind("Контактное лицо:", 0) == 0) {
            string contact = line.substr(string("Контактное лицо:").length() + 1);
            if (contact.empty()) contact = Constants::DEFAULT_EMPTY_VALUE;
            if (!isValidFio(contact) && contact != Constants::DEFAULT_EMPTY_VALUE) {
                cerr << "Некорректный формат ФИО: " << contact << endl;
            }
            currentOrg.contacts.push_back(contact);
            inAdditionalContacts = false;
        }
        else if (line == "Другие найденные данные компании:") {
            inAdditionalContacts = true;
        }
        else if (inAdditionalContacts && line.rfind("Адрес:", 0) == 0) {
            string address = line.substr(string("Адрес:").length() + 1);
            if (address.empty()) address = Constants::DEFAULT_EMPTY_VALUE;
            currentOrg.addresses.push_back(address);

            getline(file, line); // Считываем строку с контактным лицом
            if (line.rfind("Контактное лицо:", 0) == 0) {
                string contact = line.substr(string("Контактное лицо:").length() + 1);
                if (contact.empty()) contact = Constants::DEFAULT_EMPTY_VALUE;
                if (!isValidFio(contact) && contact != Constants::DEFAULT_EMPTY_VALUE) {
                    cerr << "Некорректный формат ФИО: " << contact << endl;
                }
                currentOrg.contacts.push_back(contact);
            }
        }
        else if (line.rfind("\tТип корреспонденции:", 0) == 0) {
            Correspondence corr;
            corr.type = line.substr(string("\tТип корреспонденции:").length() + 1);
            if (corr.type.empty()) corr.type = Constants::DEFAULT_EMPTY_VALUE;

            getline(file, line);
            if (line.rfind("\tДата:", 0) == 0) {
                corr.date = line.substr(string("\tДата:").length() + 1);
                if (corr.date.empty()) corr.date = Constants::DEFAULT_EMPTY_VALUE;
                if (!isValidDate(corr.date) && corr.date != Constants::DEFAULT_EMPTY_VALUE) {
                    cerr << "Некорректный формат даты: " << corr.date << endl;
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

/** @brief Выводит данные об организациях в файл. */
void writeOrganizationsToFile(const string& filename, const vector<Organization>& organizations, const string& sortField, bool ascending, const vector<TempCorrespondence>& sortedCorrespondences) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл для записи: " << filename << endl;
        return;
    }

    file << "Сортировка по полю: " << sortField << endl;
    file << "Направление сортировки: " << (ascending ? "По возрастанию" : "По убыванию") << endl;
    file << "--------------------\n";

    string previousGroupValue = ""; // Значение поля сортировки для предыдущей группы

    if (sortField == "Дата" || sortField == "Тип корреспонденции") {
        for (const auto& tempCorr : sortedCorrespondences) {
            string currentGroupValue;

            if (sortField == "Дата") {
                currentGroupValue = tempCorr.value;
            }
            else {
                currentGroupValue = tempCorr.organization->correspondences[tempCorr.correspondenceIndex].type; //Получаем тип
            }

            // Выводим заголовок группы, если значение изменилось
            if (currentGroupValue != previousGroupValue) {
                if (currentGroupValue == Constants::DEFAULT_EMPTY_VALUE) {
                    file << "-" << sortField << ": " << currentGroupValue << "-\n";
                }
                else if (sortField == "Дата" && !isValidDate(currentGroupValue)) {
                    file << Constants::INVALID_DATE_GROUP << "\n";
                }
                else {
                    file << "-" << sortField << ": " << currentGroupValue << "-\n";
                }
                previousGroupValue = currentGroupValue;
            }

            // Выводим данные организации
            file << "Организация: " << tempCorr.organization->name << endl;

            file << "Адрес: ";
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

            file << "Контактное лицо: ";
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
            //Eсли тип
            if (sortField == "Тип корреспонденции") {
                file << "\tТип корреспонденции: " << tempCorr.organization->correspondences[tempCorr.correspondenceIndex].type << endl;
                //Выводим все даты через запятую
                file << "\tДата: " << tempCorr.value << endl; // Выводим value, которое теперь содержит объединенные даты

            }
            else {
                file << "\tТип корреспонденции: " << tempCorr.organization->correspondences[tempCorr.correspondenceIndex].type << endl;
                file << "\tДата: " << tempCorr.organization->correspondences[tempCorr.correspondenceIndex].date << endl;
            }
            file << "--------------------\n";
        }
    }
    else {
        for (const auto& org : organizations) {
            //Определяем текущее значение
            string currentGroupValue = "";
            if (sortField == "Организация") {
                currentGroupValue = org.name;
            }
            else if (sortField == "Адрес") {
                currentGroupValue = org.addresses.empty() ? Constants::DEFAULT_EMPTY_VALUE : org.addresses[0];
                for (size_t i = 1; i < org.addresses.size(); ++i) {
                    currentGroupValue += "; " + org.addresses[i];
                }
            }
            else if (sortField == "Контактное лицо") {
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

            // Выводим заголовок группы, если значение изменилось

            if (currentGroupValue != previousGroupValue) {
                if (currentGroupValue == Constants::DEFAULT_EMPTY_VALUE) {
                    file << "-" << sortField << ": " << currentGroupValue << "-\n";
                }
                else if (sortField == "Контактное лицо" && currentGroupValue == Constants::INVALID_FIO_GROUP) {
                    file << Constants::INVALID_FIO_GROUP << "\n";
                }
                else {
                    file << "-" << sortField << ": " << currentGroupValue << "-\n";
                }
                previousGroupValue = currentGroupValue;
            }
            // Выводим данные организации
            file << "Организация: " << org.name << endl;

            file << "Адрес: ";
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

            file << "Контактное лицо: ";
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
                file << "\tКорреспонденция отсутствует\n";
            }
            else {
                for (const auto& corr : org.correspondences) {
                    file << "\tТип корреспонденции: " << corr.type << endl;
                    file << "\tДата: " << corr.date << endl;
                }
            }
            file << "--------------------\n";
        }
    }
    file.close();
    cout << "Данные успешно записаны в файл: " << filename << endl;
}

/** @brief Запускает процесс сортировки данных. */
void runSorting(string& folderPath, string& inputFilename) {
    inputFilename = getInputFilenameFromUser(folderPath);
    vector<Organization> organizations = readOrganizationsFromFile(inputFilename);

    if (organizations.empty()) {
        cout << "Нет данных для сортировки." << endl;
        cout << "Нажмите любую клавишу для возврата в меню...";
        _getch();
        return;
    }

    cout << "\nВыберите поле для сортировки:\n";
    cout << "1. Организация\n";
    cout << "2. Адрес\n";
    cout << "3. Контактное лицо\n";
    cout << "4. Тип корреспонденции\n";
    cout << "5. Дата\n";
    cout << "Esc - Вернуться в главное меню\n";
    cout << "\nВаш выбор: ";

    char choice = _getch();
    cout << endl;

    string sortField;
    switch (choice) {
    case '1':
        sortField = "Организация";
        break;
    case '2':
        sortField = "Адрес";
        break;
    case '3':
        sortField = "Контактное лицо";
        break;
    case '4':
        sortField = "Тип корреспонденции";
        break;
    case '5':
        sortField = "Дата";
        break;
    case 27: // Код клавиши Esc
        return;
    default:
        cerr << "Неверный выбор.\n";
        cout << "Нажмите любую клавишу для возврата в меню...";
        _getch();
        return;
    }

    cout << "\nВыберите направление сортировки:\n";
    cout << "1. По возрастанию (up)\n";
    cout << "2. По убыванию (down)\n";
    cout << "Esc - Вернуться в главное меню\n";
    cout << "\nВаш выбор: ";

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
        cerr << "Неверный выбор.\n";
        cout << "Нажмите любую клавишу для возврата в меню...";
        _getch();
        return;
    }

    vector<TempCorrespondence> sortedCorrespondences;
    sortOrganizations(organizations, sortField, ascending, sortedCorrespondences);

    string outputFilename = folderPath + "sorted_by_" + sortField + "_" + (ascending ? "up" : "down") + ".txt";
    writeOrganizationsToFile(outputFilename, organizations, sortField, ascending, sortedCorrespondences);

    cout << "Нажмите любую клавишу для возврата в меню...";
    _getch();
}

/** @brief Отображает главное меню. */
void menu() {
    bool folderPathSet = false;
    string inputFilename;

    while (true) {
        system("cls");
        cout << "Главное меню:\n";
        cout << "1. Сортировка данных ";
        if (!folderPathSet)
            cout << "(сначала укажите путь к папке)\n";
        else
            cout << '\n';
        cout << "2. Путь к папке\n";
        cout << "3. Инструкция\n";
        cout << "4. Выход\n\n";
        cout << "Текущий путь: " << currentFolderPath << "\n\n";
        cout << "> ";

        char mainChoice = _getch();

        switch (mainChoice) {
        case '1':
            if (!folderPathSet) {
                cerr << "Ошибка: сначала укажите путь к папке.\n";
                _getch();
            }
            else
                runSorting(currentFolderPath, inputFilename);
            break;
        case '2': {
            cout << "Введите путь к папке: ";
            getline(cin, currentFolderPath);

            struct stat buffer;
            if (stat(currentFolderPath.c_str(), &buffer) != 0) {
                if (errno == ENOENT)
                    cerr << "Ошибка: указанный путь не существует.\n";
                else
                    cerr << "Ошибка: не удалось проверить путь к папке. Код ошибки: "
                    << errno << "\n";
                _getch();
                continue;
            }

            if (currentFolderPath.back() != '\\')
                currentFolderPath += '\\';

            folderPathSet = true;
            cout << "\nТекущий путь к папке: " << currentFolderPath << endl;
            cout << "Нажмите любую клавишу для продолжения...";
            _getch();
            break;
        }
        case '3':
            readInstructionsFromFile(Constants::INSTRUCTIONS_FILE);
            break;
        case '4':
            return;
        default:
            cerr << "Неверный выбор.\n";
        }
    }
}

/** @brief Выполняет сортировку вектора организаций методом выбора. */
void sortOrganizations(vector<Organization>& organizations, const string& sortField, bool ascending, vector<TempCorrespondence>& sortedCorrespondences) {
    int n = organizations.size();

    if (sortField == "Дата" || sortField == "Тип корреспонденции") {
        // 1. Создаем временный вектор
        vector<TempCorrespondence> tempCorrespondences;
        for (int i = 0; i < n; ++i) {
            for (size_t j = 0; j < organizations[i].correspondences.size(); ++j) {
                TempCorrespondence tempCorr;
                tempCorr.organization = &organizations[i];
                tempCorr.correspondenceIndex = j;
                tempCorr.value = (sortField == "Дата") ? organizations[i].correspondences[j].date : organizations[i].correspondences[j].type;
                tempCorrespondences.push_back(tempCorr);
            }
        }

        // 2. Сортируем временный вектор методом выбора
        for (size_t i = 0; i < tempCorrespondences.size() - 1; ++i) {
            size_t minMaxIndex = i;
            for (size_t j = i + 1; j < tempCorrespondences.size(); ++j) {
                bool swap = false;
                if (sortField == "Дата") {
                    if (compareDates(tempCorrespondences[j].value, tempCorrespondences[minMaxIndex].value, ascending)) {
                        minMaxIndex = j;
                    }
                }
                else { // sortField == "Тип корреспонденции"
                    if (compareValues(tempCorrespondences[j].value, tempCorrespondences[minMaxIndex].value, ascending) < 0) {
                        minMaxIndex = j;
                    }
                }
            }
            if (minMaxIndex != i) {
                swap(tempCorrespondences[i], tempCorrespondences[minMaxIndex]);
            }
        }
        //Если тип
        if (sortField == "Тип корреспонденции") {
            vector<TempCorrespondence> mergedCorrespondences;
            for (const auto& tempCorr : tempCorrespondences) {
                bool merged = false;
                for (auto& mergedCorr : mergedCorrespondences) {
                    // Если орг совпадает и тип совпадает
                    if (tempCorr.organization == mergedCorr.organization &&
                        tempCorr.organization->correspondences[tempCorr.correspondenceIndex].type ==
                        mergedCorr.organization->correspondences[mergedCorr.correspondenceIndex].type) {
                        // Объединяем даты
                        mergedCorr.value += ", " + tempCorr.organization->correspondences[tempCorr.correspondenceIndex].date;
                        merged = true;
                        break;
                    }
                }
                if (!merged) {
                    // Создаем новую запись в mergedCorrespondences
                    TempCorrespondence newCorr;
                    newCorr.organization = tempCorr.organization;
                    newCorr.correspondenceIndex = tempCorr.correspondenceIndex;
                    newCorr.value = tempCorr.organization->correspondences[tempCorr.correspondenceIndex].date;  // value - это строка с датами!
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

                //Определяем значение для первой организации
                string value1;
                if (sortField == "Организация") {
                    value1 = organizations[minMaxIndex].name;
                }
                else if (sortField == "Адрес") {
                    value1 = organizations[minMaxIndex].addresses.empty() ? Constants::DEFAULT_EMPTY_VALUE : organizations[minMaxIndex].addresses[0]; // Берем первый адрес
                }
                else if (sortField == "Контактное лицо") {
                    //Объединяем
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

                //Определяем значение для второй организации
                string value2;
                if (sortField == "Организация") {
                    value2 = organizations[j].name;
                }
                else if (sortField == "Адрес") {
                    value2 = organizations[j].addresses.empty() ? Constants::DEFAULT_EMPTY_VALUE : organizations[j].addresses[0]; // Берем первый адрес
                }
                else if (sortField == "Контактное лицо") {
                    //Объединяем
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

/** @brief Сравнивает два значения value1 и value2 в зависимости от направления ascending. */
int compareValues(const string& value1, const string& value2, bool ascending) {
    // Обработка случаев, когда одно из значений является DEFAULT_EMPTY_VALUE
    if (value1 == Constants::DEFAULT_EMPTY_VALUE && value2 == Constants::DEFAULT_EMPTY_VALUE) {
        return 0; // Оба значения пусты, считаем их равными
    }
    else if (value1 == Constants::DEFAULT_EMPTY_VALUE) {
        return ascending ? 1 : -1; // value1 пустое, помещаем его в конец при любом направлении
    }
    else if (value2 == Constants::DEFAULT_EMPTY_VALUE) {
        return ascending ? -1 : 1; // value2 пустое, помещаем его в конец при любом направлении
    }

    //Обычное сравнение
    if (ascending) {
        return value1.compare(value2);
    }
    else {
        return value2.compare(value1);
    }
}

/** @brief Функция сравнения дат для сортировки. */
bool compareDates(const string& date1, const string& date2, bool ascending) {

    // Обработка случаев, когда одно из значений является DEFAULT_EMPTY_VALUE
    if (date1 == Constants::DEFAULT_EMPTY_VALUE && date2 == Constants::DEFAULT_EMPTY_VALUE) {
        return false; // Оба значения пусты, считаем их равными
    }
    else if (date1 == Constants::DEFAULT_EMPTY_VALUE) {
        return ascending ? true : false; // date1 пустое, помещаем его в конец при любом направлении
    }
    else if (date2 == Constants::DEFAULT_EMPTY_VALUE) {
        return ascending ? false : true; // date2 пустое, помещаем его в конец при любом направлении
    }

    if (!isValidDate(date1) && !isValidDate(date2)) {
        return false; // Обе даты некорректны
    }
    else if (!isValidDate(date1)) {
        return ascending; // Первая дата некорректна
    }
    else if (!isValidDate(date2)) {
        return !ascending; // Вторая дата некорректна
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