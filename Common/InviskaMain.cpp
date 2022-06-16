#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QLibraryInfo>
#include <QProcessEnvironment>
#include "IUIMainWindow.h"
#include "IComSysSingleInstance.h"
#include "IComSysIniFilePath.h"


int main(int argc, char* argv[])
{
    #ifdef Q_OS_LINUX
    if (QProcessEnvironment::systemEnvironment().value("XDG_CURRENT_DESKTOP").startsWith("KDE") == false)
        qputenv("QT_QPA_PLATFORMTHEME", "gtk2");
    #endif

    QString qstrAppName        = IUIMainWindow::GetAppName();
    QString qstrAppNameNoSpace = IUIMainWindow::GetAppNameNoSpaces();

    QCoreApplication::setOrganizationName("Inviska");
    QCoreApplication::setOrganizationDomain("inviska.com");
    QCoreApplication::setApplicationName(qstrAppName);
    QCoreApplication::setApplicationVersion(IUIMainWindow::GetVersion());
    QApplication qaApplication(argc, argv);

    QSettings qsetSettings(IComSysIniFilePath::GetSettingsFilePath(qstrAppNameNoSpace), QSettings::IniFormat);
    IComSysSingleInstance snglSingleInstance(qstrAppNameNoSpace);
    if (qsetSettings.value("Application/SingleInstanceOnly", false).toBool())
    {
        if (snglSingleInstance.TryToRun() == false)
            return 0;
    }

    QTranslator qtransApp, qtransQt;
    QString qstrLanguage = qsetSettings.value("Application/Language", "").toString();
    if (qstrLanguage.isEmpty())
    {
        QLocale qlocLocale;
        //if (qtransQt.load(qlocLocale, "qt", "_", QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        if (qtransQt.load(qlocLocale, "qt", "_", ":/Translations"))
            qaApplication.installTranslator(&qtransQt);
        if (qtransApp.load(qlocLocale, qstrAppNameNoSpace, ".", ":/Translations"))
            qaApplication.installTranslator(&qtransApp);
    }
    else if (qstrLanguage.startsWith("en_") == false)
    {
        //if (qtransQt.load("qt_" + qstrLanguage, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        if (qtransQt.load("qt_" + qstrLanguage, ":/Translations"))
            qaApplication.installTranslator(&qtransQt);
        if (qtransApp.load(qstrAppNameNoSpace + "." + qstrLanguage, ":/Translations"))
            qaApplication.installTranslator(&qtransApp);
    }

    IUIMainWindow mwMainWin(qsetSettings, snglSingleInstance);
    mwMainWin.show();

    return qaApplication.exec();
}


