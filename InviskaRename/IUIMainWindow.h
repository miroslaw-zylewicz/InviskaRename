#ifndef IUIMainWindow_h
#define IUIMainWindow_h

#define DFLT_WIN_WIDTH      1000
#define DFLT_WIN_HEIGHT     720

#include "IComUIMainWinBase.h"
class QVBoxLayout;
class QRegExpValidator;
class QIntValidator;
class IUIFileList;
class IUIRename;
class IUIToolBar;
class IUISideBar;


class IUIMainWindow : public IComUIMainWinBase
{
    Q_OBJECT

private:
    // Static pointer to self for easy access to main window from anywhere in the program
    static IUIMainWindow*       m_spmwMainWindow;

    // File list which displays the files' current and preivew name and handles file renaming
    IUIFileList*                m_puifmFileList;

    // Bottom rename settings widget
    IUIRename*                  m_puirRenameUI;

    // Navigation toolbar for navigating between directories
    IUIToolBar*                 m_puitbToolBar;

    // Side bar with arrows for re-ordering file list
    IUISideBar*                 m_puisbSideBar;

    // Validator that excludes characters that cannot be used in file names on the host operating system
    QRegExpValidator*           m_pqrevInvalidCharValidator;

    // Validator for line edits that should accept integars only
    QIntValidator*              m_pivalIntOnlyValidator;

public:
    IUIMainWindow(QSettings & rqsetSettings, IComSysSingleInstance & rsnglSingleInstance);

private:
    // Create regular expressions that are commonly used through program
    void CreateRegExpValidtors();

    // Save current directory and rename settings
    void SaveSettings();

private slots:
    // Called by a zero ms timer, this gives the address bar focus once everything else is created
    void FocusAddressBar();

public:
    // Enables or disables side bar
    void SetSideBarEnabled(const bool kbEnabled);
    bool SideBarEnabled();

    // Populate language list with translations
    void PopulateLanguageList(QComboBox* pqcboLanguages);

    // Accessor functions
    IUIFileList* &          GetFileListUI()             {return m_puifmFileList;}
    IUIRename* &            GetRenameUI()               {return m_puirRenameUI;}
    IUIToolBar* &           GetToolBar()                {return m_puitbToolBar;}
    QRegExpValidator*       GetInvalidCharValidator()   {return m_pqrevInvalidCharValidator;}
    QIntValidator*          GetIntOnlyValidator()       {return m_pivalIntOnlyValidator;}

    // Static accessor to main window for easy access from anywhere in the program
    static IUIMainWindow*   GetMainWindow()             {return IUIMainWindow::m_spmwMainWindow;}
};

#endif // IUIMainWindow_h
