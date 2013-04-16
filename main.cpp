#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include "UploadBase.h"
#include "Uploader_Windows.h"

/*! the parameter number pass by command line*/
const int PARAMER_COUNT = 1+3;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Uploader_Windows test;
    test.initLibraryReferenceInformation("c:/arduino/libraries");

    //QSet<QString> libPaths;
    //LibraryReferenceInfor bb = test.getReferenceLibrarysInformation("./test.cpp", libPaths);
    //test.compileTest("./test.cpp", "atmega328p");
    //编译自己

    QList<QString> tmpLibDirPath;
    QList<QString> tmpLibFilePath;
    QList<QString> tmpLibChildDirPath;
    test.getLibraryPath("imu.cpp", tmpLibDirPath, tmpLibFilePath, tmpLibChildDirPath);
    if(QFile::exists("cmd.txt"))
    {
        QFile::remove("cmd.txt");
    }
    QString cmd = test.getCompilerCommand("imu.cpp", "atmega328p", tmpLibDirPath, tmpLibChildDirPath);

    {

        cmd =  test.create_elf_fileCommand("imu.cpp", "atmega328p", "C:/Temp/core.a");

        cmd = test.create_eep_fileCommand("C:/arduino/hardware/tools/avr/bin/avr-objcopy", QString("./Temp/" )+ "imu.elf", QString("./Temp/") + "imu.eep");

        cmd = test.create_hex_fileCommand("C:/arduino/hardware/tools/avr/bin/avr-objcopy", QString("./Temp/" ) + "imu.elf", QString("./Temp/") + "imu.hex");

        cmd = test.getUploadCommand("C:/arduino/hardware/tools/avr/bin/avrdude", "C:/arduino/hardware/tools/avr/etc/avrdude.conf"
                                          , "atmega328p", "COM6", "115200", "./Temp/imu.hex");
    }

    foreach (const QString &dirPath, tmpLibDirPath)
    {
        test.compileLibrary(dirPath);
    }

    if(PARAMER_COUNT == argc)
    {
        //accept
        QString fileName = argv[1];
        QString board = argv[2];
        QString serialPort = argv[3];
    }
    else
    {
        qDebug() << "error";
    }

    return a.exec();
}
