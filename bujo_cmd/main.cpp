#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <map>

class JournalEntry
{
private:
    std::string date;
    std::string title;
    std::vector<std::string> tags;
    std::string content;

public:
    // �غc�l
    JournalEntry(const std::string& date, const std::string& title, const std::string& content) :
        date(date), title(title), content(content) {}

    // Getter �M Setter
    std::string getDate() const
    {
        return date;
    }
    void setDate(const std::string& date)
    {
        this->date = date;
    }

    std::string getTitle() const
    {
        return title;
    }
    void setTitle(const std::string& title)
    {
        this->title = title;
    }

    std::string getContent() const
    {
        return content;
    }
    void setContent(const std::string& content)
    {
        this->content = content;
    }

    void addTag(const std::string& tag)
    {
        tags.push_back(tag);
    }
    std::vector<std::string> getTags() const
    {
        return tags;
    }

    // �ˬd���جO�_�]�t�Ҧ����w������ (AND �޿�)
    bool hasTags(const std::vector<std::string>& searchTags) const
    {
        for (const std::string& searchTag : searchTags)
        {
            bool found = false;
            for (const std::string& tag : tags)
            {
                if (tag == searchTag)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                return false;
            }
        }
        return true;
    }
};


// �P�_�O�_���|�~
bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// �Y�~�Y�몺�Ѽ�?
int getDaysInMonth(int year, int month)
{
    static const int days[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && isLeapYear(year))
    {
        return 29;
    }
    else
    {
        return days[month];
    }
}

// �Y�~�Y��Y��O�P���X? (Zeller's congruence)
int getDayOfWeek(int year, int month, int day)
{
    if (month < 3)
    {
        month += 12;
        year--;
    }
    int c = year / 100;
    int y = year % 100;
    int w = (c / 4 - 2 * c + y + y / 4 + 13 * (month + 1) / 5 + day - 1) % 7;
    if (w < 0)
    {
        w += 7;
    }
    return w; // 0 = �P����, 1 = �P���@, ..., 6 = �P����
}

// �q����r�ꤤ�����~�B��B�� (YYYY-MM-DD)
void extractDate(const std::string& dateStr, int& year, int& month, int& day)
{
    sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day);
}

// ���Ҥ���榡�M���ĩ�
bool isValidDate(const std::string& date)
{
    int year, month, day;
    if (sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day) != 3)
    {
        return false; // �榡����
    }

    if (year < 1900 || year > 9999)
    {
        return false; // �~���W�X�d��
    }

    if (month < 1 || month > 12)
    {
        return false; // ����L��
    }

    if (day < 1 || day > getDaysInMonth(year, month))
    {
        return false; // ����L��
    }

    return true;
}

void addEntry(std::vector<JournalEntry>& journal)
{
    std::string date, title, content, tag;
    std::vector<std::string> tags;

    std::cout << "�п�J��� (YYYY-MM-DD): ";
    std::getline(std::cin, date);

    if (!isValidDate(date))
    {
        std::cout << "�L�Ī�����榡�Τ���C\n";
        return;
    }

    std::cout << "�п�J���D: ";
    std::getline(std::cin, title);

    std::cout << "�п�J���e: ";
    std::getline(std::cin, content);

    std::cout << "�п�J���� (�ΪŮ���j�A��J�����Ы�Enter): ";
    std::getline(std::cin, tag);

    std::stringstream ss(tag);
    std::string singleTag;
    while (ss >> singleTag)
    {
        tags.push_back(singleTag);
    }

    JournalEntry newEntry(date, title, content);
    for (const auto& t : tags)
    {
        newEntry.addTag(t);
    }
    journal.push_back(newEntry);

    std::cout << "���ؤw�s�W!\n";
}

