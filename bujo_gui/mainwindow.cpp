#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStackedWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QListWidget>
#include <QCalendarWidget>
#include <QPushButton>
#include <QToolButton>
#include <QComboBox>
#include <QDate>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include "journalentry.h"
#include "futurelogentry.h"
#include "reviewentry.h"
#include "datahandler.h"
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 左側導航欄
    m_navigationWidget = new QWidget(this);
    m_navigationWidget->setFixedWidth(250);
    m_navigationLayout = new QVBoxLayout(m_navigationWidget);
    m_navigationLayout->setAlignment(Qt::AlignTop);

    m_todayButton = new QToolButton(m_navigationWidget);
    m_todayButton->setText(tr("今日檢視"));
    m_todayButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(m_todayButton, &QToolButton::clicked, this, &MainWindow::onTodayButtonClicked);

    m_monthButton = new QToolButton(m_navigationWidget);
    m_monthButton->setText(tr("月計畫"));
    m_monthButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(m_monthButton, &QToolButton::clicked, this, &MainWindow::onMonthButtonClicked);

    m_futureButton = new QToolButton(m_navigationWidget);
    m_futureButton->setText(tr("未來計畫"));
    m_futureButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(m_futureButton, &QToolButton::clicked, this, &MainWindow::onFutureButtonClicked);

    m_reviewButton = new QToolButton(m_navigationWidget);
    m_reviewButton->setText(tr("每日回顧"));
    m_reviewButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(m_reviewButton, &QToolButton::clicked, this, &MainWindow::onReviewButtonClicked);

    m_navigationCalendar = new QCalendarWidget(m_navigationWidget);
    connect(m_navigationCalendar, &QCalendarWidget::clicked, this, &MainWindow::onNavigationCalendarClicked);

    m_navigationLayout->addWidget(m_todayButton);
    m_navigationLayout->addWidget(m_monthButton);
    m_navigationLayout->addWidget(m_futureButton);
    m_navigationLayout->addWidget(m_reviewButton);
    m_navigationLayout->addWidget(m_navigationCalendar);

    m_navigationLayout->addStretch();

    // 右側內容區
    m_stackedWidget = new QStackedWidget(this);

    // 1. 今日檢視
    m_todayView = new QWidget(m_stackedWidget);
    m_todayLayout = new QVBoxLayout(m_todayView);

    m_todayDateLabel = new QLabel(m_todayView);
    m_todayDateLabel->setText(QDate::currentDate().toString("yyyy 年 MM 月 dd 日 dddd"));

    // 新增標題輸入框
    m_todayEntryTitle = new QLineEdit(m_todayView);
    m_todayEntryTitle->setPlaceholderText("請輸入日記標題...");

    m_todayEntryContent = new QTextEdit(m_todayView);
    m_todayEntryContent->setPlaceholderText("請輸入今天的日記...");

    m_todayEntryList = new QListWidget(m_todayView);
    connect(m_todayEntryList, &QListWidget::itemClicked, this, &MainWindow::onEntrySelected);

    m_newEntryButton = new QToolButton(m_todayView);
    m_newEntryButton->setText(tr("新增"));
    m_newEntryButton->setFixedSize(50, 30);
    connect(m_newEntryButton, &QToolButton::clicked, this, &MainWindow::onNewEntry);

    m_editEntryButton = new QToolButton(m_todayView);
    m_editEntryButton->setText(tr("編輯"));
    m_editEntryButton->setFixedSize(50, 30);
    connect(m_editEntryButton, &QToolButton::clicked, this, &MainWindow::onEditEntry);

    m_deleteEntryButton = new QToolButton(m_todayView);
    m_deleteEntryButton->setText(tr("刪除"));
    m_deleteEntryButton->setFixedSize(50, 30);
    connect(m_deleteEntryButton, &QToolButton::clicked, this, &MainWindow::onDeleteEntry);

    // 將按鈕加入水平佈局
    m_todayButtonLayout = new QHBoxLayout;
    m_todayButtonLayout->addWidget(m_newEntryButton);
    m_todayButtonLayout->addWidget(m_editEntryButton);
    m_todayButtonLayout->addWidget(m_deleteEntryButton);

    m_todayLayout->addWidget(m_todayDateLabel);
    m_todayLayout->addWidget(m_todayEntryTitle); // 新增標題輸入框
    m_todayLayout->addWidget(m_todayEntryContent);
    m_todayLayout->addWidget(m_todayEntryList);
    m_todayLayout->addLayout(m_todayButtonLayout); // 將水平佈局加入垂直佈局

    m_stackedWidget->addWidget(m_todayView);

    // 2. 月計畫
    m_monthView = new QWidget(m_stackedWidget);
    m_monthLayout = new QVBoxLayout(m_monthView);

    m_monthCalendar = new QCalendarWidget(m_monthView);
    m_monthCalendar->setGridVisible(true);
    connect(m_monthCalendar, &QCalendarWidget::selectionChanged, this, &MainWindow::onCalendarDateChanged);

    m_monthEntryList = new QListWidget(m_monthView);

    m_prevMonthButton = new QPushButton(m_monthView);
    m_prevMonthButton->setText(tr("上個月"));
    connect(m_prevMonthButton, &QPushButton::clicked, this, &MainWindow::onPrevMonthButtonClicked);

    m_nextMonthButton = new QPushButton(m_monthView);
    m_nextMonthButton->setText(tr("下個月"));
    connect(m_nextMonthButton, &QPushButton::clicked, this, &MainWindow::onNextMonthButtonClicked);

    m_monthButtonLayout = new QHBoxLayout;
    m_monthButtonLayout->addWidget(m_prevMonthButton);
    m_monthButtonLayout->addWidget(m_nextMonthButton);

    m_monthLayout->addWidget(m_monthCalendar);
    m_monthLayout->addWidget(m_monthEntryList);
    m_monthLayout->addLayout(m_monthButtonLayout);

    m_stackedWidget->addWidget(m_monthView);

    // 3. 未來計畫
    m_futureView = new QWidget(m_stackedWidget);
    m_futureLayout = new QVBoxLayout(m_futureView);

    m_futureYearCombo = new QComboBox(m_futureView);
    for (int year = QDate::currentDate().year(); year < QDate::currentDate().year() + 5; ++year) {
        m_futureYearCombo->addItem(QString::number(year));
    }
    connect(m_futureYearCombo, &QComboBox::currentIndexChanged, this, &MainWindow::onFutureYearChanged);

    m_futureMonthList = new QListWidget(m_futureView);
    connect(m_futureMonthList, &QListWidget::itemSelectionChanged, this, &MainWindow::onFutureMonthSelected);

    m_futureLogContent = new QTextEdit(m_futureView);
    m_futureLogContent->setPlaceholderText("請輸入未來計畫...");

    m_newFutureLogEntryButton = new QToolButton(m_futureView);
    m_newFutureLogEntryButton->setText(tr("新增"));
    m_newFutureLogEntryButton->setFixedSize(50, 30);
    connect(m_newFutureLogEntryButton, &QToolButton::clicked, this, &MainWindow::onNewFutureLogEntry);

    m_editFutureLogEntryButton = new QToolButton(m_futureView);
    m_editFutureLogEntryButton->setText(tr("編輯"));
    m_editFutureLogEntryButton->setFixedSize(50, 30);
    connect(m_editFutureLogEntryButton, &QToolButton::clicked, this, &MainWindow::onEditFutureLogEntry);

    m_deleteFutureLogEntryButton = new QToolButton(m_futureView);
    m_deleteFutureLogEntryButton->setText(tr("刪除"));
    m_deleteFutureLogEntryButton->setFixedSize(50, 30);
    connect(m_deleteFutureLogEntryButton, &QToolButton::clicked, this, &MainWindow::onDeleteFutureLogEntry);

    // 將按鈕加入水平佈局
    m_futureButtonLayout = new QHBoxLayout;
    m_futureButtonLayout->addWidget(m_newFutureLogEntryButton);
    m_futureButtonLayout->addWidget(m_editFutureLogEntryButton);
    m_futureButtonLayout->addWidget(m_deleteFutureLogEntryButton);

    m_futureLayout->addWidget(m_futureYearCombo);
    m_futureLayout->addWidget(m_futureMonthList);
    m_futureLayout->addWidget(m_futureLogContent);
    m_futureLayout->addLayout(m_futureButtonLayout);

    m_stackedWidget->addWidget(m_futureView);

    // 4. 每日回顧
    m_reviewView = new QWidget(m_stackedWidget);
    m_reviewLayout = new QVBoxLayout(m_reviewView);

    m_reviewDateLabel = new QLabel(m_reviewView);
    m_reviewDateLabel->setText(tr("日期: "));

    m_reviewContent = new QTextEdit(m_reviewView);
    m_reviewContent->setPlaceholderText("請輸入今日的回顧...");

    m_saveReviewButton = new QPushButton(m_reviewView);
    m_saveReviewButton->setText(tr("儲存回顧"));
    connect(m_saveReviewButton, &QPushButton::clicked, this, &MainWindow::onSaveReview);

    m_reviewLayout->addWidget(m_reviewDateLabel);
    m_reviewLayout->addWidget(m_reviewContent);
    m_reviewLayout->addWidget(m_saveReviewButton);

    m_stackedWidget->addWidget(m_reviewView);

    // 將左右兩側的 Widget 加入到主視窗的佈局中
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(m_navigationWidget);
    mainLayout->addWidget(m_stackedWidget);
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // 設定預設視圖
    m_stackedWidget->setCurrentWidget(m_todayView);

    //選單
    //檔案
    connect(ui->action_N, &QAction::triggered, this, &MainWindow::on_action_N_triggered);
    connect(ui->action_O, &QAction::triggered, this, &MainWindow::on_action_O_triggered);
    connect(ui->action_S, &QAction::triggered, this, &MainWindow::on_action_S_triggered);
    connect(ui->action_E, &QAction::triggered, this, &MainWindow::on_action_E_triggered);
    //編輯
    connect(ui->action_U, &QAction::triggered, this, &MainWindow::on_action_U_triggered);
    connect(ui->action_R, &QAction::triggered, this, &MainWindow::on_action_R_triggered);
    connect(ui->action_C, &QAction::triggered, this, &MainWindow::on_action_C_triggered);
    connect(ui->action_X, &QAction::triggered, this, &MainWindow::on_action_X_triggered);
    connect(ui->action_V, &QAction::triggered, this, &MainWindow::on_action_V_triggered);
    connect(ui->action_D, &QAction::triggered, this, &MainWindow::on_action_D_triggered);
    connect(ui->action_A, &QAction::triggered, this, &MainWindow::on_action_A_triggered);
    //檢視
    connect(ui->action_T, &QAction::triggered, this, &MainWindow::on_action_T_triggered);
    connect(ui->action_M, &QAction::triggered, this, &MainWindow::on_action_M_triggered);
    connect(ui->action_F, &QAction::triggered, this, &MainWindow::on_action_F_triggered);
    connect(ui->action_V_2, &QAction::triggered, this, &MainWindow::on_action_V_2_triggered);
    //搜尋
    connect(ui->action_Search, &QAction::triggered, this, &MainWindow::on_action_Search_triggered);
    //說明
    connect(ui->action_A_2, &QAction::triggered, this, &MainWindow::on_action_A_2_triggered);
    connect(ui->action_H, &QAction::triggered, this, &MainWindow::on_action_H_triggered);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTodayButtonClicked()
{
    m_stackedWidget->setCurrentWidget(m_todayView);
}

