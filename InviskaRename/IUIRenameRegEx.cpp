#include <QtWidgets>
#include "IUIRenameRegEx.h"
#include "IUIRename.h"
#include "IUIFileList.h"
#include "IUIMainWindow.h"


IUIRenameRegEx::IUIRenameRegEx(IUIRename* puirRenameUI, const int kiRenameElement, const int kiTabID, const QString & krqstrTabName) : IUIRenameTabBase(puirRenameUI, kiRenameElement, kiTabID)
{
    m_qstrTabName = krqstrTabName;
    m_bTabEnabled = false;
    m_iRegExStartPos = 0;

    SetCaseSensitivity(puirRenameUI->CaseSensitive());

    setupUi(this);
    SetValidators();
    CreateConnections();
}


void IUIRenameRegEx::SetCaseSensitivity(const bool kbCaseSensitive)
{
    if (kbCaseSensitive)
        m_qreRegEx.setPatternOptions(QRegularExpression::NoPatternOption);
    else
        m_qreRegEx.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
}


void IUIRenameRegEx::SetValidators()
{
    QIntValidator* pivalIntOnlyValidator = IUIMainWindow::GetMainWindow()->GetIntOnlyValidator();

    m_pqleRegExStartPos->setValidator(pivalIntOnlyValidator);
    m_pqleInsertTheTextAtPos->setValidator(pivalIntOnlyValidator);

    QRegExpValidator* pqrevInvalidCharValidator = IUIMainWindow::GetMainWindow()->GetInvalidCharValidator();

    m_pqleReplaceName->setValidator(pqrevInvalidCharValidator);
    m_pqleReplaceMatchWith->setValidator(pqrevInvalidCharValidator);
    m_pqleInsertTheText->setValidator(pqrevInvalidCharValidator);
    m_pqleInsertAtStart->setValidator(pqrevInvalidCharValidator);
    m_pqleInsertAtEnd->setValidator(pqrevInvalidCharValidator);
}


void IUIRenameRegEx::CreateConnections()
{
    connect(m_pqleRegEx,                SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedRegEx()));

    connect(m_pqcbRegExStartPos,        SIGNAL(toggled(bool)),                  this,               SLOT(SettingsChangedStartPos()));
    connect(m_pqleRegExStartPos,        SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedStartPos()));

    connect(m_pqcbReplaceName,          SIGNAL(toggled(bool)),                  this,               SLOT(SettingsChangedReplaceName()));
    connect(m_pqleReplaceName,          SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedReplaceName()));

    connect(m_pqcbReplaceMatchWith,     SIGNAL(toggled(bool)),                  this,               SLOT(SettingsChangedReplaceMatchWith()));
    connect(m_pqleReplaceMatchWith,     SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedReplaceMatchWith()));

    connect(m_pqcbInsertTheText,        SIGNAL(toggled(bool)),                  this,               SLOT(SettingsChangedInsertTheText()));
    connect(m_pqleInsertTheText,        SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedInsertTheText()));
    connect(m_pqleInsertTheTextAtPos,   SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedInsertTheText()));

    connect(m_pqcbInsertAtStart,        SIGNAL(toggled(bool)),                  this,               SLOT(SettingsChangedInsertAtStart()));
    connect(m_pqleInsertAtStart,        SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedInsertAtStart()));

    connect(m_pqcbInsertAtEnd,          SIGNAL(toggled(bool)),                  this,               SLOT(SettingsChangedInsertAtEnd()));
    connect(m_pqleInsertAtEnd,          SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedInsertAtEnd()));

    connect(m_pqleReplaceName,          SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(StoreLastActiveLineEdit()));
    connect(m_pqleReplaceMatchWith,     SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(StoreLastActiveLineEdit()));
    connect(m_pqleInsertTheText,        SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(StoreLastActiveLineEdit()));
    connect(m_pqleInsertAtStart,        SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(StoreLastActiveLineEdit()));
    connect(m_pqleInsertAtEnd,          SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(StoreLastActiveLineEdit()));

    connect(m_pqleRegEx,                SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(ClearLastActiveLineEdit()));
    connect(m_pqleRegExStartPos,        SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(ClearLastActiveLineEdit()));
    connect(m_pqleInsertTheTextAtPos,   SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(ClearLastActiveLineEdit()));
}


void IUIRenameRegEx::SettingsChangedRegEx()
{
    m_qreRegEx.setPattern(m_pqleRegEx->text());
    SetingsChanged();
}


void IUIRenameRegEx::SettingsChangedStartPos()
{
    if (m_pqcbRegExStartPos->isChecked() && m_pqleRegExStartPos->text().isEmpty() == false)
        m_iRegExStartPos = m_pqleRegExStartPos->text().toInt();
    else
        m_iRegExStartPos = 0;
    SetingsChanged();
}


