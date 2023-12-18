// DatabaseManager.h
#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    Q_INVOKABLE void openDatabase();
    Q_INVOKABLE void closeDatabase();
    Q_INVOKABLE void createTable();
    Q_INVOKABLE void insertData(const QVariantMap &person);

public slots:
    void fetchData();
    void clearData();
    void removeData(int index);
    void addComment(int index, const QString &comment);
    QVariantMap fetchPersonData(int index);

signals:
    void dataFetched(QVariantList dataList);

private:
    QSqlDatabase m_database;
};

#endif // DATABASEMANAGER_H