void MainWindow::onMonthButtonClicked()
{
    m_stackedWidget->setCurrentWidget(m_monthView);
}

void MainWindow::onFutureButtonClicked()
{
    m_stackedWidget->setCurrentWidget(m_futureView);
    m_futureMonthList->clear();
    // 直接在 populateFutureMonthList 函式中加入月份
    populateFutureMonthList();
}

void MainWindow::onReviewButtonClicked()
{
    m_stackedWidget->setCurrentWidget(m_reviewView);
    // 預設顯示今天的日期
    m_selectedReviewDate = QDate::currentDate();
    m_reviewDateLabel->setText(tr("日期: ") + m_selectedReviewDate.toString("yyyy-MM-dd"));
}

//選單觸發的函式
void MainWindow::on_action_N_triggered()
{
    // "新增" 選單項目的操作
}

void MainWindow::on_action_O_triggered()
{
    onOpenJournal();
    // "開啟" 選單項目的操作
    loadAllData();

}

void MainWindow::on_action_S_triggered()
{
    onSaveJournal();
    // "儲存" 選單項目的操作
    saveAllData();
}

void MainWindow::on_action_E_triggered()
{
    // "關閉" 選單項目的操作
    close();
}

void MainWindow::on_action_U_triggered()
{
    // "復原" 選單項目的操作
}

