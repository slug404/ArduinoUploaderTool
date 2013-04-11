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
bool UploadBase::copyDirectory(const QString &srcPath, const QString &desPath)
{

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

}

/**
 * @brief 根据传入的文件路径获取该文件所使用到的库所有库的名字(包括库中包含其他库的所有情况)
 * @param filePath [in] 文件路径
 * @return 所有文件的路径
 * @note 返回值需要判断isEmpty
 */
QSet<QString> UploadBase::getReferenceLibrarysName(const QString filePath)
{
    QSet<QString> libReference = findReferenceLibraryName(filePath);
    if(libReference.isEmpty())
    {
        qDebug() << "reference library is empty";
    }
    else
    {
        QList<QString> list = libReference.toList();
        for(int i = 0; i != list.size(); ++i)
        {
            if(list.at(i))
            {
                //libReference << ;
            }
        }
    }
}

/**
 * @brief UploadBase::findReferenceLibraryName
 * @param filePath
 * @return
 */
QSet<QString> UploadBase::findReferenceLibraryName(const QString filePath)
{
    QSet<QString> setNames;

    QFile file(filePath);
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << file.errorString();
        return setNames;// return a empty object
    }

    QString code = file.readAll();
    QRegExp rx("\\w+\\.h");

    int count = 0;
    int pos = 0;

    while ((pos = rx.indexIn(code, pos)) != -1)
    {
        ++count;
        pos += rx.matchedLength();
        QString result = rx.cap(0);
        setNames << result;
    }

    file.close();

    return setNames;
}

/**
 * @brief 初始化给定的路径中的库的引用信息
 * @param libraryPath [in] 路径
 */
void UploadBase::initLibrarysInfor(const QString libraryPath)
{

}

