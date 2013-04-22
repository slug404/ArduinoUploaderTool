#ifndef UPLOADER_WINDOWS_H
#define UPLOADER_WINDOWS_H

#include "UploadBase.h"
/*!
 * \brief The Uploader_Windows class
 */
class Uploader_Windows : public UploadBase
{
    Q_OBJECT
public:
	explicit Uploader_Windows(const QString &codePath, const QString &serial, int boardIndex, QObject *parent = 0);
	virtual ~Uploader_Windows();
    //interface
	virtual void writePro();//! 烧写
    virtual void clear(); //! 清理
	virtual void readStandardOutput();
	virtual void readStandardError();

signals:

public slots:

};

#endif // UPLOADER_WINDOWS_H