void MainWindow::on_action_R_triggered()
{
    // "取消復原" 選單項目的操作
}

void MainWindow::on_action_C_triggered()
{
    // "複製" 選單項目的操作
}

void MainWindow::on_action_X_triggered()
{
    // "剪下" 選單項目的操作
}

void MainWindow::on_action_V_triggered()
{
    // "貼上" 選單項目的操作
}

void MainWindow::on_action_D_triggered()
{
    // "刪除" 選單項目的操作
}

void MainWindow::on_action_A_triggered()
{
    // "全選" 選單項目的操作
}

void MainWindow::on_action_T_triggered()
{
    // "今日檢視" 選單項目的操作
    m_stackedWidget->setCurrentWidget(m_todayView);
}

void MainWindow::on_action_M_triggered()
{
    // "月計畫" 選單項目的操作
    m_stackedWidget->setCurrentWidget(m_monthView);
}

void MainWindow::on_action_F_triggered()
{
    // "未來計畫" 選單項目的操作
    m_stackedWidget->setCurrentWidget(m_futureView);
}

void MainWindow::on_action_V_2_triggered()
{
    // "每日回顧" 選單項目的操作
    m_stackedWidget->setCurrentWidget(m_reviewView);
    // 預設顯示今天的日期
    m_selectedReviewDate = QDate::currentDate();
    m_reviewDateLabel->setText(tr("日期: ") + m_selectedReviewDate.toString("yyyy-MM-dd"));
}

