#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QList>
#include <QString>
#include "journalentry.h"
#include "futurelogentry.h"
#include "reviewentry.h"

class DataHandler
{
public:
    // 載入日記項目
    static QList<JournalEntry> loadJournalEntriesFromFile(const QString& filename);
    // 儲存日記項目
    static bool saveJournalEntriesToFile(const QList<JournalEntry>& entries, const QString& filename);

    // 載入未來計畫項目
    static QList<FutureLogEntry> loadFutureLogEntriesFromFile(const QString& filename);
    // 儲存未來計畫項目
    static bool saveFutureLogEntriesToFile(const QList<FutureLogEntry>& entries, const QString& filename);

    // 載入每日回顧項目
    static QList<ReviewEntry> loadReviewEntriesFromFile(const QString &filename);
    // 儲存每日回顧項目
    static bool saveReviewEntriesToFile(const QList<ReviewEntry> &entries, const QString &filename);
};

#endif // DATAHANDLER_H
