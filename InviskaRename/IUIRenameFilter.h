#ifndef IUIRenameFilter_h
#define IUIRenameFilter_h

#include <QGroupBox>
class QRadioButton;
class IComQLineEdit;
class IUIRename;
class IUIFileList;


class IUIRenameFilter : public QGroupBox
{
    Q_OBJECT

public:
    // Main rename settings UI for creating connections
    IUIRename*                  m_puirRenameUI;

    // File list for regenerating preview when settings change
    IUIFileList*                m_puifmFileList;

    // For filtering files to rename by type
    QRadioButton*               m_pqrbRenameFilesOnly;
    QRadioButton*               m_pqrbRenameFoldersOnly;
    QRadioButton*               m_pqrbRenameFilesAndFolders;
    QRadioButton*               m_pqrbRenameSelectedItemsOnly;
    QRadioButton*               m_pqrbRenameFilesWithExtension;

    // For filtering files to rename by extension
    IComQLineEdit*              m_pqleRenameFilesWithExtension;

    // List of extensions for files that should be renamed
    QStringList                 m_qstrlRenameExtensions;

    // Elements that should be renamed from below enum
    int                         m_iRenameElements;

public:
    // Values for m_iRenameElements which indicate which files/folders should be renamed
    enum                        RenameElements {RenameFilesOnly, RenameFoldersOnly, RenameFilesAndFolders, RenameSelectedItems, RenameFilesWithExtension};

public:
    IUIRenameFilter(IUIRename* puirRenameUI);

private:
    // Sets validators for line edits
    void SetValidators();

    // Create connections to indicate when settings change
    void CreateConnections();

private slots:
    // Triggers regeneration of name when settings change
    void RadioSettingsChanged(const bool kbEnabled);
    void ExtensionListChanged();

public:
    // Reads the extensions, converting them to lower case depending on kbCaseSensitive, and storing them
    void ReadExtensions(const bool kbCaseSensitive);

    // Disables all settings but doesn't clear line edits
    void DisableAllSettings();

    // Disables all settings and clears line edits
    void ClearAll();

    // Adds settings to stringlist
    void SaveSettings(const QString & krqstrSection, QStringList & rqstrlSettings);

    // Restors settings from passed save string
    void RestoreSettings(const QString & krqstrSettings, int & riIndex);

private:
    // Restors individual settings
    void RestoreRadioBox(const QString & krqstrTag);
    void RestoreLineEdit(const QString & krqstrTag, const QString & krqstrValue);

public:
    // Accessors
    const QStringList & GetRenameExtensions()           {return m_qstrlRenameExtensions;}
    int RenameElements()                                {return m_iRenameElements;}
};

#endif // IUIRenameFilter_h
