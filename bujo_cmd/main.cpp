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
    // 建構子
    JournalEntry(const std::string& date, const std::string& title, const std::string& content) :
        date(date), title(title), content(content) {}

    // Getter 和 Setter
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

    // 檢查項目是否包含所有指定的標籤 (AND 邏輯)
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


// 判斷是否為閏年
bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// 某年某月的天數?
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

// 某年某月某日是星期幾? (Zeller's congruence)
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
    return w; // 0 = 星期日, 1 = 星期一, ..., 6 = 星期六
}

// 從日期字串中提取年、月、日 (YYYY-MM-DD)
void extractDate(const std::string& dateStr, int& year, int& month, int& day)
{
    sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day);
}

// 驗證日期格式和有效性
bool isValidDate(const std::string& date)
{
    int year, month, day;
    if (sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day) != 3)
    {
        return false; // 格式不符
    }

    if (year < 1900 || year > 9999)
    {
        return false; // 年份超出範圍
    }

    if (month < 1 || month > 12)
    {
        return false; // 月份無效
    }

    if (day < 1 || day > getDaysInMonth(year, month))
    {
        return false; // 日期無效
    }

    return true;
}

void addEntry(std::vector<JournalEntry>& journal)
{
    std::string date, title, content, tag;
    std::vector<std::string> tags;

    std::cout << "請輸入日期 (YYYY-MM-DD): ";
    std::getline(std::cin, date);

    if (!isValidDate(date))
    {
        std::cout << "無效的日期格式或日期。\n";
        return;
    }

    std::cout << "請輸入標題: ";
    std::getline(std::cin, title);

    std::cout << "請輸入內容: ";
    std::getline(std::cin, content);

    std::cout << "請輸入標籤 (用空格分隔，輸入完畢請按Enter): ";
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

    std::cout << "項目已新增!\n";
}

void viewEntries(const std::vector<JournalEntry>& journal)
{
    std::string input;
    std::cout << "請選擇檢視方式：\n";
    std::cout << "1. 依日期檢視\n";
    std::cout << "2. 依標題搜尋\n";
    std::cout << "3. 依標籤搜尋 (支援多個標籤，AND 邏輯)\n";
    std::cout << "4. 依日期範圍搜尋\n";
    std::cout << "請輸入選項 (1-4): ";
    std::getline(std::cin, input);

    if (input == "1")
    {
        std::string date;
        std::cout << "請輸入日期 (YYYY-MM-DD, YYYY-MM 或 YYYY): ";
        std::getline(std::cin, date);

        // 檢查輸入的日期格式
        int dateLength = date.length();
        if (dateLength != 10 && dateLength != 7 && dateLength != 4)
        {
            std::cout << "無效的日期格式。\n";
            return;
        }

        for (const auto& entry : journal)
        {
            if (entry.getDate().substr(0, dateLength) == date)
            {
                std::cout << "日期: " << entry.getDate() << "\n";
                std::cout << "標題: " << entry.getTitle() << "\n";
                std::cout << "標籤: ";
                for (const auto& tag : entry.getTags())
                {
                    std::cout << tag << " ";
                }
                std::cout << "\n";
                std::cout << "內容: " << entry.getContent() << "\n\n";
            }
        }
    }
    else if (input == "2")
    {
        std::string title;
        std::cout << "請輸入標題關鍵字: ";
        std::getline(std::cin, title);

        for (const auto& entry : journal)
        {
            if (entry.getTitle().find(title) != std::string::npos)
            {
                std::cout << "日期: " << entry.getDate() << "\n";
                std::cout << "標題: " << entry.getTitle() << "\n";
                std::cout << "標籤: ";
                for (const auto& tag : entry.getTags())
                {
                    std::cout << tag << " ";
                }
                std::cout << "\n";
                std::cout << "內容: " << entry.getContent() << "\n\n";
            }
        }
    }
    else if (input == "3")
    {
        std::string tagInput;
        std::vector<std::string> searchTags;
        std::cout << "請輸入要搜尋的標籤 (用空格分隔): ";
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
                std::cout << "日期: " << entry.getDate() << "\n";
                std::cout << "標題: " << entry.getTitle() << "\n";
                std::cout << "標籤: ";
                for (const auto& tag : entry.getTags())
                {
                    std::cout << tag << " ";
                }
                std::cout << "\n";
                std::cout << "內容: " << entry.getContent() << "\n\n";
            }
        }
    }
    else if (input == "4")
    {
        std::string startDate, endDate;
        std::cout << "請輸入起始日期 (YYYY-MM-DD, YYYY-MM 或 YYYY): ";
        std::getline(std::cin, startDate);
        std::cout << "請輸入結束日期 (YYYY-MM-DD, YYYY-MM 或 YYYY): ";
        std::getline(std::cin, endDate);

        // 檢查輸入的日期格式
        int startDateLength = startDate.length();
        int endDateLength = endDate.length();
        if ((startDateLength != 10 && startDateLength != 7 && startDateLength != 4) ||
                (endDateLength != 10 && endDateLength != 7 && endDateLength != 4))
        {
            std::cout << "無效的日期格式。\n";
            return;
        }

        for (const auto& entry : journal)
        {
            if (entry.getDate().substr(0, startDateLength) >= startDate &&
                    entry.getDate().substr(0, endDateLength) <= endDate)
            {
                std::cout << "日期: " << entry.getDate() << "\n";
                std::cout << "標題: " << entry.getTitle() << "\n";
                std::cout << "標籤: ";
                for (const auto& tag : entry.getTags())
                {
                    std::cout << tag << " ";
                }
                std::cout << "\n";
                std::cout << "內容: " << entry.getContent() << "\n\n";
            }
        }
    }
    else
    {
        std::cout << "無效的選項。\n";
    }
}

