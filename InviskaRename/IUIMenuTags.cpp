#include <QtWidgets>
#include "IUIMenuTags.h"
#include "IUIMainWindow.h"
#include "IUIFileList.h"
#include "IUIRename.h"


IUIMenuTags::IUIMenuTags(IUIMainWindow* pmwMainWindow) : QMenu(tr("&Tags"), pmwMainWindow)
{
    m_pmwMainWindow = pmwMainWindow;
}


void IUIMenuTags::CreateActionsTags()
{
    CreateTagsActionsMusic();
    CreateTagsActionsExif();
    CreateTagsActionsFileAttributes();
    InitialiseMenu();
}


void IUIMenuTags::CreateTagsActionsMusic()
{
    m_pqactTitle                = new QAction(QString("%1\t%2").arg(tr("Title"))                    .arg(PrintTagOS("[$Mu-Title]")), this);
    m_pqactArtist               = new QAction(QString("%1\t%2").arg(tr("Artist"))                   .arg(PrintTagOS("[$Mu-Artist]")), this);
    m_pqactAlbum                = new QAction(QString("%1\t%2").arg(tr("Album"))                    .arg(PrintTagOS("[$Mu-Album]")), this);
    m_pqactTrack                = new QAction(QString("%1\t%2").arg(tr("Track"))                    .arg(PrintTagOS("[$Mu-Track]")), this);
    m_pqactYear                 = new QAction(QString("%1\t%2").arg(tr("Year"))                     .arg(PrintTagOS("[$Mu-Year]")), this);
    m_pqactGenre                = new QAction(QString("%1\t%2").arg(tr("Genre"))                    .arg(PrintTagOS("[$Mu-Genre]")), this);
    m_pqactComment              = new QAction(QString("%1\t%2").arg(tr("Comment"))                  .arg(PrintTagOS("[$Mu-Comment]")), this);
    m_pqactRunTime              = new QAction(QString("%1\t%2").arg(tr("Run Time"))                 .arg(PrintTagOS("[$Mu-RunTime]")), this);
    m_pqactChannels             = new QAction(QString("%1\t%2").arg(tr("Channels"))                 .arg(PrintTagOS("[$Mu-Channels]")), this);
    m_pqactSampleRate           = new QAction(QString("%1\t%2").arg(tr("Sample Rate Hz"))           .arg(PrintTagOS("[$Mu-SampleRate]")), this);
    m_pqactBitRate              = new QAction(QString("%1\t%2").arg(tr("Bit Rate kbps"))            .arg(PrintTagOS("[$Mu-BitRate]")), this);

    IUIRename* puirRenameUI = m_pmwMainWindow->GetRenameUI();
    puirRenameUI->AddActionLookupMusic(m_pqactTitle,                   "[$Mu-Title]");
    puirRenameUI->AddActionLookupMusic(m_pqactArtist,                  "[$Mu-Artist]");
    puirRenameUI->AddActionLookupMusic(m_pqactAlbum,                   "[$Mu-Album]");
    puirRenameUI->AddActionLookupMusic(m_pqactTrack,                   "[$Mu-Track]");
    puirRenameUI->AddActionLookupMusic(m_pqactYear,                    "[$Mu-Year]");
    puirRenameUI->AddActionLookupMusic(m_pqactGenre,                   "[$Mu-Genre]");
    puirRenameUI->AddActionLookupMusic(m_pqactComment,                 "[$Mu-Comment]");
    puirRenameUI->AddActionLookupMusic(m_pqactRunTime,                 "[$Mu-RunTime]");
    puirRenameUI->AddActionLookupMusic(m_pqactChannels,                "[$Mu-Channels]");
    puirRenameUI->AddActionLookupMusic(m_pqactSampleRate,              "[$Mu-SampleRate]");
    puirRenameUI->AddActionLookupMusic(m_pqactBitRate,                 "[$Mu-BitRate]");
}


