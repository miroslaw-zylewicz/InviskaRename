#ifndef ISysFileInfoSort_h
#define ISysFileInfoSort_h

#include <QCollator>
#include <QFileInfoList>
#include <QMimeDatabase>
#include "ISysFileInfoSortClasses.h"
class IUIFileList;


class ISysFileInfoSort
{
private:
    // Pointer to file list for getting directory listings
    IUIFileList*            m_puifmFileList;

    // For comparing and sorting by name
    QCollator               m_qcolCollator;

    // For compariting and sorting by file type
    QMimeDatabase           m_qmidbMimeDB;

    // Comparison classes for std::sort
    IFICompareName          m_compFIName;
    IFICompareModified      m_compFIModified;
    IFICompareExtension     m_compFIExtension;
    ITWICompareName         m_compTWIName;
    ITWICompareModified     m_compTWIModified;
    ITWICompareExtension    m_compTWIExtension;

    // Defines sort order
    int                     m_iSortOrder;

    // Indicates if sort order should be saved and restored
    bool                    m_bSaveSortOrder;

public:
    enum                    SortOrder {Name, Modified, Extension, Type};

public:
    ISysFileInfoSort(IUIFileList* puifmFileList);
    ~ISysFileInfoSort();

    // Returns a sorted file list with directories first and natural number sorting
    QFileInfoList GetSortedFileList();

private:
    // Returns file list sorted in specified order
    QFileInfoList GetSortedFileListName();
    QFileInfoList GetSortedFileListModified();
    QFileInfoList GetSortedFileListExtension();
    QFileInfoList GetSortedFileListType();

    // Called by GetSortedFileListExtension() and GetSortedFileListType() to get sorted file list by extension
    QFileInfoList GetSortedFileListExtension(const bool kbUseMIMEExtension);

public:
    // Resorts passed table row list into the specified order
    void ResortRows(QList<ITableRow*> & rqlstRowList, const int kiSortOrder);

private:
    // Resorts table rows in specified order
    void ResortRowsFileListName(QList<ITableRow*> & rqlstRowList);
    void ResortRowsFileListModified(QList<ITableRow*> & rqlstRowList);
    void ResortRowsFileListExtension(QList<ITableRow*> & rqlstRowList);
    void ResortRowsFileListType(QList<ITableRow*> & rqlstRowList);

    // Returns extension for file based on the last '.' accounting for filenames that start with '.'
    QString GetExtension(const QFileInfo & krqfiFile) const;

    // Returns the preferred extension for the file MIMI type
    QString GetMIMEExtension(const QFileInfo & krqfiFile) const;

public:
    // Returns true if the new file comes after the row file
    bool ComesAfter(const QString & krqstrNewFile, const QString & krqstrRowFile) const;

    // Sets sort order
    void SetOrderName()         {m_iSortOrder = Name;}
    void SetOrderModified()     {m_iSortOrder = Modified;}
    void SetOrderExtension()    {m_iSortOrder = Extension;}
    void SetOrderType()         {m_iSortOrder = Type;}
    int  GetSortOrder()         {return m_iSortOrder;}
    bool GetSaveSortOrder()     {return m_bSaveSortOrder;}
    void SetSaveSortOrder(const bool kbSaveSortOrder)       {m_bSaveSortOrder = kbSaveSortOrder;}
};

#endif // ISysFileInfoSort_h