void editEntry(std::vector<JournalEntry>& journal)
{
    if (journal.empty())
    {
        std::cout << "沒有項目可以編輯。\n";
        return;
    }

    std::cout << "請輸入要編輯的項目編號 (輸入 0 顯示所有項目):\n";
    std::string indexStr;
    std::getline(std::cin, indexStr);
    int index = std::stoi(indexStr);

    if (index == 0)
    {
        for (size_t i = 0; i < journal.size(); ++i)
        {
            std::cout << i + 1 << ". " << journal[i].getDate() << " - " << journal[i].getTitle() << "\n";
        }
        std::cout << "請輸入要編輯的項目編號: ";
        std::getline(std::cin, indexStr);
        index = std::stoi(indexStr);
    }

    if (index > 0 && index <= journal.size())
    {
        JournalEntry& entry = journal[index - 1];
        std::string input;

        std::cout << "要編輯的項目：\n";
        std::cout << "日期: " << entry.getDate() << "\n";
        std::cout << "標題: " << entry.getTitle() << "\n";
        std::cout << "標籤: ";
        for (const auto& tag : entry.getTags())
        {
            std::cout << tag << " ";
        }
        std::cout << "\n";
        std::cout << "內容: " << entry.getContent() << "\n\n";

        std::cout << "請選擇要編輯的欄位：\n";
        std::cout << "1. 日期\n";
        std::cout << "2. 標題\n";
        std::cout << "3. 標籤\n";
        std::cout << "4. 內容\n";
        std::cout << "請輸入選項 (1-4): ";
        std::getline(std::cin, input);

        if (input == "1")
        {
            std::string newDate;
            std::cout << "請輸入新的日期 (YYYY-MM-DD): ";
            std::getline(std::cin, newDate);

            if (!isValidDate(newDate))
            {
                std::cout << "無效的日期格式或日期。\n";
                return;
            }

            entry.setDate(newDate);
        }
        else if (input == "2")
        {
            std::string newTitle;
            std::cout << "請輸入新的標題: ";
            std::getline(std::cin, newTitle);
            entry.setTitle(newTitle);
        }
        else if (input == "3")
        {
            std::string tagInput;
            std::vector<std::string> newTags;
            std::cout << "請輸入新的標籤 (用空格分隔): ";
            std::getline(std::cin, tagInput);

            std::stringstream ss(tagInput);
            std::string tag;
            while (ss >> tag)
            {
                newTags.push_back(tag);
            }

            // 清空舊標籤並加入新標籤
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
            std::cout << "請輸入新的內容: ";
            std::getline(std::cin, newContent);
            entry.setContent(newContent);
        }
        else
        {
            std::cout << "無效的選項。\n";
        }

        std::cout << "項目已更新!\n";
    }
    else
    {
        std::cout << "無效的項目編號。\n";
    }
}

