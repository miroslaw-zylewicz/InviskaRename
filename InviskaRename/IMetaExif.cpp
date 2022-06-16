#include "IMetaExif.h"
#include "IMetaTagLookup.h"
#include "IRenameInvalidCharSub.h"

QHash<QString, int> IMetaExif::m_qhashTagLookup = QHash<QString, int>();


IMetaExif::IMetaExif() : IMetaBase()
{

}


IMetaExif::IMetaExif(IComMetaExif* pmexExifMeta, const IRenameInvalidCharSub & kricsInvalidCharSub, const bool kbAdvancedMode) : IMetaBase(kbAdvancedMode ? NumTagsAdvanced : NumTagsBasic)
{
    if (kbAdvancedMode)
        ReadTagsAdvanced(pmexExifMeta, kricsInvalidCharSub);
    else
        ReadTagsBasic(pmexExifMeta, kricsInvalidCharSub);
}


void IMetaExif::InitTagLookupHashBasic()
{
    m_qhashTagLookup.clear();

    m_qhashTagLookup["datetime"]                = DateTime;
    m_qhashTagLookup["date"]                    = Date;
    m_qhashTagLookup["time"]                    = Time;
    m_qhashTagLookup["dateyyyy"]                = DateYYYY;
    m_qhashTagLookup["dateyy"]                  = DateYY;
    m_qhashTagLookup["datemm"]                  = DateMM;
    m_qhashTagLookup["datedd"]                  = DateDD;
    m_qhashTagLookup["timehh"]                  = TimeHH;
    m_qhashTagLookup["timemm"]                  = TimeMM;
    m_qhashTagLookup["timess"]                  = TimeSS;
    m_qhashTagLookup["timesubsec"]              = TimeSubSec;

    m_qhashTagLookup["cameramake"]              = CameraMake;
    m_qhashTagLookup["cameramodel"]             = CameraModel;
    m_qhashTagLookup["fnumber"]                 = FNumber;
    m_qhashTagLookup["isospeed"]                = ISOSpeed;
    m_qhashTagLookup["exposuretime"]            = ExposureTime;
    m_qhashTagLookup["exposuretimedec"]         = ExposureTimeDec;
    m_qhashTagLookup["focallength"]             = FocalLength;
    m_qhashTagLookup["focallengthin35mm"]       = FocalLengthIn35mm;
    m_qhashTagLookup["program"]                 = Program;
    m_qhashTagLookup["pixeldimx"]               = PixelDimX;
    m_qhashTagLookup["pixeldimy"]               = PixelDimY;
}


void IMetaExif::InitTagLookupHashAdvanced()
{
    InitTagLookupHashBasic();

    m_qhashTagLookup["software"]                = Software;

    m_qhashTagLookup["digitalzoomratio"]        = DigitalZoomRatio;
    m_qhashTagLookup["flash"]                   = Flash;
    m_qhashTagLookup["orientation"]             = Orientation;

    m_qhashTagLookup["shutterspeedvalue"]       = ShutterSpeedValue;
    m_qhashTagLookup["exposurebiasvalue"]       = ExposureBiasValue;
    m_qhashTagLookup["brightnessvalue"]         = BrightnessValue;
    m_qhashTagLookup["aperturevalue"]           = ApertureValue;
    m_qhashTagLookup["maxaperturevalue"]        = MaxApertureValue;
    m_qhashTagLookup["scenecapturetype"]        = SceneCaptureType;
    m_qhashTagLookup["customrendered"]          = CustomRendered;
    m_qhashTagLookup["exposuremode"]            = ExposureMode;
    m_qhashTagLookup["whitebalance"]            = WhiteBalance;
    m_qhashTagLookup["gaincontrol"]             = GainControl;
    m_qhashTagLookup["contrast"]                = Contrast;
    m_qhashTagLookup["saturation"]              = Saturation;
    m_qhashTagLookup["sharpness"]               = Sharpness;
    m_qhashTagLookup["meteringmode"]            = MeteringMode;
    m_qhashTagLookup["lightsource"]             = LightSource;
    m_qhashTagLookup["focalplanexresolution"]   = FocalPlaneXResolution;
    m_qhashTagLookup["focalplaneyresolution"]   = FocalPlaneYResolution;
    m_qhashTagLookup["focalplaneresolutionunit"]= FocalPlaneResolutionUnit;
    m_qhashTagLookup["subjectdistancerange"]    = SubjectDistanceRange;
    m_qhashTagLookup["sensingmethod"]           = SensingMethod;
    m_qhashTagLookup["scenetype"]               = SceneType;

    m_qhashTagLookup["colorspace"]              = ColorSpace;
    m_qhashTagLookup["compression"]             = Compression;
    m_qhashTagLookup["componentsconfiguration"] = ComponentsConfiguration;
    m_qhashTagLookup["compressedbitsperpixel"]  = CompressedBitsPerPixel;
    m_qhashTagLookup["ycbcrpositioning"]        = YCbCrPositioning;
    m_qhashTagLookup["xresolution"]             = XResolution;
    m_qhashTagLookup["yresolution"]             = YResolution;
    m_qhashTagLookup["resolutionunit"]          = ResolutionUnit;
    m_qhashTagLookup["filesource"]              = FileSource;
    m_qhashTagLookup["exifversion"]             = ExifVersion;
    m_qhashTagLookup["flashpixversion"]         = FlashPixVersion;
    m_qhashTagLookup["interoperabilityindex"]   = InteroperabilityIndex;
    m_qhashTagLookup["interoperabilityversion"] = InteroperabilityVersion;

    m_qhashTagLookup["title"]                   = Title;
    m_qhashTagLookup["subject"]                 = Subject;
    m_qhashTagLookup["author"]                  = Author;
    m_qhashTagLookup["comment"]                 = Comment;
    m_qhashTagLookup["keywords"]                = Keywords;
    m_qhashTagLookup["copyright"]               = Copyright;

    m_qhashTagLookup["gpslatitude"]             = GPSLatitude;
    m_qhashTagLookup["gpslatituderef"]          = GPSLatitudeRef;
    m_qhashTagLookup["gpslongitude"]            = GPSLongitude;
    m_qhashTagLookup["gpslongituderef"]         = GPSLongitudeRef;
    m_qhashTagLookup["gpsaltitude"]             = GPSAltitude;
    m_qhashTagLookup["gpsaltituderef"]          = GPSAltitudeRef;
    m_qhashTagLookup["gpsdatestamp"]            = GPSDateStamp;
    m_qhashTagLookup["gpstimestamp"]            = GPSTimeStamp;
}


