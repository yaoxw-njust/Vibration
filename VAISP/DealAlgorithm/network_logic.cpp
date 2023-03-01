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
		sendHttpsMsg(QStringLiteral("Get�������ݳ���"), 1);
	}
}


/*
http_headers = curl_slist_append(http_headers, "Accept: application/json");
http_headers = curl_slist_append(http_headers, "Content-Type: application/json");//text/html
http_headers = curl_slist_append(http_headers, "charsets: utf-8");
http_headers = curl_slist_append(http_headers, "Authorization: Bearer de6cff6c-df4f-47c4-9c4c-fad348ed54a1");


//����methodΪpost
//curl_easy_setopt(m_pCurlHandlder, CURLOPT_POST, 1);
curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
//����post�����url��ַ
curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.172.99:8080/collection/checkData/lvwRecord");
//����HTTPͷ
curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
//���÷��ͳ�ʱʱ��

curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);

curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
//ִ�е�������
curl_easy_setopt(curl, CURLOPT_POSTFIELDS, spp.c_str());
//curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, spp.c_str().size());
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);//���ûص�����
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
		sendHttpsMsg(QStringLiteral("Post�������ݳ���"), 1);
	}
}

void NetworkLogic::requestFinished(QNetworkReply* reply)
{
    qDebug() << "send finish";
	QString strHttpReturn = "[";
	// ��ȡhttp״̬��
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
		// ��ȡ��������
		strHttpReturn += reply->readAll() + "]";
		sendHttpsMsg(strHttpReturn, 0);
	}
	qDebug() << ":" << strHttpReturn;
}