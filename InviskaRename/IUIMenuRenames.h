#ifndef IUIMenuRenames_h
#define IUIMenuRenames_h

#include "IUIMenuSavesBase.h"
class QComboBox;
class IUIMainWindow;
class IUIRename;


class IUIMenuRenames : public IUIMenuSavesBase
{
    Q_OBJECT

private:
    // For applying and saving rename settings
    IUIRename*              m_puirRenameUI;

    // Rename menu actions
    QAction*                m_pqactSaveCurrentSettings;
    QAction*                m_pqactOrganiseSavedRenames;
    QAction*                m_pqactClearRenameSettings;

public:
    IUIMenuRenames(IUIMainWindow* pmwMainWindow);
    ~IUIMenuRenames();

    // Creates actions for menu
    void CreateActionsRenames();

private:
    // Creates menus and adds actions
    void InitialiseMenu();

    // Returns the data string for storing in the QAction data variant
    QString GetDataString(const QString & krqstrSaveString);

    // Returns action save string for saving to QSettings
    QString GetActionSaveString(const QAction* const kpactAction);

public:
    // Saves the current rename settings under the passed name
    void SaveAction(const QString & krqstrName);

    // Updates the passed action with the current rename settings
    void UpdateAction(QAction* pqactAction, const QString & krqstrName);

    // Populutes the list of saved renmes in the passed compbo box
    void PopulateRenameCombo(QComboBox* pqcomRenames, const QString & krqstrSetValue);

private slots:
    // Loads the saved rename settings stored in the sending QAction's data varient
    void LoadData();

    // Displays dialog prompting user for name and saves settings
    void ShowSaveRenameSettingsDialog();

    // Displays dialog allowing user to reorganise and delete saved rename settings
    void ShowOrganiseSavedRenamesDialog();

private:
    // Loads renames using old save format, converts them to the new format and removes them from the settings file
    void LoadLegacyRenames();
};

#endif // IUIMenuRenames_h