void MainWindow::on_action_Search_triggered()
{
    // "搜尋" 選單項目的操作
}

void MainWindow::on_action_H_triggered()
{
    // "說明" 選單項目的操作
}

void MainWindow::on_action_A_2_triggered()
{
    // "關於" 選單項目的操作
}

void MainWindow::onFutureYearChanged(int index)
{
    // 清空月份列表，重新添加月份
    m_futureMonthList->clear();
    populateFutureMonthList();
}

void MainWindow::populateFutureMonthList()
{
    m_futureMonthList->clear();
    QStringList months;
    for (int month = 1; month <= 12; ++month) {
        months << QLocale().monthName(month, QLocale::LongFormat);
    }
    m_futureMonthList->addItems(months);
}


void MainWindow::onCalendarDateChanged()
{
    // 判斷是哪個 QCalendarWidget 觸發了訊號
    if (sender() == m_monthCalendar) {
        // 來自 m_monthCalendar (月計畫的日曆)
        QDate selectedDate = m_monthCalendar->selectedDate();
        updateMonthEntryList(selectedDate);
        m_currentDate = selectedDate.toString("yyyy-MM-dd");
        // ... 其他與月計畫相關的處理 ...
    } else if (sender() == m_navigationCalendar) {
        // 來自 m_navigationCalendar (導航列的日曆)
        QDate selectedDate = m_navigationCalendar->selectedDate();
        m_selectedReviewDate = selectedDate;
        m_reviewDateLabel->setText(tr("日期: ") + selectedDate.toString("yyyy-MM-dd"));
        m_stackedWidget->setCurrentWidget(m_reviewView);
        // ... 其他與每日檢視相關的處理 ...
    }
}