void IUIMenuTags::CreateTagsActionsExif()
{
    m_pqactCameraMake           = new QAction(QString("%1\t%2").arg(tr("Camera Make"))              .arg(PrintTagOS("[$Ex-CameraMake]")), this);
    m_pqactCameraModel          = new QAction(QString("%1\t%2").arg(tr("Camera Model"))             .arg(PrintTagOS("[$Ex-CameraModel]")), this);
    m_pqactSoftware             = new QAction(QString("%1\t%2").arg(tr("Camera Software"))          .arg(PrintTagOS("[$Ex-Software]")), this);

    m_pqactDateTime             = new QAction(QString("%1\t%2").arg(tr("Date Time"))                .arg(PrintTagOS("[$Ex-DateTime]")), this);
    m_pqactDate                 = new QAction(QString("%1\t%2").arg(tr("Date"))                     .arg(PrintTagOS("[$Ex-Date]")), this);
    m_pqactTime                 = new QAction(QString("%1\t%2").arg(tr("Time"))                     .arg(PrintTagOS("[$Ex-Time]")), this);
    m_pqactDateYYYY             = new QAction(QString("%1\t%2").arg(tr("Date YYYY"))                .arg(PrintTagOS("[$Ex-DateYYYY]")), this);
    m_pqactDateYY               = new QAction(QString("%1\t%2").arg(tr("Date YY"))                  .arg(PrintTagOS("[$Ex-DateYY]")), this);
    m_pqactDateMM               = new QAction(QString("%1\t%2").arg(tr("Date MM"))                  .arg(PrintTagOS("[$Ex-DateMM]")), this);
    m_pqactDateDD               = new QAction(QString("%1\t%2").arg(tr("Date DD"))                  .arg(PrintTagOS("[$Ex-DateDD]")), this);
    m_pqactTimeHH               = new QAction(QString("%1\t%2").arg(tr("Time HH"))                  .arg(PrintTagOS("[$Ex-TimeHH]")), this);
    m_pqactTimeMM               = new QAction(QString("%1\t%2").arg(tr("Time MM"))                  .arg(PrintTagOS("[$Ex-TimeMM]")), this);
    m_pqactTimeSS               = new QAction(QString("%1\t%2").arg(tr("Time SS"))                  .arg(PrintTagOS("[$Ex-TimeSS]")), this);
    m_pqactTimeSubSec           = new QAction(QString("%1\t%2").arg(tr("Time Sub-Second"))          .arg(PrintTagOS("[$Ex-TimeSubSec]")), this);

    m_pqactFNumber              = new QAction(QString("%1\t%2").arg(tr("F-Number"))                 .arg(PrintTagOS("[$Ex-FNumber]")), this);
    m_pqactISOSpeed             = new QAction(QString("%1\t%2").arg(tr("ISO Speed"))                .arg(PrintTagOS("[$Ex-ISOSpeed]")), this);
    m_pqactExposureTime         = new QAction(QString("%1\t%2").arg(tr("Exposure Time"))            .arg(PrintTagOS("[$Ex-ExposureTime]")), this);
    m_pqactExposureTimeDec      = new QAction(QString("%1\t%2").arg(tr("Exposure Time Decimal"))    .arg(PrintTagOS("[$Ex-ExposureTimeDec]")), this);
    m_pqactFocalLength          = new QAction(QString("%1\t%2").arg(tr("Focal Length"))             .arg(PrintTagOS("[$Ex-FocalLength]")), this);
    m_pqactFocalLength35mm      = new QAction(QString("%1\t%2").arg(tr("Focal Length 35mm Equiv"))  .arg(PrintTagOS("[$Ex-FocalLengthIn35mm]")), this);
    m_pqactDigitalZoomRatio     = new QAction(QString("%1\t%2").arg(tr("Digital Zoom Ratio"))       .arg(PrintTagOS("[$Ex-DigitalZoomRatio]")), this);
    m_pqactExposureProgram      = new QAction(QString("%1\t%2").arg(tr("Exposure Program"))         .arg(PrintTagOS("[$Ex-Program]")), this);
    m_pqactFlashSetting         = new QAction(QString("%1\t%2").arg(tr("Flash Setting"))            .arg(PrintTagOS("[$Ex-Flash]")), this);
    m_pqactOrientation          = new QAction(QString("%1\t%2").arg(tr("Orientation"))              .arg(PrintTagOS("[$Ex-Orientation]")), this);

    m_pqactShutterSpeedValue    = new QAction(QString("%1\t%2").arg(tr("Shutter Speed Value"))      .arg(PrintTagOS("[$Ex-ShutterSpeedValue]")), this);
    m_pqactExposureBiasValue    = new QAction(QString("%1\t%2").arg(tr("Exposure Bias Value"))      .arg(PrintTagOS("[$Ex-ExposureBiasValue]")), this);
    m_pqactBrightnessValue      = new QAction(QString("%1\t%2").arg(tr("Brightness Value"))         .arg(PrintTagOS("[$Ex-BrightnessValue]")), this);
    m_pqactApertureValue        = new QAction(QString("%1\t%2").arg(tr("Aperture Value"))           .arg(PrintTagOS("[$Ex-ApertureValue]")), this);
    m_pqactMaxApertureValue     = new QAction(QString("%1\t%2").arg(tr("Max Aperture Value"))       .arg(PrintTagOS("[$Ex-MaxApertureValue]")), this);
    m_pqactSceneCaptureType     = new QAction(QString("%1\t%2").arg(tr("Scene Capture Type"))       .arg(PrintTagOS("[$Ex-SceneCaptureType]")), this);
    m_pqactCustomRendered       = new QAction(QString("%1\t%2").arg(tr("Custom Rendered"))          .arg(PrintTagOS("[$Ex-CustomRendered]")), this);
    m_pqactExposureMode         = new QAction(QString("%1\t%2").arg(tr("Exposure Mode"))            .arg(PrintTagOS("[$Ex-ExposureMode]")), this);
    m_pqactWhiteBalance         = new QAction(QString("%1\t%2").arg(tr("White Balance"))            .arg(PrintTagOS("[$Ex-WhiteBalance]")), this);
    m_pqactGainControl          = new QAction(QString("%1\t%2").arg(tr("Gain Control"))             .arg(PrintTagOS("[$Ex-GainControl]")), this);
    m_pqactContrast             = new QAction(QString("%1\t%2").arg(tr("Contrast"))                 .arg(PrintTagOS("[$Ex-Contrast]")), this);
    m_pqactSaturation           = new QAction(QString("%1\t%2").arg(tr("Saturation"))               .arg(PrintTagOS("[$Ex-Saturation]")), this);
    m_pqactSharpness            = new QAction(QString("%1\t%2").arg(tr("Sharpness"))                .arg(PrintTagOS("[$Ex-Sharpness]")), this);
    m_pqactMeteringMode         = new QAction(QString("%1\t%2").arg(tr("Metering Mode"))            .arg(PrintTagOS("[$Ex-MeteringMode]")), this);
    m_pqactLightSource          = new QAction(QString("%1\t%2").arg(tr("Light Source"))             .arg(PrintTagOS("[$Ex-LightSource]")), this);
    m_pqactFocalPlaneXRes       = new QAction(QString("%1\t%2").arg(tr("Focal Plane X Resolution")) .arg(PrintTagOS("[$Ex-FocalPlaneXResolution]")), this);
    m_pqactFocalPlaneYRes       = new QAction(QString("%1\t%2").arg(tr("Focal Plane Y Resolution")) .arg(PrintTagOS("[$Ex-FocalPlaneYResolution]")), this);
    m_pqactFocalPlaneResUnit    = new QAction(QString("%1\t%2").arg(tr("Focal Plane Resolution Unit")).arg(PrintTagOS("[$Ex-FocalPlaneResolutionUnit]")), this);
    m_pqactSubjectDistanceRange = new QAction(QString("%1\t%2").arg(tr("Subject Distance Range"))   .arg(PrintTagOS("[$Ex-SubjectDistanceRange]")), this);
    m_pqactSensingMethod        = new QAction(QString("%1\t%2").arg(tr("Sensing Method"))           .arg(PrintTagOS("[$Ex-SensingMethod]")), this);
    m_pqactSceneType            = new QAction(QString("%1\t%2").arg(tr("Scene Type"))               .arg(PrintTagOS("[$Ex-SceneType]")), this);

    m_pqactPixelDimX            = new QAction(QString("%1\t%2").arg(tr("Pixel Dimension X"))        .arg(PrintTagOS("[$Ex-PixelDimX]")), this);
    m_pqactPixelDimY            = new QAction(QString("%1\t%2").arg(tr("Pixel Dimension Y"))        .arg(PrintTagOS("[$Ex-PixelDimY]")), this);
    m_pqactColorSpace           = new QAction(QString("%1\t%2").arg(tr("Color Space"))              .arg(PrintTagOS("[$Ex-ColorSpace]")), this);
    m_pqactCompression          = new QAction(QString("%1\t%2").arg(tr("Compression"))              .arg(PrintTagOS("[$Ex-Compression]")), this);
    m_pqactComponentsConfig     = new QAction(QString("%1\t%2").arg(tr("Components Configuration")) .arg(PrintTagOS("[$Ex-ComponentsConfiguration]")), this);
    m_pqactCompressedBPP        = new QAction(QString("%1\t%2").arg(tr("Compressed Bits Per Pixel")).arg(PrintTagOS("[$Ex-CompressedBitsPerPixel]")), this);
    m_pqactYCbCrPositioning     = new QAction(QString("%1\t%2").arg(tr("YCbCr Positioning"))        .arg(PrintTagOS("[$Ex-YCbCrPositioning]")), this);
    m_pqactXResolution          = new QAction(QString("%1\t%2").arg(tr("X Resolution"))             .arg(PrintTagOS("[$Ex-XResolution]")), this);
    m_pqactYResolution          = new QAction(QString("%1\t%2").arg(tr("Y Resolution"))             .arg(PrintTagOS("[$Ex-YResolution]")), this);
    m_pqactResolutionUnit       = new QAction(QString("%1\t%2").arg(tr("Resolution Unit"))          .arg(PrintTagOS("[$Ex-ResolutionUnit]")), this);
    m_pqactFileSource           = new QAction(QString("%1\t%2").arg(tr("File Source"))              .arg(PrintTagOS("[$Ex-FileSource]")), this);
    m_pqactExifVersion          = new QAction(QString("%1\t%2").arg(tr("Exif Version"))             .arg(PrintTagOS("[$Ex-ExifVersion]")), this);
    m_pqactFlashPixVersion      = new QAction(QString("%1\t%2").arg(tr("FlashPix Version"))         .arg(PrintTagOS("[$Ex-FlashPixVersion]")), this);
    m_pqactInteropIndex         = new QAction(QString("%1\t%2").arg(tr("Interoperability Index"))   .arg(PrintTagOS("[$Ex-InteroperabilityIndex]")), this);
    m_pqactInteropVersion       = new QAction(QString("%1\t%2").arg(tr("Interoperability Version")) .arg(PrintTagOS("[$Ex-InteroperabilityVersion]")), this);

    m_pqactXPTitle              = new QAction(QString("%1\t%2").arg(tr("Title"))                    .arg(PrintTagOS("[$Ex-Title]")), this);
    m_pqactXPSubject            = new QAction(QString("%1\t%2").arg(tr("Subject"))                  .arg(PrintTagOS("[$Ex-Subject]")), this);
    m_pqactXPAuthor             = new QAction(QString("%1\t%2").arg(tr("Author"))                   .arg(PrintTagOS("[$Ex-Author]")), this);
    m_pqactXPComment            = new QAction(QString("%1\t%2").arg(tr("Comment"))                  .arg(PrintTagOS("[$Ex-Comment]")), this);
    m_pqactXPKeywords           = new QAction(QString("%1\t%2").arg(tr("Keywords"))                 .arg(PrintTagOS("[$Ex-Keywords]")), this);
    m_pqactCopyright            = new QAction(QString("%1\t%2").arg(tr("Copyright"))                .arg(PrintTagOS("[$Ex-Copyright]")), this);

    m_pqactGPSLatitude          = new QAction(QString("%1\t%2").arg(tr("Latitude"))                 .arg(PrintTagOS("[$Ex-GPSLatitude]")), this);
    m_pqactGPSLatitudeRef       = new QAction(QString("%1\t%2").arg(tr("Latitude Ref"))             .arg(PrintTagOS("[$Ex-GPSLatitudeRef]")), this);
    m_pqactGPSLongitude         = new QAction(QString("%1\t%2").arg(tr("Longitude"))                .arg(PrintTagOS("[$Ex-GPSLongitude]")), this);
    m_pqactGPSLongitudeRef      = new QAction(QString("%1\t%2").arg(tr("Longitude Ref"))            .arg(PrintTagOS("[$Ex-GPSLongitudeRef]")), this);
    m_pqactGPSAltitude          = new QAction(QString("%1\t%2").arg(tr("Altitude"))                 .arg(PrintTagOS("[$Ex-GPSAltitude]")), this);
    m_pqactGPSAltitudeRef       = new QAction(QString("%1\t%2").arg(tr("Altitude Ref"))             .arg(PrintTagOS("[$Ex-GPSAltitudeRef]")), this);
    m_pqactGPSDateStamp         = new QAction(QString("%1\t%2").arg(tr("Date Stamp"))               .arg(PrintTagOS("[$Ex-GPSDateStamp]")), this);
    m_pqactGPSTimeStamp         = new QAction(QString("%1\t%2").arg(tr("Time Stamp"))               .arg(PrintTagOS("[$Ex-GPSTimeStamp]")), this);


    IUIRename* puirRenameUI = m_pmwMainWindow->GetRenameUI();
    puirRenameUI->AddActionLookupExif(m_pqactCameraMake,               "[$Ex-CameraMake]");
    puirRenameUI->AddActionLookupExif(m_pqactCameraModel,              "[$Ex-CameraModel]");
    puirRenameUI->AddActionLookupExif(m_pqactSoftware,                 "[$Ex-Software]");

    puirRenameUI->AddActionLookupExif(m_pqactDateTime,                 "[$Ex-DateTime]");
    puirRenameUI->AddActionLookupExif(m_pqactDate,                     "[$Ex-Date]");
    puirRenameUI->AddActionLookupExif(m_pqactTime,                     "[$Ex-Time]");
    puirRenameUI->AddActionLookupExif(m_pqactDateYYYY,                 "[$Ex-DateYYYY]");
    puirRenameUI->AddActionLookupExif(m_pqactDateYY,                   "[$Ex-DateYY]");
    puirRenameUI->AddActionLookupExif(m_pqactDateMM,                   "[$Ex-DateMM]");
    puirRenameUI->AddActionLookupExif(m_pqactDateDD,                   "[$Ex-DateDD]");
    puirRenameUI->AddActionLookupExif(m_pqactTimeHH,                   "[$Ex-TimeHH]");
    puirRenameUI->AddActionLookupExif(m_pqactTimeMM,                   "[$Ex-TimeMM]");
    puirRenameUI->AddActionLookupExif(m_pqactTimeSS,                   "[$Ex-TimeSS]");
    puirRenameUI->AddActionLookupExif(m_pqactTimeSubSec,               "[$Ex-TimeSubSec]");

    puirRenameUI->AddActionLookupExif(m_pqactFNumber,                  "[$Ex-FNumber]");
    puirRenameUI->AddActionLookupExif(m_pqactISOSpeed,                 "[$Ex-ISOSpeed]");
    puirRenameUI->AddActionLookupExif(m_pqactExposureTime,             "[$Ex-ExposureTime]");
    puirRenameUI->AddActionLookupExif(m_pqactExposureTimeDec,          "[$Ex-ExposureTimeDec]");
    puirRenameUI->AddActionLookupExif(m_pqactFocalLength,              "[$Ex-FocalLength]");
    puirRenameUI->AddActionLookupExif(m_pqactFocalLength35mm,          "[$Ex-FocalLengthIn35mm]");
    puirRenameUI->AddActionLookupExif(m_pqactDigitalZoomRatio,         "[$Ex-DigitalZoomRatio]");
    puirRenameUI->AddActionLookupExif(m_pqactExposureProgram,          "[$Ex-Program]");
    puirRenameUI->AddActionLookupExif(m_pqactFlashSetting,             "[$Ex-Flash]");
    puirRenameUI->AddActionLookupExif(m_pqactOrientation,              "[$Ex-Orientation]");

    puirRenameUI->AddActionLookupExif(m_pqactShutterSpeedValue,        "[$Ex-ShutterSpeedValue]");
    puirRenameUI->AddActionLookupExif(m_pqactExposureBiasValue,        "[$Ex-ExposureBiasValue]");
    puirRenameUI->AddActionLookupExif(m_pqactBrightnessValue,          "[$Ex-BrightnessValue]");
    puirRenameUI->AddActionLookupExif(m_pqactApertureValue,            "[$Ex-ApertureValue]");
    puirRenameUI->AddActionLookupExif(m_pqactMaxApertureValue,         "[$Ex-MaxApertureValue]");
    puirRenameUI->AddActionLookupExif(m_pqactSceneCaptureType,         "[$Ex-SceneCaptureType]");
    puirRenameUI->AddActionLookupExif(m_pqactCustomRendered,           "[$Ex-CustomRendered]");
    puirRenameUI->AddActionLookupExif(m_pqactExposureMode,             "[$Ex-ExposureMode]");
    puirRenameUI->AddActionLookupExif(m_pqactWhiteBalance,             "[$Ex-WhiteBalance]");
    puirRenameUI->AddActionLookupExif(m_pqactGainControl,              "[$Ex-GainControl]");
    puirRenameUI->AddActionLookupExif(m_pqactContrast,                 "[$Ex-Contrast]");
    puirRenameUI->AddActionLookupExif(m_pqactSaturation,               "[$Ex-Saturation]");
    puirRenameUI->AddActionLookupExif(m_pqactSharpness,                "[$Ex-Sharpness]");
    puirRenameUI->AddActionLookupExif(m_pqactMeteringMode,             "[$Ex-MeteringMode]");
    puirRenameUI->AddActionLookupExif(m_pqactLightSource,              "[$Ex-LightSource]");
    puirRenameUI->AddActionLookupExif(m_pqactFocalPlaneXRes,           "[$Ex-FocalPlaneXResolution]");
    puirRenameUI->AddActionLookupExif(m_pqactFocalPlaneYRes,           "[$Ex-FocalPlaneYResolution]");
    puirRenameUI->AddActionLookupExif(m_pqactFocalPlaneResUnit,        "[$Ex-FocalPlaneResolutionUnit]");
    puirRenameUI->AddActionLookupExif(m_pqactSubjectDistanceRange,     "[$Ex-SubjectDistanceRange]");
    puirRenameUI->AddActionLookupExif(m_pqactSensingMethod,            "[$Ex-SensingMethod]");
    puirRenameUI->AddActionLookupExif(m_pqactSceneType,                "[$Ex-SceneType]");

    puirRenameUI->AddActionLookupExif(m_pqactPixelDimX,                "[$Ex-PixelDimX]");
    puirRenameUI->AddActionLookupExif(m_pqactPixelDimY,                "[$Ex-PixelDimY]");
    puirRenameUI->AddActionLookupExif(m_pqactColorSpace,               "[$Ex-ColorSpace]");
    puirRenameUI->AddActionLookupExif(m_pqactCompression,              "[$Ex-Compression]");
    puirRenameUI->AddActionLookupExif(m_pqactComponentsConfig,         "[$Ex-ComponentsConfiguration]");
    puirRenameUI->AddActionLookupExif(m_pqactCompressedBPP,            "[$Ex-CompressedBitsPerPixel]");
    puirRenameUI->AddActionLookupExif(m_pqactYCbCrPositioning,         "[$Ex-YCbCrPositioning]");
    puirRenameUI->AddActionLookupExif(m_pqactXResolution,              "[$Ex-XResolution]");
    puirRenameUI->AddActionLookupExif(m_pqactYResolution,              "[$Ex-YResolution]");
    puirRenameUI->AddActionLookupExif(m_pqactResolutionUnit,           "[$Ex-ResolutionUnit]");
    puirRenameUI->AddActionLookupExif(m_pqactFileSource,               "[$Ex-FileSource]");
    puirRenameUI->AddActionLookupExif(m_pqactExifVersion,              "[$Ex-ExifVersion]");
    puirRenameUI->AddActionLookupExif(m_pqactFlashPixVersion,          "[$Ex-FlashPixVersion]");
    puirRenameUI->AddActionLookupExif(m_pqactInteropIndex,             "[$Ex-InteroperabilityIndex]");
    puirRenameUI->AddActionLookupExif(m_pqactInteropVersion,           "[$Ex-InteroperabilityVersion]");

    puirRenameUI->AddActionLookupExif(m_pqactXPTitle,                  "[$Ex-Title]");
    puirRenameUI->AddActionLookupExif(m_pqactXPSubject,                "[$Ex-Subject]");
    puirRenameUI->AddActionLookupExif(m_pqactXPAuthor,                 "[$Ex-Author]");
    puirRenameUI->AddActionLookupExif(m_pqactXPComment,                "[$Ex-Comment]");
    puirRenameUI->AddActionLookupExif(m_pqactXPKeywords,               "[$Ex-Keywords]");
    puirRenameUI->AddActionLookupExif(m_pqactCopyright,                "[$Ex-Copyright]");

    puirRenameUI->AddActionLookupExif(m_pqactGPSLatitude,              "[$Ex-GPSLatitude]");
    puirRenameUI->AddActionLookupExif(m_pqactGPSLatitudeRef,           "[$Ex-GPSLatitudeRef]");
    puirRenameUI->AddActionLookupExif(m_pqactGPSLongitude,             "[$Ex-GPSLongitude]");
    puirRenameUI->AddActionLookupExif(m_pqactGPSLongitudeRef,          "[$Ex-GPSLongitudeRef]");
    puirRenameUI->AddActionLookupExif(m_pqactGPSAltitude,              "[$Ex-GPSAltitude]");
    puirRenameUI->AddActionLookupExif(m_pqactGPSAltitudeRef,           "[$Ex-GPSAltitudeRef]");
    puirRenameUI->AddActionLookupExif(m_pqactGPSDateStamp,             "[$Ex-GPSDateStamp]");
    puirRenameUI->AddActionLookupExif(m_pqactGPSTimeStamp,             "[$Ex-GPSTimeStamp]");
}


