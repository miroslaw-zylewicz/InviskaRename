#include <QTableWidgetItem>
#include <QDateTime>
#include "IMetaTagLookup.h"
#include "IMetaMusic.h"
#include "IMetaExif.h"
#include "IMetaAttrib.h"
#include "IUIFileList.h"


IMetaTagLookup::IMetaTagLookup()
{
    IMetaMusic::InitTagLookupHash();
    IMetaAttrib::InitTagLookupHash();
}


void IMetaTagLookup::InitExifLookupHash(const bool kbExifAdvancedMode)
{
    if (kbExifAdvancedMode)
        IMetaExif::InitTagLookupHashAdvanced();
    else
        IMetaExif::InitTagLookupHashBasic();
}


void IMetaTagLookup::LookupTag(ITagInfo & rtagiTagInfo, const QString & krqstrCategory, const QString & krqstrTagCode)
{
    if      (krqstrCategory == "mu")
    {
        rtagiTagInfo.m_tcatCatagory = ITagInfo::Music;
        rtagiTagInfo.m_iTagID       = IMetaMusic::GetTagID(krqstrTagCode);
    }
    else if (krqstrCategory == "ex")
    {
        rtagiTagInfo.m_tcatCatagory = ITagInfo::Exif;
        rtagiTagInfo.m_iTagID       = IMetaExif::GetTagID(krqstrTagCode);
    }
    else if (krqstrCategory == "at")
    {
        rtagiTagInfo.m_tcatCatagory = ITagInfo::Attrib;
        rtagiTagInfo.m_iTagID       = IMetaAttrib::GetTagID(krqstrTagCode);
    }
    else
    {
        rtagiTagInfo.m_tcatCatagory = ITagInfo::Invalid;
        rtagiTagInfo.m_iTagID       = ITagInfo::Invalid;
    }
}


QString IMetaTagLookup::GetValueForTagCode(const QTableWidgetItem* kpqtwiFileItem, const ITagInfo & krtagiTagInfo)
{
    if (krtagiTagInfo.m_tcatCatagory == ITagInfo::Music)
    {
        if (kpqtwiFileItem->data(IUIFileList::MusicMeta).isNull())
            return QString();
        return kpqtwiFileItem->data(IUIFileList::MusicMeta).value<IMetaMusic>().GetTagValue(krtagiTagInfo.m_iTagID);
    }

    if (krtagiTagInfo.m_tcatCatagory == ITagInfo::Exif)
    {
        if (kpqtwiFileItem->data(IUIFileList::ExifMeta).isNull())
            return QString();
        return kpqtwiFileItem->data(IUIFileList::ExifMeta).value<IMetaExif>().GetTagValue(krtagiTagInfo.m_iTagID);
    }

    if (krtagiTagInfo.m_tcatCatagory == ITagInfo::Attrib)
    {
        return IMetaAttrib::GetTagValue(kpqtwiFileItem, krtagiTagInfo.m_iTagID);
    }

    return QString();
}


QString IMetaTagLookup::ReplaceTagCodesWithValues(const QString & krqstrString, const QList<ITagInfo> & krqlstReplaceNameTags, const QTableWidgetItem* kpqtwiFileItem)
{
    int iSubStringStart = 0;
    QString qstrSubstituted;

    QList<ITagInfo>::const_iterator kitTagInfo;
    for (kitTagInfo = krqlstReplaceNameTags.constBegin() ; kitTagInfo != krqlstReplaceNameTags.constEnd() ; ++kitTagInfo)
    {
        qstrSubstituted += krqstrString.mid(iSubStringStart, kitTagInfo->m_iStartIndex - iSubStringStart) + GetValueForTagCode(kpqtwiFileItem, *kitTagInfo);
        iSubStringStart = kitTagInfo->m_iEndIndex+1;
    }

    if (iSubStringStart < krqstrString.length())
        qstrSubstituted += krqstrString.mid(iSubStringStart);


    return qstrSubstituted;
}
