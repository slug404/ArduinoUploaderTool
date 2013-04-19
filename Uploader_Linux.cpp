#include "Uploader_Linux.h"

Uploader_Linux::Uploader_Linux(const QString &codePath, const QString &serial, const QString &board, QObject *parent)
	: UploadBase(codePath, serial, board, parent)
{
	scanAllLibraryHeaderFile("./Arduino/libraries");
	scanAllheaderFile("./Arduino/libraries");
}

Uploader_Linux::~Uploader_Linux()
{
}

void Uploader_Linux::setup()
{
}

void Uploader_Linux::compile()
{
}

void Uploader_Linux::writePro()
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
