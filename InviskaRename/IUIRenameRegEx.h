#ifndef UIRenameRegEx_h
#define UIRenameRegEx_h

#include <QList>
#include <QRegularExpression>
#include "IUIRenameTabBase.h"
#include "ui_UIRenameRegEx.h"
class IUIRename;
class IUIFileList;


class IUIRenameRegEx : public IUIRenameTabBase, public Ui::UIRenameRegEx
{
    Q_OBJECT

private:
    // Lists of meta tags present in line edits
    QList<ITagInfo>             m_qlstReplaceNameTags;
    QList<ITagInfo>             m_qlstReplaceTheTextWithTags;
    QList<ITagInfo>             m_qlstInsertTheTextTags;
    QList<ITagInfo>             m_qlstInsertAtStartTags;
    QList<ITagInfo>             m_qlstInsertAtEndTags;

    // Regular expression and match
    QRegularExpression          m_qreRegEx;
    QRegularExpressionMatch     m_qremRegExMatch;

    // Used to indicate if there is a match for the regular expression
    bool                        m_bRegExMatch;

    // Saves number values from line edits as integers to save repeatedly converting the string to an int during renames
    int                         m_iRegExStartPos;
    int                         m_iInsertTheTextAtPos;

    // Stores name to use for tab when enabled
    QString                     m_qstrTabName;

    // Indicates if the tab associated with this object is enabled
    bool                        m_bTabEnabled;


public:
    IUIRenameRegEx(IUIRename* puirRenameUI, const int kiRenameElement, const int kiTabID, const QString & krqstrTabName);

    // Configures QRegularExpression case sensitivity setting
    void SetCaseSensitivity(const bool kbCaseSensitive);

private:
    // Sets validators for line edits
    void SetValidators();

    // Create connections to indicate when settings change
    void CreateConnections();

private slots:
    // Reads updated settings from UI and calls update on file viewer
    void SettingsChangedRegEx();
    void SettingsChangedStartPos();
    void SettingsChangedReplaceName();
    void SettingsChangedReplaceMatchWith();
    void SettingsChangedInsertTheText();
    void SettingsChangedInsertAtStart();
    void SettingsChangedInsertAtEnd();
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
    void InsertRegExMatches(QString & rqstrString);

    // Adds settings to stringlist
    void SaveSettings(const QString & krqstrSection, QStringList & rqstrlSettings);

    // Restores settings from passed save string
    void RestoreSettings(const QString & krqstrSettings, int & riIndex);

private:
    // Restors individual settings
    void RestoreCheckBox(const QString & krqstrTag);
    void RestoreLineEdit(const QString & krqstrTag, const QString & krqstrValue);

public:
    // Sets if tab is enabled, hiding widget and clearing settings as neccessary
    void SetTabEnabled(const bool kbEnabled);

    // Sets initial enabled state of tab
    void InitTabEnabled(const bool kbEnabled)   {m_bTabEnabled = kbEnabled; setVisible(kbEnabled);};

    // Accessors
    QString & GetTabName()                      {return m_qstrTabName;}
    bool TabEnabled()                           {return m_bTabEnabled;}
};

#endif // UIRenameRegEx_h