void IMetaExif::ReadTagsBasic(IComMetaExif* pmexExifMeta, const IRenameInvalidCharSub & kricsInvalidCharSub)
{
    QString qstrDateTime                        = GetDateTimeSeparatorSub(pmexExifMeta);
    m_prgqstrTagValues[DateTime]                = qstrDateTime;
    m_prgqstrTagValues[Date]                    = qstrDateTime.left(10);
    m_prgqstrTagValues[Time]                    = qstrDateTime.mid(11, 9);
    m_prgqstrTagValues[DateYYYY]                = qstrDateTime.left(4);
    m_prgqstrTagValues[DateYY]                  = qstrDateTime.mid(2, 2);
    m_prgqstrTagValues[DateMM]                  = qstrDateTime.mid(5, 2);
    m_prgqstrTagValues[DateDD]                  = qstrDateTime.mid(8, 2);
    m_prgqstrTagValues[TimeHH]                  = qstrDateTime.mid(11, 2);
    m_prgqstrTagValues[TimeMM]                  = qstrDateTime.mid(14, 2);
    m_prgqstrTagValues[TimeSS]                  = qstrDateTime.mid(17, 2);
    m_prgqstrTagValues[TimeSubSec]              = GetTimeSubSec3Digit(pmexExifMeta);

    m_prgqstrTagValues[CameraMake]              = pmexExifMeta->GetValue(   EXIF_IFD_0,                 EXIF_TAG_MAKE);
    m_prgqstrTagValues[CameraModel]             = GetValueInvalidCharSub(   EXIF_IFD_0,                 EXIF_TAG_MODEL,                     pmexExifMeta,       kricsInvalidCharSub);
    m_prgqstrTagValues[FNumber]                 = GetFNumberWithoutUnit(pmexExifMeta);
    m_prgqstrTagValues[ISOSpeed]                = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_ISO_SPEED_RATINGS);
    m_prgqstrTagValues[ExposureTime]            = GetExposureTime(pmexExifMeta);
    m_prgqstrTagValues[ExposureTimeDec]         = GetExposureTimeDecimal(pmexExifMeta);
    m_prgqstrTagValues[FocalLength]             = GetFocalLengthWithoutUnit(EXIF_IFD_EXIF,              EXIF_TAG_FOCAL_LENGTH,              pmexExifMeta);
    m_prgqstrTagValues[FocalLengthIn35mm]       = GetFocalLengthWithoutUnit(EXIF_IFD_EXIF,              EXIF_TAG_FOCAL_LENGTH_IN_35MM_FILM, pmexExifMeta);
    m_prgqstrTagValues[Program]                 = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_EXPOSURE_PROGRAM);
    m_prgqstrTagValues[PixelDimX]               = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_PIXEL_X_DIMENSION);
    m_prgqstrTagValues[PixelDimY]               = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_PIXEL_Y_DIMENSION);
}


