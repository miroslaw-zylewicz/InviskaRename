#ifndef IComMetaExif_h
#define IComMetaExif_h

#include <QString>
#include "libexif/exif-data.h"


class IComMetaExif
{
public:
    // LibExif data structure for storing Exif Data
    ExifData*                   m_pexdExifData;

    // Buffer for reading values into
    char                        m_rgcBuffer[256];

public:
    IComMetaExif(const QString & krqstrFilePath);
    ~IComMetaExif();

    // Returns the tag associated with the passed ID
    const char* GetDescription(const ExifIfd kexidID, const ExifTag kextaTag);

    // Sets passed string to value for specified ID and Tag
    void GetValue(QString & rqstrValue, const ExifIfd kexidID, const ExifTag kextaTag);

    // Returns the value for the specified ID and Tag
    QString GetValue(const ExifIfd kexidID, const ExifTag kextaTag);

private:
    // Remove spaces on the right of the string
    void TrimSpacesFromEnd(char* rgcBuffer);

public:
    // Returns true if this file extension can contain Exif inforamtion
    static bool FileCanContainExif(const QString & kqstrExtension);

    // Indicates if Exif data is pressent in file passed at construction
    bool ExifDataPresent()          {return m_pexdExifData != nullptr;}
};

#endif // IComMetaExif_h
