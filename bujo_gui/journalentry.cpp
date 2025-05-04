#include "journalentry.h"

JournalEntry::JournalEntry()
{
}

JournalEntry::JournalEntry(const QDateTime& date, const QString& title, const QString& content) :
    m_dateTime(date),
    m_title(title),
    m_content(content)
{
    m_date = date.toString("yyyy-MM-dd");
}

QDateTime JournalEntry::getDateTime() const
{
    return m_dateTime;
}

void JournalEntry::setDateTime(const QDateTime& dateTime)
{
    m_dateTime = dateTime;
    m_date = dateTime.toString("yyyy-MM-dd");
}

QString JournalEntry::getDateString() const
{
    return m_date;
}

void JournalEntry::setDateString(const QString& date)
{
    m_date = date;
    m_dateTime = QDateTime::fromString(date, "yyyy-MM-dd");
}

QString JournalEntry::getTitle() const
{
    return m_title;
}

void JournalEntry::setTitle(const QString& title)
{
    m_title = title;
}

QString JournalEntry::getContent() const
{
    return m_content;
}

void JournalEntry::setContent(const QString& content)
{
    m_content = content;
}

QList<QString> JournalEntry::getTags() const
{
    return m_tags;
}

void JournalEntry::setTags(const QList<QString>& tags)
{
    m_tags = tags;
}

void JournalEntry::addTag(const QString &tag)
{
    m_tags.append(tag);
}

void JournalEntry::removeTag(const QString &tag)
{
    m_tags.removeOne(tag);
}
