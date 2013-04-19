#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMap>
#include "Uploader_Windows.h"
#include "Uploader_Linux.h"
#include "Uploader_Mac.h"
#include "UploadFactory.h"

/*! the parameter number pass by command line*/
const int PARAMER_COUNT = 1+3;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	if(QFile::exists("cmd.txt"))
	{
		QFile::remove("cmd.txt");
	}

	if(PARAMER_COUNT == argc)
	{
		//accept
		QString fileName = argv[1];
		QString board = argv[2];
		QString serialPort = argv[3];

//		QSet<QString> headerFiles = test.getReferenceHeaderFilesFromSingleFile("imu.cpp");
//		QSet<QString> tmpLibDirPath;
//		foreach (const QString header, headerFiles)
//		{
//			if(test.map_libName_infor_.contains(header))
//			{
//				tmpLibDirPath << test.map_libName_infor_.value(header).libPath;
//			}
//		}

//		QString cmd = test.getCompilerCommand("imu.cpp", "atmega328p", tmpLibDirPath.toList());
//		foreach (const QString &dirPath, tmpLibDirPath)
//		{
//			test.compileLibrary(dirPath);
//		}

		{
			//这里要清理
//#ifdef Q_OS_WIN32
//			cmd =  test.create_elf_fileCommand("imu.cpp", "atmega328p", "C:/Temp/core.a");

//			cmd = test.create_eep_fileCommand("C:/arduino/hardware/tools/avr/bin/avr-objcopy", QString("./Temp/" )+ "imu.elf", QString("./Temp/") + "imu.eep");

//			cmd = test.create_hex_fileCommand("C:/arduino/hardware/tools/avr/bin/avr-objcopy", QString("./Temp/" ) + "imu.elf", QString("./Temp/") + "imu.hex");

//			cmd = test.getUploadCommand("C:/arduino/hardware/tools/avr/bin/avrdude", "C:/arduino/hardware/tools/avr/etc/avrdude.conf"
//										, "atmega328p", "COM6", "115200", "./Temp/imu.hex");
//#else
//			cmd =  test.create_elf_fileCommand("imu.cpp", "atmega328p", "./Temp/core.a");

//			cmd = test.create_eep_fileCommand("./Arduino/hardware/tools/avr/bin/avr-objcopy", QString("./Temp/" )+ "imu.elf", QString("./Temp/") + "imu.eep");

//			cmd = test.create_hex_fileCommand("./Arduino/hardware/tools/avr/bin/avr-objcopy", QString("./Temp/" ) + "imu.elf", QString("./Temp/") + "imu.hex");

//			cmd = test.getUploadCommand("./Arduino/hardware/tools/avr/bin/avrdude", "C:/arduino/hardware/tools/avr/etc/avrdude.conf"
//										, "atmega328p", "tty.usbmodem1d1321", "115200", "./Temp/imu.hex");
//#endif
		}

	}
	else
	{
		qDebug() << "error";
	}

	return a.exec();
}