void IMetaExif::ReadTagsAdvanced(IComMetaExif* pmexExifMeta, const IRenameInvalidCharSub & kricsInvalidCharSub)
{
    ReadTagsBasic(pmexExifMeta, kricsInvalidCharSub);

    m_prgqstrTagValues[Software]                = pmexExifMeta->GetValue(   EXIF_IFD_0,                 EXIF_TAG_SOFTWARE);

    m_prgqstrTagValues[DigitalZoomRatio]        = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_DIGITAL_ZOOM_RATIO);
    m_prgqstrTagValues[Flash]                   = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_FLASH);
    m_prgqstrTagValues[Orientation]             = pmexExifMeta->GetValue(   EXIF_IFD_0,                 EXIF_TAG_ORIENTATION);

    m_prgqstrTagValues[ShutterSpeedValue]       = GetValueReplaceSlash(     EXIF_IFD_EXIF,              EXIF_TAG_SHUTTER_SPEED_VALUE,       pmexExifMeta);
    m_prgqstrTagValues[ExposureBiasValue]       = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_EXPOSURE_BIAS_VALUE);
    m_prgqstrTagValues[BrightnessValue]         = GetValueReplaceSlash(     EXIF_IFD_EXIF,              EXIF_TAG_BRIGHTNESS_VALUE,          pmexExifMeta);
    m_prgqstrTagValues[ApertureValue]           = GetValueReplaceSlash(     EXIF_IFD_EXIF,              EXIF_TAG_APERTURE_VALUE,            pmexExifMeta);
    m_prgqstrTagValues[MaxApertureValue]        = GetValueReplaceSlash(     EXIF_IFD_EXIF,              EXIF_TAG_MAX_APERTURE_VALUE,        pmexExifMeta);
    m_prgqstrTagValues[SceneCaptureType]        = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_SCENE_CAPTURE_TYPE);
    m_prgqstrTagValues[CustomRendered]          = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_CUSTOM_RENDERED);
    m_prgqstrTagValues[ExposureMode]            = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_EXPOSURE_MODE);
    m_prgqstrTagValues[WhiteBalance]            = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_WHITE_BALANCE);
    m_prgqstrTagValues[GainControl]             = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_GAIN_CONTROL);
    m_prgqstrTagValues[Contrast]                = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_CONTRAST);
    m_prgqstrTagValues[Saturation]              = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_SATURATION);
    m_prgqstrTagValues[Sharpness]               = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_SHARPNESS);
    m_prgqstrTagValues[MeteringMode]            = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_METERING_MODE);
    m_prgqstrTagValues[LightSource]             = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_LIGHT_SOURCE);
    m_prgqstrTagValues[FocalPlaneXResolution]   = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_FOCAL_PLANE_X_RESOLUTION);
    m_prgqstrTagValues[FocalPlaneYResolution]   = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_FOCAL_PLANE_Y_RESOLUTION);
    m_prgqstrTagValues[FocalPlaneResolutionUnit]= pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_FOCAL_PLANE_RESOLUTION_UNIT);
    m_prgqstrTagValues[SubjectDistanceRange]    = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_SUBJECT_DISTANCE_RANGE);
    m_prgqstrTagValues[SensingMethod]           = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_SENSING_METHOD);
    m_prgqstrTagValues[SceneType]               = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_SCENE_TYPE);

    m_prgqstrTagValues[ColorSpace]              = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_COLOR_SPACE);
    m_prgqstrTagValues[Compression]             = pmexExifMeta->GetValue(   EXIF_IFD_1,                 EXIF_TAG_COMPRESSION);
    m_prgqstrTagValues[ComponentsConfiguration] = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_COMPONENTS_CONFIGURATION);
    m_prgqstrTagValues[CompressedBitsPerPixel]  = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_COMPRESSED_BITS_PER_PIXEL);
    m_prgqstrTagValues[YCbCrPositioning]        = pmexExifMeta->GetValue(   EXIF_IFD_0,                 EXIF_TAG_YCBCR_POSITIONING);
    m_prgqstrTagValues[XResolution]             = pmexExifMeta->GetValue(   EXIF_IFD_1,                 EXIF_TAG_X_RESOLUTION);
    m_prgqstrTagValues[YResolution]             = pmexExifMeta->GetValue(   EXIF_IFD_1,                 EXIF_TAG_Y_RESOLUTION);
    m_prgqstrTagValues[ResolutionUnit]          = pmexExifMeta->GetValue(   EXIF_IFD_1,                 EXIF_TAG_RESOLUTION_UNIT);
    m_prgqstrTagValues[FileSource]              = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_FILE_SOURCE);
    m_prgqstrTagValues[ExifVersion]             = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_EXIF_VERSION);
    m_prgqstrTagValues[FlashPixVersion]         = pmexExifMeta->GetValue(   EXIF_IFD_EXIF,              EXIF_TAG_FLASH_PIX_VERSION);
    m_prgqstrTagValues[InteroperabilityIndex]   = pmexExifMeta->GetValue(   EXIF_IFD_INTEROPERABILITY,  EXIF_TAG_INTEROPERABILITY_INDEX);
    m_prgqstrTagValues[InteroperabilityVersion] = pmexExifMeta->GetValue(   EXIF_IFD_INTEROPERABILITY,  EXIF_TAG_INTEROPERABILITY_VERSION);

    m_prgqstrTagValues[Title]                   = GetValueInvalidCharSub(   EXIF_IFD_0,                 EXIF_TAG_XP_TITLE,                  pmexExifMeta,       kricsInvalidCharSub);
    m_prgqstrTagValues[Subject]                 = GetValueInvalidCharSub(   EXIF_IFD_0,                 EXIF_TAG_XP_SUBJECT,                pmexExifMeta,       kricsInvalidCharSub);
    m_prgqstrTagValues[Author]                  = GetValueInvalidCharSub(   EXIF_IFD_0,                 EXIF_TAG_XP_AUTHOR,                 pmexExifMeta,       kricsInvalidCharSub);
    m_prgqstrTagValues[Comment]                 = GetValueInvalidCharSub(   EXIF_IFD_0,                 EXIF_TAG_XP_COMMENT,                pmexExifMeta,       kricsInvalidCharSub);
    m_prgqstrTagValues[Keywords]                = GetValueInvalidCharSub(   EXIF_IFD_0,                 EXIF_TAG_XP_KEYWORDS,               pmexExifMeta,       kricsInvalidCharSub);
    m_prgqstrTagValues[Copyright]               = GetValueInvalidCharSub(   EXIF_IFD_0,                 EXIF_TAG_COPYRIGHT,                 pmexExifMeta,       kricsInvalidCharSub);

    m_prgqstrTagValues[GPSLatitude]             = pmexExifMeta->GetValue(   EXIF_IFD_GPS,               static_cast<ExifTag>(EXIF_TAG_GPS_LATITUDE));
    m_prgqstrTagValues[GPSLatitudeRef]          = pmexExifMeta->GetValue(   EXIF_IFD_GPS,               static_cast<ExifTag>(EXIF_TAG_GPS_LATITUDE_REF));
    m_prgqstrTagValues[GPSLongitude]            = pmexExifMeta->GetValue(   EXIF_IFD_GPS,               static_cast<ExifTag>(EXIF_TAG_GPS_LONGITUDE));
    m_prgqstrTagValues[GPSLongitudeRef]         = pmexExifMeta->GetValue(   EXIF_IFD_GPS,               static_cast<ExifTag>(EXIF_TAG_GPS_LONGITUDE_REF));
    m_prgqstrTagValues[GPSAltitude]             = pmexExifMeta->GetValue(   EXIF_IFD_GPS,               static_cast<ExifTag>(EXIF_TAG_GPS_ALTITUDE));
    m_prgqstrTagValues[GPSAltitudeRef]          = pmexExifMeta->GetValue(   EXIF_IFD_GPS,               static_cast<ExifTag>(EXIF_TAG_GPS_ALTITUDE_REF));
    m_prgqstrTagValues[GPSDateStamp]            = GetGPSDateSeparatorSub(pmexExifMeta);
    m_prgqstrTagValues[GPSTimeStamp]            = GetGPSTimeSeparatorSub(pmexExifMeta);
}


