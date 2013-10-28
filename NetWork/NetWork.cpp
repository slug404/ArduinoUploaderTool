#include "NetWork.h"

#include <QDebug>
#include <QTcpSocket>
#include <QSettings>
#include <QDataStream>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHostAddress>

NetWork::NetWork(QObject *parent)
	: QObject(parent)
	, pTcpSocket_(NULL)
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
	pTcpSocket_ = new QTcpSocket(this);
	pTcpSocket_->waitForConnected(30000);
//	connect(pTcpSocket_, SIGNAL(connected()), this, SLOT(slotConnected()));

	QSettings settings("setting.ini", QSettings::IniFormat);
	QString ip = settings.value("ip").toString();
	quint16 port = settings.value("port").toInt();
	qDebug() << ip << " " << port;

	pTcpSocket_->connectToHost(QHostAddress(ip), port);
}
