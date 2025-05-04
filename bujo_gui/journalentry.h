#ifndef JOURNALENTRY_H
#define JOURNALENTRY_H

#include <QString>
#include <QDateTime>
#include <QList>

class JournalEntry
{
public:
    JournalEntry();
    JournalEntry(const QDateTime& date, const QString& title, const QString& content);

    QDateTime getDateTime() const;
    void setDateTime(const QDateTime& dateTime);

    QString getDateString() const;
    void setDateString(const QString& date);

    QString getTitle() const;
    void setTitle(const QString& title);

    QString getContent() const;
    void setContent(const QString& content);

    QList<QString> getTags() const;
    void setTags(const QList<QString>& tags);
    void addTag(const QString& tag);
    void removeTag(const QString& tag);

private:
    QDateTime m_dateTime;
    QString m_date;
    QString m_title;
    QString m_content;
    QList<QString> m_tags;
};

#endif // JOURNALENTRY_H
