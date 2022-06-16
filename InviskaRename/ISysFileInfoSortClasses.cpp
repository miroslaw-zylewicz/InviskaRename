#include <QDateTime>
#include "ISysFileInfoSortClasses.h"
#include "IUIFileList.h"


bool IFICompareName::operator()(const QFileInfo & krqfiFile1, const QFileInfo & krqfiFile2) const
{
    return m_rqcolCollator.compare(krqfiFile1.fileName(), krqfiFile2.fileName()) < 0;
}


bool IFICompareModified::operator()(const QFileInfo & krqfiFile1, const QFileInfo & krqfiFile2) const
{
    if (krqfiFile1.lastModified() < krqfiFile2.lastModified())
        return true;

    if (krqfiFile1.lastModified() == krqfiFile2.lastModified())
        return m_rqcolCollator.compare(krqfiFile1.fileName(), krqfiFile2.fileName()) < 0;

    return false;
}


bool IFICompareExtension::operator()(const IFileExtension* kpfieFile1, const IFileExtension* kpfieFile2) const
{
    if (kpfieFile1->m_qstrExtension < kpfieFile2->m_qstrExtension)
        return true;

    if (kpfieFile1->m_qstrExtension == kpfieFile2->m_qstrExtension)
        return m_rqcolCollator.compare(kpfieFile1->m_rqfinfFile.fileName(), kpfieFile2->m_rqfinfFile.fileName()) < 0;

    return false;
}



bool ITWICompareName::operator()(const ITableRow* kptarFile1, const ITableRow* kptarFile2) const
{
    return m_rqcolCollator.compare(kptarFile1->m_pqtwiCurrent->text(), kptarFile2->m_pqtwiCurrent->text()) < 0;
}



bool ITWICompareModified::operator()(const ITableRow* kptarFile1, const ITableRow* kptarFile2) const
{
    QDateTime qtdFile1Mod = kptarFile1->m_pqtwiCurrent->data(IUIFileList::FileInfo).value<QFileInfo>().lastModified();
    QDateTime qtdFile2Mod = kptarFile2->m_pqtwiCurrent->data(IUIFileList::FileInfo).value<QFileInfo>().lastModified();

    if (qtdFile1Mod < qtdFile2Mod)
        return true;

    if (qtdFile1Mod == qtdFile2Mod)
        return m_rqcolCollator.compare(kptarFile1->m_pqtwiCurrent->text(), kptarFile2->m_pqtwiCurrent->text()) < 0;

    return false;
}



bool ITWICompareExtension::operator()(const ITableRow* kptarFile1, const ITableRow* kptarFile2) const
{
    if (kptarFile1->m_qstrExtension < kptarFile2->m_qstrExtension)
        return true;

    if (kptarFile1->m_qstrExtension == kptarFile2->m_qstrExtension)
        return m_rqcolCollator.compare(kptarFile1->m_pqtwiCurrent->text(), kptarFile2->m_pqtwiCurrent->text()) < 0;

    return false;
}