void viewEntries(const std::vector<JournalEntry>& journal)
{
    std::string input;
    std::cout << "�п���˵��覡�G\n";
    std::cout << "1. �̤���˵�\n";
    std::cout << "2. �̼��D�j�M\n";
    std::cout << "3. �̼��ҷj�M (�䴩�h�Ӽ��ҡAAND �޿�)\n";
    std::cout << "4. �̤���d��j�M\n";
    std::cout << "�п�J�ﶵ (1-4): ";
    std::getline(std::cin, input);

    if (input == "1")
    {
        std::string date;
        std::cout << "�п�J��� (YYYY-MM-DD, YYYY-MM �� YYYY): ";
        std::getline(std::cin, date);

        // �ˬd��J������榡
        int dateLength = date.length();
        if (dateLength != 10 && dateLength != 7 && dateLength != 4)
        {
            std::cout << "�L�Ī�����榡�C\n";
            return;
        }

        for (const auto& entry : journal)
        {
            if (entry.getDate().substr(0, dateLength) == date)
            {
                std::cout << "���: " << entry.getDate() << "\n";
                std::cout << "���D: " << entry.getTitle() << "\n";
                std::cout << "����: ";
                for (const auto& tag : entry.getTags())
                {
                    std::cout << tag << " ";
                }
                std::cout << "\n";
                std::cout << "���e: " << entry.getContent() << "\n\n";
            }
        }
    }
    else if (input == "2")
    {
        std::string title;
        std::cout << "�п�J���D����r: ";
        std::getline(std::cin, title);

        for (const auto& entry : journal)
        {
            if (entry.getTitle().find(title) != std::string::npos)
            {
                std::cout << "���: " << entry.getDate() << "\n";
                std::cout << "���D: " << entry.getTitle() << "\n";
                std::cout << "����: ";
                for (const auto& tag : entry.getTags())
                {
                    std::cout << tag << " ";
                }
                std::cout << "\n";
                std::cout << "���e: " << entry.getContent() << "\n\n";
            }
        }
    }
    else if (input == "3")
    {
        std::string tagInput;
        std::vector<std::string> searchTags;
        std::cout << "�п�J�n�j�M������ (�ΪŮ���j): ";
        std::getline(std::cin, tagInput);

        std::stringstream ss(tagInput);
        std::string tag;
        while (ss >> tag)
        {
            searchTags.push_back(tag);
        }

        for (const auto& entry : journal)
        {
            if (entry.hasTags(searchTags))
            {
                std::cout << "���: " << entry.getDate() << "\n";
                std::cout << "���D: " << entry.getTitle() << "\n";
                std::cout << "����: ";
                for (const auto& tag : entry.getTags())
                {
                    std::cout << tag << " ";
                }
                std::cout << "\n";
                std::cout << "���e: " << entry.getContent() << "\n\n";
            }
        }
    }
    else if (input == "4")
    {
        std::string startDate, endDate;
        std::cout << "�п�J�_�l��� (YYYY-MM-DD, YYYY-MM �� YYYY): ";
        std::getline(std::cin, startDate);
        std::cout << "�п�J������� (YYYY-MM-DD, YYYY-MM �� YYYY): ";
        std::getline(std::cin, endDate);

        // �ˬd��J������榡
        int startDateLength = startDate.length();
        int endDateLength = endDate.length();
        if ((startDateLength != 10 && startDateLength != 7 && startDateLength != 4) ||
                (endDateLength != 10 && endDateLength != 7 && endDateLength != 4))
        {
            std::cout << "�L�Ī�����榡�C\n";
            return;
        }

        for (const auto& entry : journal)
        {
            if (entry.getDate().substr(0, startDateLength) >= startDate &&
                    entry.getDate().substr(0, endDateLength) <= endDate)
            {
                std::cout << "���: " << entry.getDate() << "\n";
                std::cout << "���D: " << entry.getTitle() << "\n";
                std::cout << "����: ";
                for (const auto& tag : entry.getTags())
                {
                    std::cout << tag << " ";
                }
                std::cout << "\n";
                std::cout << "���e: " << entry.getContent() << "\n\n";
            }
        }
    }
    else
    {
        std::cout << "�L�Ī��ﶵ�C\n";
    }
}