int IMetaExif::GetTagID(const QString & krqstrTagCode)
{
    QHash<QString, int>::iterator itTagID = m_qhashTagLookup.find(krqstrTagCode);
    if (itTagID == m_qhashTagLookup.end())
        return ITagInfo::Invalid;
    return *itTagID;
}


QString IMetaExif::GetValueInvalidCharSub(const ExifIfd kexidID, const ExifTag kextaTag,  IComMetaExif* pmexExifMeta, const IRenameInvalidCharSub & kricsInvalidCharSub)
{
    return kricsInvalidCharSub.PerformSubstitution(pmexExifMeta->GetValue(kexidID, kextaTag));
}


QString IMetaExif::GetValueReplaceSlash(const ExifIfd kexidID, const ExifTag kextaTag, IComMetaExif* pmexExifMeta)
{
    QString qstrValue = pmexExifMeta->GetValue(kexidID, kextaTag);
    qstrValue.replace('/', m_qchSeparatorFraction);
    return qstrValue;
}


QString IMetaExif::GetDateTimeSeparatorSub(IComMetaExif* pmexExifMeta)
{
    QString qstrDateTime = pmexExifMeta->GetValue(EXIF_IFD_EXIF, EXIF_TAG_DATE_TIME_ORIGINAL);

    if (qstrDateTime.length() > 16)
    {
        qstrDateTime[4] = m_qchSeparatorDate;
        qstrDateTime[7] = m_qchSeparatorDate;
        qstrDateTime[13] = m_qchSeparatorTime;
        qstrDateTime[16] = m_qchSeparatorTime;
    }
    return qstrDateTime;
}


