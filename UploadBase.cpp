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
    , compiler_c("")
    , compiler_cplusplus("")
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
    , compiler_c("")
    , compiler_cplusplus("")
{
}

UploadBase::~UploadBase()
{
}

/**
 * @brief 遍历获取某个父目录下的所有子目录的路径
 * @param [in] parentDirPath 父目录路径
 * @return 所有子目录的路径
 */
QSet<QString> UploadBase::getAllChildDirPath(const QString &parentDirPath)
{
    QDir dir(parentDirPath);
    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

    QSet<QString> tmpDirs;
    if(dir.entryList().isEmpty())
    {
        QSet<QString>tmp;
        tmp << parentDirPath;
        return tmp;
    }
    else
    {
        foreach (const QString &dirName, dir.entryList())
        {
            QString childDirPath = parentDirPath + "/" + dirName;
            if(childDirPath.contains("examples")
                    || childDirPath.contains("Examples"))
            {
                continue;
            }
            tmpDirs << childDirPath;
            tmpDirs += getAllChildDirPath(childDirPath);
        }
    }

    return tmpDirs;
}

QString UploadBase::getCompilerCommand(const QString &filePath, const QString &cpuType, const QList<QString> &libPaths, const QList<QString> &childDirPath, QString workPath, QString workingFrequency)
{
    QFileInfo infor(filePath);
    QString suffix = infor.suffix();
    QString cmd;
    if("c" == suffix
            || "C" == suffix)
    {
        cmd = QString("%1/avr-gcc -c -g -Os -Wall -ffunction-sections -fdata-sections -mmcu=%2 -DF_CPU=%3L -MMD -DUSB_VID=null -DARDUINO=103 ").arg("C:/arduino/hardware/tools/avr/bin").arg(cpuType).arg(workingFrequency);
    }
    else if("cpp" == suffix
            || "CPP" == suffix)
    {
        cmd = QString("%1/avr-g++ -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=%2 -DF_CPU=%3L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=103 ").arg("C:/arduino/hardware/tools/avr/bin").arg(cpuType).arg(workingFrequency);
    }
    else
    {
        return QString();
    }

    cmd += "-IC:/arduino/hardware/arduino/cores/arduino ";
    cmd += "-IC:/arduino/hardware/arduino/variants/standard ";
    //加引用路径
    for(int i = 0; i != libPaths.size(); ++i)
    {
        cmd += QString("-I")+libPaths.at(i) + " ";
    }
    //加子目录路径
    for(int i = 0; i != childDirPath.size(); ++i)
    {
        cmd += QString("-I")+childDirPath.at(i) + " ";
    }
    //这里结构测试的时候再仔细考虑一下
    if(filePath.contains("/")
            || filePath.contains("\\"))
    {
        cmd += QString("-I") + QFileInfo(filePath).path() + " ";
    }
    cmd += QString(filePath + " -o "  +workPath + "/" + infor.fileName() + ".o");
    QFile file("cmd.txt");
    if(!file.open(QFile::Append))
    {
        qDebug() << "cmd.txt can't open!!";
    }
    file.write(cmd.toAscii());
    file.write("\n");
    file.close();
    return cmd;
}

/**
 * @brief UploadBase::linkerCommand
 * @param filePath
 * @param cpuType
 * @param staticLibraryPath
 * @param workPath
 * @param workingFrequency
 */
void UploadBase::linkerCommand(const QString &filePath, const QString &cpuType, const QString &staticLibraryPath, QString workPath, QString workingFrequency)
{

}

QString UploadBase::create_elf_fileCommand(const QString &filePath, const QString &cpuType, const QString &staticLibraryPath,  QString workPath, QString workingFrequency)
{
    QString t = QFileInfo(filePath).baseName();
    QString cmd = QString("%1 -Os -Wl,--gc-sections -mmcu=%2 -o %3 ").arg("C:/arduino/hardware/tools/avr/bin/avr-gcc").arg(cpuType).arg(workPath + "/" + t + ".elf");

    QDir dir(workPath);
    dir.setFilter(QDir::Files);
    QStringList tmp = dir.entryList();
    foreach (const QString &fileName, tmp)
    {
        if(fileName.contains(".cpp.o"))
        {
            cmd += workPath+"/" + fileName + " ";
        }
    }
    cmd += staticLibraryPath + " -LC:" + workPath + " -lm";

    return cmd;
}

QString UploadBase::create_eep_fileCommand(const QString &toolPath, const QString &elfPath, const QString &eepPath)
{
    QString cmd = QString("%1 -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 %2 %3").arg(toolPath).arg(elfPath).arg(eepPath);

    return cmd;
}

QString UploadBase::create_hex_fileCommand(const QString &toolPath, const QString &elfPath, const QString &hexPath)
{
    QString cmd = QString("%1 -O ihex -R .eeprom %2 %3").arg(toolPath).arg(elfPath).arg(hexPath);

    return cmd;
}