void editEntry(std::vector<JournalEntry>& journal)
{
    if (journal.empty())
    {
        std::cout << "�S�����إi�H�s��C\n";
        return;
    }

    std::cout << "�п�J�n�s�誺���ؽs�� (��J 0 ��ܩҦ�����):\n";
    std::string indexStr;
    std::getline(std::cin, indexStr);
    int index = std::stoi(indexStr);

    if (index == 0)
    {
        for (size_t i = 0; i < journal.size(); ++i)
        {
            std::cout << i + 1 << ". " << journal[i].getDate() << " - " << journal[i].getTitle() << "\n";
        }
        std::cout << "�п�J�n�s�誺���ؽs��: ";
        std::getline(std::cin, indexStr);
        index = std::stoi(indexStr);
    }

    if (index > 0 && index <= journal.size())
    {
        JournalEntry& entry = journal[index - 1];
        std::string input;

        std::cout << "�n�s�誺���ءG\n";
        std::cout << "���: " << entry.getDate() << "\n";
        std::cout << "���D: " << entry.getTitle() << "\n";
        std::cout << "����: ";
        for (const auto& tag : entry.getTags())
        {
            std::cout << tag << " ";
        }
        std::cout << "\n";
        std::cout << "���e: " << entry.getContent() << "\n\n";

        std::cout << "�п�ܭn�s�誺���G\n";
        std::cout << "1. ���\n";
        std::cout << "2. ���D\n";
        std::cout << "3. ����\n";
        std::cout << "4. ���e\n";
        std::cout << "�п�J�ﶵ (1-4): ";
        std::getline(std::cin, input);

        if (input == "1")
        {
            std::string newDate;
            std::cout << "�п�J�s����� (YYYY-MM-DD): ";
            std::getline(std::cin, newDate);

            if (!isValidDate(newDate))
            {
                std::cout << "�L�Ī�����榡�Τ���C\n";
                return;
            }

            entry.setDate(newDate);
        }
        else if (input == "2")
        {
            std::string newTitle;
            std::cout << "�п�J�s�����D: ";
            std::getline(std::cin, newTitle);
            entry.setTitle(newTitle);
        }
        else if (input == "3")
        {
            std::string tagInput;
            std::vector<std::string> newTags;
            std::cout << "�п�J�s������ (�ΪŮ���j): ";
            std::getline(std::cin, tagInput);

            std::stringstream ss(tagInput);
            std::string tag;
            while (ss >> tag)
            {
                newTags.push_back(tag);
            }

            // �M���¼��Ҩå[�J�s����
            while (!entry.getTags().empty())
            {
                entry.getTags().pop_back();
            }

            for (const auto& t : newTags)
            {
                entry.addTag(t);
            }
        }
        else if (input == "4")
        {
            std::string newContent;
            std::cout << "�п�J�s�����e: ";
            std::getline(std::cin, newContent);
            entry.setContent(newContent);
        }
        else
        {
            std::cout << "�L�Ī��ﶵ�C\n";
        }

        std::cout << "���ؤw��s!\n";
    }
    else
    {
        std::cout << "�L�Ī����ؽs���C\n";
    }
}

void deleteEntry(std::vector<JournalEntry>& journal)
{
    if (journal.empty())
    {
        std::cout << "�S�����إi�H�R���C\n";
        return;
    }

    std::cout << "�п�J�n�R�������ؽs�� (��J 0 ��ܩҦ�����):\n";
    std::string indexStr;
    std::getline(std::cin, indexStr);
    int index = std::stoi(indexStr);

    if (index == 0)
    {
        for (size_t i = 0; i < journal.size(); ++i)
        {
            std::cout << i + 1 << ". " << journal[i].getDate() << " - " << journal[i].getTitle() << "\n";
        }
        std::cout << "�п�J�n�R�������ؽs��: ";
        std::getline(std::cin, indexStr);
        index = std::stoi(indexStr);
    }

    if (index > 0 && index <= journal.size())
    {
        journal.erase(journal.begin() + index - 1);
        std::cout << "���ؤw�R��!\n";
    }
    else
    {
        std::cout << "�L�Ī����ؽs���C\n";
    }
}

