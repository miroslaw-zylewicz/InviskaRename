#include <QTableWidgetItem>
#include <QFileInfo>
#include <QDateTime>
#include "IUIFileList.h"
#include "IMetaAttrib.h"
#include "IMetaTagLookup.h"
#include "IMetaBase.h"

QHash<QString, int> IMetaAttrib::m_qhashTagLookup = QHash<QString, int>();


void IMetaAttrib::InitTagLookupHash()
{
    m_qhashTagLookup["createddatetime"]       = CreatedDateTime;
    m_qhashTagLookup["createddate"]           = CreatedDate;
    m_qhashTagLookup["createdtime"]           = CreatedTime;
    m_qhashTagLookup["createddateyyyy"]       = CreatedDateYYYY;
    m_qhashTagLookup["createddateyy"]         = CreatedDateYY;
    m_qhashTagLookup["createddatemm"]         = CreatedDateMM;
    m_qhashTagLookup["createddatedd"]         = CreatedDateDD;
    m_qhashTagLookup["createdtimehh"]         = CreatedTimeHH;
    m_qhashTagLookup["createdtimemm"]         = CreatedTimeMM;
    m_qhashTagLookup["createdtimess"]         = CreatedTimeSS;
    m_qhashTagLookup["modifieddatetime"]      = ModifiedDateTime;
    m_qhashTagLookup["modifieddate"]          = ModifiedDate;
    m_qhashTagLookup["modifiedtime"]          = ModifiedTime;
    m_qhashTagLookup["modifieddateyyyy"]      = ModifiedDateYYYY;
    m_qhashTagLookup["modifieddateyy"]        = ModifiedDateYY;
    m_qhashTagLookup["modifieddatemm"]        = ModifiedDateMM;
    m_qhashTagLookup["modifieddatedd"]        = ModifiedDateDD;
    m_qhashTagLookup["modifiedtimehh"]        = ModifiedTimeHH;
    m_qhashTagLookup["modifiedtimemm"]        = ModifiedTimeMM;
    m_qhashTagLookup["modifiedtimess"]        = ModifiedTimeSS;

}


QString IMetaAttrib::GetTagValue(const QTableWidgetItem* kpqtwiFileItem, const int kiTagID)
{
    if (kpqtwiFileItem->data(IUIFileList::FileInfo).isNull())
        return QString();
    const QFileInfo & krqfiFileInfo = kpqtwiFileItem->data(IUIFileList::FileInfo).value<QFileInfo>();

    switch (kiTagID)
    {
    case CreatedDateTime    :   return ProcessDateTime(krqfiFileInfo.birthTime().toString("yyyy-MM-dd HH.mm.ss"));
    case CreatedDate        :   return krqfiFileInfo.birthTime().date().toString(Qt::ISODate).replace('-', IMetaBase::GetSeparatorDate());
    case CreatedTime        :   return krqfiFileInfo.birthTime().time().toString("HH.mm.ss").replace('.', IMetaBase::GetSeparatorTime());
    case CreatedDateYYYY    :   return krqfiFileInfo.birthTime().date().toString("yyyy");
    case CreatedDateYY      :   return krqfiFileInfo.birthTime().date().toString("yy");
    case CreatedDateMM      :   return krqfiFileInfo.birthTime().date().toString("MM");
    case CreatedDateDD      :   return krqfiFileInfo.birthTime().date().toString("dd");
    case CreatedTimeHH      :   return krqfiFileInfo.birthTime().time().toString("HH");
    case CreatedTimeMM      :   return krqfiFileInfo.birthTime().time().toString("mm");
    case CreatedTimeSS      :   return krqfiFileInfo.birthTime().time().toString("ss");
    case ModifiedDateTime   :   return ProcessDateTime(krqfiFileInfo.lastModified().toString("yyyy-MM-dd HH.mm.ss"));
    case ModifiedDate       :   return krqfiFileInfo.lastModified().date().toString(Qt::ISODate).replace('-', IMetaBase::GetSeparatorDate());
    case ModifiedTime       :   return krqfiFileInfo.lastModified().time().toString("HH.mm.ss").replace('.', IMetaBase::GetSeparatorTime());
    case ModifiedDateYYYY   :   return krqfiFileInfo.lastModified().date().toString("yyyy");
    case ModifiedDateYY     :   return krqfiFileInfo.lastModified().date().toString("yy");
    case ModifiedDateMM     :   return krqfiFileInfo.lastModified().date().toString("MM");
    case ModifiedDateDD     :   return krqfiFileInfo.lastModified().date().toString("dd");
    case ModifiedTimeHH     :   return krqfiFileInfo.lastModified().time().toString("HH");
    case ModifiedTimeMM     :   return krqfiFileInfo.lastModified().time().toString("mm");
    case ModifiedTimeSS     :   return krqfiFileInfo.lastModified().time().toString("ss");
    }
    return QString();
}


QString IMetaAttrib::ProcessDateTime(QString qstrDateTime)
{
    qstrDateTime[4]  = IMetaBase::GetSeparatorDate();
    qstrDateTime[7]  = IMetaBase::GetSeparatorDate();
    qstrDateTime[13] = IMetaBase::GetSeparatorTime();
    qstrDateTime[16] = IMetaBase::GetSeparatorTime();
    return qstrDateTime;
}


int IMetaAttrib::GetTagID(const QString & krqstrTagCode)
{
    QHash<QString, int>::iterator itTagID = m_qhashTagLookup.find(krqstrTagCode);
    if (itTagID == m_qhashTagLookup.end())
        return ITagInfo::Invalid;
    return *itTagID;
}
