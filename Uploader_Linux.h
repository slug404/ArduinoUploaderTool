#ifndef UPLOADER_LINUX_H
#define UPLOADER_LINUX_H

#include "UploadBase.h"

/*!
 * \brief 实现了Linux平台下的上传
 */
class Uploader_Linux : public UploadBase
{
	Q_OBJECT
public:
	explicit Uploader_Linux(const QString &codePath, const QString &serial, const QString &board, QObject *parent = 0);
	virtual ~Uploader_Linux();
signals:
	
public slots:

protected:
	//interface
	virtual void setup(); //! 准备
	virtual void compile(); //! 编译
	virtual void writePro();//! 烧写
	virtual void clear(); //! 清理
	virtual void readStandardOutput();
	virtual void readStandardError();
	
};

#endif // UPLOADER_LINUX_H
