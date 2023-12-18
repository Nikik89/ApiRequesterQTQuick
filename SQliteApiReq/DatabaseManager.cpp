// DatabaseManager.cpp
#include "DatabaseManager.h"
DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    // Создаем уникальное имя соединения
    QString connectionName = QString::number(reinterpret_cast<quintptr>(this));
    m_database = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    m_database.setDatabaseName("users.db");
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

void DatabaseManager::openDatabase()
{
    if (m_database.open()) {
        qDebug() << "Database opened successfully";
    } else {
        qWarning() << "Error opening database:" << m_database.lastError().text();
    }
}

void DatabaseManager::closeDatabase()
{
    m_database.close();
    QSqlDatabase::removeDatabase(m_database.connectionName());
}

void DatabaseManager::createTable()
{
    QSqlQuery query(m_database);
    if (query.exec("CREATE TABLE IF NOT EXISTS users_table ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "gender TEXT, "
                    "name TEXT, "
                    "location TEXT, "
                    "email TEXT, "
                    "login TEXT, "
                    "dob TEXT, "
                    "registered TEXT, "
                    "phone TEXT, "
                    "cell TEXT, "
                    "ids TEXT, "
                    "nat TEXT,"
                    "comments TEXT)")) {
        qDebug() << "Table created successfully";
    } else {
        qWarning() << "Error creating table:" << query.lastError().text();
    }
}

void DatabaseManager::insertData(const QVariantMap &resultMap)
{
    QSqlQuery query(m_database);
    query.prepare("INSERT INTO users_table ("
                      "gender, name, location, email, login, dob, registered, phone, cell, ids, nat, comments) "
                      "VALUES (:gender, :name, :location, :email, :login, :dob, :registered, :phone, :cell, :ids, :nat, :comments)");

    query.bindValue(":gender", resultMap["gender"]);
    query.bindValue(":name", resultMap["name"]);
    query.bindValue(":location", resultMap["location"]);
    query.bindValue(":email", resultMap["email"]);
    query.bindValue(":login", resultMap["login"]);
    query.bindValue(":dob", resultMap["dob"]);
    query.bindValue(":registered", resultMap["registered"]);
    query.bindValue(":phone", resultMap["phone"]);
    query.bindValue(":cell", resultMap["cell"]);
    query.bindValue(":ids", resultMap["ids"]);
    query.bindValue(":nat", resultMap["nat"]);
    query.bindValue(":comments", "missed");
    if (query.exec()) {
        qDebug() << "Data inserted successfully";
    } else {
        qWarning() << "Error inserting data:" << query.lastError().text();
    }
}

void DatabaseManager::fetchData()
{
    QVariantList dataList;

    if (m_database.isOpen()) {
        QSqlQuery query(m_database);
        if (query.exec("SELECT * FROM users_table")) {
            while (query.next()) {
                QVariantMap userData;
                userData["gender"] = query.value("gender").toString();
                userData["name"] = query.value("name").toString();
                userData["comments"] = query.value("comments").toString();

                dataList.append(userData);
            }
        } else {
            qWarning() << "Error executing query:" << query.lastError().text();
        }
    } else {
        qWarning() << "Database is not open";
    }

    emit dataFetched(dataList);
}
void DatabaseManager::clearData()
{
    if (m_database.isOpen()) {
        QSqlQuery query(m_database);
        if (query.exec("DELETE FROM users_table")) {
            qDebug() << "Database cleared";
            QString updateSequenceQuery = "UPDATE sqlite_sequence SET seq = 0 WHERE name = 'users_table'";
            if (query.exec(updateSequenceQuery)) {
                qDebug() << "Sequence updated successfully";
            } else {
                qWarning() << "Error updating sequence:" << query.lastError().text();
            }
            fetchData(); // Fetch data again to update the UI
        } else {
            qWarning() << "Error clearing database:" << query.lastError().text();
        }
    } else {
        qWarning() << "Database is not open";
    }
}
void DatabaseManager::removeData(int index)
{
    QString queryDelete = "DELETE FROM users_table WHERE id = " + QString::number(index);
    qDebug() << queryDelete;
    if (m_database.isOpen()) {
        QSqlQuery query(m_database);
        if (query.exec(queryDelete)) {
            qDebug() << "Элемент удален";
            QString updateSequenceQuery = "UPDATE users_table SET id = id - 1 WHERE id > " + QString::number(index);
            query.exec(updateSequenceQuery);
            fetchData(); // Fetch data again to update the UI
        } else {
            qWarning() << "Error clearing database:" << query.lastError().text();
        }
    } else {
        qWarning() << "Database is not open";
    }
}
QVariantMap DatabaseManager::fetchPersonData(int index)
{
    QVariantMap personData;

    if (m_database.isOpen()) {
        QSqlQuery query(m_database);
        query.prepare("SELECT * FROM users_table WHERE id = :id");
        query.bindValue(":id", index + 1);

        if (query.exec() && query.next()) {
            personData["gender"] = query.value("gender").toString();
            personData["name"] = query.value("name").toString();
            personData["location"] = query.value("location").toString();
            personData["email"] = query.value("email").toString();
            personData["login"] = query.value("login").toString();
            personData["dob"] = query.value("dob").toString();
            return personData;
        } else {
            qWarning() << "Error fetching person data:" << query.lastError().text();
        }
    } else {
        qWarning() << "Database is not open";
    }

    return personData;
}

void DatabaseManager::addComment(int index, const QString &comment)
{
    if (m_database.isOpen()) {
        QSqlQuery query(m_database);
        query.prepare("UPDATE users_table SET comments = :comments WHERE id = :id");
        query.bindValue(":comments", comment);
        query.bindValue(":id", index);
        if (!query.exec()) {
            qWarning() << "Error updating comments:" << query.lastError().text();
        }
        else{
            qDebug() << "Comment added";
        }
        fetchData();
    } else {
        qWarning() << "Database is not open";
    }
}

