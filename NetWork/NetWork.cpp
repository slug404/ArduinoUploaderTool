#include "NetWork.h"

#include <QDebug>
#include <QSettings>
#include <QDataStream>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHostAddress>

NetWork::NetWork(QObject *parent)
	: QObject(parent)
	, pTcpSocket_(NULL)
	, pFile_(NULL)
{
	initData();
}

QVariantMap NetWork::getGrammaticalError(const QString &str)
{
	QVariantMap map;
	map.insert("index", 1);
	map.insert("str", str);

	return map;
}

QVariantMap NetWork::getBoradError(const QString &str)
{
	QVariantMap map;
	map.insert("index", 2);
	map.insert("str", str);

	return map;
}

QVariantMap NetWork::getSerialPortError(const QString &str)
{
	QVariantMap map;
	map.insert("index", 3);
	map.insert("str", str);

	return map;
}

QVariantMap NetWork::getCompileSuccess(const QString &str)
{
	QVariantMap map;
	map.insert("index", 4);
	map.insert("str", str);

	return map;
}

QVariantMap NetWork::getUploaderSuccess(const QString &str)
{
	QVariantMap map;
	map.insert("index", 5);
	map.insert("str", str);

	return map;
}

QVariantMap NetWork::getOtherInfor(const QString &str)
{
	QVariantMap map;
	map.insert("index", 0);
	map.insert("str", str);

	return map;
}

void NetWork::initData()
{
	pFile_ = new QFile("log");
#ifdef USE_DEBUG
	if(!pFile_->open(QFile::WriteOnly))
	{
		qDebug() << "无法写入日志";
	}
#endif

	pTcpSocket_ = new QTcpSocket(this);

	connect(pTcpSocket_, SIGNAL(connected()), this, SLOT(slotConnected()));

	QSettings settings("setting.ini", QSettings::IniFormat);
	QString ip = settings.value("ip").toString();
	quint16 port = settings.value("port").toInt();
	qDebug() << ip << " " << port;

	pTcpSocket_->connectToHost(QHostAddress("127.0.0.1"), 8660);
	pTcpSocket_->waitForConnected();
}

void NetWork::slotConnected()
{
	qDebug() << "连接上服务器";

#ifdef USE_DEBUG
	pFile_->write("连接上服务器");
#endif

	QByteArray bytes;
	QDataStream out(&bytes, QIODevice::WriteOnly);

	out << qint32(0);

	{
		//该死的json
		QVariantMap map;
		map.insert("index", 1);
		map.insert("string", "xxxxxxxxxx");

		QJsonDocument jsonDoc = QJsonDocument::fromVariant(map);
		out << jsonDoc.toJson();
	}

	out.device()->seek(0);
	out << qint32(qint32(bytes.size()) - qint32(sizeof(qint32)));

	pTcpSocket_->write(bytes);
	pTcpSocket_->waitForBytesWritten();

#ifdef USE_DEBUG
	pFile_->write("socket 写入数据");
#endif
	qDebug() << "老子就不信";
}