void saveToFile(const std::vector<JournalEntry>& journal, const std::map<std::string, std::vector<JournalEntry>>& futureLog, const std::string& filename)
{
    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        std::cerr << "�L�k�}���ɮ�: " << filename << " (�g�J�Ҧ�)" << std::endl;
        return;
    }

    // �x�s��x����
    for (const auto& entry : journal)
    {
        outFile << "Date:" << entry.getDate() << "\n";
        outFile << "Title:" << entry.getTitle() << "\n";
        outFile << "Tags:";
        for (const auto& tag : entry.getTags())
        {
            outFile << tag << " ";
        }
        outFile << "\n";
        outFile << "Content:" << entry.getContent() << "\n";
        outFile << "---\n";
    }

    // �x�s���ӻx����
    outFile << "FutureLog\n";
    for (const auto& pair : futureLog)
    {
        outFile << "YearMonth:" << pair.first << "\n";
        for (const auto& entry : pair.second)
        {
            outFile << "Date:" << entry.getDate() << "\n";
            outFile << "Title:" << entry.getTitle() << "\n";
            outFile << "Tags:";
            for (const auto& tag : entry.getTags())
            {
                outFile << tag << " ";
            }
            outFile << "\n";
            outFile << "Content:" << entry.getContent() << "\n";
            outFile << "---\n"; // ���ؤ��j��
        }
    }
    outFile << "EndFutureLog\n";

    if (outFile.fail())
    {
        std::cerr << "�g�J�ɮ׮ɵo�Ϳ��~: " << filename << std::endl;
    }

    outFile.close();
    std::cout << "��x�w�x�s�� " << filename << "\n";
}

