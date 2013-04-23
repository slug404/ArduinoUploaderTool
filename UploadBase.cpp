#include "UploadBase.h"
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QRegExp>
#include <QProcess>

/**
 * @brief 构造函数
 * @param [in] serial 串口号
 * @param [in] board 开发版型号
 * @param [in] sketch ?
 * @param [in] debug debug模式?
 * @param [in] parent
 */
UploadBase::UploadBase(const QString &codePath, const QString &serial, int boardIndex, QObject *parent)
    : QObject(parent)
    , pExternalProcess_(NULL)
    , serialPort_(serial)
    , boardIndex_(boardIndex)
    , compilerPath_("")
    , codePath_(codePath)
    , cmd_("")
    , hexPath_("")
{
    pExternalProcess_ = new QProcess(this);
    connect(pExternalProcess_, SIGNAL(readyReadStandardError()), this, SLOT(slotreadyReadStandardError()));
    connect(pExternalProcess_, SIGNAL(readyReadStandardOutput()), this, SLOT(slotReadyReadStandardOutput()));

    //init board information
    {
        map_boardIndex_infor_[0] = Board("Arduino Uno", "atmega328p", "standard", "115200","Uno.a", "16000000L", 32256);
        map_boardIndex_infor_[1] = Board("Arduino Duemilanove (328)", "atmega328p", "standard", "57600","Duemilanove328.a", "16000000L", 30720);
        map_boardIndex_infor_[2] = Board("Arduino Duemilanove (168)", "atmega168", "standard", "19200","Duemilanove168.a", "16000000L", 14336);
        map_boardIndex_infor_[3] = Board("Arduino Nano (328)", "atmega328p", "eightanaloginputs", "57600","Nano328.a", "16000000L", 30720);
        map_boardIndex_infor_[4] = Board("Arduino Nano (168)", "atmega168", "eightanaloginputs", "19200","Nano168.a", "16000000L", 14336);
        map_boardIndex_infor_[5] = Board("Arduino Mega 2560/ADK", "atmega2560", "mega", "115200", "Mega2560.a", "16000000L", 258048);
        map_boardIndex_infor_[6] = Board("Arduino Mega 1280", "atmega1280", "mega", "57600", "Mega1280.a", "16000000L", 126976);
        map_boardIndex_infor_[7] = Board("Arduino Leonardo", "atmega32u4", "leonardo", "57600","Leonardo.a", "16000000L", 28672);
        map_boardIndex_infor_[8] = Board("Arduino Esplora", "atmega32u4", "leonardo", "57600","Esplora.a", "16000000L", 28672);
        map_boardIndex_infor_[9] = Board("Arduino Micro", "atmega32u4", "micro", "57600","Micro.a", "16000000L", 28672);
        map_boardIndex_infor_[10] = Board("Arduino Mini (328)", "atmega328p", "eightanaloginputs", "115200","Mini328.a", "16000000L", 28672);
        map_boardIndex_infor_[11] = Board("Arduino Mini (168)", "atmega168", "eightanaloginputs", "19200","Mini168.a", "16000000L", 14336);
        //Ethernet
        map_boardIndex_infor_[12] = Board("Arduino Ethernet", "atmega328p", "standard", "115200", "Ethernet.a", "16000000L", 32256);
        //Fio
        map_boardIndex_infor_[13] = Board("Arduino Fio", "atmega328p", "eightanaloginputs", "57600","Fio.a", "8000000L", 30720);
        //BT
        map_boardIndex_infor_[14] = Board("Arduino BT (328)", "atmega328p", "eightanaloginputs", "19200","BT328.a", "16000000L", 28672);
        map_boardIndex_infor_[15] = Board("Arduino Esplora", "atmega168", "eightanaloginputs", "19200","BT168.a", "16000000L", 14336);
        //LilyPad
        map_boardIndex_infor_[16] = Board("LilyPad Arduino USB", "atmega32u4", "leonardo", "57600","LilyPadUsb.a", "8000000L", 28672);
        map_boardIndex_infor_[17] = Board("LilyPad Arduino (328)", "atmega328p", "standard", "57600","LilyPad328.a", "8000000L", 30720);
        map_boardIndex_infor_[18] = Board("LilyPad Arduino (168)", "atmega168", "standard", "19200","LilyPad168.a", "8000000L", 14336);
        //ProMini
        map_boardIndex_infor_[19] = Board("Arduino Pro Mini (328) 5V", "atmega328p", "standard", "57600", "ProMini328_5.a", "16000000L", 30720);
        map_boardIndex_infor_[20] = Board("Arduino Pro Mini (168) 5V", "atmega168", "standard", "19200", "ProMini168_5.a", "16000000L", 14336);
        map_boardIndex_infor_[21] = Board("Arduino Pro Mini (328) 3.3V", "atmega328p", "standard", "57600", "ProMini328_3.a", "8000000L", 30720);
        map_boardIndex_infor_[22] = Board("Arduino Pro Mini (168) 3.3V", "atmega168", "standard", "19200", "ProMini168_3.a", "8000000L", 14336);
        //NG
        map_boardIndex_infor_[23] = Board("Arduino Nano (328)", "atmega168", "standard", "19200","NG168.a", "16000000L", 14336);
        map_boardIndex_infor_[24] = Board("Arduino Nano (168)", "atmega8", "standard", "19200","NG8.a", "16000000L", 7168);
    }
}