void MainWindow::onNavigationCalendarClicked(const QDate &date)
{
    m_selectedReviewDate = date;
    m_reviewDateLabel->setText(tr("日期: ") + date.toString("yyyy-MM-dd"));
    // 切換到每日回顧
    m_stackedWidget->setCurrentWidget(m_reviewView);
}


void MainWindow::onNewEntry()
{
    addOrEditJournalEntry(nullptr); // 傳遞 nullptr 表示新增條目
}

void MainWindow::onEditEntry()
{
    QListWidgetItem *currentItem = m_todayEntryList->currentItem();
    if (!currentItem) return;

    JournalEntry currentEntry = currentItem->data(Qt::UserRole).value<JournalEntry>();

    // 傳遞指向當前條目的指標給 addOrEditJournalEntry 函式
    addOrEditJournalEntry(&currentEntry);

    // 更新列表項目
    currentItem->setText(currentEntry.getTitle());
}


void MainWindow::onDeleteEntry()
{
    // 獲取當前選中的日記條目
    QListWidgetItem *currentItem = m_todayEntryList->currentItem();
    if (!currentItem) return; // 如果沒有選中任何項目，則直接返回

    // 獲取與該項目關聯的 JournalEntry 物件
    JournalEntry currentEntry = currentItem->data(Qt::UserRole).value<JournalEntry>();

    // 從 m_journalEntries 中移除該條目
    for (int i = 0; i < m_journalEntries.size(); ++i) {
        if (m_journalEntries[i].getDateTime() == currentEntry.getDateTime() &&
            m_journalEntries[i].getTitle() == currentEntry.getTitle()) {
            m_journalEntries.removeAt(i);
            break;
        }
    }

    // 從列表視圖中移除該項目
    delete m_todayEntryList->takeItem(m_todayEntryList->row(currentItem));
}

void MainWindow::onEntrySelected(QListWidgetItem *item)
{
    if (!item) return;

    // 獲取與點擊項目相關聯的 JournalEntry 物件
    JournalEntry entry = item->data(Qt::UserRole).value<JournalEntry>();

    // 將所選條目的內容顯示在 QTextEdit 中
    m_todayEntryContent->setText(entry.getContent());
}

void MainWindow::onPrevMonthButtonClicked()
{
    m_monthCalendar->showPreviousMonth();
}

void MainWindow::onNextMonthButtonClicked()
{
    m_monthCalendar->showNextMonth();
}

void MainWindow::onFutureMonthSelected()
{
    // 獲取選中的年份和月份
    int year = m_futureYearCombo->currentText().toInt();
    QListWidgetItem* selectedMonthItem = m_futureMonthList->currentItem();
    if (!selectedMonthItem) return;
    QString monthName = selectedMonthItem->text();

    // 解析月份名稱為月份數字
    QDate date = QDate::fromString(monthName, "MMMM");
    int month = date.month();

    // 檢查是否有選中的月份
    if (month == 0) return;

    // 組合年份和月份
    QString yearMonth = QString("%1-%2").arg(year).arg(month, 2, 10, QChar('0'));

    // 尋找匹配的 FutureLogEntry
    for (const FutureLogEntry& entry : m_futureLogEntries) {
        if (entry.getYearMonth() == yearMonth) {
            // 顯示該項目的內容
            displayFutureLogEntry(entry);
            return;
        }
    }

    // 如果沒有找到匹配的項目，則清空顯示區域
    clearFutureLogContent();
}
void MainWindow::onNewFutureLogEntry()
{
    addOrEditFutureLogEntry(nullptr); // 傳遞 nullptr 表示新增條目
}

