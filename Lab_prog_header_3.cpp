#include "Lab_prog_header_3.h"

using namespace std;

// Инициализация глобальной переменной
string currentFolderPath = "";

/** @brief Выводит табуляцию. */
void tabul(int x) {
    for (int i = x; i != 0; i--)
        printf("   "); // Вывод трех пробелов
}

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

    // Если пользователь нажал Enter, используем имя файла по умолчанию
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
        return organizations; // Возвращаем пустой вектор
    }

    string line;
    Organization currentOrg;
    bool inAdditionalContacts = false;

    while (getline(file, line)) {
        if (line.rfind("Организация:", 0) == 0) {
            // Начало нового блока организации
            if (!currentOrg.name.empty()) {
                // Сохраняем предыдущую организацию, если она была заполнена
                organizations.push_back(currentOrg);
            }
            currentOrg = {}; // Создаем новый пустой объект Organization
            currentOrg.name = line.substr(string("Организация:").length() + 1);
            if (currentOrg.name.empty()) currentOrg.name = Constants::DEFAULT_EMPTY_VALUE; // Обработка пустого значения
            inAdditionalContacts = false;
        }
        else if (line.rfind("Адрес:", 0) == 0) {
            currentOrg.address = line.substr(string("Адрес:").length() + 1);
            if (currentOrg.address.empty()) currentOrg.address = Constants::DEFAULT_EMPTY_VALUE; // Обработка пустого значения
            inAdditionalContacts = false;
        }
        else if (line.rfind("Контактное лицо:", 0) == 0) {
            currentOrg.contactPerson = line.substr(string("Контактное лицо:").length() + 1);
            if (currentOrg.contactPerson.empty()) currentOrg.contactPerson = Constants::DEFAULT_EMPTY_VALUE; // Обработка пустого значения
            inAdditionalContacts = false;
        }
        else if (line == "Другие найденные данные компании:") {
            inAdditionalContacts = true;
        }
        else if (inAdditionalContacts && line.rfind("Адрес:", 0) == 0) {
            string address = line.substr(string("Адрес:").length() + 1);
            if (address.empty()) address = Constants::DEFAULT_EMPTY_VALUE; // Обработка пустого значения
            getline(file, line); // Считываем строку с контактным лицом
            if (line.rfind("Контактное лицо:", 0) == 0) {
                string contactPerson = line.substr(string("Контактное лицо:").length() + 1);
                if (contactPerson.empty()) contactPerson = Constants::DEFAULT_EMPTY_VALUE; // Обработка пустого значения
                currentOrg.additionalContacts.push_back({ address, contactPerson });
            }
        }
        else if (line.rfind("\tТип корреспонденции:", 0) == 0) {
            // Читаем данные о корреспонденции
            Correspondence corr;
            corr.type = line.substr(string("\tТип корреспонденции:").length() + 1);
            if (corr.type.empty()) corr.type = Constants::DEFAULT_EMPTY_VALUE; // Обработка пустого значения

            getline(file, line); // Считываем строку с датой
            if (line.rfind("\tДата:", 0) == 0) {
                corr.date = line.substr(string("\tДата:").length() + 1);
                if (corr.date.empty()) corr.date = Constants::DEFAULT_EMPTY_VALUE; // Обработка пустого значения
            }
            currentOrg.correspondences.push_back(corr);
            inAdditionalContacts = false;
        }
        else if (line == "--------------------") {
            // Конец блока организации
            organizations.push_back(currentOrg);
            currentOrg = {};
            inAdditionalContacts = false;
        }
    }

    // Добавляем последнюю организацию, если файл не заканчивается разделителем
    if (!currentOrg.name.empty()) {
        organizations.push_back(currentOrg);
    }

    file.close();
    return organizations;
}

/** @brief Выводит данные об организациях в файл. */
void writeOrganizationsToFile(const string& filename, const vector<Organization>& organizations, const string& sortField, bool ascending, const vector<int>& missingValuesIndices) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл для записи: " << filename << endl;
        return;
    }

    // Записываем информацию о сортировке в файл
    file << "Сортировка по полю: " << sortField << endl;
    file << "Направление сортировки: " << (ascending ? "По возрастанию" : "По убыванию") << endl;
    file << "--------------------\n";

    for (size_t i = 0; i < organizations.size(); ++i) {
        const auto& org = organizations[i];

        file << "Организация: " << org.name << endl;
        file << "Адрес: " << org.address << endl;
        file << "Контактное лицо: " << org.contactPerson << endl;

        if (!org.additionalContacts.empty()) {
            file << "Другие найденные данные компании:\n";
            for (const auto& contact : org.additionalContacts) {
                file << "Адрес: " << contact.first << endl;
                file << "Контактное лицо: " << contact.second << endl;
            }
        }

        if (org.correspondences.empty()) {
            file << "\tКорреспонденция отсутствует\n";
        }
        else {
            for (const auto& corr : org.correspondences) {
                file << "\tТип корреспонденции: " << corr.type << endl;
                file << "\tДата: " << corr.date << endl;
            }
        }

        // Выводим сообщение об отсутствии значения, если организация была перемещена в конец списка из-за этого
        bool missingValue = false;
        for (int index : missingValuesIndices) {
            if (index == i) {
                missingValue = true;
                break;
            }
        }
        if (missingValue) {
            file << "\t" << sortField << " отсутствует\n";
        }

        file << "--------------------\n";
    }

    file.close();
    cout << "Данные успешно записаны в файл: " << filename << endl;
}

