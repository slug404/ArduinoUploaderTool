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
    void initLibraryReferenceInformation(const QString libraryPath);
    //在这里得到所有的依赖
    LibraryReferenceInfor getReferenceLibrarysInformation(const QString filePath, QSet<QString> &libPaths);
    QSet<QString> getHeaderFiles(const QString &filePath);
    QSet<QString> getAllMatchResults(const QString text, const QString regexp = "\\w+\\.h");

    //复制库相关
    void copyDirectory(const QString &srcPath, const QString &desPath);
    bool copyFile(const QString &srcPath, const QString &desPath);

    void compileTest(const QString &filePath, const QString &cpuType, QString workPath = "./Temp", QString workingFrequency = "16000000");

    //递归编译指定目录以及其子目录中所有*c,*cpp
    void compileLibrary(const QString libraryDirPath);

    void getLibraryPath(const QString &filePath, QList<QString> &libDirPath, QList<QString> &libFilePath, QList<QString> &childDirPath);

    //给QProcess调用
    //编译
    QString getCompilerCommand(const QString &filePath, const QString &cpuType, const QList<QString> &libPaths, const QList<QString> &childDirPath, QString workPath = "./Temp", QString workingFrequency = "16000000");
    //链接
    void linkerCommand(const QString &filePath, const QString &cpuType, const QString &staticLibraryPath, QString workPath = "./Temp", QString workingFrequency = "16000000");
    QString create_elf_fileCommand(const QString &filePath, const QString &cpuType, const QString &staticLibraryPath, QString workPath = "./Temp", QString workingFrequency = "16000000");
    QString create_eep_fileCommand(const QString &toolPath, const QString &elfPath, const QString &eepPath);
    QString create_hex_fileCommand(const QString &toolPath, const QString &elfPath, const QString &hexPath);

    QString getUploadCommand(const QString &avrdudePath, const QString &configPath, const QString &cpuType, const QString &serialPort, const QString &baudrate, const QString &hexPath);
    //上传


signals:

public slots:

protected:
    //fucntion
    explicit UploadBase(QObject *parent = 0);
    explicit UploadBase(const QString &serial, const QString &board, QObject *parent = 0);
    virtual ~UploadBase();

    QSet<QString> getAllChildDirPath(const QString &parentDirPath);

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

    QMultiMap<QString, LibraryReferenceInfor> map_libName_infor_;
    QString compiler_c;
    QString compiler_cplusplus;
};

#endif // UPLOADBASE_H