UploadBase::~UploadBase()
{
}

void UploadBase::start()
{
    prepare();//prepare to compile
    compile();//compileLibrary, linkerCommand
    writePro();//getUploadCommand, and call QProcess to execute
    clear();
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

/**
 * @brief 获取调用avr-gcc的编译命令
 * @param [in] sketchPath 要编译的文件路径
 * @param [in] cpuType CPU的类型,参照Arduino官方
 * @param [in] libPaths 需要的Library的路径
 * @param [in] workPath 编译的工作路径
 * @param [in] workingFrequency 开发版的工作频率
 * @return
 */
QString UploadBase::getCompilerCommand(const QString &sketchPath, const QString &cpuType, const QList<QString> &libPaths, QString workPath, QString workingFrequency)
{
    QFileInfo infor(sketchPath);
    QString suffix = infor.suffix();
    QString cmd;
    if("c" == suffix
            || "C" == suffix)
    {
        cmd = QString("%1/avr-gcc -c -g -Os -Wall -ffunction-sections -fdata-sections -mmcu=%2 -DF_CPU=%3L -MMD -DUSB_VID=null -DARDUINO=103 ").arg("./Arduino/hardware/tools/avr/bin").arg(cpuType).arg(workingFrequency);
    }
    else if("cpp" == suffix
            || "CPP" == suffix)
    {
        cmd = QString("%1/avr-g++ -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=%2 -DF_CPU=%3L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=103 ").arg("./Arduino/hardware/tools/avr/bin").arg(cpuType).arg(workingFrequency);
    }
    else
    {
        return QString();
    }
#ifdef Q_OS_WIN32
    cmd += "-I./Arduino/hardware/arduino/cores/arduino ";
    cmd += "-I./Arduino/hardware/arduino/variants/standard ";
#else
    cmd += "-I./Arduino/hardware/arduino/cores/arduino ";
    cmd += "-I./Arduino/hardware/arduino/variants/standard ";
#endif
    //加引用路径
    for(int i = 0; i != libPaths.size(); ++i)
    {
        cmd += QString("-I")+libPaths.at(i) + " ";
    }
    //这里结构测试的时候再仔细考虑一下
    if(sketchPath.contains("/")
            || sketchPath.contains("\\"))
    {
        cmd += QString("-I") + QFileInfo(sketchPath).path() + " ";
    }
    cmd += QString(sketchPath + " -o "  +workPath + "/" + infor.fileName() + ".o");
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
    QString elfPath = filePath + ".elf";
    QString eepPath = filePath + ".eep";
    QString hexPath = filePath + ".hex";
    hexPath_ = hexPath;
    QString elf = create_elf_fileCommand(filePath, cpuType, staticLibraryPath, workPath, workingFrequency);
    //这部分到整合目录之后就可以统一了
    QString eep = create_eep_fileCommand("./Arduino/hardware/tools/avr/bin/avr-objcopy", elfPath, eepPath);
    QString hex = create_hex_fileCommand("./Arduino/hardware/tools/avr/bin/avr-objcopy",elfPath, hexPath);

    qDebug() << "elf: " << elf;
    qDebug() << "eep: " << eep;
    qDebug() << "hex: " << hex;

    //调用QProcess
    if(pExternalProcess_)
    {
        pExternalProcess_->execute(elf);//生成elf文件
        pExternalProcess_->execute(eep);//生成eep文件
        pExternalProcess_->execute(hex);//生成hex文件
    }
}

/**
 * @brief 获取创建elf文件的命令
 * @param [in] filePath 文件路径
 * @param [in] cpuType CPU类型,参考Arduino官方的类型设置
 * @param [in] staticLibraryPath 要链接的静态库路径, 这个静态库可以通过Arduino官方的IDE编译生成
 * @param [in] workPath 编译文件的临时目录
 * @param [in] workingFrequency 开发版的工作频率, 默认16Mhz目前不做使用
 * @return 可以扔给avr-gcc的命令
 */
QString UploadBase::create_elf_fileCommand(const QString &filePath, const QString &cpuType, const QString &staticLibraryPath, QString workPath, QString workingFrequency)
{
    QString baseName = QFileInfo(filePath).fileName();
    QString cmd = QString("%1 -Os -Wl,--gc-sections -mmcu=%2 -o %3 ").arg("./Arduino/hardware/tools/avr/bin/avr-gcc").arg(cpuType).arg(workPath + "/" + baseName + ".elf");

    QDir dir(workPath);
    dir.setFilter(QDir::Files);
    QStringList tmp = dir.entryList();
    foreach (const QString &fileName, tmp)
    {
        if(fileName.contains(".cpp.o")
                || fileName.contains(".c.o"))
        {
            cmd += workPath+"/" + fileName + " ";
        }
    }
    //cmd += staticLibraryPath + " -LC:" + workPath + " -lm";
    cmd += staticLibraryPath + " -L" + workPath + " -lm";

    return cmd;
}

/**
 * @brief 获取生成eep文件的命令
 * @param [in] toolPath avr-objcopy的路径
 * @param [in] elfPath elf文件路径(elf文件由create_elf_fileCommand函数生成)
 * @see [in] create_elf_fileCommand
 * @param [in] eepPath 要生成的eep文件路径
 * @return 可以扔给avr-gcc的命令
 */
QString UploadBase::create_eep_fileCommand(const QString &toolPath, const QString &elfPath, const QString &eepPath)
{
    QString cmd = QString("%1 -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 %2 %3").arg(toolPath).arg(elfPath).arg(eepPath);

    return cmd;
}

/**
 * @brief 获取生成hex文件的命令
 * @param [in] toolPath avr-objcopy的路径
 * @param [in] elfPath elf文件路径(elf文件由create_elf_fileCommand函数生成)
 * @see [in] create_elf_fileCommand
 * @param [in] hexPath 要生成的hex文件路径
 * @return 可以扔给avr-gcc的命令
 */
QString UploadBase::create_hex_fileCommand(const QString &toolPath, const QString &elfPath, const QString &hexPath)
{
    QString cmd = QString("%1 -O ihex -R .eeprom %2 %3").arg(toolPath).arg(elfPath).arg(hexPath);

    return cmd;
}

/**
 * @brief 获取上传命令
 * @param [in] avrdudePath avrdude的路径, 上传功能是依靠avrdude的
 * @param [in] configPath avrdudeconfig路径
 * @param [in] cpuType CPU类型
 * @param [in] serialPort 串口号
 * @param [in] baudrate 波特率
 * @param [in] hexPath 要上传的hex文件路径
 * @return 扔给avrdude的命令
 */
QString UploadBase::getUploadCommand(const QString &avrdudePath, const QString &configPath,
                                     const QString &cpuType, const QString &serialPort, const QString &baudrate, const QString &hexPath)
{
    QString cmd = QString("%1 -C%2 -v -v -v -v -p%3 -carduino -P%4 -b%5 -D -Uflash:w:%6:i").arg(avrdudePath).arg(configPath).arg(cpuType).arg(serialPort).arg(baudrate).arg(hexPath);

    return cmd;
}

/**
 * @brief 递归编译指定库目录中的所有*c,*cpp
 * @param [in] libraryDirPath 库目录路径
 */
void UploadBase::compileLibrary(const QString &libraryDirPath, const QString &mcu)
{
    QDir dir(libraryDirPath);
    QStringList filters;
    filters << "*.cpp" << "*.CPP" << "*.cxx" << "*.cc" << "*.c" << "*.C";
    dir.setNameFilters(filters);
    QStringList fileNames = dir.entryList();
    foreach (const QString &fileName, fileNames)
    {
        QString filePath = libraryDirPath + "/" + fileName;

        QSet<QString> headerFiles = getAllReferenceHeaderFileSet(filePath);
        QSet<QString> libPaths;
        foreach (const QString &headerFile, headerFiles)
        {
            if(map_headerFile_path_.contains(headerFile))
            {
                if(map_headerFile_path_.count(headerFile) > 1)
                {//处理有重复的情况, 有重复的情况下只处理子目录下的
                    QMultiMap<QString, QString>::iterator iter = map_headerFile_path_.find(headerFile);
                    while (iter != map_headerFile_path_.end()
                           && headerFile == iter.key())
                    {
                        QString childFilePath =iter.value();
                        QString parentPath = QFileInfo(filePath).path();
                        if(childFilePath.contains(parentPath))
                        {//子目录路径必定包含父级目录的路径
                            libPaths += parentPath;
                        }
                        ++iter;
                    }
                }
                else
                {
                    QString childFilePath = map_headerFile_path_.value(headerFile);
                    if(!childFilePath.isEmpty())
                    {
                        libPaths += childFilePath;
                    }
                }
            }
            else
            {//
                qDebug() << "ni mei de!!!!!!!!!!";
            }
        }

        //编译本文件
        QString cmd = getCompilerCommand(filePath, mcu, libPaths.toList());
        qDebug() << cmd;
        pExternalProcess_->execute(cmd);
        alreadyCompile_.clear();
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
            if(".settings" == dirName
                    || "examples" == dirName
                    || "doc" == dirName
                    || "Examples" == dirName
                    || ".svn" == dirName)
            {
                continue;
            }
            compileLibrary(libraryDirPath + "/" + dirName, mcu);
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

void UploadBase::prepare()
{
    QSet<QString> headerFiles = getReferenceHeaderFilesFromSingleFile(codePath_);
    QSet<QString> tmpLibDirPath;
    foreach (const QString header, headerFiles)
    {
        if(map_libName_infor_.contains(header))
        {
            tmpLibDirPath << map_libName_infor_.value(header).libPath;
        }
    }

    libraryPaths_ = tmpLibDirPath;
}

void UploadBase::compile()
{
    //这里的CPU类型是需要通过一个数据结构获取的
    if(map_boardIndex_infor_.contains(boardIndex_))
    {
        QString mcu = map_boardIndex_infor_[boardIndex_].mcu;
        QString cmd = getCompilerCommand(codePath_, mcu, libraryPaths_.toList());
        qDebug() << cmd;
        pExternalProcess_->execute(cmd);
        foreach (const QString &dirPath, libraryPaths_)
        {
            compileLibrary(dirPath, mcu);
        }

        linkerCommand(codePath_, mcu, "./Temp/core.a");
    }
    else
    {
        qDebug() << "can't find index: "<< boardIndex_;
        return;
    }
}

void UploadBase::writePro()
{
    if(map_boardIndex_infor_.contains(boardIndex_))
    {
        QString mcu = map_boardIndex_infor_[boardIndex_].mcu;
        QString baudrate = map_boardIndex_infor_[boardIndex_].baudrate;
#ifdef Q_OS_WIN32
        QString cmd = getUploadCommand("./Arduino/hardware/tools/avr/bin/avrdude", "./Arduino/hardware/tools/avr/etc/avrdude.conf", mcu, serialPort_, baudrate, hexPath_);
#elif defined(Q_OS_LINUX)
        QString cmd = getUploadCommand("./Arduino/hardware/tools/avrdude", "./Arduino/hardware/tools/avrdude.conf", mcu, QString("/dev/") + serialPort_, baudrate, hexPath_);
#elif defined(Q_OS_MAC)
        QString cmd = getUploadCommand("./Arduino/hardware/tools/avr/bin/avrdude", "./Arduino/hardware/tools/avr/etc/avrdude.conf", mcu, QString("/dev/") + serialPort_, baudrate, hexPath_);
#endif

        qDebug() << cmd;
        pExternalProcess_->execute(cmd);
    }
    else
    {
        qDebug() << "uploader error!";
    }
}

/**
 * @brief 根据传入的文件路径获递归遍历其所有include的文件名字的集合(包括库中包含其他库的所有情况)
 * @param filePath [in] 文件路径
 * @return include的文件名集合
 * @note 注意与getReferenceHeaderFilesFromSingleFile的区别,getReferenceHeaderFilesFromSingleFile是获取一个文本中的#include的文件名.
 * @see 函数 getReferenceHeaderFilesFromSingleFile
 */
QSet<QString> UploadBase::getAllReferenceHeaderFileSet(const QString &filePath)
{
    QSet<QString> libReference;
    libReference = getReferenceHeaderFilesFromSingleFile(filePath);

    foreach (const QString &fileName, libReference.toList())
    {//其实在这个循环之中就过滤了标准的库函数了
        if(map_libName_infor_.contains(fileName))
        {//如果是library
            LibraryReferenceInfor libRefInforInfor = map_libName_infor_.value(fileName);
            libReference += libRefInforInfor.libReference;
        }
        else if(QFileInfo(filePath).baseName() == QFileInfo(fileName).baseName())
        {//处理当前目录 因为.cpp与.h都会包含其他library
            QString tmpPath =filePath.left(filePath.indexOf(".")) + ".h";
            if(!alreadyCompile_.contains(tmpPath))
            {
                alreadyCompile_ << tmpPath;
                libReference += getAllReferenceHeaderFileSet(tmpPath);
            }
        }
        else if (map_headerFile_path_.contains(fileName))
        {
            if(map_headerFile_path_.count(fileName) > 1)
            {//处理有重复的情况, 有重复的情况下只处理子目录下的
                QMultiMap<QString, QString>::iterator iter = map_headerFile_path_.find(fileName);
                while (iter != map_headerFile_path_.end()
                       && fileName == iter.key())
                {
                    QString childFilePath =iter.value() + "/" + fileName;
                    QString parentPath = QFileInfo(filePath).path();
                    if(childFilePath.contains(parentPath))
                    {//子目录路径必定包含父级目录的路径
                        if(!alreadyCompile_.contains(childFilePath))
                        {
                            alreadyCompile_ << childFilePath;
                            libReference += getAllReferenceHeaderFileSet(childFilePath);
                        }
                    }
                    ++iter;
                }
            }
            else
            {
                QString childFilePath = map_headerFile_path_.value(fileName) + "/" + fileName;
                if(!alreadyCompile_.contains(childFilePath))
                {
                    alreadyCompile_ << childFilePath;
                    libReference += getAllReferenceHeaderFileSet(childFilePath );
                }
            }
        }
    }

    libReference.remove("Arduino.h");
    return libReference;
}

/**
 * @brief 给定一个文件路径获取其头文件集合
 * @param [in] filePath 文件路径
 * @return 匹配的头文件集合
 * @note 仅处理单一文件, 而getAllReferenceHeaderFileSet则会递归调用
 */
QSet<QString> UploadBase::getReferenceHeaderFilesFromSingleFile(const QString &filePath)
{
    QFile file(filePath);
    {
        if(QFileInfo(filePath).baseName() == "twi.c")
        {
            qDebug() << "twi.c";
        }
    }
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << "getHeaderFiles file can't open " << filePath;
        qDebug() << file.errorString();
        file.close();
        return QSet<QString>();
    }

    QString text = file.readAll();
    QSet<QString> headerFiles = getAllMatchResults(text);
    headerFiles.remove("Arduino.h");

    file.close();

    return headerFiles;
}

/**
 * @brief 获取所有的匹配结果
 * @param text 要匹配的文本
 * @param regexp 正则表达式串
 * @return 匹配的结果集
 * @note 目前的正则表达式仅仅匹配*.h,还不太严谨,容易匹配出注释中形如 *.h*的字符串. 不过这是无害的,这些无效的匹配最终会被过滤.
 * @note 主要在getReferenceHeaderFilesFromText函数中使用
 * @see 函数::getReferenceHeaderFilesFromText
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

    resultSet.remove("Arduino.h");
    return resultSet;
}

/**
 * @brief 扫描Library目录下的头文件极其依赖信息
 * @param libraryPath [in] 路径, 需要指定到Arduino的libraries所在路径
 */
void UploadBase::scanAllLibraryHeaderFile(const QString &libraryPath)
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
        libReferInfor.libPath = libraryPath + "/" + dirName;
        libReferInfor.libReference = library;

        map_libName_infor_.insert(QString(dirName + ".h"), libReferInfor);
    }
}

/**
 * @brief 扫描指定目录中所有的头文件及其路径(包含其子目录)
 * @param [in] path 要扫描的路径(librarys目录)
 */
void UploadBase::scanAllheaderFile(const QString &path)
{
    //处理文件
    {
        QDir dir(path);
        QStringList filter;
        filter << "*.h";
        dir.setNameFilters(filter);
        foreach (const QString &fileName, dir.entryList())
        {
            QString filePath = path + "/" + fileName;
            map_headerFile_path_.insert(fileName, path);
        }
    }
    //处理子目录
    {
        QDir dir(path);
        dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
        foreach (const QString &dirName, dir.entryList())
        {
            QString dirPath = path + "/" + dirName;
            scanAllheaderFile(dirPath);
        }
    }
}

void UploadBase::slotReadyReadStandardOutput()
{
    readStandardOutput();
}

void UploadBase::slotreadyReadStandardError()
{
    readStandardError();
}