void deleteEntry(std::vector<JournalEntry>& journal)
{
    if (journal.empty())
    {
        std::cout << "沒有項目可以刪除。\n";
        return;
    }

    std::cout << "請輸入要刪除的項目編號 (輸入 0 顯示所有項目):\n";
    std::string indexStr;
    std::getline(std::cin, indexStr);
    int index = std::stoi(indexStr);

    if (index == 0)
    {
        for (size_t i = 0; i < journal.size(); ++i)
        {
            std::cout << i + 1 << ". " << journal[i].getDate() << " - " << journal[i].getTitle() << "\n";
        }
        std::cout << "請輸入要刪除的項目編號: ";
        std::getline(std::cin, indexStr);
        index = std::stoi(indexStr);
    }

    if (index > 0 && index <= journal.size())
    {
        journal.erase(journal.begin() + index - 1);
        std::cout << "項目已刪除!\n";
    }
    else
    {
        std::cout << "無效的項目編號。\n";
    }
}

void saveToFile(const std::vector<JournalEntry>& journal, const std::map<std::string, std::vector<JournalEntry>>& futureLog, const std::string& filename)
{
    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        std::cerr << "無法開啟檔案: " << filename << " (寫入模式)" << std::endl;
        return;
    }

    // 儲存日誌項目
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

    // 儲存未來誌項目
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
            outFile << "---\n"; // 項目分隔符
        }
    }
    outFile << "EndFutureLog\n";

    if (outFile.fail())
    {
        std::cerr << "寫入檔案時發生錯誤: " << filename << std::endl;
    }

    outFile.close();
    std::cout << "日誌已儲存到 " << filename << "\n";
}

void loadFromFile(std::vector<JournalEntry>& journal, std::map<std::string, std::vector<JournalEntry>>& futureLog, const std::string& filename)
{
    std::ifstream inFile(filename);
    if (!inFile.is_open())
    {
        std::cerr << "無法開啟檔案: " << filename << " (讀取模式)" << std::endl;
        return;
    }

    journal.clear(); // 清空現有的日誌項目
    futureLog.clear(); // 清空現有的未來誌項目

    std::string line;
    JournalEntry currentEntry("", "", ""); // 臨時的空白項目
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
            // 讀取日誌項目
            if (line.find("Date:") == 0)
            {
                if (readingContent)
                {
                    //儲存上一筆資料
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
                // 項目結束
                journal.push_back(currentEntry);
                currentEntry = JournalEntry("", "", ""); // 重置為空白項目
                readingContent = false;
            }
        }
        else
        {
            // 讀取未來誌項目
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
                currentEntry = JournalEntry("", "", ""); // 重置為空白項目
            }
        }
    }

    // 處理最後一筆資料
    if (currentEntry.getDate() != "" && currentEntry.getTitle() != "" && currentEntry.getContent() != "")
    {
        if (!readingFutureLog)
        {
            journal.push_back(currentEntry);
        }
    }

    if (inFile.fail() && !inFile.eof())
    {
        std::cerr << "讀取檔案時發生錯誤: " << filename << std::endl;
    }

    inFile.close();
    std::cout << "日誌已從 " << filename << " 載入\n";
}

