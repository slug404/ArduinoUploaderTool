#include "Uploader_Mac.h"

Uploader_Mac::Uploader_Mac(const QString &codePath, const QString &serial, const QString &board, QObject *parent)
	: UploadBase(codePath, serial, board, parent)
{
	scanAllLibraryHeaderFile("./Arduino/libraries");
	scanAllheaderFile("./Arduino/libraries");

	QSet<QString> headerFiles = getReferenceHeaderFilesFromSingleFile(codePath);
	QSet<QString> tmpLibDirPath;
	foreach (const QString header, headerFiles)
	{
		if(map_libName_infor_.contains(header))
		{
			tmpLibDirPath << map_libName_infor_.value(header).libPath;
		}
	}

	//这里的CPU类型是需要通过一个数据结构获取的
	QString cmd = getCompilerCommand(codePath, "atmega328p", tmpLibDirPath.toList());
	foreach (const QString &dirPath, tmpLibDirPath)
	{
		compileLibrary(dirPath);
	}
}

Uploader_Mac::~Uploader_Mac()
{
}

void Uploader_Mac::clear()
{
}

void Uploader_Mac::writePro()
{
}

void Uploader_Mac::compile()
{
}

void Uploader_Mac::setup()
{
}

void Uploader_Mac::readStandardError()
{
}

void Uploader_Mac::readStandardOutput()
{
}
