#include <QCoreApplication>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include "IComSysIniFilePath.h"


QString IComSysIniFilePath::GetSettingsFilePath(const QString & krqstrAppName)
{
    const QString kqstrAppDir = QCoreApplication::applicationDirPath();

    QString qstrSettingsDir;
    if (AppIsInstalled(kqstrAppDir))
        qstrSettingsDir = GetStandardSettingsPath();
    else if (AppIsInAppImage(kqstrAppDir))
        qstrSettingsDir = GetAppImageSettingsPath();
    else
        qstrSettingsDir = kqstrAppDir;

    return qstrSettingsDir + '/' + krqstrAppName + ".ini";
}


QString IComSysIniFilePath::GetStandardSettingsPath()
{
    // On Windows we use AppDataLocation (%USERPROFILE%\AppData\Roaming) because AppConfigLocation returns %USERPROFILE%\AppData\Local so settings wouldn't move with profile

    #ifdef Q_OS_WIN
    QString qstrIniFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    #else
    QString qstrIniFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    #endif

    // We cut off the app name at the end so all the inviska applications put their settings file in the same directory
    return qstrIniFilePath.left(qstrIniFilePath.lastIndexOf('/'));
}


QString IComSysIniFilePath::GetAppImageSettingsPath()
{
    // Portable version includes Portable.sys in the directory to indicate it should run in portable mode
    if (QFileInfo::exists("./Portable.sys"))
        return "./";

    // AppImage documenation says QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation) doesn't work reliably so we don't use it
    return QDir::homePath() + "/.config/Inviska";
}


bool IComSysIniFilePath::AppIsInstalled(const QString & krqstrAppDir)
{
    // On windows and Mac we include an Install.sys file in the executable directory to indicate it should run in install mode.
    // We can't do that on Linux so we just check if it is installed to /usr/bin

    #if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
    return QFileInfo::exists(krqstrAppDir + "/Install.sys");
    #else
    return (krqstrAppDir == "/usr/bin");
    #endif
}


bool IComSysIniFilePath::AppIsInAppImage(const QString & krqstrAppDir)
{
    return krqstrAppDir.startsWith("/tmp/.mount_Inv");
}
