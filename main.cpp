#include <QCoreApplication>
#include <QDebug>
#include "UploadBase.h"
#include "Uploader_Windows.h"

/*! the parameter number pass by command line*/
const int PARAMER_COUNT = 1+3;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Uploader_Windows test;
    test.initLibrarysInfor("c:/arduino/libraries");

    //QSet<QString> libPaths;
    //LibraryReferenceInfor bb = test.getReferenceLibrarysInformation("./test.cpp", libPaths);
    //test.compileTest("./test.cpp", "atmega328p");
    //编译自己

    QList<QString> tmpLibDirPath;
    QList<QString> tmpLibFilePath;
    test.getLibraryPath("CardInfo.cpp", tmpLibDirPath, tmpLibFilePath);
     QString cmd = test.getCompilerCommand("CardInfo.cpp", "atmega328p", tmpLibDirPath);


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