void MainWindow::onEditFutureLogEntry()
{
    // 獲取當前選中的年份和月份
    int year = m_futureYearCombo->currentText().toInt();
    QString monthName = m_futureMonthList->currentItem()->text();
    QDate date = QDate::fromString(monthName, "MMMM");
    int month = date.month();

    // 檢查是否有選中的月份
    if (month == 0) return;

    // 組合年份和月份
    QString yearMonth = QString("%1-%2").arg(year).arg(month, 2, 10, QChar('0'));

    // 尋找匹配的 FutureLogEntry
    for (int i = 0; i < m_futureLogEntries.size(); ++i) {
        if (m_futureLogEntries[i].getYearMonth() == yearMonth) {
            // 找到匹配的條目，傳遞給 addOrEditFutureLogEntry 函式進行編輯
            addOrEditFutureLogEntry(&m_futureLogEntries[i]);
            return;
        }
    }
}



void MainWindow::onDeleteFutureLogEntry()
{
    // 獲取當前選中的年份和月份
    int year = m_futureYearCombo->currentText().toInt();
    QString monthName = m_futureMonthList->currentItem()->text();
    QDate date = QDate::fromString(monthName, "MMMM");
    int month = date.month();

    // 檢查是否有選中的月份
    if (month == 0) return;

    // 組合年份和月份
    QString yearMonth = QString("%1-%2").arg(year).arg(month, 2, 10, QChar('0'));

    // 尋找匹配的 FutureLogEntry 並刪除
    for (int i = 0; i < m_futureLogEntries.size(); ++i) {
        if (m_futureLogEntries[i].getYearMonth() == yearMonth) {
            m_futureLogEntries.removeAt(i);
            // 更新顯示
            updateFutureLogTree();
            // 清空編輯區域
            m_futureLogContent->clear();
            return;
        }
    }
}

void MainWindow::onSaveReview()
{
    // 獲取當前回顧的日期和內容
    QString reviewContent = m_reviewContent->toPlainText();

    // 使用目前的日期和時間建立 QDateTime 物件 (時間會是 00:00:00)
    QDateTime reviewDateTime = QDateTime::currentDateTime();
    reviewDateTime.setDate(m_selectedReviewDate);

    // 建立 ReviewEntry 物件
    ReviewEntry newReviewEntry(reviewDateTime, reviewContent);

    // 將新的回顧條目添加到列表中
    m_reviewEntries.append(newReviewEntry);

    // 清空回顧內容編輯區
    m_reviewContent->clear();

    // 可以選擇更新或顯示回顧列表（如果有的話）
    // ...
}

void MainWindow::onOpenJournal()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("開啟日誌"), "", tr("JSON 檔案 (*.json);;所有檔案 (*.*)"));
    if (filename.isEmpty()) {
        return;
    }

    // 清空當前的日記項目
    m_journalEntries.clear();

    // 從檔案載入日記項目
    m_journalEntries = DataHandler::loadJournalEntriesFromFile(filename);

    // 刷新今天的日記項目列表
    updateTodayEntryList();

    // 可以選擇更新其他視圖的內容，例如月計畫和未來計畫
    // ...
}


void MainWindow::onSaveJournal()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("儲存日誌"), "", tr("JSON 檔案 (*.json);;所有檔案 (*.*)"));
    if (filename.isEmpty()) {
        return;
    }

    // 將當前的日記項目儲存到檔案
    bool success = DataHandler::saveJournalEntriesToFile(m_journalEntries, filename);

    if (success) {
        QMessageBox::information(this, tr("儲存成功"), tr("日誌已成功儲存到檔案。"));
    } else {
        QMessageBox::critical(this, tr("儲存失敗"), tr("無法儲存日誌到檔案。"));
    }
}

void MainWindow::updateMonthEntryList(const QDate& date)
{
    m_monthEntryList->clear();
    int year = date.year();
    int month = date.month();

    for (const JournalEntry& entry : m_journalEntries) {
        QDate entryDate = entry.getDateTime().date();
        if (entryDate.year() == year && entryDate.month() == month) {
            QListWidgetItem *item = new QListWidgetItem(entry.getTitle(), m_monthEntryList);
            item->setData(Qt::UserRole, QVariant::fromValue(entry));
        }
    }
}