void IUIRenameRegEx::SettingsChangedReplaceName()
{
    ReadTagCodes(m_pqcbReplaceName, m_pqleReplaceName, m_qlstReplaceNameTags);
    SetingsChanged();
}


void IUIRenameRegEx::SettingsChangedReplaceMatchWith()
{
    ReadTagCodes(m_pqcbReplaceMatchWith, m_pqleReplaceMatchWith, m_qlstReplaceTheTextWithTags);
    SetingsChanged();
}


void IUIRenameRegEx::SettingsChangedInsertTheText()
{
    m_iInsertTheTextAtPos = m_pqleInsertTheTextAtPos->text().toInt();
    ReadTagCodes(m_pqcbInsertTheText, m_pqleInsertTheText, m_qlstInsertTheTextTags);
    SetingsChanged();
}


void IUIRenameRegEx::SettingsChangedInsertAtStart()
{
    ReadTagCodes(m_pqcbInsertAtStart, m_pqleInsertAtStart, m_qlstInsertAtStartTags);
    SetingsChanged();
}


void IUIRenameRegEx::SettingsChangedInsertAtEnd()
{
    ReadTagCodes(m_pqcbInsertAtEnd, m_pqleInsertAtEnd, m_qlstInsertAtEndTags);
    SetingsChanged();
}


void IUIRenameRegEx::SetingsChanged()
{
    if (m_iRenameElement == Name)
        m_puifmFileList->GeneratePreviewName();
    else
        m_puifmFileList->GeneratePreviewNameAndExtension();
}


void IUIRenameRegEx::ReReadTagCodes()
{
    ReadTagCodes(m_pqcbReplaceName, m_pqleReplaceName, m_qlstReplaceNameTags);
    ReadTagCodes(m_pqcbReplaceMatchWith, m_pqleReplaceMatchWith, m_qlstReplaceTheTextWithTags);
    ReadTagCodes(m_pqcbInsertTheText, m_pqleInsertTheText, m_qlstInsertTheTextTags);
    ReadTagCodes(m_pqcbInsertAtStart, m_pqleInsertAtStart, m_qlstInsertAtStartTags);
    ReadTagCodes(m_pqcbInsertAtEnd, m_pqleInsertAtEnd, m_qlstInsertAtEndTags);
}


void IUIRenameRegEx::DisableAllSettings()
{
    m_pqcbRegExStartPos->setChecked(false);
    m_pqcbReplaceName->setChecked(false);
    m_pqcbReplaceMatchWith->setChecked(false);
    m_pqcbInsertTheText->setChecked(false);
    m_pqcbInsertAtStart->setChecked(false);
    m_pqcbInsertAtEnd->setChecked(false);
}


void IUIRenameRegEx::ClearAll()
{
    DisableAllSettings();

    m_pqleRegEx->clear();
    m_pqleRegExStartPos->clear();
    m_pqleReplaceName->clear();
    m_pqleReplaceMatchWith->clear();
    m_pqleInsertTheText->clear();
    m_pqleInsertTheTextAtPos->clear();
    m_pqleInsertAtStart->clear();
    m_pqleInsertAtEnd->clear();
}


void IUIRenameRegEx::CheckForMetaTags(bool & rbMusicTags, bool & rbExifTags)
{
    CheckListForMetaTags(m_qlstReplaceNameTags, rbMusicTags, rbExifTags);
    CheckListForMetaTags(m_qlstReplaceTheTextWithTags, rbMusicTags, rbExifTags);
    CheckListForMetaTags(m_qlstInsertTheTextTags, rbMusicTags, rbExifTags);
    CheckListForMetaTags(m_qlstInsertAtStartTags, rbMusicTags, rbExifTags);
    CheckListForMetaTags(m_qlstInsertAtEndTags, rbMusicTags, rbExifTags);
}


