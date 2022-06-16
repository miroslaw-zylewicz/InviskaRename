#ifndef IUIRename_h
#define IUIRename_h

#include <QWidget>
#include "IMetaTagLookup.h"
#include "IUIRenameRegEx.h"
class QTabWidget;
class QPushButton;
class QLineEdit;
class QListWidget;
class IUIMainWindow;
class IUIFileList;
class IUIRenameFilter;
class IUIRenameName;
class IUIRenameNumber;


class IUIRename : public QWidget
{
    Q_OBJECT

private:
    // Pointer to main window so menu bar can be updated with saved renames
    IUIMainWindow*              m_pmwMainWindow;

    // Pointer to file list UI so we can alert it to update name previews when settings change
    IUIFileList*                m_puifmFileList;

    // For looking up values associated with meta tags and replacing those tags in name strings
    IMetaTagLookup              m_mtlMetaTagLookup;

    // Tab widget on which rename settings are placed
    QTabWidget*                 m_pqtwRenameSettingsTab;

    // Widgets for rename name, extension and numbering tabs
    IUIRenameFilter*            m_purfFilter;
    IUIRenameName*              m_purnName;
    IUIRenameName*              m_purnExten;
    IUIRenameNumber*            m_purnNumber;
    IUIRenameRegEx*             m_purnRegExName1;
    IUIRenameRegEx*             m_purnRegExName2;
    IUIRenameRegEx*             m_purnRegExName3;
    IUIRenameRegEx*             m_purnRegExExten;

    // Rename and undo buttons
    QPushButton*                m_pqpbRenameButton;
    QPushButton*                m_pqpbUndoButton;

    // Maps insert tag QActions onto the String to insert into the settings box
    QHash<QAction*, QString>    m_qhashActionLookupFiAt;
    QHash<QAction*, QString>    m_qhashActionLookupMusic;
    QHash<QAction*, QString>    m_qhashActionLookupExif;

    // So we can insert tags from the menu bar, this stores the last active line edit and the cursor position
    QLineEdit*                  m_pqleLastActiveLineEdit;

    // Indicates if string comparisons should be case sensitive (for "Replace The Text" feature and file extension comparison)
    bool                        m_bCaseSensitive;

    // Indicates if a confirmation dialog should be shown before the rename is performed
    bool                        m_bShowConfirmBeforeRename;

    // Indicates if rename settings should be deactivated after the rename operation
    bool                        m_bDeactivateSettingsAfterRename;

    // Indicates if the active tab should be saved and restored along with the saved rename
    bool                        m_bSaveActiveTabWithRename;

    // Indicates if case sensitivity should be saved and restored along with the saved rename
    bool                        m_bSaveCaseSensitivityWithRename;

    // Set during batch changes (clearing settings, loading saved renames) to avoid name being repeatedly generated
    bool                        m_bChangingSettings;

    // Tab IDs used to indicate order when adding tabs
    enum                        TabID {Name, Extension, Numbering, RegExName1, RegExName2, RegExName3, RegExExten};

public:
    IUIRename(IUIMainWindow* pmwMainWindow);
    ~IUIRename();

private:
    // Adds RegEx tabs at startup based on settings
    void AddRegExTabs();

public:
    // Adds passed RegEx to QTabWidget in appropriate position
    void AddRegExTab(IUIRenameRegEx* purnRegExTab);

    // Called by Preferences dialog to set which tabs are visible
    void SetRegExTabVisibility(const bool kbRegExName1, const bool kbRegExName2, const bool kbRegExName3, const bool kbRegExExten);

private:
    // Adds/removed passed tab depending on enabled falg
    void SetRegExTabEnabled(IUIRenameRegEx* purnRegExTab, const bool kbEnabled, int & riIndex);

private slots:
    // Sets case sensitivity for comparisons, extensions and RegEx
    void SetCaseSensitivity(const bool kbCaseSensitive);

private:
    // Disables all rename settings, but doesn't clear boxes
    void DisableAllSettings();

public slots:
    // Disables all rename settings, and clears all boxes
    void ClearAll(const bool kbRegenerateName = true);

public:
    // Clears settings on passed tab while avoiding repeated name refreshes
    void ClearTab(IUIRenameTabBase* purnTab);

private slots:
    // Stores last active line edit for insertion of tags
    void StoreLastActiveLineEdit();