void MainWindow::updateTodayEntryList()
{
    m_todayEntryList->clear(); // 清空列表

    QDate today = QDate::currentDate();
    QString todayStr = today.toString("yyyy-MM-dd");

    for (const JournalEntry& entry : m_journalEntries) {
        if (entry.getDateString() == todayStr) {
            QListWidgetItem* item = new QListWidgetItem(entry.getTitle(), m_todayEntryList);
            item->setData(Qt::UserRole, QVariant::fromValue(entry)); // 將 JournalEntry 物件儲存在 item 的 data 中
        }
    }
}

void MainWindow::updateFutureLogTree()
{

}

void MainWindow::clearTodayEntryContent()
{
    m_todayEntryContent->clear();
}

void MainWindow::clearTodayEntryTitle()
{
    m_todayEntryTitle->clear();
}


void MainWindow::displayTodayEntry(const JournalEntry& entry)
{
    m_todayEntryContent->setText(entry.getContent());
    m_todayEntryTitle->setText(entry.getTitle()); // 顯示標題
}


void MainWindow::clearFutureLogContent()
{
    m_futureLogContent->clear();
}

void MainWindow::displayFutureLogEntry(const FutureLogEntry& entry)
{
    m_futureLogContent->setText(entry.getContent());
}

void MainWindow::displayReviewEntry(const ReviewEntry& entry)
{
    m_reviewContent->setText(entry.getContent());
}

void MainWindow::clearReviewContent()
{
    m_reviewContent->clear();
}

void MainWindow::addOrEditJournalEntry(JournalEntry* entry)
{
    // 顯示一個對話框，讓使用者輸入標題、內容和標籤
    QDialog dialog(this);
    dialog.setWindowTitle(entry ? tr("編輯日記") : tr("新增日記"));

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *titleLabel = new QLabel(tr("標題:"), &dialog);
    QLineEdit *titleLineEdit = new QLineEdit(&dialog);
    if (entry) titleLineEdit->setText(entry->getTitle());

    QLabel *contentLabel = new QLabel(tr("內容:"), &dialog);
    QTextEdit *contentEdit = new QTextEdit(&dialog);
    if (entry) contentEdit->setText(entry->getContent());

    // 標籤輸入
    QLabel *tagsLabel = new QLabel(tr("標籤 (用空格分隔):"), &dialog);
    QLineEdit *tagsLineEdit = new QLineEdit(&dialog);
    if (entry) {
        QStringList tags = entry->getTags();
        tagsLineEdit->setText(tags.join(" "));
    }

    layout->addWidget(titleLabel);
    layout->addWidget(titleLineEdit);
    layout->addWidget(contentLabel);
    layout->addWidget(contentEdit);
    layout->addWidget(tagsLabel);
    layout->addWidget(tagsLineEdit);

    // 確定和取消按鈕
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton(tr("確定"), &dialog);
    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    QPushButton *cancelButton = new QPushButton(tr("取消"), &dialog);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    if (dialog.exec() == QDialog::Accepted) {
        // 獲取使用者輸入的標題、內容和標籤
        QString title = titleLineEdit->text();
        QString content = contentEdit->toPlainText();
        QStringList tags = tagsLineEdit->text().split(" ", Qt::SkipEmptyParts);

        if (entry) {
            // 編輯現有條目
            entry->setTitle(title);
            entry->setContent(content);
            entry->setTags(tags);
        } else {
            // 創建新的條目
            JournalEntry newEntry(QDateTime::currentDateTime(), title, content);
            newEntry.setTags(tags);
            m_journalEntries.append(newEntry);
        }

        // 更新今日檢視列表
        updateTodayEntryList();
    }
}

