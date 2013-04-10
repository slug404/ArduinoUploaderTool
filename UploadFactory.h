#ifndef UPLOADFACTORY_H
#define UPLOADFACTORY_H

#include <QObject>

/*!
 * \brief 根据不同的运行平台(操作系统) 创建不同的处理对象的工厂
 */
class UploadFactory : public QObject
{
	Q_OBJECT
public:
	explicit UploadFactory(QObject *parent = 0);
	
signals:
	
public slots:
	
};

#endif // UPLOADFACTORY_H
