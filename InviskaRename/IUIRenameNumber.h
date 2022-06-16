#ifndef UIRenameNumber_h
#define UIRenameNumber_h

#include "IUIRenameTabBase.h"
#include "ui_UIRenameNumber.h"
class IUIRename;


class IUIRenameNumber : public IUIRenameTabBase, public Ui::UIRenameNumber
{
    Q_OBJECT

private:
    // Stores the default value for all QLineEdits apart from Numbering...At Position
    QString                     m_qstrLineEditDefault;

    // Number to use for next file
    int                         m_iCurrentNumber;

    // Increment for numbering
    int                         m_iIncrement;

    // Number of characters the number will take up with zero padding
    int                         m_iNumberCharWidth;

    // Position at which to insert the number
    int                         m_iNumberingAtPos;

public:
    IUIRenameNumber(IUIRename* puirRenameUI, const int kiRenameElement, const int kiTabID);

private:
    // Sets validators for line edits
    void SetValidators();

    // Create connections to indicate when settings change
    void CreateConnections();

private slots:
    // Triggers regeneration of name when settings change
    void RadioSettingsChanged(const bool kbEnabled);
    void SettingsChanged();

    // Sets Zero Fill line edit enabled/disabled depending on state of other radio buttons
    void EnableDisableZeroFillLineEdit();

public:
    // Disables all settings but doesn't clear line edits
    void DisableAllSettings();

    // Disables all settings and clears line edits
    void ClearAll();

    // Initialises variables for rename operation based on current settings andumber of files to be renamed
    void InitNumberingVals(const int kiNumFilesToRename);

    // Inserts nubering into passed name
    void GenerateName(QString & rqstrName);

    // Adds settings to stringlist
    void SaveSettings(const QString & krqstrSection, QStringList & rqstrlSettings);

    // Restors settings from passed save string
    void RestoreSettings(const QString & krqstrSettings, int & riIndex);

private:
    // Restors individual settings
    void RestoreRadioBox(const QString & krqstrTag);
    void RestoreLineEdit(const QString & krqstrTag, const QString & krqstrValue);

public:
    // Indicates if auto-numbering is enabled
    bool Numberingenabled() {return !m_pqrbNumberingNoNumber->isChecked();}
};

#endif // UIRenameNumber_h