void MainWindow::addOrEditFutureLogEntry(FutureLogEntry* entry)
{
    // 獲取當前選中的年份和月份
    int year = m_futureYearCombo->currentText().toInt();
    QString monthName = m_futureMonthList->currentItem()->text();
    QDate date = QDate::fromString(monthName, "MMMM");
    int month = date.month();

    // 檢查是否有選中的月份
    if (month == 0) return;

    // 組合年份和月份
    QString yearMonth = QString("%1-%2").arg(year).arg(month, 2, 10, QChar('0'));

    // 顯示一個對話框，讓使用者輸入標題、內容和標籤
    QDialog dialog(this);
    dialog.setWindowTitle(entry ? tr("編輯未來計畫") : tr("新增未來計畫"));

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *titleLabel = new QLabel(tr("標題:"), &dialog);
    QLineEdit *titleLineEdit = new QLineEdit(&dialog);
    if (entry) titleLineEdit->setText(entry->getTitle());

    QLabel *contentLabel = new QLabel(tr("內容:"), &dialog);
    QTextEdit *contentEdit = new QTextEdit(&dialog);
    if (entry) contentEdit->setText(entry->getContent());

    layout->addWidget(titleLabel);
    layout->addWidget(titleLineEdit);
    layout->addWidget(contentLabel);
    layout->addWidget(contentEdit);

    // 確定和取消按鈕
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton(tr("確定"), &dialog);
    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    QPushButton *cancelButton = new QPushButton(tr("取消"), &dialog);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    if (dialog.exec() == QDialog::Accepted) {
        // 獲取使用者輸入的標題和內容
        QString title = titleLineEdit->text();
        QString content = contentEdit->toPlainText();

        if (entry) {
            // 編輯現有條目
            entry->setTitle(title);
            entry->setContent(content);
            entry->setYearMonth(yearMonth);
        } else {
            // 創建新的條目
            FutureLogEntry newEntry(yearMonth, title, content);
            m_futureLogEntries.append(newEntry);
        }

        // 更新未來計畫列表
        updateFutureLogTree();
    }
}

void MainWindow::loadAllData()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("開啟日誌"),
                                                    "",
                                                    tr("JSON 檔案 (*.json);;所有檔案 (*.*)"));
    if (filename.isEmpty()) {
        return;
    }

    // 載入日記項目
    m_journalEntries = DataHandler::loadJournalEntriesFromFile(filename);

    // 載入未來計畫項目
    m_futureLogEntries = DataHandler::loadFutureLogEntriesFromFile(filename);

    // 載入每日回顧項目
    m_reviewEntries = DataHandler::loadReviewEntriesFromFile(filename);

    // 更新各個視圖的顯示
    updateTodayEntryList();
    updateFutureLogTree();
    // ... 更新月計畫視圖 ...
    // ... 更新每日回顧視圖 ... (如果你有相關的顯示函式)

    QMessageBox::information(this, tr("載入成功"), tr("日誌資料已成功載入。"));
}

void MainWindow::saveAllData()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("儲存日誌"),
                                                    "",
                                                    tr("JSON 檔案 (*.json);;所有檔案 (*.*)"));
    if (filename.isEmpty()) {
        return;
    }

    // 儲存日記項目
    if (!DataHandler::saveJournalEntriesToFile(m_journalEntries, filename)) {
        QMessageBox::critical(this, tr("儲存失敗"), tr("無法儲存日記資料。"));
        return;
    }

    // 儲存未來計畫項目
    if (!DataHandler::saveFutureLogEntriesToFile(m_futureLogEntries, filename)) {
        QMessageBox::critical(this, tr("儲存失敗"), tr("無法儲存未來計畫資料。"));
        return;
    }

    // 儲存每日回顧項目
    if (!DataHandler::saveReviewEntriesToFile(m_reviewEntries, filename)) {
        QMessageBox::critical(this, tr("儲存失敗"), tr("無法儲存每日回顧資料。"));
        return;
    }

    QMessageBox::information(this, tr("儲存成功"), tr("日誌資料已成功儲存。"));
}
