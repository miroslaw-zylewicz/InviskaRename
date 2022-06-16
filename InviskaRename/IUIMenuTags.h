#ifndef IUIMenuTags_h
#define IUIMenuTags_h

#include <QMenu>
class IUIMainWindow;


class IUIMenuTags : public QMenu
{
    Q_OBJECT

private:
    // Main window
    IUIMainWindow*      m_pmwMainWindow;

    // Tags Music
    QMenu*              m_pqmenuMusic;  
    QAction*            m_pqactTitle;
    QAction*            m_pqactArtist;
    QAction*            m_pqactAlbum;
    QAction*            m_pqactTrack;
    QAction*            m_pqactYear;
    QAction*            m_pqactGenre;
    QAction*            m_pqactComment;
    QAction*            m_pqactRunTime;
    QAction*            m_pqactChannels;
    QAction*            m_pqactSampleRate;
    QAction*            m_pqactBitRate;

    // Tags Exif
    QMenu*              m_pqmenuExif;
    QMenu*              m_pqmenuExifCamera;
    QAction*            m_pqactCameraMake;
    QAction*            m_pqactCameraModel;
    QAction*            m_pqactSoftware;

    QMenu*              m_pqmenuExifDateTime;
    QAction*            m_pqactDateTime;
    QAction*            m_pqactDate;
    QAction*            m_pqactTime;
    QAction*            m_pqactDateYYYY;
    QAction*            m_pqactDateYY;
    QAction*            m_pqactDateMM;
    QAction*            m_pqactDateDD;
    QAction*            m_pqactTimeHH;
    QAction*            m_pqactTimeMM;
    QAction*            m_pqactTimeSS;
    QAction*            m_pqactTimeSubSec;

    QMenu*              m_pqmenuExifCameraSettings;
    QAction*            m_pqactFNumber;
    QAction*            m_pqactISOSpeed;
    QAction*            m_pqactExposureTime;
    QAction*            m_pqactExposureTimeDec;
    QAction*            m_pqactFocalLength;
    QAction*            m_pqactFocalLength35mm;
    QAction*            m_pqactDigitalZoomRatio;
    QAction*            m_pqactExposureProgram;
    QAction*            m_pqactFlashSetting;
    QAction*            m_pqactOrientation;

    QMenu*              m_pqmenuExifAdvacedCameraSettings;
    QAction*            m_pqactShutterSpeedValue;
    QAction*            m_pqactMaxApertureValue;
    QAction*            m_pqactExposureBiasValue;
    QAction*            m_pqactBrightnessValue;
    QAction*            m_pqactApertureValue;
    QAction*            m_pqactSceneCaptureType;
    QAction*            m_pqactCustomRendered;
    QAction*            m_pqactExposureMode;
    QAction*            m_pqactWhiteBalance;
    QAction*            m_pqactGainControl;
    QAction*            m_pqactContrast;
    QAction*            m_pqactSaturation;
    QAction*            m_pqactSharpness;
    QAction*            m_pqactMeteringMode;
    QAction*            m_pqactLightSource;
    QAction*            m_pqactFocalPlaneXRes;
    QAction*            m_pqactFocalPlaneYRes;
    QAction*            m_pqactFocalPlaneResUnit;
    QAction*            m_pqactSubjectDistanceRange;
    QAction*            m_pqactSensingMethod;
    QAction*            m_pqactSceneType;

    QMenu*              m_pqmenuExifImageInformation;
    QAction*            m_pqactPixelDimX;
    QAction*            m_pqactPixelDimY;
    QAction*            m_pqactColorSpace;
    QAction*            m_pqactCompression;
    QAction*            m_pqactComponentsConfig;
    QAction*            m_pqactCompressedBPP;
    QAction*            m_pqactYCbCrPositioning;
    QAction*            m_pqactXResolution;
    QAction*            m_pqactYResolution;
    QAction*            m_pqactResolutionUnit;
    QAction*            m_pqactFileSource;
    QAction*            m_pqactExifVersion;
    QAction*            m_pqactFlashPixVersion;
    QAction*            m_pqactInteropIndex;
    QAction*            m_pqactInteropVersion;

    QMenu*              m_pqmenuExifImageDescription;
    QAction*            m_pqactXPTitle;
    QAction*            m_pqactXPSubject;
    QAction*            m_pqactXPAuthor;
    QAction*            m_pqactXPComment;
    QAction*            m_pqactXPKeywords;
    QAction*            m_pqactCopyright;

    QMenu*              m_pqmenuExifGPS;
    QAction*            m_pqactGPSLatitude;
    QAction*            m_pqactGPSLatitudeRef;
    QAction*            m_pqactGPSLongitude;
    QAction*            m_pqactGPSLongitudeRef;
    QAction*            m_pqactGPSAltitude;
    QAction*            m_pqactGPSAltitudeRef;
    QAction*            m_pqactGPSDateStamp;
    QAction*            m_pqactGPSTimeStamp;

    // Tags File Attributes
    QMenu*              m_pqmenuAttributes;
    QAction*            m_pqactFiCreatedDateTime;
    QAction*            m_pqactFiCreatedDate;
    QAction*            m_pqactFiCreatedTime;
    QAction*            m_pqactFiCreatedDateYYYY;
    QAction*            m_pqactFiCreatedDateYY;
    QAction*            m_pqactFiCreatedDateMM;
    QAction*            m_pqactFiCreatedDateDD;
    QAction*            m_pqactFiCreatedTimeHH;
    QAction*            m_pqactFiCreatedTimeMM;
    QAction*            m_pqactFiCreatedTimeSS;
    QAction*            m_pqactFiModifiedDateTime;
    QAction*            m_pqactFiModifiedDate;
    QAction*            m_pqactFiModifiedTime;
    QAction*            m_pqactFiModifiedDateYYYY;
    QAction*            m_pqactFiModifiedDateYY;
    QAction*            m_pqactFiModifiedDateMM;
    QAction*            m_pqactFiModifiedDateDD;
    QAction*            m_pqactFiModifiedTimeHH;
    QAction*            m_pqactFiModifiedTimeMM;
    QAction*            m_pqactFiModifiedTimeSS;

public:
    IUIMenuTags(IUIMainWindow* pmwMainWindow);

    // Calls below create action fuctions
    void CreateActionsTags();

private:
    // Creates actions for menus
    void CreateTagsActionsMusic();
    void CreateTagsActionsExif();
    void CreateTagsActionsFileAttributes();

    // Creates menus and adds actions
    void InitialiseMenu();
    void InitExifMenuBasic();
    void InitExifMenuAdvanced();

    // Including the tag in the menu item on MacOS causes a question mark to display instead of the tag
    // We therefore don't want to include the tag in the menu item on MacOS, so this function returns an empty string on the Mac
    QString PrintTagOS(const QString & krqstrTag);

public:
    // Recrates the Exif menu in appropriate mode
    void SetExifMode(const bool kbExifAdvancedMode);
};

#endif // IUIMenuTags_h
