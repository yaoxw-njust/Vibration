#pragma once

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QCoreApplication>

#pragma comment(lib, "Qt5Network.lib")

class NetworkLogic : public QObject
{
	Q_OBJECT

public:
	NetworkLogic(QObject *parent=Q_NULLPTR);
	~NetworkLogic();

public:
	void getHttps(QString strUrlPath);
	void postHttps(QString strUrlPath, QByteArray postData);

	private slots:
	void requestFinished(QNetworkReply* reply);

signals:
	void sendHttpsMsg(QString msg, int flag);

private:
	QNetworkRequest request;
	QNetworkAccessManager* naManager = NULL;;


};