void loadFromFile(std::vector<JournalEntry>& journal, std::map<std::string, std::vector<JournalEntry>>& futureLog, const std::string& filename)
{
    std::ifstream inFile(filename);
    if (!inFile.is_open())
    {
        std::cerr << "�L�k�}���ɮ�: " << filename << " (Ū���Ҧ�)" << std::endl;
        return;
    }

    journal.clear(); // �M�Ų{������x����
    futureLog.clear(); // �M�Ų{�������ӻx����

    std::string line;
    JournalEntry currentEntry("", "", ""); // �{�ɪ��ťն���
    bool readingContent = false;
    bool readingFutureLog = false;
    std::string currentYearMonth = "";

    while (std::getline(inFile, line))
    {
        if (line == "FutureLog")
        {
            readingFutureLog = true;
            continue;
        }
        if (line == "EndFutureLog")
        {
            readingFutureLog = false;
            continue;
        }

        if (!readingFutureLog)
        {
            // Ū����x����
            if (line.find("Date:") == 0)
            {
                if (readingContent)
                {
                    //�x�s�W�@�����
                    journal.push_back(currentEntry);
                    currentEntry = JournalEntry("", "", "");
                }
                currentEntry.setDate(line.substr(5));
                readingContent = false;
            }
            else if (line.find("Title:") == 0)
            {
                currentEntry.setTitle(line.substr(6));
                readingContent = false;
            }
            else if (line.find("Tags:") == 0)
            {
                std::string tagsStr = line.substr(5);
                std::stringstream ss(tagsStr);
                std::string tag;
                while (ss >> tag)
                {
                    currentEntry.addTag(tag);
                }
                readingContent = false;
            }
            else if (line.find("Content:") == 0)
            {
                currentEntry.setContent(line.substr(8) + "\n");
                readingContent = true;
            }
            else if (readingContent)
            {
                currentEntry.setContent(currentEntry.getContent() + line + "\n");
            }
            else if (line == "---")
            {
                // ���ص���
                journal.push_back(currentEntry);
                currentEntry = JournalEntry("", "", ""); // ���m���ťն���
                readingContent = false;
            }
        }
        else
        {
            // Ū�����ӻx����
            if (line.find("YearMonth:") == 0)
            {
                currentYearMonth = line.substr(10);
            }
            else if (line.find("Date:") == 0)
            {
                currentEntry.setDate(line.substr(5));
            }
            else if (line.find("Title:") == 0)
            {
                currentEntry.setTitle(line.substr(6));
            }
            else if (line.find("Tags:") == 0)
            {
                std::string tagsStr = line.substr(5);
                std::stringstream ss(tagsStr);
                std::string tag;
                while (ss >> tag)
                {
                    currentEntry.addTag(tag);
                }
            }
            else if (line.find("Content:") == 0)
            {
                currentEntry.setContent(line.substr(8) + "\n");
            }
            else if (line == "---")
            {
                futureLog[currentYearMonth].push_back(currentEntry);
                currentEntry = JournalEntry("", "", ""); // ���m���ťն���
            }
        }
    }

    // �B�z�̫�@�����
    if (currentEntry.getDate() != "" && currentEntry.getTitle() != "" && currentEntry.getContent() != "")
    {
        if (!readingFutureLog)
        {
            journal.push_back(currentEntry);
        }
    }

    if (inFile.fail() && !inFile.eof())
    {
        std::cerr << "Ū���ɮ׮ɵo�Ϳ��~: " << filename << std::endl;
    }

    inFile.close();
    std::cout << "��x�w�q " << filename << " ���J\n";
}

void displayMonthlyView(const std::vector<JournalEntry>& journal, int year, int month)
{
    // �ˬd�~���M����O�_����
    if (year < 1900 || year > 9999)
    {
        std::cout << "�L�Ī��~���C\n";
        return;
    }
    if (month < 1 || month > 12)
    {
        std::cout << "�L�Ī�����C\n";
        return;
    }

    // ���o�Ӥ몺�Ѽ�
    int daysInMonth = getDaysInMonth(year, month);

    // ���o�Ӥ�Ĥ@�ѬO�P���X
    int firstDayOfWeek = getDayOfWeek(year, month, 1);

    // ����W��
    static const char* monthNames[] = { "January", "February", "March", "April", "May", "June",
                                        "July", "August", "September", "October", "November", "December"
                                      };

    // ��X����M�~��
    std::string monthYearStr = std::string(monthNames[month - 1]) + " " + std::to_string(year);
    int totalWidth = 28; // (4 * 7)
    int padding = (totalWidth - monthYearStr.length()) / 2;
    std::cout << "\n";
    std::cout << std::string(padding, ' ') << monthYearStr << std::string(padding, ' ') << "\n";
    std::cout << "----------------------------\n"; // �ϥ� 28 �� '-'

    // ��X�P���X�����D (4 �Ӧr��)
    std::cout << " Sun Mon Tue Wed Thu Fri Sat\n";

    // ��X����e���Ů�
    for (int i = 0; i < firstDayOfWeek; ++i)
    {
        std::cout << "    ";
    }

    // ��X���
    for (int day = 1; day <= daysInMonth; ++day)
    {
        // �ˬd��ѬO�_����x����
        bool hasEntry = false;
        for (const auto& entry : journal)
        {
            int entryYear, entryMonth, entryDay;
            extractDate(entry.getDate(), entryYear, entryMonth, entryDay);

            if (entryYear == year && entryMonth == month && entryDay == day)
            {
                hasEntry = true;
                break;
            }
        }

        // ��X����M�аO (�p�G����x����)
        if (hasEntry)
        {
            // ����x������� [] �ذ_��
            if (day < 10)
            {
                std::cout << "[ " << day << "]";
            }
            else
            {
                std::cout << "[" << day << "]";
            }
        }
        else
        {
            if(day < 10)
            {
                std::cout << "  " << day << " ";
            }
            else
            {
                std::cout << " " << day << " ";
            }
        }

        // ����
        if ((day + firstDayOfWeek) % 7 == 0)
        {
            std::cout << "\n";
        }
    }

    // �p�G�̫�@��S������A�h�ɤW����
    if ((daysInMonth + firstDayOfWeek) % 7 != 0)
    {
        std::cout << "\n";
    }
    std::cout << "----------------------------\n"; // �ϥ� 28 �� '-'
    std::cout << "\n";
}

