#ifndef IUIMenuBar_h
#define IUIMenuBar_h

#include <QVector>
#include "IComUIMenuBarBase.h"
class QToolBar;
class IUISideBar;
class IUIMenuTags;
class IUIMenuRenames;
class IUIMenuBookmarks;


class IUIMenuBar : public IComUIMenuBarBase
{
    Q_OBJECT

private:
    // File Menu
    QAction*                m_pqactOpenDirectory;
    QAction*                m_pqactViewFileMetaTags;
    QAction*                m_pqactCaseSensitive;

    // Navigate
    QMenu*                  m_pqmenuNavigate;
    QAction*                m_pqactNavigateBack;
    QAction*                m_pqactNavigateForward;
    QAction*                m_pqactNavigateUp;
    QAction*                m_pqactNavigateRefresh;
    QAction*                m_pqactNavigateStartDir;
    QAction*                m_pqactShowHiddenFiles;
    QAction*                m_pqactMoveSelectedLinesUp;
    QAction*                m_pqactMoveSelectedLinesDown;

    // Other menus
    IUIMenuTags*            m_pimenuTags;
    IUIMenuRenames*         m_pimenuRenames;
    IUIMenuBookmarks*       m_pimenuBookmarks;

public:
    IUIMenuBar(IUIMainWindow* pmwMainWindow);

private:
    // Creates actions for menus
    void CreateActions();
    void CreateActionsFile();
    void CreateActionsNavigate();

    // Creates menus and adds actions
    void InitialiseMenuBar();

public:
    // Adds navigation actions to navigation toolbar
    void AddNavigationActions(QToolBar* pqtpNavigationToolBar);
    void AddPreferencesActions(QToolBar* pqtpNavigationToolBar);

    // Adds actions to side bar
    void AddSideBarActions(IUISideBar* puisbSideBar);

    // Enables/disables navigation actions depending on current directory
    void EnableBackAction(const bool kbEnable);
    void EnableForwardAction(const bool kbEnable);
    void EnableUpAction(const bool kbEnable);

    // Returns true if shown hidden files option is checked
    bool ShowHiddenFiles();

    // Returns true if case sensitive compare option is checked
    bool CaseSensitive();
    void SetCaseSensitive(const bool kbCaseSensitive);

    // Returns credits for display in credits dialog
    QString GetCreditsTranslations();
    QString GetCreditsDependency();

    // Accessors
    IUIMenuTags*        TagsMenu()      {return m_pimenuTags;}
    IUIMenuRenames*     RenamesMenu()   {return m_pimenuRenames;}
    IUIMenuBookmarks*   BookmarksMenu() {return m_pimenuBookmarks;}
};

#endif // IUIMenuBar_h
