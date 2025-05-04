#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QDate>
#include "journalentry.h"
#include "futurelogentry.h"
#include "reviewentry.h"
#include <QToolButton>
#include <QPushButton>
#include <QCalendarWidget>
#include <QComboBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QListWidget>
#include <QStackedWidget>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNewEntry();
    void onEditEntry();
    void onDeleteEntry();
    void onEntrySelected(QListWidgetItem* item);
    void onCalendarDateChanged();
    void onNavigationCalendarClicked(const QDate &date);
    void onPrevMonthButtonClicked();
    void onNextMonthButtonClicked();
    void onFutureYearChanged(int index);
    void onFutureMonthSelected();
    void onNewFutureLogEntry();
    void onEditFutureLogEntry();
    void onDeleteFutureLogEntry();
    void onSaveReview();
    void onTodayButtonClicked();
    void onMonthButtonClicked();
    void onFutureButtonClicked();
    void onReviewButtonClicked();

    //選單
    void onOpenJournal();
    void onSaveJournal();
    void on_action_N_triggered();
    void on_action_O_triggered();
    void on_action_S_triggered();
    void on_action_E_triggered();
    void on_action_U_triggered();
    void on_action_R_triggered();
    void on_action_C_triggered();
    void on_action_X_triggered();
    void on_action_V_triggered();
    void on_action_D_triggered();
    void on_action_A_triggered();
    void on_action_T_triggered();
    void on_action_M_triggered();
    void on_action_F_triggered();
    void on_action_V_2_triggered();
    void on_action_Search_triggered();
    void on_action_H_triggered();
    void on_action_A_2_triggered();


private:
    Ui::MainWindow *ui;

    QStackedWidget* m_stackedWidget;

    // 今日檢視
    QWidget* m_todayView;
    QVBoxLayout* m_todayLayout;
    QLabel* m_todayDateLabel;
    QLineEdit* m_todayEntryTitle; // 新增標題輸入框
    QTextEdit* m_todayEntryContent;
    QListWidget* m_todayEntryList;
    QHBoxLayout* m_todayButtonLayout;
    QToolButton* m_newEntryButton;
    QToolButton* m_editEntryButton;
    QToolButton* m_deleteEntryButton;

    // 月計畫
    QWidget* m_monthView;
    QVBoxLayout* m_monthLayout;
    QCalendarWidget* m_monthCalendar;
    QListWidget* m_monthEntryList;
    QHBoxLayout* m_monthButtonLayout;
    QPushButton* m_prevMonthButton;
    QPushButton* m_nextMonthButton;

    // 未來計畫
    QWidget* m_futureView;
    QVBoxLayout* m_futureLayout;
    QComboBox* m_futureYearCombo;
    QListWidget* m_futureMonthList;
    QTextEdit* m_futureLogContent;
    QHBoxLayout* m_futureButtonLayout;
    QToolButton* m_newFutureLogEntryButton;
    QToolButton* m_editFutureLogEntryButton;
    QToolButton* m_deleteFutureLogEntryButton;

    // 每日回顧
    QWidget* m_reviewView;
    QVBoxLayout * m_reviewLayout;
    QLabel* m_reviewDateLabel;
    QTextEdit* m_reviewContent;
    QPushButton* m_saveReviewButton;
    QDate m_selectedReviewDate;

    // 左側導航欄
    QWidget* m_navigationWidget;
    QVBoxLayout* m_navigationLayout;
    QToolButton* m_todayButton;
    QToolButton* m_monthButton;
    QToolButton* m_futureButton;
    QToolButton* m_reviewButton;
    QCalendarWidget* m_navigationCalendar;

    QList<JournalEntry> m_journalEntries; // 儲存所有日記條目
    QList<FutureLogEntry> m_futureLogEntries; // 儲存所有未來計畫條目
    QList<ReviewEntry> m_reviewEntries; //儲存所有每日回顧
    QString m_currentDate;

    void loadJournal();
    void saveJournal();
    void loadAllData();
    void saveAllData();
    void updateTodayEntryList();
    void updateMonthEntryList(const QDate& date);
    void updateFutureLogTree();
    void clearTodayEntryContent(); //清除今日檢視的內容
    void displayTodayEntry(const JournalEntry& entry);
    void clearFutureLogContent();
    void displayFutureLogEntry(const FutureLogEntry& entry);
    void displayReviewEntry(const ReviewEntry& entry);
    void clearReviewContent();
    void populateFutureMonthList();
    void clearTodayEntryTitle(); //清除今日檢視的標題
    void addOrEditJournalEntry(JournalEntry* entry);
    void addOrEditFutureLogEntry(FutureLogEntry* entry);
};
#endif // MAINWINDOW_H
