#include "Uploader_Windows.h"

Uploader_Windows::Uploader_Windows(const QString &codePath, const QString &serial, const QString &board, QObject *parent)
	: UploadBase(codePath, serial, board, parent)
{
	scanAllLibraryHeaderFile("c:/arduino/libraries");
	scanAllheaderFile("c:/arduino/libraries");
}

Uploader_Windows::~Uploader_Windows()
{
}

void Uploader_Windows::setup()
{
}

void Uploader_Windows::compile()
{
}

void Uploader_Windows::writePro()
{
}

void Uploader_Windows::clear()
{
}

void Uploader_Windows::readStandardOutput()
{
}

void Uploader_Windows::readStandardError()
{
}
