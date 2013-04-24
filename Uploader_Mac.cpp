#include "Uploader_Mac.h"

Uploader_Mac::Uploader_Mac(const QString &codePath, const QString &serial, int boardIndex, QObject *parent)
    : UploadBase(codePath, serial, boardIndex, parent)
{
}

Uploader_Mac::~Uploader_Mac()
{
}

