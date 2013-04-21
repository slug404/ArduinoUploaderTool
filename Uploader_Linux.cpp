#include "Uploader_Linux.h"

Uploader_Linux::Uploader_Linux(const QString &codePath, const QString &serial, int boardIndex, QObject *parent)
	: UploadBase(codePath, serial, boardIndex, parent)
{
	scanAllLibraryHeaderFile("./Arduino/libraries");
	scanAllheaderFile("./Arduino/libraries");
}

Uploader_Linux::~Uploader_Linux()
{
}

void Uploader_Linux::clear()
{
}

void Uploader_Linux::readStandardOutput()
{
}

void Uploader_Linux::readStandardError()
{
}