void IUIRenameRegEx::GenerateName(QString & rqstrName, QTableWidgetItem* pqtwiFileItem)
{
    if (m_bTabEnabled == false)
        return;

    m_bRegExMatch = false;
    if (m_pqleRegEx->text().isEmpty() == false && m_qreRegEx.isValid())
    {
        m_qremRegExMatch = m_qreRegEx.match(rqstrName, m_iRegExStartPos);
        if (m_qremRegExMatch.hasMatch())
            m_bRegExMatch = true;
    }

    if (m_pqcbReplaceName->isChecked())
    {
        QString qstrReplaceName = m_pqleReplaceName->text();
        InsertRegExMatches(qstrReplaceName);

        if (m_qlstReplaceNameTags.isEmpty() == false)
            rqstrName = m_rmtlMetaTagLookup.ReplaceTagCodesWithValues(qstrReplaceName, m_qlstReplaceNameTags, pqtwiFileItem);
        else
            rqstrName = qstrReplaceName;
    }

    if (m_pqcbReplaceMatchWith->isChecked() && m_qremRegExMatch.captured().isEmpty() == false)
    {
        QString qstrReplaceMatchWith = m_pqleReplaceMatchWith->text();
        InsertRegExMatches(qstrReplaceMatchWith);

        if (m_qlstReplaceTheTextWithTags.isEmpty() == false)
            rqstrName.replace(m_qremRegExMatch.captured(), m_rmtlMetaTagLookup.ReplaceTagCodesWithValues(qstrReplaceMatchWith, m_qlstReplaceTheTextWithTags, pqtwiFileItem), m_puirRenameUI->CaseSensitive() ? Qt::CaseSensitive : Qt::CaseInsensitive);
        else
            rqstrName.replace(m_qremRegExMatch.captured(), qstrReplaceMatchWith, m_puirRenameUI->CaseSensitive() ? Qt::CaseSensitive : Qt::CaseInsensitive);
    }

    if (m_pqcbInsertTheText->isChecked() && m_pqleInsertTheText->text().isEmpty() == false && m_pqleInsertTheTextAtPos->text().isEmpty() == false)
    {
        QString qstrInsertTheText = m_pqleInsertTheText->text();
        InsertRegExMatches(qstrInsertTheText);

        if (m_iInsertTheTextAtPos <= rqstrName.length())
        {
            if (m_qlstInsertTheTextTags.isEmpty() == false)
                rqstrName.insert(m_iInsertTheTextAtPos, m_rmtlMetaTagLookup.ReplaceTagCodesWithValues(qstrInsertTheText, m_qlstInsertTheTextTags, pqtwiFileItem));
            else
                rqstrName.insert(m_iInsertTheTextAtPos, qstrInsertTheText);
        }
    }

    if (m_pqcbInsertAtStart->isChecked() && m_pqleInsertAtStart->text().isEmpty() == false)
    {
        QString qstrInsertAtStart = m_pqleInsertAtStart->text();
        InsertRegExMatches(qstrInsertAtStart);

        if (m_qlstInsertAtStartTags.isEmpty() == false)
            rqstrName.prepend(m_rmtlMetaTagLookup.ReplaceTagCodesWithValues(qstrInsertAtStart, m_qlstInsertAtStartTags, pqtwiFileItem));
        else
            rqstrName.prepend(qstrInsertAtStart);
    }

    if (m_pqcbInsertAtEnd->isChecked() && m_pqleInsertAtEnd->text().isEmpty() == false)
    {
        QString qstrInsertAtEnd = m_pqleInsertAtEnd->text();
        InsertRegExMatches(qstrInsertAtEnd);

        if (m_qlstInsertAtEndTags.isEmpty() == false)
            rqstrName.append(m_rmtlMetaTagLookup.ReplaceTagCodesWithValues(qstrInsertAtEnd, m_qlstInsertAtEndTags, pqtwiFileItem));
        else
            rqstrName.append(qstrInsertAtEnd);
    }
}


void IUIRenameRegEx::InsertRegExMatches(QString & rqstrString)
{
    int iSubExNum;
    int iIndex = rqstrString.indexOf('$');
    while (iIndex != -1 && ++iIndex < rqstrString.length())
    {
        if (rqstrString.at(iIndex).isDigit())
        {
            iSubExNum = rqstrString.at(iIndex).digitValue();
            rqstrString.replace(iIndex-1, 2, m_bRegExMatch ? m_qremRegExMatch.captured(iSubExNum) : ""); ;
        }
        iIndex = rqstrString.indexOf('$', iIndex);
    }
}


