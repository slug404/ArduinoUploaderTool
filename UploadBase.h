#ifndef UPLOADBASE_H
#define UPLOADBASE_H

class UploadBasePrivate;
#include <QObject>
#include "d_pointer.h"

/*!
 * \brief The UploadBase class is baseclass for the uploader
 */
class UploadBase : public QObject
{
	Q_OBJECT
	DPTR_DECLARE_PRIVATE(UploadBase)
public:
	explicit UploadBase(QObject *parent = 0);
	
signals:
	
public slots:

private:
	DPTR_DECLARE(UploadBase)
};

#endif // UPLOADBASE_H
