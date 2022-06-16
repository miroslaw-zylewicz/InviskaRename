#ifndef IUIMenuBookmarks_h
#define IUIMenuBookmarks_h

#include "IUIMenuSavesBase.h"
class IUIMainWindow;


class IUIMenuBookmarks : public IUIMenuSavesBase
{
    Q_OBJECT

private:
    // Bookmars menu actions
    QAction*                m_pqactBookmarkFolder;
    QAction*                m_pqactOrganiseBookmars;

public:
    IUIMenuBookmarks(IUIMainWindow* pmwMainWindow);
    ~IUIMenuBookmarks();

    // Creates actions for menu
    void CreateActionsBookmarks();

private:
    // Creates menus and adds actions
    void InitialiseMenu();

    // Returns the data string for storing in the QAction data variant
    QString GetDataString(const QString & krqstrSaveString);

    // Returns action save string for saving to QSettings
    QString GetActionSaveString(const QAction* const kpactAction);

public:
    // Bookmarks current directory using the passed name
    void SaveAction(const QString & krqstrName);

    // Updates the passed action with the current directory
    void UpdateAction(QAction* pqactAction, const QString & krqstrName);

private slots:
    // Loads the bookmared directory stored in the sending QAction's data varient
    void LoadData();

    // Displays dialog prompting user for name and saves bookmark
    void ShowSaveBookmarkDialog();

    // Displays dialog allowing user to reorganise and delete bookmarks
    void ShowOrganiseBookmarksDialog();
};

#endif // IUIMenuBookmarks_h
