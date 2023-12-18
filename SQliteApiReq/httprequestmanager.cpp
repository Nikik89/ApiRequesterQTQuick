#include "httprequestmanager.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>



HttpRequestManager::HttpRequestManager(QObject *parent) : QObject(parent)
{
    // Инициализация менеджера сетевого доступа
    networkManager = new QNetworkAccessManager(this);
}

void HttpRequestManager::makeRequest()
{
    // Создаем объект запроса
    QNetworkRequest request;
    request.setUrl(QUrl("https://randomuser.me/api/"));

    // Отправляем GET-запрос
    QNetworkReply *reply = networkManager->get(request);

    // Подключаем слот для обработки ответа
    connect(reply, &QNetworkReply::finished, this, &HttpRequestManager::onReplyFinished);
}

void HttpRequestManager::onReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            // Чтение ответа
            QByteArray data = reply->readAll();
            auto result = parseJson(data);
            emit requestFinished(result);
        } else {
            // Обработка ошибки
            qWarning() << "Error:" << reply->errorString();
        }

        // Освобождение ресурсов
        reply->deleteLater();
    }
}

QVariantMap HttpRequestManager::parseJson(const QByteArray &data)
{
    QVariantMap resultMap;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    // Проверяем, что JSON парсится успешно
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();

        // Извлекаем данные из "results" (массив)
        if (jsonObj.contains("results") && jsonObj["results"].isArray()) {
            QJsonArray resultsArray = jsonObj["results"].toArray();

            // Предполагаем, что у нас всегда есть хотя бы один элемент в массиве
            if (resultsArray.size() > 0 && resultsArray[0].isObject()) {
                QJsonObject resultObj = resultsArray[0].toObject();

                // Извлекаем нужные поля
                resultMap["gender"] = resultObj["gender"].toString();
                resultMap["name"] = resultObj["name"].toObject()["first"].toString() + " " + resultObj["name"].toObject()["last"].toString();
                resultMap["location"] = resultObj["location"].toObject()["city"].toString();
                resultMap["email"] = resultObj["email"].toString();
                resultMap["login"] = resultObj["login"].toObject()["username"].toString();
                resultMap["dob"] = resultObj["dob"].toObject()["date"].toString();
                resultMap["registered"] = resultObj["registered"].toObject()["date"].toString();
                resultMap["phone"] = resultObj["phone"].toString();
                resultMap["cell"] = resultObj["cell"].toString();
                resultMap["ids"] = resultObj["id"].toObject()["value"].toString();
                resultMap["nat"] = resultObj["nat"].toString();
            }
        }
    } else {
        qWarning() << "Ошибка при парсинге JSON";
    }

    return resultMap;
}

