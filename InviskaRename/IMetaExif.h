#ifndef IMetaExif_h
#define IMetaExif_h

#include <QObject>
#include <QHash>
#include "IMetaBase.h"
#include "IComMetaExif.h"
class IRenameInvalidCharSub;


class IMetaExif : public IMetaBase
{
public:
    // Index in array at which each tag value is stored
    enum ExifTagsIDs                   {DateTime, Date, Time, DateYYYY, DateYY, DateMM, DateDD, TimeHH, TimeMM, TimeSS, TimeSubSec,
                                        CameraMake, CameraModel, FNumber, ISOSpeed, ExposureTime, ExposureTimeDec, FocalLength, FocalLengthIn35mm, Program, PixelDimX, PixelDimY,
                                        NumTagsBasic,

                                        Software = NumTagsBasic,
                                        DigitalZoomRatio, Flash, Orientation,
                                        ShutterSpeedValue, ExposureBiasValue, BrightnessValue, ApertureValue, MaxApertureValue, SceneCaptureType, CustomRendered, ExposureMode, WhiteBalance, GainControl,
                                        Contrast, Saturation, Sharpness, MeteringMode, LightSource, FocalPlaneXResolution, FocalPlaneYResolution, FocalPlaneResolutionUnit, SubjectDistanceRange, SensingMethod, SceneType,
                                        ColorSpace, Compression, ComponentsConfiguration, CompressedBitsPerPixel, YCbCrPositioning, XResolution, YResolution, ResolutionUnit, FileSource,
                                        ExifVersion, FlashPixVersion, InteroperabilityIndex, InteroperabilityVersion,
                                        Title, Subject, Author, Comment, Keywords, Copyright,
                                        GPSLatitude, GPSLatitudeRef, GPSLongitude, GPSLongitudeRef, GPSAltitude, GPSAltitudeRef, GPSDateStamp, GPSTimeStamp,
                                        NumTagsAdvanced};

private:
    // Hash contining valid tag strings and their associated enum value
    static QHash<QString, int>          m_qhashTagLookup;

public:
    IMetaExif();
    IMetaExif(IComMetaExif* pmexExifMeta, const IRenameInvalidCharSub & kricsInvalidCharSub, const bool kbAdvancedMode);

    // Initialised the tag lookup hash with valid tags and associated tag IDs
    static void InitTagLookupHashBasic();
    static void InitTagLookupHashAdvanced();

    // Read tags from passed ExifMeta object and store in string array
    void ReadTagsBasic(IComMetaExif* pmexExifMeta, const IRenameInvalidCharSub & kricsInvalidCharSub);
    void ReadTagsAdvanced(IComMetaExif* pmexExifMeta, const IRenameInvalidCharSub & kricsInvalidCharSub);

    // Returns tag ID from MusicTagsIDs enum or ITagInfo::Invalid if passed tag string is invalid
    static int GetTagID(const QString & krqstrTagCode);

private:
    // Gets Exif value associated with ID and Tag and performs invalid character substitution
    QString GetValueInvalidCharSub(const ExifIfd kexidID, const ExifTag kextaTag,  IComMetaExif* pmexExifMeta, const IRenameInvalidCharSub & kricsInvalidCharSub);

    // Gets the focal length without the mm unit and also strip s off .0 which
    QString GetValueReplaceSlash(const ExifIfd kexidID, const ExifTag kextaTag, IComMetaExif* pmexExifMeta);

    // Gets the date/time value with seperators substituted for valid characters
    QString GetDateTimeSeparatorSub(IComMetaExif* pmexExifMeta);

    // Returns GPS Date and Time with seperators substituted
    QString GetGPSDateSeparatorSub(IComMetaExif* pmexExifMeta);
    QString GetGPSTimeSeparatorSub(IComMetaExif* pmexExifMeta);

    // TimeSubSec padded/truncated to three digits
    QString GetTimeSubSec3Digit(IComMetaExif* pmexExifMeta);

    // Gets the f number without the f/ at the start
    QString GetFNumberWithoutUnit(IComMetaExif* pmexExifMeta);

    // Gets the focal length without the mm unit and also strip s off .0 which
    QString GetFocalLengthWithoutUnit(const ExifIfd kexidID, const ExifTag kextaTag, IComMetaExif* pmexExifMeta);

    // Returns exposure time with / replaced with a character that can be used in file names
    QString GetExposureTime(IComMetaExif* pmexExifMeta);

    // Returns exposure time ad a dicimal number
    QString GetExposureTimeDecimal(IComMetaExif* pmexExifMeta);
};


Q_DECLARE_METATYPE(IMetaExif)
Q_DECLARE_METATYPE(IMetaExif*)

#endif // IMetaExif_h
