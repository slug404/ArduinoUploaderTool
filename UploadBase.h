#ifndef UPLOADBASE_H
#define UPLOADBASE_H

#include <QObject>

/*!
 * \brief The UploadBase class is baseclass for the uploader
 */
class UploadBase : public QObject
{
    Q_OBJECT
public:
    void start();

signals:

public slots:

protected:
    explicit UploadBase(QObject *parent = 0);
    virtual ~UploadBase();

protected:
};

#endif // UPLOADBASE_H
