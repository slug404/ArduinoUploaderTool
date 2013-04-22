#ifndef UPLOADFACTORY_H
#define UPLOADFACTORY_H

#include <QObject>
#include <QSharedPointer>
#include "UploadBase.h"
/*!
 * \brief 根据不同的运行平台(操作系统) 创建不同的处理对象的工厂
 */
class UploadFactory : public QObject
{
	Q_OBJECT
public:
	explicit UploadFactory(QObject *parent = 0);
	
	/**
	 * @brief The UploadPlatform enum 运行平台
	 */
	enum UploadPlatform
	{
		OS_WINDOWS,/**< Windows平台*/
		OS_LINUX,/**< Linux平台*/
		OS_MAC/**< Mac平台*/
	};

	static QSharedPointer<UploadBase> create(UploadPlatform platformType, const QString &codePath, const QString &serial, int boardIndex);
};

#endif // UPLOADFACTORY_H