QString IMetaExif::GetGPSDateSeparatorSub(IComMetaExif* pmexExifMeta)
{
    QString qstrGPSDate = pmexExifMeta->GetValue(EXIF_IFD_GPS, static_cast<ExifTag>(EXIF_TAG_GPS_DATE_STAMP));

    if (qstrGPSDate.length() > 7)
    {
        qstrGPSDate[4] = m_qchSeparatorDate;
        qstrGPSDate[7] = m_qchSeparatorDate;
    }
    return qstrGPSDate;
}


QString IMetaExif::GetGPSTimeSeparatorSub(IComMetaExif* pmexExifMeta)
{
    QString qstrGPSTime = pmexExifMeta->GetValue(EXIF_IFD_GPS, static_cast<ExifTag>(EXIF_TAG_GPS_TIME_STAMP));

    if (qstrGPSTime.length() > 5)
    {
        qstrGPSTime[2] = m_qchSeparatorTime;
        qstrGPSTime[5] = m_qchSeparatorTime;
    }
    return qstrGPSTime;
}


QString IMetaExif::GetTimeSubSec3Digit(IComMetaExif* pmexExifMeta)
{
    QString strTimeSubSec = pmexExifMeta->GetValue(EXIF_IFD_EXIF, EXIF_TAG_SUB_SEC_TIME_ORIGINAL);

    int iLength = strTimeSubSec.length();
    switch (iLength)
    {
    case 3 : return strTimeSubSec;
    case 2 : return strTimeSubSec + "0";
    case 1 : return strTimeSubSec + "00";
    case 0 : return "000";
    }

    return strTimeSubSec.left(3);
}


QString IMetaExif::GetFNumberWithoutUnit(IComMetaExif* pmexExifMeta)
{
    QString qstrFNumber = pmexExifMeta->GetValue(EXIF_IFD_EXIF,  EXIF_TAG_FNUMBER);
    return qstrFNumber.mid(2);
}


QString IMetaExif::GetFocalLengthWithoutUnit(const ExifIfd kexidID, const ExifTag kextaTag, IComMetaExif* pmexExifMeta)
{
    QString qstrFocalLength = pmexExifMeta->GetValue(kexidID,  kextaTag);
    if (qstrFocalLength.endsWith(".0 mm"))
        return qstrFocalLength.left(qstrFocalLength.length()-5);
    if (qstrFocalLength.endsWith(" mm"))
        return qstrFocalLength.left(qstrFocalLength.length()-3);
    return qstrFocalLength;
}


QString IMetaExif::GetExposureTime(IComMetaExif* pmexExifMeta)
{
    QString qstrExposureTime = pmexExifMeta->GetValue(EXIF_IFD_EXIF,  EXIF_TAG_EXPOSURE_TIME);

    if (qstrExposureTime.endsWith(" sec."))
        qstrExposureTime = qstrExposureTime.left(qstrExposureTime.length()-5);

    qstrExposureTime.replace('/', m_qchSeparatorFraction);
    return qstrExposureTime;
}


QString IMetaExif::GetExposureTimeDecimal(IComMetaExif* pmexExifMeta)
{
    QString qstrExposureTime = pmexExifMeta->GetValue(EXIF_IFD_EXIF,  EXIF_TAG_EXPOSURE_TIME);

    if (qstrExposureTime.endsWith(" sec."))
        qstrExposureTime = qstrExposureTime.left(qstrExposureTime.length()-5);

    int iSlashIndex = qstrExposureTime.indexOf('/');
    if (iSlashIndex != -1)
        qstrExposureTime = QString::number(qstrExposureTime.left(iSlashIndex).toDouble() / qstrExposureTime.mid(iSlashIndex+1).toDouble());

    return qstrExposureTime;
}
