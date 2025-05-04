#include "datahandler.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>


QList<JournalEntry> DataHandler::loadJournalEntriesFromFile(const QString& filename)
{
    QList<JournalEntry> entries;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "無法開啟檔案:" << filename;
        return entries;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject())
    {
        qWarning() << "檔案格式錯誤: 不是 JSON 物件";
        return entries;
    }

    QJsonObject root = doc.object();
    if (!root.contains("journalEntries") || !root["journalEntries"].isArray())
    {
        qWarning() << "檔案格式錯誤: 缺少 journalEntries 陣列";
        return entries;
    }

    QJsonArray entriesArray = root["journalEntries"].toArray();
    for (const QJsonValue& value : entriesArray)
    {
        if (value.isObject())
        {
            QJsonObject entryObject = value.toObject();
            JournalEntry entry;
            entry.setDateString(entryObject["date"].toString());
            entry.setTitle(entryObject["title"].toString());
            entry.setContent(entryObject["content"].toString());
            // 假設 tags 是字串列表
            QJsonArray tagsArray = entryObject["tags"].toArray();
            QList<QString> tags;
            for (const QJsonValue& tagValue : tagsArray) {
                tags.append(tagValue.toString());
            }
            entry.setTags(tags);
            entries.append(entry);
        }
    }

    return entries;
}

bool DataHandler::saveJournalEntriesToFile(const QList<JournalEntry>& entries, const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "無法開啟檔案:" << filename;
        return false;
    }

    QJsonArray entriesArray;
    for (const JournalEntry& entry : entries)
    {
        QJsonObject entryObject;
        entryObject["date"] = entry.getDateString();
        entryObject["title"] = entry.getTitle();
        entryObject["content"] = entry.getContent();
        // 儲存 tags
        QJsonArray tagsArray;
        for (const QString& tag : entry.getTags()) {
            tagsArray.append(tag);
        }
        entryObject["tags"] = tagsArray;
        entriesArray.append(entryObject);
    }

    QJsonObject root;
    root["journalEntries"] = entriesArray;
    QJsonDocument doc(root);

    file.write(doc.toJson());
    file.close();

    return true;
}

QList<FutureLogEntry> DataHandler::loadFutureLogEntriesFromFile(const QString& filename)
{
    QList<FutureLogEntry> entries;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "無法開啟檔案:" << filename;
        return entries;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "檔案格式錯誤: 不是 JSON 物件";
        return entries;
    }

    QJsonObject root = doc.object();
    if (!root.contains("futureLogEntries") || !root["futureLogEntries"].isArray()) {
        qWarning() << "檔案格式錯誤: 缺少 futureLogEntries 陣列";
        return entries;
    }

    QJsonArray entriesArray = root["futureLogEntries"].toArray();
    for (const QJsonValue& value : entriesArray) {
        if (value.isObject()) {
            QJsonObject entryObject = value.toObject();
            FutureLogEntry entry;
            entry.setYearMonth(entryObject["yearMonth"].toString());
            entry.setTitle(entryObject["title"].toString());
            entry.setContent(entryObject["content"].toString());
            entries.append(entry);
        }
    }

    return entries;
}

bool DataHandler::saveFutureLogEntriesToFile(const QList<FutureLogEntry>& entries, const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "無法開啟檔案:" << filename;
        return false;
    }

    QJsonArray entriesArray;
    for (const FutureLogEntry& entry : entries) {
        QJsonObject entryObject;
        entryObject["yearMonth"] = entry.getYearMonth();
        entryObject["title"] = entry.getTitle();
        entryObject["content"] = entry.getContent();
        entriesArray.append(entryObject);
    }

    QJsonObject root;
    root["futureLogEntries"] = entriesArray;
    QJsonDocument doc(root);

    file.write(doc.toJson());
    file.close();

    return true;
}

QList<ReviewEntry> DataHandler::loadReviewEntriesFromFile(const QString& filename)
{
    QList<ReviewEntry> entries;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "無法開啟檔案:" << filename;
        return entries;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "檔案格式錯誤: 不是 JSON 物件";
        return entries;
    }

    QJsonObject root = doc.object();
    if (!root.contains("reviewEntries") || !root["reviewEntries"].isArray()) {
        qWarning() << "檔案格式錯誤: 缺少 reviewEntries 陣列";
        return entries;
    }

    QJsonArray entriesArray = root["reviewEntries"].toArray();
    for (const QJsonValue& value : entriesArray) {
        if (value.isObject()) {
            QJsonObject entryObject = value.toObject();
            ReviewEntry entry;
            entry.setDateString(entryObject["date"].toString());
            entry.setContent(entryObject["content"].toString());
            entries.append(entry);
        }
    }

    return entries;
}

bool DataHandler::saveReviewEntriesToFile(const QList<ReviewEntry>& entries, const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "無法開啟檔案:" << filename;
        return false;
    }

    QJsonArray entriesArray;
    for (const ReviewEntry& entry : entries) {
        QJsonObject entryObject;
        entryObject["date"] = entry.getDateString();
        entryObject["content"] = entry.getContent();
        entriesArray.append(entryObject);
    }

    QJsonObject root;
    root["reviewEntries"] = entriesArray;
    QJsonDocument doc(root);

    file.write(doc.toJson());
    file.close();

    return true;
}
