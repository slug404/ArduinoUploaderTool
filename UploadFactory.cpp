#include "UploadFactory.h"
#include "Uploader_Windows.h"
#include "Uploader_Linux.h"
#include "Uploader_Mac.h"

UploadFactory::UploadFactory(QObject *parent)
    : QObject(parent)
{
}

QSharedPointer<UploadBase> UploadFactory::create(UploadFactory::UploadPlatform platformType, const QString &codePath, const QString &serial, int boardIndex)
{
	switch (platformType)
	{
		case OS_WINDOWS:
		{
			return QSharedPointer<UploadBase>(new Uploader_Windows(codePath, serial, boardIndex));
		}
		case OS_LINUX:
		{
			return QSharedPointer<UploadBase>(new Uploader_Linux(codePath, serial, boardIndex));
		}
		case OS_MAC:
		{
			return QSharedPointer<UploadBase>(new Uploader_Mac(codePath, serial, boardIndex));
		}
		default:
		{
			return QSharedPointer<UploadBase>(new Uploader_Windows(codePath, serial, boardIndex));//Windows用户最多
		}
	}
}
