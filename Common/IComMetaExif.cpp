#include <QFile>
#include "IComMetaExif.h"

// https://libexif.github.io/


IComMetaExif::IComMetaExif(const QString & krqstrFilePath)
{
    m_pexdExifData = nullptr;

    #ifdef Q_OS_WIN
    // libExif opens files with fopen() so it won't work with Unicode file paths on Windows
    // We therefore have to open the file with QFile and create the Exif Data with exif_data_new_from_data()
    QFile qfiImageFile(krqstrFilePath);
    if (!qfiImageFile.open(QIODevice::ReadOnly))
        return;

    const QByteArray qbaImageFileData = qfiImageFile.readAll();
    m_pexdExifData = exif_data_new_from_data(reinterpret_cast<const unsigned char*>(qbaImageFileData.constData()), static_cast<unsigned int>(qbaImageFileData.size()));

    // Using this approach libexif always returns an ExifData object even if there's no Exif data present in the file
    // The camera make seems to always be encoded, so if that's empty we'll assume there's no data
    if (m_pexdExifData != nullptr)
    {
        ExifEntry* exenEntry = exif_content_get_entry(m_pexdExifData->ifd[EXIF_IFD_0], EXIF_TAG_MAKE);
        if (exenEntry == nullptr)
        {
            exif_data_free(m_pexdExifData);
            m_pexdExifData = nullptr;
        }
    }
    #else
    m_pexdExifData = exif_data_new_from_file(krqstrFilePath.toLocal8Bit().constData());
    #endif
}


IComMetaExif::~IComMetaExif()
{
    if (m_pexdExifData != nullptr)
        exif_data_unref(m_pexdExifData);
}


const char* IComMetaExif::GetDescription(const ExifIfd kexidID, const ExifTag kextaTag)
{
    return exif_tag_get_name_in_ifd(kextaTag, kexidID);
}


void IComMetaExif::GetValue(QString & rqstrValue, const ExifIfd kexidID, const ExifTag kextaTag)
{   
    ExifEntry* exenEntry = exif_content_get_entry(m_pexdExifData->ifd[kexidID], kextaTag);
    if (exenEntry != nullptr)
    {
        exif_entry_get_value(exenEntry, m_rgcBuffer, sizeof(m_rgcBuffer));
        TrimSpacesFromEnd(m_rgcBuffer);
        if (*m_rgcBuffer)
            rqstrValue = m_rgcBuffer;
        else
            rqstrValue.clear();
    }
    else
    {
        rqstrValue.clear();
    }
}


QString IComMetaExif::GetValue(const ExifIfd kexidID, const ExifTag kextaTag)
{
    ExifEntry* exenEntry = exif_content_get_entry(m_pexdExifData->ifd[kexidID], kextaTag);
    if (exenEntry != nullptr)
    {
        exif_entry_get_value(exenEntry, m_rgcBuffer, sizeof(m_rgcBuffer));
        TrimSpacesFromEnd(m_rgcBuffer);
        if (*m_rgcBuffer)
            return QString(m_rgcBuffer);
        else
            return QString();
    }
    else
    {
        return QString();
    }
}


void IComMetaExif::TrimSpacesFromEnd(char* rgcBuffer)
{
    char *pchEnd = rgcBuffer-1;
    while (*rgcBuffer)
    {
        if (*rgcBuffer != ' ')
            pchEnd = rgcBuffer;
        ++rgcBuffer;
    }
    *++pchEnd = '\0';
}


bool IComMetaExif::FileCanContainExif(const QString & kqstrExtension)
{
    QString qstrExtension = kqstrExtension.toLower();
    if (qstrExtension == "jpg" || qstrExtension == "jpeg")
        return true;
    return false;
}
