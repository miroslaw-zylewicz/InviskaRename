#include "ISysFileInfoSort.h"
#include "IUIFileList.h"
#include "IUIMainWindow.h"


ISysFileInfoSort::ISysFileInfoSort(IUIFileList* puifmFileList) : m_compFIName(m_qcolCollator),
                                                                 m_compFIModified(m_qcolCollator),
                                                                 m_compFIExtension(m_qcolCollator),
                                                                 m_compTWIName(m_qcolCollator),
                                                                 m_compTWIModified(m_qcolCollator),
                                                                 m_compTWIExtension(m_qcolCollator)
{
    m_puifmFileList = puifmFileList;
    m_qcolCollator.setNumericMode(true);

    QSettings & rqsetSettings = IUIMainWindow::GetMainWindow()->GetSettings();
    m_bSaveSortOrder = rqsetSettings.value("FileList/SaveSortOrder", false).toBool();
    m_iSortOrder = m_bSaveSortOrder ? rqsetSettings.value("FileList/SortOrder", Name).toInt() : Name;
}


ISysFileInfoSort::~ISysFileInfoSort()
{
    IUIMainWindow::GetMainWindow()->GetSettings().setValue("FileList/SortOrder", m_iSortOrder);
}


QFileInfoList ISysFileInfoSort::GetSortedFileList()
{
    switch (m_iSortOrder)
    {
    case Name       : return GetSortedFileListName();
    case Type       : return GetSortedFileListType();
    case Modified   : return GetSortedFileListModified();
    case Extension  : return GetSortedFileListExtension();
    }
    return GetSortedFileListName();
}


QFileInfoList ISysFileInfoSort::GetSortedFileListName()
{
    QFileInfoList qfilDirList = m_puifmFileList->GetDirectoryFileList(QDir::Dirs);
    std::sort(qfilDirList.begin(), qfilDirList.end(), m_compFIName);

    QFileInfoList qfilFileList = m_puifmFileList->GetDirectoryFileList(QDir::Files);
    std::sort(qfilFileList.begin(), qfilFileList.end(), m_compFIName);

    qfilDirList.append(qfilFileList);
    return qfilDirList;
}


QFileInfoList ISysFileInfoSort::GetSortedFileListModified()
{
    QFileInfoList qfilDirList = m_puifmFileList->GetDirectoryFileList(QDir::Dirs);
    std::sort(qfilDirList.begin(), qfilDirList.end(), m_compFIModified);

    QFileInfoList qfilFileList = m_puifmFileList->GetDirectoryFileList(QDir::Files);
    std::sort(qfilFileList.begin(), qfilFileList.end(), m_compFIModified);

    qfilDirList.append(qfilFileList);
    return qfilDirList;
}


QFileInfoList ISysFileInfoSort::GetSortedFileListExtension()
{
    return GetSortedFileListExtension(true);
}


QFileInfoList ISysFileInfoSort::GetSortedFileListType()
{
    return GetSortedFileListExtension(false);
}


QFileInfoList ISysFileInfoSort::GetSortedFileListExtension(const bool kbUseMIMEExtension)
{
    QFileInfoList qfilDirList = m_puifmFileList->GetDirectoryFileList(QDir::Dirs);
    std::sort(qfilDirList.begin(), qfilDirList.end(), m_compFIName);

    QFileInfoList qfilFileList = m_puifmFileList->GetDirectoryFileList(QDir::Files);
    qfilDirList.reserve(qfilDirList.size() + qfilFileList.size());

    QList<IFileExtension*> qlstFileExtensionList;
    qlstFileExtensionList.reserve(qfilFileList.size());

    QFileInfoList::iterator itFile;
    if (kbUseMIMEExtension)
    {
        for (itFile = qfilFileList.begin() ; itFile < qfilFileList.end() ; ++itFile)
            qlstFileExtensionList.append(new IFileExtension(*itFile, GetMIMEExtension(*itFile)));
    }
    else
    {
        for (itFile = qfilFileList.begin() ; itFile < qfilFileList.end() ; ++itFile)
            qlstFileExtensionList.append(new IFileExtension(*itFile, GetExtension(*itFile)));
    }
    std::sort(qlstFileExtensionList.begin(), qlstFileExtensionList.end(), m_compFIExtension);

    QList<IFileExtension*>::iterator itFileExten;
    for (itFileExten = qlstFileExtensionList.begin() ; itFileExten < qlstFileExtensionList.end() ; ++itFileExten)
    {
        qfilDirList.append((*itFileExten)->m_rqfinfFile);
        delete *itFileExten;
    }

    return qfilDirList;
}


void ISysFileInfoSort::ResortRows(QList<ITableRow*> & rqlstRowList, const int kiSortOrder)
{
    switch (kiSortOrder)
    {
    case Name       :   ResortRowsFileListName(rqlstRowList);
                        break;
    case Type       :   ResortRowsFileListType(rqlstRowList);
                        break;
    case Modified   :   ResortRowsFileListModified(rqlstRowList);
                        break;
    case Extension  :   ResortRowsFileListExtension(rqlstRowList);
    }
}


void ISysFileInfoSort::ResortRowsFileListName(QList<ITableRow*> & rqlstRowList)
{
    std::sort(rqlstRowList.begin(), rqlstRowList.end(), m_compTWIName);
}


void ISysFileInfoSort::ResortRowsFileListModified(QList<ITableRow*> & rqlstRowList)
{
    std::sort(rqlstRowList.begin(), rqlstRowList.end(), m_compTWIModified);
}


void ISysFileInfoSort::ResortRowsFileListExtension(QList<ITableRow*> & rqlstRowList)
{
    QList<ITableRow*>::iterator itTableRow;
    for (itTableRow = rqlstRowList.begin() ; itTableRow < rqlstRowList.end() ; ++itTableRow)
        (*itTableRow)->m_qstrExtension = GetExtension((*itTableRow)->m_pqtwiCurrent->data(IUIFileList::FileInfo).value<QFileInfo>());

    std::sort(rqlstRowList.begin(), rqlstRowList.end(), m_compTWIExtension);
}


void ISysFileInfoSort::ResortRowsFileListType(QList<ITableRow*> & rqlstRowList)
{
    QList<ITableRow*>::iterator itTableRow;
    for (itTableRow = rqlstRowList.begin() ; itTableRow < rqlstRowList.end() ; ++itTableRow)
        (*itTableRow)->m_qstrExtension = GetMIMEExtension((*itTableRow)->m_pqtwiCurrent->data(IUIFileList::FileInfo).value<QFileInfo>());

    std::sort(rqlstRowList.begin(), rqlstRowList.end(), m_compTWIExtension);
}


QString ISysFileInfoSort::GetExtension(const QFileInfo & krqfiFile) const
{
    int iExtenStart = krqfiFile.fileName().lastIndexOf('.');
    if (iExtenStart < 1)
        return QString();
    return krqfiFile.fileName().mid(iExtenStart+1).toLower();
}


QString ISysFileInfoSort::GetMIMEExtension(const QFileInfo & krqfiFile) const
{
    QMimeType mityMime = m_qmidbMimeDB.mimeTypeForFile(krqfiFile.fileName(), QMimeDatabase::MatchExtension);
    QString qstrExten = mityMime.preferredSuffix();
    if (qstrExten.isEmpty())
        return GetExtension(krqfiFile);
    return qstrExten;
}


bool ISysFileInfoSort::ComesAfter(const QString & krqstrNewFile, const QString & krqstrRowFile) const
{
    return m_qcolCollator.compare(krqstrNewFile, krqstrRowFile) > 0;
}