    // Clears the last active line pointer, for when the last active line was a line that doesn't support tags
    void ClearLastActiveLineEdit();

public:
    // Checks if there are meta tags present and sets flags accordingly
    void CheckForMetaTags(bool & rbMusicTags, bool & rbExifTags);

    // Adds items to Action to TagCode lookup hashes
    void AddActionLookupMusic(QAction* qactAction, const QString & qstrTagCode);
    void AddActionLookupExif(QAction* qactAction, const QString & qstrTagCode);
    void AddActionLookupFiAt(QAction* qactAction, const QString & qstrTagCode);

private slots:
    // Inserts the tag code corresponding the the action that called this slot into the last active QLineEdit
    void InsertTagCodeMusic();
    void InsertTagCodeExif();
    void InsertTagCodeFiAt();
    void InsertTagCode(const QString & krqstrTagCode);

public:
    // Re-reads tag codes from all line edits
    void ReReadTagCodes();

private slots:
    // Displays messagebox to confirm rename/undo action
    void ConfirmRename();
    void ConfirmUndo();

public:
    // Enable/disable rename and undo buttons and handle control focus issues
    void EnableRenameButton(const bool kbEnabled);
    void EnableUndoButton(const bool kbEnabled);

    // Applies current rename settings to passed string
    void GenerateName(QString & rqstrName, QTableWidgetItem* pqtwiFileItem);
    void GenerateExtension(QString & rqstrExtension, QTableWidgetItem* pqtwiFileItem);

private:
    void SaveSettings(const QString & krqstrSection, QStringList & rqstrlSettings);
    void RestoreSettings(const QString & krqstrSettings, int & riIndex);

public:
    // Returns the save string for the current settings
    QString GetCurrentSettingsSaveString();

    // Restores settings from save string
    void RestoreSettingsFromSaveString(const QString & krqstrSaveString);

    // Reads setting value from passed setting string and stores ID and value in passed strings
    bool ReadSettingsValue(const QString & krqstrSettings, int & riIndex, QString & rqstrTag, QString & rqstrValue);

    // Reads next setting ID from passed settings string
    bool ReadSetting(const QString & krqstrSettings, int & riIndex, QString & rqstrTag);

    // Accessors for flags
    bool ShowConfirmBeforeRename()          {return m_bShowConfirmBeforeRename;}
    void SetShowConfirmBeforeRename(const bool kbShowConfirmBeforeRename)               {m_bShowConfirmBeforeRename = kbShowConfirmBeforeRename;}
    bool DeactivateSettingsAfterRename()    {return m_bDeactivateSettingsAfterRename;}
    void SetDeactivateSettingsAfterRename(const bool kbDeactivateSettingsAfterRename)   {m_bDeactivateSettingsAfterRename = kbDeactivateSettingsAfterRename;}
    bool SaveActiveTabWithRename()          {return m_bSaveActiveTabWithRename;}
    void SetSaveActiveTabWithRename(const bool kbSaveActiveTabWithRename)               {m_bSaveActiveTabWithRename = kbSaveActiveTabWithRename;}
    bool SaveCaseSensitivityWithRename()    {return m_bSaveCaseSensitivityWithRename;}
    void SetSaveCaseSensitivityWithRename(const bool kbSaveCaseSensitivityWithRename)   {m_bSaveCaseSensitivityWithRename = kbSaveCaseSensitivityWithRename;}

    // Accessors
    IUIRenameFilter* GetRenameUIFilter()                    {return m_purfFilter;}
    IUIRenameNumber* GetRenameUINumber()                    {return m_purnNumber;}
    IMetaTagLookup & GetMetaTagLookup()                     {return m_mtlMetaTagLookup;}
    bool RegExName1TabEnbled()                              {return m_purnRegExName1->TabEnabled();}
    bool RegExName2TabEnbled()                              {return m_purnRegExName2->TabEnabled();}
    bool RegExName3TabEnbled()                              {return m_purnRegExName3->TabEnabled();}
    bool RegExExtenTabEnbled()                              {return m_purnRegExExten->TabEnabled();}
    bool ChangingSettings()                                 {return m_bChangingSettings;}
    bool CaseSensitive()                                    {return m_bCaseSensitive;}
    void SetExifAdvancedMode(const bool kbExifAdvancedMode) {m_mtlMetaTagLookup.InitExifLookupHash(kbExifAdvancedMode);}
};

#endif // IUIRename_h