QString UploadBase::getUploadCommand(const QString &avrdudePath, const QString &configPath,
                                           const QString &cpuType, const QString &serialPort, const QString &baudrate, const QString &hexPath)
{
    QString cmd = QString("%1 -C%2 -v -v -v -v -p%3 -carduino -P%4 -b%5 -D -Uflash:w:%6:i").arg(avrdudePath).arg(configPath).arg(cpuType).arg(serialPort).arg(baudrate).arg(hexPath);

    return cmd;
}

/**
 * @brief 扔一个文件路径, 设置好它引用的库的目录路径以及库的文件路径
 * @param [in] filePath 文件路径
 * @param [out] libDirPath 引用库的目录路径
 * @param [out] libFilePath 引用库的文件路径
 */
void UploadBase::getLibraryPath(const QString &filePath, QList<QString> &libDirPath, QList<QString> &libFilePath, QList<QString> &childDirPath)
{
    QSet<QString> libFilePathSet;
    QSet<QString> libDirPathSet;
    getReferenceLibrarysInformation(filePath, libFilePathSet);

    libFilePath = libFilePathSet.toList();

    for(int i = 0; i != libFilePath.size(); ++i)
    {
        libDirPathSet += QFileInfo(libFilePath.at(i)).path();
        childDirPath += getAllChildDirPath(QFileInfo(libFilePath.at(i)).path()).toList();
    }

    libDirPath = libDirPathSet.toList();
}

void UploadBase::compileTest(const QString &filePath, const QString &cpuType, QString workPath, QString workingFrequency)
{
    QList<QString> tmplibDirPath;
    QList<QString> tmplibFilePath;
    QList<QString> tmplibChildDirPath;
    getLibraryPath(filePath, tmplibDirPath, tmplibFilePath, tmplibChildDirPath);

    //编译本文件
    QString cmd = getCompilerCommand(filePath, cpuType, tmplibDirPath, tmplibChildDirPath, workPath, workingFrequency);
    qDebug() << cmd;

    //递归编译其他引用
    if(!tmplibFilePath.isEmpty())
    {
        for(int i = 0; i != tmplibFilePath.size(); ++i)
        {
            QString filePath = tmplibFilePath.at(i);
            compileTest(filePath, cpuType);
        }
    }
    else
    {
        return;
    }
}

/**
 * @brief 递归编译指定库目录中的所有*c,*cpp
 * @param libraryDirPath 库目录路径
 */
void UploadBase::compileLibrary(const QString libraryDirPath)
{
    QDir dir(libraryDirPath);
    QStringList filters;
    filters << "*.cpp" << "*.CPP" << "*.cxx" << "*.cc" << "*.c" << "*.C";
    dir.setNameFilters(filters);

    foreach (const QString &fileName, dir.entryList())
    {
        QString filePath = libraryDirPath + "/" + fileName;
        QList<QString> tmplibDirPath;
        QList<QString> tmplibFilePath;
        QList<QString> tmplibChildDirPath;
        getLibraryPath(filePath, tmplibDirPath, tmplibFilePath, tmplibChildDirPath);

        //编译本文件
        QString cmd = getCompilerCommand(filePath, "atmega328p", tmplibDirPath, tmplibChildDirPath);
        qDebug() << cmd;
    }

    //递归遍历子目录
    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    QStringList dirNames = dir.entryList();
    if(dirNames.isEmpty())
    {
        return;
    }
    else
    {
        foreach (const QString &dirName, dirNames)
        {
            compileLibrary(libraryDirPath + "/" + dirName);
        }
    }
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

        for(int i = 0; i != list.size(); ++i)
        {
            if(map_libName_infor_.contains(list.at(i)))
            {
                libRefInforInfor = map_libName_infor_.value(list.at(i));

                if(map_libName_infor_.contains(list.at(i)))
                {
                    libPaths += libRefInforInfor.libPath;
                }
            }
            else if(QFileInfo(filePath).baseName() == QFileInfo(list.at(i)).baseName())
            {//处理cpp对应的.h 你妹的
                QString headerPath = QFileInfo(filePath).path() + "/" + QFileInfo(filePath).baseName() + ".h";
                QFile file(headerPath);
                if(!file.open(QFile::ReadOnly))
                {
                    qDebug() << "can't open file in path: " << headerPath;
                    file.close();
                    continue;
                }

                QString code = file.readAll();
                QSet<QString> libReference = getAllMatchResults(code, "\\w+\\.h");
                if(libReference.isEmpty())
                {
                    qDebug() << "reference library is empty";
                }
                else
                {
                    QList<QString> list = libReference.toList();

                    for(int i = 0; i != list.size(); ++i)
                    {
                        if(map_libName_infor_.contains(list.at(i)))
                        {
                            libRefInforInfor = map_libName_infor_.value(list.at(i));

                            if(map_libName_infor_.contains(list.at(i)))
                            {
                                libPaths += libRefInforInfor.libPath;
                            }
                        }
                    }
                }
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
void UploadBase::initLibraryReferenceInformation(const QString libraryPath)
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

        map_libName_infor_.insert(QString(dirName + ".h"), libReferInfor);
    }
}

