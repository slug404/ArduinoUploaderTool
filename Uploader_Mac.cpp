#include "Uploader_Mac.h"

Uploader_Mac::Uploader_Mac(const QString &codePath, const QString &serial, int boardIndex, QObject *parent)
	: UploadBase(codePath, serial, boardIndex, parent)
{
	scanAllLibraryHeaderFile("./Arduino/libraries");
	scanAllheaderFile("./Arduino/libraries");
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

void Uploader_Mac::readStandardError()
{
}

void Uploader_Mac::readStandardOutput()
{
}
