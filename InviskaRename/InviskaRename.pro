QT += core gui widgets network
TEMPLATE = app

VERSION = 12.0
NAME = "Inviska Rename"
NAME_NO_SPACES = "InviskaRename"
LINUX_TARGET = invren

INCLUDEPATH += ../Common ../../Libs/TagLib/include ../../Libs/LibExif/include
DEPENDPATH  += ../Common ../../Libs/TagLib/include ../../Libs/LibExif/include

LIBS += \
    $$PWD/../../Libs/TagLib/lib/libtag.a \
    $$PWD/../../Libs/LibExif/libexif/.libs/libexif.a

DEFINES += TAGLIB_STATIC

HEADERS += \
    ../Common/IComDlgFileProperties.h \
    ../Common/IComDlgHelpAbout.h \
    ../Common/IComDlgProgress.h \
    ../Common/IComMetaExif.h \
    ../Common/IComMetaMusic.h \
    ../Common/IComQLineEdit.h \
    ../Common/IComSysAbsoluteDay.h \
    ../Common/IComSysIniFilePath.h \
    ../Common/IComSysLatestVersion.h \
    ../Common/IComSysSingleInstance.h \
    ../Common/IComSysSorts.h \
    ../Common/IComUIMainWinBase.h \
    ../Common/IComUIMenuBarBase.h \
    ../Common/IComUIPrefGeneral.h \
    ../Common/IComUtilityFuncs.h \
    ../Common/IComWdgtMetaExif.h \
    ../Common/IComWdgtMetaMusic.h \
    IDlgOrganiseMenu.h \
    IDlgPreferences.h \
    IDlgRenameErrorList.h \
    IDlgRenameBase.h \
    IDlgRenameFile.h \
    IDlgRenameMenuItem.h \
    IDlgSaveAction.h \
    IMetaAttrib.h \
    IMetaBase.h \
    IMetaExif.h \
    IMetaMusic.h \
    IMetaTagLookup.h \
    IRenameInvalidCharSub.h \
    IRenameLegacySave.h \
    ISysFileInfoSort.h \
    ISysFileInfoSortClasses.h \
    IUIFileList.h \
    IUIMainWindow.h \
    IUIMenuBar.h \
    IUIMenuBookmarks.h \
    IUIMenuRenames.h \
    IUIMenuSavesBase.h \
    IUIMenuTags.h \
    IUIRenameFilter.h \
    IUIRenameName.h \
    IUIRenameNumber.h \
    IUIRenameRegEx.h \
    IUIRenameTabBase.h \
    IUIRename.h \
    IUISideBar.h \
    IUIToolBar.h

SOURCES += \
    ../Common/IComDlgFileProperties.cpp \
    ../Common/IComDlgHelpAbout.cpp \
    ../Common/IComDlgProgress.cpp \
    ../Common/IComMetaExif.cpp \
    ../Common/IComMetaMusic.cpp \
    ../Common/IComQLineEdit.cpp \
    ../Common/IComSysAbsoluteDay.cpp \
    ../Common/IComSysIniFilePath.cpp \
    ../Common/IComSysLatestVersion.cpp \
    ../Common/IComSysSingleInstance.cpp \
    ../Common/IComUIMainWinBase.cpp \
    ../Common/IComUIMenuBarBase.cpp \
    ../Common/IComUIPrefGeneral.cpp \
    ../Common/IComUtilityFuncs.cpp \
    ../Common/IComWdgtMetaExif.cpp \
    ../Common/IComWdgtMetaMusic.cpp \
    ../Common/InviskaMain.cpp \
    IDlgOrganiseMenu.cpp \
    IDlgPreferences.cpp \
    IDlgRenameErrorList.cpp \
    IDlgRenameBase.cpp \
    IDlgRenameFile.cpp \
    IDlgRenameMenuItem.cpp \
    IDlgSaveAction.cpp \
    IMetaAttrib.cpp \
    IMetaBase.cpp \
    IMetaExif.cpp \
    IMetaMusic.cpp \
    IMetaTagLookup.cpp \
    IRenameInvalidCharSub.cpp \
    IRenameLegacySave.cpp \
    ISysFileInfoSort.cpp \
    ISysFileInfoSortClasses.cpp \
    IUIFileList.cpp \
    IUIMainWindow.cpp \
    IUIMenuBar.cpp \
    IUIMenuBookmarks.cpp \
    IUIMenuRenames.cpp \
    IUIMenuSavesBase.cpp \
    IUIMenuTags.cpp \
    IUIRenameFilter.cpp \
    IUIRenameName.cpp \
    IUIRenameNumber.cpp \
    IUIRenameRegEx.cpp \
    IUIRenameTabBase.cpp \
    IUIRename.cpp \
    IUISideBar.cpp \
    IUIToolBar.cpp

FORMS += \
    ../Common/UIComPrefGeneral.ui \
    UIPreferences.ui \
    UIRenameName.ui \
    UIRenameNumber.ui \
    UIRenameRegEx.ui

RESOURCES += \
    Resources.qrc

TRANSLATIONS += \
    ./Translations/InviskaRename.pt_BR.ts

include(../Common/Inviska.pri)

unix: {
    # Required to avoid linker error with static TagLib on Mac and Linux
    LIBS += -lz
}