/** @brief Запускает процесс сортировки данных. */
void runSorting(string& folderPath, string& inputFilename) {
    // Получаем имя входного файла от пользователя
    inputFilename = getInputFilenameFromUser(folderPath);

    // Считываем данные об организациях из файла
    vector<Organization> organizations = readOrganizationsFromFile(inputFilename);

    // Выводим сообщение, если не удалось прочитать организации
    if (organizations.empty()) {
        cout << "Нет данных для сортировки." << endl;
        cout << "Нажмите любую клавишу для возврата в меню...";
        _getch();
        return;
    }

    // Выбор поля для сортировки
    cout << "\nВыберите поле для сортировки:\n";
    cout << "1. Организация\n";
    cout << "2. Адрес\n";
    cout << "3. Контактное лицо\n";
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
    case 27:
        return;
    default:
        cerr << "Неверный выбор.\n";
        cout << "Нажмите любую клавишу для возврата в меню...";
        _getch();
        return;
    }

    // Выбор направления сортировки
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

    // Вектор для хранения индексов организаций, у которых отсутствуют значения в поле сортировки
    vector<int> missingValuesIndices;

    // Выполняем сортировку
    sortOrganizations(organizations, sortField, ascending, missingValuesIndices);

    // Формируем имя файла для сохранения результатов
    string outputFilename = folderPath + "sorted_by_" + sortField + "_" + (ascending ? "up" : "down") + ".txt";

    // Записываем отсортированные данные в файл
    writeOrganizationsToFile(outputFilename, organizations, sortField, ascending, missingValuesIndices);

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
                continue; // Возврат в начало цикла, если путь некорректен
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
            return; // Выход из программы
        default:
            cerr << "Неверный выбор.\n";
        }
    }
}

/** @brief Выполняет сортировку вектора организаций методом выбора.
   @param organizations Вектор организаций для сортировки.
   @param sortField Поле, по которому выполняется сортировка.
   @param ascending Направление сортировки (true - по возрастанию, false - по убыванию).
   @param missingValuesIndices Вектор индексов организаций, у которых отсутствуют значения в поле сортировки.
*/
void sortOrganizations(vector<Organization>& organizations, const string& sortField, bool ascending, vector<int>& missingValuesIndices) {
    int n = organizations.size();

    // Сначала собираем индексы организаций с отсутствующими значениями
    for (int i = 0; i < n; ++i) {
        bool hasValue = true;
        if (sortField == "Организация") {
            if (organizations[i].name == Constants::DEFAULT_EMPTY_VALUE) hasValue = false;
        }
        else if (sortField == "Адрес") {
            if (organizations[i].address == Constants::DEFAULT_EMPTY_VALUE) hasValue = false;
        }
        else if (sortField == "Контактное лицо") {
            if (organizations[i].contactPerson == Constants::DEFAULT_EMPTY_VALUE) hasValue = false;
        }

        if (!hasValue) {
            missingValuesIndices.push_back(i);
        }
    }

    // Сортируем только организации с имеющимися значениями
    for (int i = 0; i < n - 1; ++i) {
        // Пропускаем организации, которые будут перемещены в конец списка
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
            // Пропускаем организации, которые будут перемещены в конец списка
            bool skipJ = false;
            for (int index : missingValuesIndices) {
                if (index == j) {
                    skipJ = true;
                    break;
                }
            }
            if (skipJ) continue;

            bool swap = false;
            if (sortField == "Организация") {
                if (ascending) {
                    swap = organizations[j].name < organizations[minIndex].name;
                }
                else {
                    swap = organizations[j].name > organizations[minIndex].name;
                }
            }
            else if (sortField == "Адрес") {
                if (ascending) {
                    swap = organizations[j].address < organizations[minIndex].address;
                }
                else {
                    swap = organizations[j].address > organizations[minIndex].address;
                }
            }
            else if (sortField == "Контактное лицо") {
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

    // Перемещаем организации с отсутствующими значениями в конец списка
    int missingCount = missingValuesIndices.size();
    for (int i = 0; i < missingCount; ++i) {
        int indexToMove = missingValuesIndices[i] - i; // Корректируем индекс, так как элементы сдвигаются
        if (indexToMove >= 0 && indexToMove < organizations.size()) {
            Organization temp = organizations[indexToMove];
            organizations.erase(organizations.begin() + indexToMove);
            organizations.push_back(temp);
            // Обновляем индексы в missingValuesIndices, чтобы они указывали на правильные позиции
            for (int j = i + 1; j < missingCount; ++j) {
                if (missingValuesIndices[j] > indexToMove) {
                    missingValuesIndices[j]--;
                }
            }
        }
    }
}

/** @brief Функция сравнения дат для сортировки. */
bool compareDates(const string& date1, const string& date2, bool ascending) {
    // TODO: Реализовать функцию сравнения дат с учетом неверных форматов.
    // Неверные даты считать наименьшими/наибольшими в зависимости от направления сортировки.
    return false; // Заглушка
}