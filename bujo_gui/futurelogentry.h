#ifndef FUTURELOGENTRY_H
#define FUTURELOGENTRY_H

#include <QString>

class FutureLogEntry
{
public:
    FutureLogEntry();
    FutureLogEntry(const QString& yearMonth, const QString& title, const QString& content);

    QString getYearMonth() const;
    void setYearMonth(const QString& yearMonth);

    QString getTitle() const;
    void setTitle(const QString& title);

    QString getContent() const;
    void setContent(const QString& content);

private:
    QString m_yearMonth;
    QString m_title;
    QString m_content;
};

#endif // FUTURELOGENTRY_H
