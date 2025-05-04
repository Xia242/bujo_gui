#include "futurelogentry.h"

FutureLogEntry::FutureLogEntry()
{
}

FutureLogEntry::FutureLogEntry(const QString& yearMonth, const QString& title, const QString& content) :
    m_yearMonth(yearMonth),
    m_title(title),
    m_content(content)
{
}

QString FutureLogEntry::getYearMonth() const
{
    return m_yearMonth;
}

void FutureLogEntry::setYearMonth(const QString& yearMonth)
{
    m_yearMonth = yearMonth;
}

QString FutureLogEntry::getTitle() const
{
    return m_title;
}

void FutureLogEntry::setTitle(const QString& title)
{
    m_title = title;
}

QString FutureLogEntry::getContent() const
{
    return m_content;
}

void FutureLogEntry::setContent(const QString& content)
{
    m_content = content;
}
