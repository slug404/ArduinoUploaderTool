#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QSharedPointer>
#include "Uploader_Windows.h"
#include "Uploader_Linux.h"
#include "Uploader_Mac.h"
#include "UploadFactory.h"

/*! the parameter number pass by command line*/
const int PARAMER_COUNT = 1+3;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	if(QFile::exists("cmd.txt"))
	{
		QFile::remove("cmd.txt");
	}

	if(PARAMER_COUNT == argc)
	{
		//accept
		QString fileName = argv[1];
		QString board = argv[2];
		QString serialPort = argv[3];
#ifdef Q_OS_WIN32
		QSharedPointer<UploadBase> pUploader = UploadFactory::create(UploadFactory::OS_WINDOWS, fileName, serialPort, board.toInt());
#elif defined(Q_OS_LINUX)
		QSharedPointer<UploadBase> pUploader = UploadFactory::create(UploadFactory::OS_LINUX, fileName, serialPort, board.toInt());
#elif defined(Q_OS_MAC)
		QSharedPointer<UploadBase> pUploader = UploadFactory::create(UploadFactory::OS_MAC, fileName, serialPort, board.toInt());
#endif
		pUploader->start();
	}
	else
	{
		qDebug() << "error";
	}

	return a.exec();
}
