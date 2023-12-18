#ifndef HTTPREQUESTMANAGER_H
#define HTTPREQUESTMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>


class HttpRequestManager : public QObject
{
    Q_OBJECT
public:
    explicit HttpRequestManager(QObject *parent = nullptr);

public slots:
    // Метод для выполнения GET-запроса
    void makeRequest();

signals:
    // Сигнал, отправляемый при завершении запроса
    void requestFinished(const QVariantMap &data);

private slots:
    // Слот для обработки ответа от сервера
    void onReplyFinished();

private:
    // Менеджер сетевого доступа
    QNetworkAccessManager *networkManager;

    QVariantMap parseJson(const QByteArray &data);
};

#endif // HTTPREQUESTMANAGER_H