void IUIMenuTags::CreateTagsActionsFileAttributes()
{   
    m_pqactFiCreatedDateTime    = new QAction(QString("%1\t%2").arg(tr("Created Date Time"))        .arg(PrintTagOS("[$At-CreatedDateTime]")), this);
    m_pqactFiCreatedDate        = new QAction(QString("%1\t%2").arg(tr("Created Date"))             .arg(PrintTagOS("[$At-CreatedDate]")), this);
    m_pqactFiCreatedTime        = new QAction(QString("%1\t%2").arg(tr("Created Time"))             .arg(PrintTagOS("[$At-CreatedTime]")), this);
    m_pqactFiCreatedDateYYYY    = new QAction(QString("%1\t%2").arg(tr("Created Date YYYY"))        .arg(PrintTagOS("[$At-CreatedDateYYYY]")), this);
    m_pqactFiCreatedDateYY      = new QAction(QString("%1\t%2").arg(tr("Created Date YY"))          .arg(PrintTagOS("[$At-CreatedDateYY]")), this);
    m_pqactFiCreatedDateMM      = new QAction(QString("%1\t%2").arg(tr("Created Date MM"))          .arg(PrintTagOS("[$At-CreatedDateMM]")), this);
    m_pqactFiCreatedDateDD      = new QAction(QString("%1\t%2").arg(tr("Created Date DD"))          .arg(PrintTagOS("[$At-CreatedDateDD]")), this);
    m_pqactFiCreatedTimeHH      = new QAction(QString("%1\t%2").arg(tr("Created Time HH"))          .arg(PrintTagOS("[$At-CreatedTimeHH]")), this);
    m_pqactFiCreatedTimeMM      = new QAction(QString("%1\t%2").arg(tr("Created Time MM"))          .arg(PrintTagOS("[$At-CreatedTimeMM]")), this);
    m_pqactFiCreatedTimeSS      = new QAction(QString("%1\t%2").arg(tr("Created Time SS"))          .arg(PrintTagOS("[$At-CreatedTimeSS]")), this);
    m_pqactFiModifiedDateTime   = new QAction(QString("%1\t%2").arg(tr("Modified Date Time"))       .arg(PrintTagOS("[$At-ModifiedDateTime]")), this);
    m_pqactFiModifiedDate       = new QAction(QString("%1\t%2").arg(tr("Modified Date"))            .arg(PrintTagOS("[$At-ModifiedDate]")), this);
    m_pqactFiModifiedTime       = new QAction(QString("%1\t%2").arg(tr("Modified Time"))            .arg(PrintTagOS("[$At-ModifiedTime]")), this);
    m_pqactFiModifiedDateYYYY   = new QAction(QString("%1\t%2").arg(tr("Modified Date YYYY"))       .arg(PrintTagOS("[$At-ModifiedDateYYYY]")), this);
    m_pqactFiModifiedDateYY     = new QAction(QString("%1\t%2").arg(tr("Modified Date YY"))         .arg(PrintTagOS("[$At-ModifiedDateYY]")), this);
    m_pqactFiModifiedDateMM     = new QAction(QString("%1\t%2").arg(tr("Modified Date MM"))         .arg(PrintTagOS("[$At-ModifiedDateMM]")), this);
    m_pqactFiModifiedDateDD     = new QAction(QString("%1\t%2").arg(tr("Modified Date DD"))         .arg(PrintTagOS("[$At-ModifiedDateDD]")), this);
    m_pqactFiModifiedTimeHH     = new QAction(QString("%1\t%2").arg(tr("Modified Time HH"))         .arg(PrintTagOS("[$At-ModifiedTimeHH]")), this);
    m_pqactFiModifiedTimeMM     = new QAction(QString("%1\t%2").arg(tr("Modified Time MM"))         .arg(PrintTagOS("[$At-ModifiedTimeMM]")), this);
    m_pqactFiModifiedTimeSS     = new QAction(QString("%1\t%2").arg(tr("Modified Time SS"))         .arg(PrintTagOS("[$At-ModifiedTimeSS]")), this);

    IUIRename* puirRenameUI = m_pmwMainWindow->GetRenameUI();
    puirRenameUI->AddActionLookupFiAt(m_pqactFiCreatedDateTime,        "[$At-CreatedDateTime]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiCreatedDate,            "[$At-CreatedDate]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiCreatedTime,            "[$At-CreatedTime]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiCreatedDateYYYY,        "[$At-CreatedDateYYYY]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiCreatedDateYY,          "[$At-CreatedDateYY]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiCreatedDateMM,          "[$At-CreatedDateMM]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiCreatedDateDD,          "[$At-CreatedDateDD]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiCreatedTimeHH,          "[$At-CreatedTimeHH]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiCreatedTimeMM,          "[$At-CreatedTimeMM]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiCreatedTimeSS,          "[$At-CreatedTimeSS]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiModifiedDateTime,       "[$At-ModifiedDateTime]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiModifiedDate,           "[$At-ModifiedDate]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiModifiedTime,           "[$At-ModifiedTime]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiModifiedDateYYYY,       "[$At-ModifiedDateYYYY]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiModifiedDateYY,         "[$At-ModifiedDateYY]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiModifiedDateMM,         "[$At-ModifiedDateMM]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiModifiedDateDD,         "[$At-ModifiedDateDD]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiModifiedTimeHH,         "[$At-ModifiedTimeHH]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiModifiedTimeMM,         "[$At-ModifiedTimeMM]");
    puirRenameUI->AddActionLookupFiAt(m_pqactFiModifiedTimeSS,         "[$At-ModifiedTimeSS]");
}