void IUIRenameRegEx::SaveSettings(const QString & krqstrSection, QStringList & rqstrlSettings)
{
    QStringList qstrlSettings;

    if (m_pqcbRegExStartPos->isChecked())
        qstrlSettings.push_back("<CB01RegExStartPos>1</CB01RegExStartPos>");
    if (m_pqcbReplaceName->isChecked())
        qstrlSettings.push_back("<CB02ReplaceName>1</CB02ReplaceName>");
    if (m_pqcbReplaceMatchWith->isChecked())
        qstrlSettings.push_back("<CB03ReplaceMatchWithTheText>1</CB03ReplaceMatchWithTheText>");
    if (m_pqcbInsertTheText->isChecked())
        qstrlSettings.push_back("<CB04InsertTheText>1</CB04InsertTheText>");
    if (m_pqcbInsertAtStart->isChecked())
        qstrlSettings.push_back("<CB05InsertAtStart>1</CB05InsertAtStart>");
    if (m_pqcbInsertAtEnd->isChecked())
        qstrlSettings.push_back("<CB06InsertAtEnd>1</CB06InsertAtEnd>");

    if (m_pqleRegEx->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE01RegEx>%1</LE01RegEx>").arg(m_pqleRegEx->text()));
    if (m_pqleRegExStartPos->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE02RegExStartPos>%1</LE02RegExStartPos>").arg(m_pqleRegExStartPos->text()));
    if (m_pqleReplaceName->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE03ReplaceName>%1</LE03ReplaceName>").arg(m_pqleReplaceName->text()));
    if (m_pqleReplaceMatchWith->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE04ReplaceMatchWith>%1</LE04ReplaceMatchWith>").arg(m_pqleReplaceMatchWith->text()));
    if (m_pqleInsertTheText->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE05InsertTheText>%1</LE05InsertTheText>").arg(m_pqleInsertTheText->text()));
    if (m_pqleInsertTheTextAtPos->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE06InsertTheTextAtPos>%1</LE06InsertTheTextAtPos>").arg(m_pqleInsertTheTextAtPos->text()));
    if (m_pqleInsertAtStart->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE07InsertAtStart>%1</LE07InsertAtStart>").arg(m_pqleInsertAtStart->text()));
    if (m_pqleInsertAtEnd->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE08InsertAtEnd>%1</LE08InsertAtEnd>").arg(m_pqleInsertAtEnd->text()));

    if (qstrlSettings.isEmpty() == false)
    {
        rqstrlSettings.push_back(QString("<%1>").arg(krqstrSection));
        rqstrlSettings.append(qstrlSettings);
        rqstrlSettings.push_back(QString("</%1>").arg(krqstrSection));
    }
}


void IUIRenameRegEx::RestoreSettings(const QString & krqstrSettings, int & riIndex)
{
    if (m_bTabEnabled == false)
    {
        SetTabEnabled(true);
        m_puirRenameUI->AddRegExTab(this);
    }

    QString qstrTag, qstrValue;
    while (m_puirRenameUI->ReadSettingsValue(krqstrSettings, riIndex, qstrTag, qstrValue) == true)
    {
        if      (qstrTag.startsWith("LE"))
            RestoreLineEdit(qstrTag, qstrValue);
        else if (qstrTag.startsWith("CB"))
            RestoreCheckBox(qstrTag);
    }
}


void IUIRenameRegEx::RestoreCheckBox(const QString & krqstrTag)
{
    int iCheckBoxID = krqstrTag.mid(2, 2).toInt();
    switch (iCheckBoxID)
    {
    case 1  :   m_pqcbRegExStartPos->setChecked(true);          // CB01RegExStartPos
                break;
    case 2  :   m_pqcbReplaceName->setChecked(true);            // CB02ReplaceName
                break;
    case 3  :   m_pqcbReplaceMatchWith->setChecked(true);       // CB03ReplaceMatchWithTheText
                break;
    case 4  :   m_pqcbInsertTheText->setChecked(true);          // CB04InsertTheText
                break;
    case 5  :   m_pqcbInsertAtStart->setChecked(true);          // CB05InsertAtStart
                break;
    case 6  :   m_pqcbInsertAtEnd->setChecked(true);            // CB06InsertAtEnd
    }
}


void IUIRenameRegEx::RestoreLineEdit(const QString & krqstrTag, const QString & krqstrValue)
{
    int iCheckBoxID = krqstrTag.mid(2, 2).toInt();
    switch (iCheckBoxID)
    {
    case 1  :   m_pqleRegEx->setText(krqstrValue);              // LE01RegEx
                break;
    case 2  :   m_pqleRegExStartPos->setText(krqstrValue);      // LE02RegExStartPos
                break;
    case 3  :   m_pqleReplaceName->setText(krqstrValue);        // LE03ReplaceName
                break;
    case 4  :   m_pqleReplaceMatchWith->setText(krqstrValue);   // LE04ReplaceMatchWith
                break;
    case 5  :   m_pqleInsertTheText->setText(krqstrValue);      // LE05InsertTheText
                break;
    case 6  :   m_pqleInsertTheTextAtPos->setText(krqstrValue); // LE06InsertTheTextAtPos
                break;
    case 7  :   m_pqleInsertAtStart->setText(krqstrValue);      // LE07InsertAtStart
                break;
    case 8  :   m_pqleInsertAtEnd->setText(krqstrValue);        // LE08InsertAtEnd
    }
}


void IUIRenameRegEx::SetTabEnabled(const bool kbEnabled)
{
    m_bTabEnabled = kbEnabled;
    setVisible(kbEnabled);

    if (kbEnabled == false)
        m_puirRenameUI->ClearTab(this);
}
