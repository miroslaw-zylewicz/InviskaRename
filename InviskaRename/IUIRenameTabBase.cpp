#include <QtWidgets>
#include "IUIRenameTabBase.h"
#include "IUIRename.h"
#include "IUIFileList.h"
#include "IUIMainWindow.h"


IUIRenameTabBase::IUIRenameTabBase(IUIRename* puirRenameUI, const int kiRenameElement, const int kiTabID) : QWidget(puirRenameUI),
                                                                                                            m_rmtlMetaTagLookup(puirRenameUI->GetMetaTagLookup())
{
    m_puirRenameUI = puirRenameUI;
    m_puifmFileList = IUIMainWindow::GetMainWindow()->GetFileListUI();
    m_iRenameElement = kiRenameElement;
    m_iTabID = kiTabID;
}


void IUIRenameTabBase::ReadTagCodes(QCheckBox* pqcbCheckBox, QLineEdit* pqleLineEdit, QList<ITagInfo> & rqlstTagList)
{
    rqlstTagList.clear();
    if (pqcbCheckBox->isChecked() == false || pqleLineEdit->text().isEmpty())
        return;
    QString kqstrString = pqleLineEdit->text();

    int iSearchStart = 0;
    ITagInfo tagiTagInfo;
    QString qstrCategory, qstrTagCode;
    while (true)
    {
        tagiTagInfo.m_iStartIndex = kqstrString.indexOf("[$", iSearchStart);
        if (tagiTagInfo.m_iStartIndex == -1)
            break;
        int iCatIndex = tagiTagInfo.m_iStartIndex + 2;

        int iLength = kqstrString.length();
        int iTagIndex = iCatIndex;
        while (iTagIndex < iLength && kqstrString.at(iTagIndex) != '-')
            ++iTagIndex;

        if (iTagIndex >= iLength)
            break;
        ++iTagIndex;

        tagiTagInfo.m_iEndIndex = iTagIndex;
        while (tagiTagInfo.m_iEndIndex < iLength && kqstrString.at(tagiTagInfo.m_iEndIndex).isLetterOrNumber())
            ++tagiTagInfo.m_iEndIndex;

        if (tagiTagInfo.m_iEndIndex >= iLength)
            break;

        if (kqstrString.at(tagiTagInfo.m_iEndIndex) != ']')
        {
             iSearchStart = tagiTagInfo.m_iEndIndex;
             continue;
        }

        qstrCategory = kqstrString.mid(iCatIndex, iTagIndex - iCatIndex - 1).toLower();
        qstrTagCode  = kqstrString.mid(iTagIndex, tagiTagInfo.m_iEndIndex - iTagIndex).toLower();
        m_rmtlMetaTagLookup.LookupTag(tagiTagInfo, qstrCategory, qstrTagCode);
        if (tagiTagInfo.m_iTagID != ITagInfo::Invalid)
        {
            rqlstTagList.push_back(tagiTagInfo);

            if      (tagiTagInfo.m_tcatCatagory == ITagInfo::Music && m_puifmFileList->MetaTagsReadMusic() == false)
                m_puifmFileList->ReadMetaTagsMusic();
            else if (tagiTagInfo.m_tcatCatagory == ITagInfo::Exif && m_puifmFileList->MetaTagsReadExif() == false)
                m_puifmFileList->ReadMetaTagsExif();
        }

        iSearchStart = tagiTagInfo.m_iEndIndex + 1;
    }
}


void IUIRenameTabBase::CheckListForMetaTags(const QList<ITagInfo> & krqlstTagList, bool & rbMusicTags, bool & rbExifTags)
{
    if (krqlstTagList.isEmpty())
        return;

    QList<ITagInfo>::const_iterator kitTag;
    for (kitTag = krqlstTagList.constBegin() ; kitTag != krqlstTagList.constEnd() ; ++kitTag)
    {
        if (rbMusicTags == false && kitTag->m_tcatCatagory == ITagInfo::Music)
            rbMusicTags = true;
        if (rbExifTags == false && kitTag->m_tcatCatagory == ITagInfo::Exif)
            rbExifTags = true;
    }
}
