#ifndef IUIRenameTabBase_h
#define IUIRenameTabBase_h

#include <QWidget>
#include "IMetaTagLookup.h"
class QCheckBox;
class QLineEdit;
class IUIRename;
class IUIFileList;


class IUIRenameTabBase : public QWidget
{
    Q_OBJECT

protected:
    // Main rename settings UI for creating connections
    IUIRename*                  m_puirRenameUI;

    // File list for regenerating preview when settings change
    IUIFileList*                m_puifmFileList;

    // For looking up values associated with meta tags and replacing those tags in name strings
    IMetaTagLookup &            m_rmtlMetaTagLookup;

    // Indicates if this object is for renaming the name or extension element of the filname
    int                         m_iRenameElement;

    // Tab ID used to save active tab and determine order of tabs in QTabWidget
    int                         m_iTabID;

public:
    // Indicates if this object is for name or extension settings
    enum                        RenameElement {Name, Extension, Numbering};

public:
    IUIRenameTabBase(IUIRename* puirRenameUI, const int kiRenameElement, const int kiTabID);

protected:
    // Reads tag codes from passed string and stores tag information in passed list
    void ReadTagCodes(QCheckBox* pqcbCheckBox, QLineEdit* pqleLineEdit, QList<ITagInfo> & rqlstTagList);

    // Checks if passed list contains Music or Exif tags and sets passed pools to indicate which are present
    void CheckListForMetaTags(const QList<ITagInfo> & krqlstTagList, bool & rbMusicTags, bool & rbExifTags);

public:
    // Clears all settings on tab
    virtual void ClearAll() = 0;

    // Accessors
    int GetTabID()      {return m_iTabID;}
};

#endif // IUIRenameTabBase_h