void displayMonthlyView(const std::vector<JournalEntry>& journal, int year, int month)
{
    // 檢查年份和月份是否有效
    if (year < 1900 || year > 9999)
    {
        std::cout << "無效的年份。\n";
        return;
    }
    if (month < 1 || month > 12)
    {
        std::cout << "無效的月份。\n";
        return;
    }

    // 取得該月的天數
    int daysInMonth = getDaysInMonth(year, month);

    // 取得該月第一天是星期幾
    int firstDayOfWeek = getDayOfWeek(year, month, 1);

    // 月份名稱
    static const char* monthNames[] = { "January", "February", "March", "April", "May", "June",
                                        "July", "August", "September", "October", "November", "December"
                                      };

    // 輸出月份和年份
    std::string monthYearStr = std::string(monthNames[month - 1]) + " " + std::to_string(year);
    int totalWidth = 28; // (4 * 7)
    int padding = (totalWidth - monthYearStr.length()) / 2;
    std::cout << "\n";
    std::cout << std::string(padding, ' ') << monthYearStr << std::string(padding, ' ') << "\n";
    std::cout << "----------------------------\n"; // 使用 28 個 '-'

    // 輸出星期幾的標題 (4 個字元)
    std::cout << " Sun Mon Tue Wed Thu Fri Sat\n";

    // 輸出日期前的空格
    for (int i = 0; i < firstDayOfWeek; ++i)
    {
        std::cout << "    ";
    }

    // 輸出日期
    for (int day = 1; day <= daysInMonth; ++day)
    {
        // 檢查當天是否有日誌項目
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

        // 輸出日期和標記 (如果有日誌項目)
        if (hasEntry)
        {
            // 有日誌的日期用 [] 框起來
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

        // 換行
        if ((day + firstDayOfWeek) % 7 == 0)
        {
            std::cout << "\n";
        }
    }

    // 如果最後一行沒有換行，則補上換行
    if ((daysInMonth + firstDayOfWeek) % 7 != 0)
    {
        std::cout << "\n";
    }
    std::cout << "----------------------------\n"; // 使用 28 個 '-'
    std::cout << "\n";
}

void addFutureLogEntry(std::map<std::string, std::vector<JournalEntry>>& futureLog)
{
    std::string yearMonthDay;
    std::string title, content, tag;
    std::vector<std::string> tags;

    std::cout << "請輸入年份、月份和日期 (YYYY-MM-DD): ";
    std::getline(std::cin, yearMonthDay);

    if (!isValidDate(yearMonthDay))
    {
        std::cout << "無效的日期格式或日期。\n";
        return;
    }

    std::cout << "請輸入標題: ";
    std::getline(std::cin, title);

    std::cout << "請輸入內容: ";
    std::getline(std::cin, content);

    std::cout << "請輸入標籤 (用空格分隔，輸入完畢請按Enter): ";
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

    std::cout << "未來誌項目已新增!\n";
}

void viewFutureLog(const std::map<std::string, std::vector<JournalEntry>>& futureLog)
{
    if (futureLog.empty())
    {
        std::cout << "未來誌中沒有項目。\n";
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

        // 將該月份的項目按日期排序
        std::vector<JournalEntry> sortedEntries = *pair.second;
        std::sort(sortedEntries.begin(), sortedEntries.end(), [](const JournalEntry& a, const JournalEntry& b)
        {
            return a.getDate() < b.getDate();
        });

        for (const auto& entry : sortedEntries)
        {
            std::cout << "\n* " << entry.getDate() << " - " << entry.getTitle() << "\n";
            std::cout << "  標籤: ";
            if (entry.getTags().empty())
            {
                std::cout << "無";
            }
            else
            {
                for (const auto& tag : entry.getTags())
                {
                    std::cout << tag << " ";
                }
            }
            std::cout << "\n";
            std::cout << "  內容: " << entry.getContent() << "\n";
        }
        std::cout << "\n";
    }
}

void editFutureLogEntry(std::map<std::string, std::vector<JournalEntry>>& futureLog)
{
    if (futureLog.empty())
    {
        std::cout << "未來誌中沒有項目可以編輯。\n";
        return;
    }

    std::cout << "請輸入要編輯的項目編號 (輸入 0 顯示所有項目):\n";
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
        std::cout << "請輸入要編輯的項目編號: ";
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

                std::cout << "要編輯的項目：\n";
                std::cout << "日期: " << entry.getDate() << "\n";
                std::cout << "標題: " << entry.getTitle() << "\n";
                std::cout << "標籤: ";
                for (const auto& tag : entry.getTags())
                {
                    std::cout << tag << " ";
                }
                std::cout << "\n";
                std::cout << "內容: " << entry.getContent() << "\n\n";

                std::cout << "請選擇要編輯的欄位：\n";
                std::cout << "1. 日期\n";
                std::cout << "2. 標題\n";
                std::cout << "3. 標籤\n";
                std::cout << "4. 內容\n";
                std::cout << "請輸入選項 (1-4): ";
                std::getline(std::cin, input);

                if (input == "1")
                {
                    std::string newDate;
                    std::cout << "請輸入新的日期 (YYYY-MM-DD): ";
                    std::getline(std::cin, newDate);

                    if (!isValidDate(newDate))
                    {
                        std::cout << "無效的日期格式或日期。\n";
                        return;
                    }

                    // 先把舊項目刪除
                    pair.second.erase(std::remove_if(pair.second.begin(), pair.second.end(),
                                                     [&entry](const JournalEntry& e)
                    {
                        return e.getDate() == entry.getDate() && e.getTitle() == entry.getTitle();
                    }),
                    pair.second.end());

                    // 更新日期和所屬月份
                    entry.setDate(newDate);
                    std::string newYearMonth = newDate.substr(0, 7);

                    // 將項目加入新的月份
                    futureLog[newYearMonth].push_back(entry);

                    // 如果舊月份已經沒有項目，則刪除該月份
                    if (pair.second.empty() && newYearMonth != pair.first)
                    {
                        futureLog.erase(pair.first);
                    }
                }
                else if (input == "2")
                {
                    std::string newTitle;
                    std::cout << "請輸入新的標題: ";
                    std::getline(std::cin, newTitle);
                    entry.setTitle(newTitle);
                }
                else if (input == "3")
                {
                    std::string tagInput;
                    std::vector<std::string> newTags;
                    std::cout << "請輸入新的標籤 (用空格分隔): ";
                    std::getline(std::cin, tagInput);

                    std::stringstream ss(tagInput);
                    std::string tag;
                    while (ss >> tag)
                    {
                        newTags.push_back(tag);
                    }

                    // 清空舊標籤並加入新標籤
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
                    std::cout << "請輸入新的內容: ";
                    std::getline(std::cin, newContent);
                    entry.setContent(newContent);
                }
                else
                {
                    std::cout << "無效的選項。\n";
                }

                std::cout << "項目已更新!\n";
                found = true;
                break;
            }
            count++;
        }
        if (found) break;
    }

    if (!found)
    {
        std::cout << "無效的項目編號。\n";
    }
}

