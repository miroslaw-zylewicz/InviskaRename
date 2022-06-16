#ifndef IDlgOrganiseMenu_h
#define IDlgOrganiseMenu_h

#include <QDialog>
#include <QVector>
class QListWidget;
class QPushButton;
class QLineEdit;
class QListWidgetItem;
class QDialogButtonBox;
class IUIMenuSavesBase;


class IDlgOrganiseMenu : public QDialog
{
    Q_OBJECT

private:
    // Pointer to menu to be reorganised
    IUIMenuSavesBase*           m_pimenuMenu;

    // List of menu items
    QListWidget*                m_pqlwMenuItems;

    // Buttons for organising actions
    QPushButton*                m_qpbMoveUp;
    QPushButton*                m_qpbMoveDown;
    QPushButton*                m_qpbRename;
    QPushButton*                m_qpbDelete;

    // Displays details about the selected entry in the list widget if appropriate
    QLineEdit*                  m_pqleDetails;

    // OK and cancel buttons
    QDialogButtonBox*           m_qdbbOKCancel;

    // List of actions the user has selected to delete
    QVector<QAction*>           m_qvecpqactActionsToDelete;

    // Indicates if changes have been made so we know if the menu needs reconstructing
    bool                        m_bChangesMade;

public:
    // Enum to indicate type of menu being organised
    enum MenuType               {Bookmarks, Renames};

public:
    IDlgOrganiseMenu(IUIMenuSavesBase* pimenuMenu, const QString & krqstrTitle, const MenuType iMenuType);

private:
    // Populates list with actions from menu
    void PopulateActionList();

private slots:
    // Slots for performing button actions
    void MoveUp();
    void MoveDown();
    void Rename();
    void Delete();

public:
    // Checks if entry already exists in Menu Items list widget
    bool EntryExists(const QString & krqstrEntryName);

    // Calls IUIMenuSavesBase to update name of passed action
    void UpdateActionName(QAction* pqactAction, const QString & krqstrName);

public slots:
    // Shows details of currently selected QListWidget in the details line edit
    void ShowSelectedItemDetails();

    // Override accept() function to process settings when Save is pressed
    void accept();
};

#endif // IDlgOrganiseMenu_h