void addFutureLogEntry(std::map<std::string, std::vector<JournalEntry>>& futureLog)
{
    std::string yearMonthDay;
    std::string title, content, tag;
    std::vector<std::string> tags;

    std::cout << "�п�J�~���B����M��� (YYYY-MM-DD): ";
    std::getline(std::cin, yearMonthDay);

    if (!isValidDate(yearMonthDay))
    {
        std::cout << "�L�Ī�����榡�Τ���C\n";
        return;
    }

    std::cout << "�п�J���D: ";
    std::getline(std::cin, title);

    std::cout << "�п�J���e: ";
    std::getline(std::cin, content);

    std::cout << "�п�J���� (�ΪŮ���j�A��J�����Ы�Enter): ";
    std::getline(std::cin, tag);

    std::stringstream ss(tag);
    std::string singleTag;
    while (ss >> singleTag)
    {
        tags.push_back(singleTag);
    }

    JournalEntry newEntry(yearMonthDay, title, content);
    for (const auto& t : tags)
    {
        newEntry.addTag(t);
    }

    futureLog[yearMonthDay.substr(0, 7)].push_back(newEntry);

    std::cout << "���ӻx���ؤw�s�W!\n";
}

void viewFutureLog(const std::map<std::string, std::vector<JournalEntry>>& futureLog)
{
    if (futureLog.empty())
    {
        std::cout << "���ӻx���S�����ءC\n";
        return;
    }

    std::vector<std::pair<std::string, const std::vector<JournalEntry>*>> sortedMonths;
    for (const auto& pair : futureLog)
    {
        sortedMonths.push_back({pair.first, &pair.second});
    }
    std::sort(sortedMonths.begin(), sortedMonths.end());

    for (const auto& pair : sortedMonths)
    {
        std::cout << "\n----- " << pair.first << " -----\n";

        // �N�Ӥ�������ث�����Ƨ�
        std::vector<JournalEntry> sortedEntries = *pair.second;
        std::sort(sortedEntries.begin(), sortedEntries.end(), [](const JournalEntry& a, const JournalEntry& b)
        {
            return a.getDate() < b.getDate();
        });

        for (const auto& entry : sortedEntries)
        {
            std::cout << "\n* " << entry.getDate() << " - " << entry.getTitle() << "\n";
            std::cout << "  ����: ";
            if (entry.getTags().empty())
            {
                std::cout << "�L";
            }
            else
            {
                for (const auto& tag : entry.getTags())
                {
                    std::cout << tag << " ";
                }
            }
            std::cout << "\n";
            std::cout << "  ���e: " << entry.getContent() << "\n";
        }
        std::cout << "\n";
    }
}