void IUIMenuTags::InitialiseMenu()
{
    m_pqmenuMusic = addMenu(tr("&Music"));
    m_pqmenuMusic->addAction(m_pqactTitle);
    m_pqmenuMusic->addAction(m_pqactArtist);
    m_pqmenuMusic->addAction(m_pqactAlbum);
    m_pqmenuMusic->addAction(m_pqactTrack);
    m_pqmenuMusic->addAction(m_pqactYear);
    m_pqmenuMusic->addAction(m_pqactGenre);
    m_pqmenuMusic->addAction(m_pqactComment);
    m_pqmenuMusic->addAction(m_pqactRunTime);
    m_pqmenuMusic->addAction(m_pqactChannels);
    m_pqmenuMusic->addAction(m_pqactSampleRate);
    m_pqmenuMusic->addAction(m_pqactBitRate);

    m_pqmenuExif = addMenu(tr("&Exif"));
    if (m_pmwMainWindow->GetFileListUI()->ExifAdvancedModeEnabled())
        InitExifMenuAdvanced();
    else
        InitExifMenuBasic();

    m_pqmenuAttributes = addMenu(tr("&Attributes"));
    m_pqmenuAttributes->addAction(m_pqactFiCreatedDateTime);
    m_pqmenuAttributes->addAction(m_pqactFiCreatedDate);
    m_pqmenuAttributes->addAction(m_pqactFiCreatedTime);
    m_pqmenuAttributes->addAction(m_pqactFiCreatedDateYYYY);
    m_pqmenuAttributes->addAction(m_pqactFiCreatedDateYY);
    m_pqmenuAttributes->addAction(m_pqactFiCreatedDateMM);
    m_pqmenuAttributes->addAction(m_pqactFiCreatedDateDD);
    m_pqmenuAttributes->addAction(m_pqactFiCreatedTimeHH);
    m_pqmenuAttributes->addAction(m_pqactFiCreatedTimeMM);
    m_pqmenuAttributes->addAction(m_pqactFiCreatedTimeSS);
    m_pqmenuAttributes->addAction(m_pqactFiModifiedDateTime);
    m_pqmenuAttributes->addAction(m_pqactFiModifiedDate);
    m_pqmenuAttributes->addAction(m_pqactFiModifiedTime);
    m_pqmenuAttributes->addAction(m_pqactFiModifiedDateYYYY);
    m_pqmenuAttributes->addAction(m_pqactFiModifiedDateYY);
    m_pqmenuAttributes->addAction(m_pqactFiModifiedDateMM);
    m_pqmenuAttributes->addAction(m_pqactFiModifiedDateDD);
    m_pqmenuAttributes->addAction(m_pqactFiModifiedTimeHH);
    m_pqmenuAttributes->addAction(m_pqactFiModifiedTimeMM);
    m_pqmenuAttributes->addAction(m_pqactFiModifiedTimeSS);
}


