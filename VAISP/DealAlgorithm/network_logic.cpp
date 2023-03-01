#include "network_logic.h"

NetworkLogic::NetworkLogic(QObject *parent) : QObject(parent)
{
	naManager = new QNetworkAccessManager(this);
	QMetaObject::Connection connRet = QObject::connect(naManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
	Q_ASSERT(connRet);
}

NetworkLogic::~NetworkLogic()
{
	if (NULL != naManager)
	{
		delete naManager;
	}
	naManager = NULL;
}


void NetworkLogic::getHttps(QString strUrlPath)
{
	try
	{
		request.setUrl(QUrl(strUrlPath));
		QNetworkReply* reply = naManager->get(request);
	}
	catch (...)
	{
		sendHttpsMsg(QStringLiteral("Get网络数据出错！"), 1);
	}
}


/*
http_headers = curl_slist_append(http_headers, "Accept: application/json");
http_headers = curl_slist_append(http_headers, "Content-Type: application/json");//text/html
http_headers = curl_slist_append(http_headers, "charsets: utf-8");
http_headers = curl_slist_append(http_headers, "Authorization: Bearer de6cff6c-df4f-47c4-9c4c-fad348ed54a1");


//设置method为post
//curl_easy_setopt(m_pCurlHandlder, CURLOPT_POST, 1);
curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
//设置post请求的url地址
curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.172.99:8080/collection/checkData/lvwRecord");
//设置HTTP头
curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
//设置发送超时时间

curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);

curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
//执行单条请求
curl_easy_setopt(curl, CURLOPT_POSTFIELDS, spp.c_str());
//curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, spp.c_str().size());
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);//设置回调函数
*/
void NetworkLogic::postHttps(QString strUrlPath, QByteArray postData)
{
	try
	{
		request.setUrl(QUrl(strUrlPath));
		request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
		request.setRawHeader("Authorization", "Bearer de6cff6c-df4f-47c4-9c4c-fad348ed54a1");
		qDebug() << "send start";
		QEventLoop loop;
		QObject::connect(naManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

		QNetworkReply* reply = naManager->post(request, postData);
		loop.exec();
	}
	catch (...)
	{
		sendHttpsMsg(QStringLiteral("Post网络数据出错！"), 1);
	}
}

void NetworkLogic::requestFinished(QNetworkReply* reply)
{
    qDebug() << "send finish";
	QString strHttpReturn = "[";
	// 获取http状态码
	QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (statusCode.isValid())
	{
		qDebug() <<"status:"<< statusCode.toString();
		strHttpReturn += "Status: " + statusCode.toString() + ";";
	}
	QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
	if (reason.isValid())
	{
		qDebug() <<"reason"<< reason.toString();
		strHttpReturn += "Reason: " + reason.toString() + ";";
	}
	QNetworkReply::NetworkError err = reply->error();
	if (err != QNetworkReply::NoError) 
	{
		qDebug() <<"error:"<< reply->errorString();
		sendHttpsMsg("Failed: " + reply->errorString(), 1);
	}
	else 
	{
		// 获取返回内容
		strHttpReturn += reply->readAll() + "]";
		sendHttpsMsg(strHttpReturn, 0);
	}
	qDebug() << ":" << strHttpReturn;
}