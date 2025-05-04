#include "reviewentry.h"

ReviewEntry::ReviewEntry()
{
}

ReviewEntry::ReviewEntry(const QDateTime& date, const QString& content) :
    m_dateTime(date),
    m_content(content)
{
    m_date = date.toString("yyyy-MM-dd");
}

QDateTime ReviewEntry::getDateTime() const
{
    return m_dateTime;
}

void ReviewEntry::setDateTime(const QDateTime& dateTime)
{
    m_dateTime = dateTime;
    m_date = dateTime.toString("yyyy-MM-dd");
}

QString ReviewEntry::getDateString() const
{
    return m_date;
}

void ReviewEntry::setDateString(const QString& date)
{
    m_date = date;
    m_dateTime = QDateTime::fromString(date, "yyyy-MM-dd");
}

QString ReviewEntry::getContent() const
{
    return m_content;
}

void ReviewEntry::setContent(const QString& content)
{
    m_content = content;
}
