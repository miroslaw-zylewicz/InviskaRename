#ifndef IComSysIniFilePath_h
#define IComSysIniFilePath_h

#include <QString>


class IComSysIniFilePath
{
public:
    // Returns the path to use for the settings file
    static QString GetSettingsFilePath(const QString & krqstrAppName);

private:
    // Returns the standard path for storing setting files for the current operating system
    static QString GetStandardSettingsPath();

    // Returns the path to use for the settings file when application is running in an AppImage
    static QString GetAppImageSettingsPath();

    // Returns true if application is installed
    static bool AppIsInstalled(const QString & krqstrAppDir);

    // Indicates if application is running in an AppImage
    static bool AppIsInAppImage(const QString & krqstrAppDir);
};

#endif // IComSysIniFilePath_h
