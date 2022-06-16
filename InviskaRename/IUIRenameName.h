#ifndef UIRenameName_h
#define UIRenameName_h

#include <QList>
#include "IUIRenameTabBase.h"
#include "ui_UIRenameName.h"


class IUIRenameName : public IUIRenameTabBase, public Ui::UIRenameName
{
    Q_OBJECT

private:
    // Lists of meta tags present in line edits
    QList<ITagInfo>             m_qlstReplaceNameTags;
    QList<ITagInfo>             m_qlstReplaceTheTextWithTags;
    QList<ITagInfo>             m_qlstInsertTheTextTags;
    QList<ITagInfo>             m_qlstInsertAtStartTags;
    QList<ITagInfo>             m_qlstInsertAtEndTags;

    // Saves number values from line edits as integers to save repeatedly converting the string to an int during renames
    int                         m_iInsertTheTextAtPos;
    int                         m_iCropAtPos;
    int                         m_iCropAtPosNextNChar;
    int                         m_iLeftCropNChar;
    int                         m_iRightCropNChar;

    // Values for Change Case combo box
    enum                        ChangeCase {CaseNoChange, CaseTitle, CaseSentance, CaseLower, CaseUpper};

public:
    IUIRenameName(IUIRename* puirRenameUI, const int kiRenameElement, const int kiTabID);

private:
    // Sets validators for line edits
    void SetValidators();

    // Create connections to indicate when settings change
    void CreateConnections();

private slots:
    // Reads updated settings from UI and calls update on file viewer
    void SettingsChangedReplaceName();
    void SettingsChangedReplaceTheText();
    void SettingsChangedInsertTheText();
    void SettingsChangedInsertAtStart();
    void SettingsChangedInsertAtEnd();
    void SettingsChangedCropAtPos();
    void SettingsChangedLeftCrop();
    void SettingsChangedRightCrop();
    void SetingsChanged();

public:
    // Initialises re-read of all tag codes from line edits
    void ReReadTagCodes();

    // Disables all settings but doesn't clear line edits
    void DisableAllSettings();

    // Disables all settings and clears line edits
    void ClearAll();

    // Checks if there are music or Exif tags in any of the boxes and sets passed flags accordingly
    void CheckForMetaTags(bool & rbMusicTags, bool & rbExifTags);

    // Generates name using current settings
    void GenerateName(QString & rqstrName, QTableWidgetItem* pqtwiFileItem);
    void ConvertNameToTitleCase(QString & rqstrName);

    // Adds settings to stringlist
    void SaveSettings(const QString & krqstrSection, QStringList & rqstrlSettings);

    // Restores settings from passed save string
    void RestoreSettings(const QString & krqstrSettings, int & riIndex);

private:
    // Restors individual settings
    void RestoreCheckBox(const QString & krqstrTag);
    void RestoreLineEdit(const QString & krqstrTag, const QString & krqstrValue);
    void RestoreComboBox(const QString & krqstrTag, const QString & krqstrValue);
};

#endif // UIRenameName_h