void deleteFutureLogEntry(std::map<std::string, std::vector<JournalEntry>>& futureLog)
{
    if (futureLog.empty())
    {
        std::cout << "未來誌中沒有項目可以刪除。\n";
        return;
    }

    std::cout << "請輸入要刪除的項目編號 (輸入 0 顯示所有項目):\n";
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
        std::cout << "請輸入要刪除的項目編號: ";
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
                std::cout << "項目已刪除!\n";
                found = true;
                break;
            }
            count++;
        }
        if (found) break;
    }

    if (!found)
    {
        std::cout << "無效的項目編號。\n";
    }
}

int main()
{
    std::vector<JournalEntry> myJournal;
    std::map<std::string, std::vector<JournalEntry>> futureLog;
    std::string filename = "journal.txt";

    // 嘗試載入現有的日誌
    loadFromFile(myJournal, futureLog, filename);

    while (true)
    {
        std::cout << "\n請選擇操作：\n";
        std::cout << "1. 新增項目\n";
        std::cout << "2. 檢視項目\n";
        std::cout << "3. 編輯項目\n";
        std::cout << "4. 刪除項目\n";
        std::cout << "5. 儲存日誌\n";
        std::cout << "6. 載入日誌\n";
        std::cout << "7. 顯示月曆視圖\n";
        std::cout << "8. 新增未來誌項目\n";
        std::cout << "9. 檢視未來誌\n";
        std::cout << "10. 編輯未來誌項目\n";
        std::cout << "11. 刪除未來誌項目\n";
        std::cout << "12. 離開\n";
        std::cout << "請輸入選項 (1-12): ";

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
            std::cout << "請輸入年份: ";
            std::cin >> year;
            std::cout << "請輸入月份 (1-12): ";
            std::cin >> month;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清除輸入緩衝區

            if (year < 1900 || year > 9999)
            {
                std::cout << "無效的年份。\n";
            }
            else if (month < 1 || month > 12)
            {
                std::cout << "無效的月份。\n";
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
            std::cout << "無效的選項。\n";
        }
    }

    return 0;
}