void editFutureLogEntry(std::map<std::string, std::vector<JournalEntry>>& futureLog)
{
    if (futureLog.empty())
    {
        std::cout << "���ӻx���S�����إi�H�s��C\n";
        return;
    }

    std::cout << "�п�J�n�s�誺���ؽs�� (��J 0 ��ܩҦ�����):\n";
    std::string indexStr;
    std::getline(std::cin, indexStr);
    int index = std::stoi(indexStr);

    if (index == 0)
    {
        int i = 1;
        for (const auto& pair : futureLog)
        {
            for (const auto& entry : pair.second)
            {
                std::cout << i << ". " << entry.getDate() << " - " << entry.getTitle() << "\n";
                i++;
            }
        }
        std::cout << "�п�J�n�s�誺���ؽs��: ";
        std::getline(std::cin, indexStr);
        index = std::stoi(indexStr);
    }

    int count = 1;
    bool found = false;
    for (auto& pair : futureLog)
    {
        for (auto& entry : pair.second)
        {
            if (count == index)
            {
                std::string input;

                std::cout << "�n�s�誺���ءG\n";
                std::cout << "���: " << entry.getDate() << "\n";
                std::cout << "���D: " << entry.getTitle() << "\n";
                std::cout << "����: ";
                for (const auto& tag : entry.getTags())
                {
                    std::cout << tag << " ";
                }
                std::cout << "\n";
                std::cout << "���e: " << entry.getContent() << "\n\n";

                std::cout << "�п�ܭn�s�誺���G\n";
                std::cout << "1. ���\n";
                std::cout << "2. ���D\n";
                std::cout << "3. ����\n";
                std::cout << "4. ���e\n";
                std::cout << "�п�J�ﶵ (1-4): ";
                std::getline(std::cin, input);

                if (input == "1")
                {
                    std::string newDate;
                    std::cout << "�п�J�s����� (YYYY-MM-DD): ";
                    std::getline(std::cin, newDate);

                    if (!isValidDate(newDate))
                    {
                        std::cout << "�L�Ī�����榡�Τ���C\n";
                        return;
                    }

                    // �����¶��اR��
                    pair.second.erase(std::remove_if(pair.second.begin(), pair.second.end(),
                                                     [&entry](const JournalEntry& e)
                    {
                        return e.getDate() == entry.getDate() && e.getTitle() == entry.getTitle();
                    }),
                    pair.second.end());

                    // ��s����M���ݤ��
                    entry.setDate(newDate);
                    std::string newYearMonth = newDate.substr(0, 7);

                    // �N���إ[�J�s�����
                    futureLog[newYearMonth].push_back(entry);

                    // �p�G�¤���w�g�S�����ءA�h�R���Ӥ��
                    if (pair.second.empty() && newYearMonth != pair.first)
                    {
                        futureLog.erase(pair.first);
                    }
                }
                else if (input == "2")
                {
                    std::string newTitle;
                    std::cout << "�п�J�s�����D: ";
                    std::getline(std::cin, newTitle);
                    entry.setTitle(newTitle);
                }
                else if (input == "3")
                {
                    std::string tagInput;
                    std::vector<std::string> newTags;
                    std::cout << "�п�J�s������ (�ΪŮ���j): ";
                    std::getline(std::cin, tagInput);

                    std::stringstream ss(tagInput);
                    std::string tag;
                    while (ss >> tag)
                    {
                        newTags.push_back(tag);
                    }

                    // �M���¼��Ҩå[�J�s����
                    while (!entry.getTags().empty())
                    {
                        entry.getTags().pop_back();
                    }

                    for (const auto& t : newTags)
                    {
                        entry.addTag(t);
                    }
                }
                else if (input == "4")
                {
                    std::string newContent;
                    std::cout << "�п�J�s�����e: ";
                    std::getline(std::cin, newContent);
                    entry.setContent(newContent);
                }
                else
                {
                    std::cout << "�L�Ī��ﶵ�C\n";
                }

                std::cout << "���ؤw��s!\n";
                found = true;
                break;
            }
            count++;
        }
        if (found) break;
    }

    if (!found)
    {
        std::cout << "�L�Ī����ؽs���C\n";
    }
}

