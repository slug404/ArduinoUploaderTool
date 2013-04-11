#ifndef UPLOADBASE_H
#define UPLOADBASE_H

#include <QObject>
#include <QSharedPointer>
#include <QProcess>
#include <QSet>
#include <QMap>

/**
 * @brief 库引用信息
 */
struct LibraryReferenceInfor
{
    QString libName; /**< 库的名字(aa.h) */
    QString libPath;/**< 库所在路径(arduino/libraries/aa) */
    QSet<QString> libReference;/**< 该库所包含的其他库 */
};

/*!
 * \brief Uploader的基类
 */
class UploadBase : public QObject
{
    Q_OBJECT
public:
    //扫描库相关
    void initLibrarysInfor(const QString libraryPath);
    QSet<QString> getReferenceLibrarysName(const QString filePath);
    QSet<QString> getAllMatchResults(const QString text, const QString regexp);

    //复制库相关
    bool copyDirectory(const QString &srcPath, const QString &desPath);
    bool copyFile(const QString &srcPath, const QString &desPath);

signals:

public slots:

protected:
    //fucntion
    explicit UploadBase(QObject *parent = 0);
    explicit UploadBase(const QString &serial, const QString &board, QObject *parent = 0);
    virtual ~UploadBase();
    //need c++11 support, and you can use QDISABLE_COPY(CLASS)
//    UploadBase & UploadBase(const UploadBase &) = delete;
//    UploadBase & operator = (const UploadBase &) = delete;
    Q_DISABLE_COPY(UploadBase)

    //interface
    virtual void setup() = 0; //! 准备
    virtual void compile() = 0; //! 编译
    virtual void writePro() = 0;//! 烧写
    virtual void clear() = 0; //! 清理

protected:
    //data
    QSharedPointer<QProcess> pExternalProcess_;//! 调用的外部程序指针
    QString serialPort_;//! 串口号
    QString boardType_;//! 板子类型
    QString compilerPath_;
    QString codePath_;
    QString cmd_;

    QMap<QString, LibraryReferenceInfor> map_libName_infor_;
};

#endif // UPLOADBASE_H
