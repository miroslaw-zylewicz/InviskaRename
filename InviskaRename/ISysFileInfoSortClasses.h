#ifndef ISysFileInfoSortClasses_h
#define ISysFileInfoSortClasses_h

#include <QCollator>
#include <QFileInfo>
#include <QTableWidgetItem>


// Used to sort file by extension.  Stores extension to avoid repeatedly having to look up MIME type
class IFileExtension
{
public:
    QFileInfo &             m_rqfinfFile;
    QString                 m_qstrExtension;

public:
    IFileExtension(QFileInfo & rqfinfFile, QString qstrExtension) : m_rqfinfFile(rqfinfFile), m_qstrExtension(qstrExtension) {}
};


// Used when resorting the table
class ITableRow
{
public:
    QTableWidgetItem*       m_pqtwiCurrent;
    QTableWidgetItem*       m_pqtwiPreview;
    QString                 m_qstrExtension;

public:
    ITableRow(QTableWidgetItem* pqtwiCurrent, QTableWidgetItem* pqtwiPreview) : m_pqtwiCurrent(pqtwiCurrent), m_pqtwiPreview(pqtwiPreview) {}
};


// For sorting QFileInfo objects by name
class IFICompareName
{
private:
    QCollator &             m_rqcolCollator;
public:
    IFICompareName(QCollator & rqcolCollator) : m_rqcolCollator(rqcolCollator) {}
    bool operator()(const QFileInfo & krqfiFile1, const QFileInfo & krqfiFile2) const;
};


// For sorting QFileInfo objects by date modified
class IFICompareModified
{
private:
    QCollator &             m_rqcolCollator;
public:
    IFICompareModified(QCollator & rqcolCollator) : m_rqcolCollator(rqcolCollator) {}
    bool operator()(const QFileInfo & krqfiFile1, const QFileInfo & krqfiFile2) const;
};


// For sorting QFileInfo objects by file extension
class IFICompareExtension
{
private:
    QCollator &             m_rqcolCollator;
public:
    IFICompareExtension(QCollator & rqcolCollator) : m_rqcolCollator(rqcolCollator) {}
    bool operator()(const IFileExtension * kpfieFile1, const IFileExtension * kpfieFile2) const;
};


// For sorting QTableWidgetItem objects by name
class ITWICompareName
{
private:
    QCollator &             m_rqcolCollator;
public:
    ITWICompareName(QCollator & rqcolCollator) : m_rqcolCollator(rqcolCollator) {}
    bool operator()(const ITableRow* kptarFile1, const ITableRow* kptarFile2) const;
};


// For sorting QTableWidgetItem objects by date modified
class ITWICompareModified
{
private:
    QCollator &             m_rqcolCollator;
public:
    ITWICompareModified(QCollator & rqcolCollator) : m_rqcolCollator(rqcolCollator) {}
    bool operator()(const ITableRow* kptarFile1, const ITableRow* kptarFile2) const;
};


// For sorting QTableWidgetItem objects by file extension
class ITWICompareExtension
{
private:
    QCollator &             m_rqcolCollator;
public:
    ITWICompareExtension(QCollator & rqcolCollator) : m_rqcolCollator(rqcolCollator) {}
    bool operator()(const ITableRow* kptarFile1, const ITableRow* kptarFile2) const;
};




#endif // ISysFileInfoSortClasses_h
