#include "Uploader_Mac.h"

Uploader_Mac::Uploader_Mac(QObject *parent)
	: UploadBase(parent)
{
}


Uploader_Mac::Uploader_Mac(const QString &codePath, const QString &serial, const QString &board, QObject *parent)
	: UploadBase(codePath, serial, board, parent)
{
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