void deleteFutureLogEntry(std::map<std::string, std::vector<JournalEntry>>& futureLog)
{
    if (futureLog.empty())
    {
        std::cout << "���ӻx���S�����إi�H�R���C\n";
        return;
    }

    std::cout << "�п�J�n�R�������ؽs�� (��J 0 ��ܩҦ�����):\n";
    std::string indexStr;
    std::getline(std::cin, indexStr);
    int index = std::stoi(indexStr);

    if (index == 0)
    {
        int i = 1;
        for (const auto& pair : futureLog)
        {
            for (const auto& entry : pair.second)
            {
                std::cout << i << ". " << entry.getDate() << " - " << entry.getTitle() << "\n";
                i++;
            }
        }
        std::cout << "�п�J�n�R�������ؽs��: ";
        std::getline(std::cin, indexStr);
        index = std::stoi(indexStr);
    }

    int count = 1;
    bool found = false;
    for (auto& pair : futureLog)
    {
        for (size_t i = 0; i < pair.second.size(); ++i)
        {
            if (count == index)
            {
                pair.second.erase(pair.second.begin() + i);
                if (pair.second.empty())
                {
                    futureLog.erase(pair.first);
                }
                std::cout << "���ؤw�R��!\n";
                found = true;
                break;
            }
            count++;
        }
        if (found) break;
    }

    if (!found)
    {
        std::cout << "�L�Ī����ؽs���C\n";
    }
}

int main()
{
    std::vector<JournalEntry> myJournal;
    std::map<std::string, std::vector<JournalEntry>> futureLog;
    std::string filename = "journal.txt";

    // ���ո��J�{������x
    loadFromFile(myJournal, futureLog, filename);

    while (true)
    {
        std::cout << "\n�п�ܾާ@�G\n";
        std::cout << "1. �s�W����\n";
        std::cout << "2. �˵�����\n";
        std::cout << "3. �s�趵��\n";
        std::cout << "4. �R������\n";
        std::cout << "5. �x�s��x\n";
        std::cout << "6. ���J��x\n";
        std::cout << "7. ��ܤ�����\n";
        std::cout << "8. �s�W���ӻx����\n";
        std::cout << "9. �˵����ӻx\n";
        std::cout << "10. �s�襼�ӻx����\n";
        std::cout << "11. �R�����ӻx����\n";
        std::cout << "12. ���}\n";
        std::cout << "�п�J�ﶵ (1-12): ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1")
        {
            addEntry(myJournal);
        }
        else if (choice == "2")
        {
            viewEntries(myJournal);
        }
        else if (choice == "3")
        {
            editEntry(myJournal);
        }
        else if (choice == "4")
        {
            deleteEntry(myJournal);
        }
        else if (choice == "5")
        {
            saveToFile(myJournal, futureLog, filename);
        }
        else if (choice == "6")
        {
            loadFromFile(myJournal, futureLog, filename);
        }
        else if (choice == "7")
        {
            int year, month;
            std::cout << "�п�J�~��: ";
            std::cin >> year;
            std::cout << "�п�J��� (1-12): ";
            std::cin >> month;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // �M����J�w�İ�

            if (year < 1900 || year > 9999)
            {
                std::cout << "�L�Ī��~���C\n";
            }
            else if (month < 1 || month > 12)
            {
                std::cout << "�L�Ī�����C\n";
            }
            else
            {
                displayMonthlyView(myJournal, year, month);
            }
        }
        else if (choice == "8")
        {
            addFutureLogEntry(futureLog);
        }
        else if (choice == "9")
        {
            viewFutureLog(futureLog);
        }
        else if (choice == "10")
        {
            editFutureLogEntry(futureLog);
        }
        else if (choice == "11")
        {
            deleteFutureLogEntry(futureLog);
        }
        else if (choice == "12")
        {
            break;
        }
        else
        {
            std::cout << "�L�Ī��ﶵ�C\n";
        }
    }

    return 0;
}

