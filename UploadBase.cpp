#include "UploadBase.h"
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QRegExp>

UploadBase::UploadBase(QObject *parent)
    : QObject(parent)
    , pExternalProcess_(NULL)
    , serialPort_("")
    , boardType_("")
    , compilerPath_("")
    , codePath_("")
    , cmd_("")
{
}

/**
 * @brief 构造函数
 * @param [in] serial 串口号
 * @param [in] board 开发版型号
 * @param [in] sketch ?
 * @param [in] debug debug模式?
 * @param [in] parent
 */
UploadBase::UploadBase(const QString &serial, const QString &board, QObject *parent)
    : QObject(parent)
    , pExternalProcess_(NULL)
    , serialPort_(serial)
    , boardType_(board)
    , compilerPath_("")
    , codePath_("")
    , cmd_("")
{

}

UploadBase::~UploadBase()
{
}

/**
 * @brief 递归复制复制目录
 * @param srcPath [in] 源目录
 * @param desPath [in] 目标目录
 * @return 返回执行结果
 * @retval true 执行成功
 * @retval false 执行失败
 */
void UploadBase::copyDirectory(const QString &srcPath, const QString &desPath)
{
    QDir dir(srcPath);
    {
        dir.setFilter(QDir::Files);
        foreach (const QString &fileName, dir.entryList())
        {
            QString srcFilePath = srcPath + "/" + fileName;
            QString desFilePath = srcPath + "/" + fileName;
            if(!copyFile(srcFilePath, desFilePath))
            {
                qDebug() << "copy file error! from " << srcFilePath << "to " << desFilePath;
            }
        }
    }
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList dirNames = dir.entryList();
    if(dirNames.isEmpty())
    {
        return;
    }
    else
    {
        foreach (const QString &dirName, dirNames)
        {
            QString newSrcPath = srcPath + "/" + dirName;
            QString newDesPath = desPath + "/" + dirName;
            copyDirectory(newSrcPath, newDesPath);
        }
    }
}

/**
 * @brief 复制文件,从srcPath到desPath
 * @param srcPath [in] 文件原路径
 * @param desPath [in] 文件目标路径
 * @retval true 执行成功
 * @retval false 执行失败
 * @pre 目的路径要存在(mkpath)
 */
bool UploadBase::copyFile(const QString &srcPath, const QString &desPath)
{
    //读数据
    QFile file(srcPath);
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << "open src file fail" << srcPath;
        return false;
    }

    QByteArray bytes =  file.readAll();
    file.close();

    ////////////////////////////////
    //在这里要确保path可用
    {
        QDir pathTest("./");
        QString oldPath = pathTest.currentPath();

        pathTest.setPath(desPath);
        if(!pathTest.exists())
        {
            qDebug() << "not exists the path, and it will try to cerate it:" << desPath;
            if(!pathTest.mkpath(desPath))
            {
                qDebug() << "create path fail!!!!";
                return false;
            }
        }
        pathTest.setPath(oldPath);
    }
    ////////////////////////////////

    QFile desFile(desPath);
    if(!desFile.open(QFile::WriteOnly))
    {
        qDebug() << "open des file fail " << desPath;
        return false;
    }

    desFile.write(bytes);
    desFile.waitForBytesWritten(10);
    desFile.close();

    return true;
}

/**
 * @brief 根据传入的文件路径获取该文件所使用到的库所有库的名字(包括库中包含其他库的所有情况)
 * @param filePath [in] 文件路径
 * @param libPaths [out] 库路径
 * @return 文件引用信息
 * - -1 文件名
 * - -2 文件路径
 * - -3 包含的所有头文件的路径
 */
LibraryReferenceInfor UploadBase::getReferenceLibrarysInformation(const QString filePath, QSet<QString> &libPaths)
{
    LibraryReferenceInfor libRefInforInfor;
    QSet<QString> libReference;
    QFile file(filePath);
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << file.errorString();
        file.close();
        return libRefInforInfor;
    }

    QString code = file.readAll();

    libReference = getAllMatchResults(code, "\\w+\\.h");
    if(libReference.isEmpty())
    {
        qDebug() << "reference library is empty";
    }
    else
    {
        QList<QString> list = libReference.toList();
        qDebug() << map_libName_infor_.keys();
        for(int i = 0; i != list.size(); ++i)
        {
            if(map_libName_infor_.contains(list.at(i)))
            {
                libRefInforInfor.libReference += map_libName_infor_.value(list.at(i)).libReference;
                QFileInfo infor(map_libName_infor_.value(list.at(i)).libPath);
                libPaths += infor.path();
            }
        }
    }

    file.close();
    libRefInforInfor.libName = filePath.right(filePath.size() - filePath.lastIndexOf("/") - 1);
    libRefInforInfor.libPath = filePath;

    return libRefInforInfor;
}

/**
 * @brief 获取所有的匹配结果
 * @param text 要匹配的文本
 ** @param regexp 正则表达式串
 * @return 匹配的结果集
 */
QSet<QString> UploadBase::getAllMatchResults(const QString text, const QString regexp)
{
    QSet<QString> resultSet;

    QRegExp rx(regexp);
    int pos = 0;

    while ((pos = rx.indexIn(text, pos)) != -1)
    {
        pos += rx.matchedLength();
        QString result = rx.cap(0);
        resultSet << result;
    }

    return resultSet;
}

/**
 * @brief 初始化给定的路径中的库的引用信息
 * @param libraryPath [in] 路径, 需要指定到Arduino的libraries所在路径
 */
void UploadBase::initLibrarysInfor(const QString libraryPath)
{
    QDir dir(libraryPath);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot); // filter . and ..

    foreach (const QString dirName, dir.entryList())
    {
        QString dirPath = libraryPath + "/" + dirName + "/" + dirName + ".h";
        QFile file(dirPath);

        if(!file.open(QFile::ReadOnly))
        {
            qDebug() << "can't open file in path: " << dirPath;
            file.close();
            continue;
        }

        QString code = file.readAll();
        QSet<QString> library = getAllMatchResults(code, "\\w+\\.h");

        if(library.isEmpty())
        {
            qDebug() << tr("file: %1").arg(dirName) <<"not have include other library";
            file.close();
            continue;
        }

        LibraryReferenceInfor libReferInfor;
        libReferInfor.libName = dirName;
        libReferInfor.libPath = dirPath;
        libReferInfor.libReference = library;

        map_libName_infor_[dirName + ".h"] = libReferInfor;
    }
}

