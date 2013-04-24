#include "Uploader_Linux.h"

Uploader_Linux::Uploader_Linux(const QString &codePath, const QString &serial, int boardIndex, QObject *parent)
    : UploadBase(codePath, serial, boardIndex, parent)
{
}

Uploader_Linux::~Uploader_Linux()
{
}
