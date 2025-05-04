#ifndef REVIEWENTRY_H
#define REVIEWENTRY_H

#include <QString>
#include <QDateTime>

class ReviewEntry
{
public:
    ReviewEntry();
    ReviewEntry(const QDateTime& date, const QString& content);

    QDateTime getDateTime() const;
    void setDateTime(const QDateTime& dateTime);

    QString getDateString() const;
    void setDateString(const QString& date);

    QString getContent() const;
    void setContent(const QString& content);

private:
    QDateTime m_dateTime;
    QString m_date;
    QString m_content;
};

#endif // REVIEWENTRY_H