void IUIMenuTags::InitExifMenuBasic()
{
    m_pqmenuExif->clear();

    m_pqmenuExif->addAction(m_pqactDateTime);
    m_pqmenuExif->addAction(m_pqactDate);
    m_pqmenuExif->addAction(m_pqactTime);
    m_pqmenuExif->addAction(m_pqactDateYYYY);
    m_pqmenuExif->addAction(m_pqactDateYY);
    m_pqmenuExif->addAction(m_pqactDateMM);
    m_pqmenuExif->addAction(m_pqactDateDD);
    m_pqmenuExif->addAction(m_pqactTimeHH);
    m_pqmenuExif->addAction(m_pqactTimeMM);
    m_pqmenuExif->addAction(m_pqactTimeSS);
    m_pqmenuExif->addAction(m_pqactTimeSubSec);
    m_pqmenuExif->addAction(m_pqactCameraMake);
    m_pqmenuExif->addAction(m_pqactCameraModel);
    m_pqmenuExif->addAction(m_pqactFNumber);
    m_pqmenuExif->addAction(m_pqactISOSpeed);
    m_pqmenuExif->addAction(m_pqactExposureTime);
    m_pqmenuExif->addAction(m_pqactExposureTimeDec);
    m_pqmenuExif->addAction(m_pqactFocalLength);
    m_pqmenuExif->addAction(m_pqactFocalLength35mm);
    m_pqmenuExif->addAction(m_pqactExposureProgram);
    m_pqmenuExif->addAction(m_pqactPixelDimX);
    m_pqmenuExif->addAction(m_pqactPixelDimY);
}


