#include "Uploader_Windows.h"

Uploader_Windows::Uploader_Windows(const QString &codePath, const QString &serial, int boardIndex, QObject *parent)
    : UploadBase(codePath, serial, boardIndex, parent)
{

}

Uploader_Windows::~Uploader_Windows()
{
}
