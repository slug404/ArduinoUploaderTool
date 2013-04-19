#include "UploadFactory.h"
#include "Uploader_Windows.h"
#include "Uploader_Linux.h"
#include "Uploader_Mac.h"

UploadFactory::UploadFactory(QObject *parent)
    : QObject(parent)
{
}

QSharedPointer<UploadBase> UploadFactory::create(UploadFactory::UploadPlatform platformType, const QString &codePath, const QString &serial, const QString &board)
{
	switch (platformType)
	{
		case OS_WINDOWS:
		{
			return QSharedPointer<UploadBase>(new Uploader_Windows(codePath, serial, board));
		}
		case OS_LINUX:
		{
			return QSharedPointer<UploadBase>(new Uploader_Linux(codePath, serial, board));
		}
		case OS_MAC:
		{
			return QSharedPointer<UploadBase>(new Uploader_Mac(codePath, serial, board));
		}
		default:
		{
			return QSharedPointer<UploadBase>(new Uploader_Windows(codePath, serial, board));//Windows用户最多
		}
	}
}