void IUIMenuTags::InitExifMenuAdvanced()
{
    m_pqmenuExif->clear();

    m_pqmenuExifDateTime = m_pqmenuExif->addMenu(tr("&Date/Time"));
    m_pqmenuExifDateTime->addAction(m_pqactDateTime);
    m_pqmenuExifDateTime->addAction(m_pqactDate);
    m_pqmenuExifDateTime->addAction(m_pqactTime);
    m_pqmenuExifDateTime->addAction(m_pqactDateYYYY);
    m_pqmenuExifDateTime->addAction(m_pqactDateYY);
    m_pqmenuExifDateTime->addAction(m_pqactDateMM);
    m_pqmenuExifDateTime->addAction(m_pqactDateDD);
    m_pqmenuExifDateTime->addAction(m_pqactTimeHH);
    m_pqmenuExifDateTime->addAction(m_pqactTimeMM);
    m_pqmenuExifDateTime->addAction(m_pqactTimeSS);
    m_pqmenuExifDateTime->addAction(m_pqactTimeSubSec);

    m_pqmenuExifCamera = m_pqmenuExif->addMenu(tr("&Camera"));
    m_pqmenuExifCamera->addAction(m_pqactCameraMake);
    m_pqmenuExifCamera->addAction(m_pqactCameraModel);
    m_pqmenuExifCamera->addAction(m_pqactSoftware);

    m_pqmenuExifCameraSettings = m_pqmenuExif->addMenu(tr("Camera &Settings"));
    m_pqmenuExifCameraSettings->addAction(m_pqactFNumber);
    m_pqmenuExifCameraSettings->addAction(m_pqactISOSpeed);
    m_pqmenuExifCameraSettings->addAction(m_pqactExposureTime);
    m_pqmenuExifCameraSettings->addAction(m_pqactExposureTimeDec);
    m_pqmenuExifCameraSettings->addAction(m_pqactFocalLength);
    m_pqmenuExifCameraSettings->addAction(m_pqactFocalLength35mm);
    m_pqmenuExifCameraSettings->addAction(m_pqactDigitalZoomRatio);
    m_pqmenuExifCameraSettings->addAction(m_pqactExposureProgram);
    m_pqmenuExifCameraSettings->addAction(m_pqactFlashSetting);
    m_pqmenuExifCameraSettings->addAction(m_pqactOrientation);

    m_pqmenuExifAdvacedCameraSettings = m_pqmenuExif->addMenu(tr("&Advanced Settings"));
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactShutterSpeedValue);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactExposureBiasValue);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactBrightnessValue);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactApertureValue);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactMaxApertureValue);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactSceneCaptureType);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactCustomRendered);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactExposureMode);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactWhiteBalance);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactGainControl);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactContrast);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactSaturation);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactSharpness);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactMeteringMode);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactLightSource);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactFocalPlaneXRes);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactFocalPlaneYRes);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactFocalPlaneResUnit);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactSubjectDistanceRange);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactSensingMethod);
    m_pqmenuExifAdvacedCameraSettings->addAction(m_pqactSceneType);

    m_pqmenuExifImageInformation = m_pqmenuExif->addMenu(tr("Image I&nformation"));
    m_pqmenuExifImageInformation->addAction(m_pqactPixelDimX);
    m_pqmenuExifImageInformation->addAction(m_pqactPixelDimY);
    m_pqmenuExifImageInformation->addAction(m_pqactColorSpace);
    m_pqmenuExifImageInformation->addAction(m_pqactCompression);
    m_pqmenuExifImageInformation->addAction(m_pqactComponentsConfig);
    m_pqmenuExifImageInformation->addAction(m_pqactCompressedBPP);
    m_pqmenuExifImageInformation->addAction(m_pqactYCbCrPositioning);
    m_pqmenuExifImageInformation->addAction(m_pqactXResolution);
    m_pqmenuExifImageInformation->addAction(m_pqactYResolution);
    m_pqmenuExifImageInformation->addAction(m_pqactResolutionUnit);
    m_pqmenuExifImageInformation->addAction(m_pqactFileSource);
    m_pqmenuExifImageInformation->addAction(m_pqactExifVersion);
    m_pqmenuExifImageInformation->addAction(m_pqactFlashPixVersion);
    m_pqmenuExifImageInformation->addAction(m_pqactInteropIndex);
    m_pqmenuExifImageInformation->addAction(m_pqactInteropVersion);

    m_pqmenuExifImageDescription = m_pqmenuExif->addMenu(tr("Image D&escription"));
    m_pqmenuExifImageDescription->addAction(m_pqactXPTitle);
    m_pqmenuExifImageDescription->addAction(m_pqactXPSubject);
    m_pqmenuExifImageDescription->addAction(m_pqactXPAuthor);
    m_pqmenuExifImageDescription->addAction(m_pqactXPComment);
    m_pqmenuExifImageDescription->addAction(m_pqactXPKeywords);
    m_pqmenuExifImageDescription->addAction(m_pqactCopyright);

    m_pqmenuExifGPS = m_pqmenuExif->addMenu(tr("&GPS"));
    m_pqmenuExifGPS->addAction(m_pqactGPSLatitude);
    m_pqmenuExifGPS->addAction(m_pqactGPSLatitudeRef);
    m_pqmenuExifGPS->addAction(m_pqactGPSLongitude);
    m_pqmenuExifGPS->addAction(m_pqactGPSLongitudeRef);
    m_pqmenuExifGPS->addAction(m_pqactGPSAltitude);
    m_pqmenuExifGPS->addAction(m_pqactGPSAltitudeRef);
    m_pqmenuExifGPS->addAction(m_pqactGPSDateStamp);
    m_pqmenuExifGPS->addAction(m_pqactGPSTimeStamp);
}

#ifdef Q_OS_MACOS
QString IUIMenuTags::PrintTagOS(const QString & /*krqstrTag*/)
{
    return QString();
}
#else
QString IUIMenuTags::PrintTagOS(const QString & krqstrTag)
{
    return krqstrTag;
}
#endif


void IUIMenuTags::SetExifMode(const bool kbExifAdvancedMode)
{
    if (kbExifAdvancedMode)
        InitExifMenuAdvanced();
    else
        InitExifMenuBasic();
}
