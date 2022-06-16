#ifndef IComUIMenuBarBase_h
#define IComUIMenuBarBase_h

#include <QObject>
class IUIMainWindow;
class QMenu;
class QAction;


class IComUIMenuBarBase : public QObject
{
    Q_OBJECT

protected:
    // Main window
    IUIMainWindow*              m_pmwMainWindow;

    // File Menu
    QMenu*                      m_pqmenuFile;
    QAction*                    m_pqactPreferences;
    QAction*                    m_pqactExit;

    // Help
    QMenu*                      m_pqmenuHelp;
    QAction*                    m_pqactInstructions;
    QAction*                    m_pqactWebsite;
    QAction*                    m_pqactForum;
    QAction*                    m_pqactChangelog;
    QAction*                    m_pqactHelpAbout;

protected:
    IComUIMenuBarBase(IUIMainWindow* pmwMainWindow);

protected:
    // Creates actions present in all applications
    virtual void CreateActions();

    // Creates credits action, which is only present in some applications
    void CreateCreditsAction();

    // Creates menus and adds actions
    virtual void InitialiseMenuBar();

private slots:
    // Menu items action functions
    void ShowPreferencesDialog();
    void ShowInstructions();
    void ShowWebsite();
    void ShowForum();
    void ShowChangelog();
    void ShowHelpAboutDialog();

public:
    // Returns credits for display in About dialog
    virtual QString GetCreditsTranslations()    {return QString();}
    virtual QString GetCreditsCodeContrib()     {return QString();}
    virtual QString GetCreditsDependency()      {return QString();}
};

#endif // IComUIMenuBarBase_h
