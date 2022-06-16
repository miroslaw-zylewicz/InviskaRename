#include <QtWidgets>
#include "IUIRenameName.h"
#include "IUIRename.h"
#include "IUIFileList.h"
#include "IUIMainWindow.h"


IUIRenameName::IUIRenameName(IUIRename* puirRenameUI, const int kiRenameElement, const int kiTabID) : IUIRenameTabBase(puirRenameUI, kiRenameElement, kiTabID)
{
    setupUi(this);
    SetValidators();
    CreateConnections();
}


void IUIRenameName::SetValidators()
{
    QIntValidator* pivalIntOnlyValidator = IUIMainWindow::GetMainWindow()->GetIntOnlyValidator();

    m_pqleInsertTheTextAtPos->setValidator(pivalIntOnlyValidator);
    m_pqleCropAtPos->setValidator(pivalIntOnlyValidator);
    m_pqleCropAtPosNextNChar->setValidator(pivalIntOnlyValidator);
    m_pqleLeftCropNChar->setValidator(pivalIntOnlyValidator);
    m_pqleRightCropNChar->setValidator(pivalIntOnlyValidator);

    QRegExpValidator* pqrevInvalidCharValidator = IUIMainWindow::GetMainWindow()->GetInvalidCharValidator();

    m_pqleReplaceName->setValidator(pqrevInvalidCharValidator);
    m_pqleReplaceTheTextWith->setValidator(pqrevInvalidCharValidator);
    m_pqleInsertTheText->setValidator(pqrevInvalidCharValidator);
    m_pqleInsertAtStart->setValidator(pqrevInvalidCharValidator);
    m_pqleInsertAtEnd->setValidator(pqrevInvalidCharValidator);
}


void IUIRenameName::CreateConnections()
{
    connect(m_pqcbReplaceName,          SIGNAL(toggled(bool)),                  this,               SLOT(SettingsChangedReplaceName()));
    connect(m_pqleReplaceName,          SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedReplaceName()));

    connect(m_pqcbReplaceTheText,       SIGNAL(toggled(bool)),                  this,               SLOT(SettingsChangedReplaceTheText()));
    connect(m_pqleReplaceTheText,       SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedReplaceTheText()));
    connect(m_pqleReplaceTheTextWith,   SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedReplaceTheText()));

    connect(m_pqcbInsertTheText,        SIGNAL(toggled(bool)),                  this,               SLOT(SettingsChangedInsertTheText()));
    connect(m_pqleInsertTheText,        SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedInsertTheText()));
    connect(m_pqleInsertTheTextAtPos,   SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedInsertTheText()));

    connect(m_pqcbInsertAtStart,        SIGNAL(toggled(bool)),                  this,               SLOT(SettingsChangedInsertAtStart()));
    connect(m_pqleInsertAtStart,        SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedInsertAtStart()));

    connect(m_pqcbInsertAtEnd,          SIGNAL(toggled(bool)),                  this,               SLOT(SettingsChangedInsertAtEnd()));
    connect(m_pqleInsertAtEnd,          SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedInsertAtEnd()));

    connect(m_pqcbCropAtPos,            SIGNAL(toggled(bool)),                  this,               SLOT(SettingsChangedCropAtPos()));
    connect(m_pqleCropAtPos,            SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedCropAtPos()));
    connect(m_pqleCropAtPosNextNChar,   SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedCropAtPos()));

    connect(m_pqcbLeftCropNChar,        SIGNAL(toggled(bool)),                  this,               SLOT(SettingsChangedLeftCrop()));
    connect(m_pqleLeftCropNChar,        SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedLeftCrop()));

    connect(m_pqcbRightCropNChar,       SIGNAL(toggled(bool)),                  this,               SLOT(SettingsChangedRightCrop()));
    connect(m_pqleRightCropNChar,       SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChangedRightCrop()));

    connect(m_pqcboChangeCase,          SIGNAL(currentIndexChanged(int)),       this,               SLOT(SetingsChanged()));

    connect(m_pqleReplaceName,          SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(StoreLastActiveLineEdit()));
    connect(m_pqleReplaceTheTextWith,   SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(StoreLastActiveLineEdit()));
    connect(m_pqleInsertTheText,        SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(StoreLastActiveLineEdit()));
    connect(m_pqleInsertAtStart,        SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(StoreLastActiveLineEdit()));
    connect(m_pqleInsertAtEnd,          SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(StoreLastActiveLineEdit()));

    connect(m_pqleReplaceTheText,       SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(ClearLastActiveLineEdit()));
    connect(m_pqleInsertTheTextAtPos,   SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(ClearLastActiveLineEdit()));
    connect(m_pqleCropAtPos,            SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(ClearLastActiveLineEdit()));
    connect(m_pqleCropAtPosNextNChar,   SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(ClearLastActiveLineEdit()));
    connect(m_pqleLeftCropNChar,        SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(ClearLastActiveLineEdit()));
    connect(m_pqleRightCropNChar,       SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(ClearLastActiveLineEdit()));
}


void IUIRenameName::SettingsChangedReplaceName()
{
    ReadTagCodes(m_pqcbReplaceName, m_pqleReplaceName, m_qlstReplaceNameTags);
    SetingsChanged();
}


void IUIRenameName::SettingsChangedReplaceTheText()
{
    ReadTagCodes(m_pqcbReplaceTheText, m_pqleReplaceTheTextWith, m_qlstReplaceTheTextWithTags);
    SetingsChanged();
}


void IUIRenameName::SettingsChangedInsertTheText()
{
    m_iInsertTheTextAtPos = m_pqleInsertTheTextAtPos->text().toInt();
    ReadTagCodes(m_pqcbInsertTheText, m_pqleInsertTheText, m_qlstInsertTheTextTags);
    SetingsChanged();
}


void IUIRenameName::SettingsChangedInsertAtStart()
{
    ReadTagCodes(m_pqcbInsertAtStart, m_pqleInsertAtStart, m_qlstInsertAtStartTags);
    SetingsChanged();
}


void IUIRenameName::SettingsChangedInsertAtEnd()
{
    ReadTagCodes(m_pqcbInsertAtEnd, m_pqleInsertAtEnd, m_qlstInsertAtEndTags);
    SetingsChanged();
}


void IUIRenameName::SettingsChangedCropAtPos()
{
    m_iCropAtPos          = m_pqleCropAtPos->text().toInt();
    m_iCropAtPosNextNChar = m_pqleCropAtPosNextNChar->text().toInt();
    SetingsChanged();
}


void IUIRenameName::SettingsChangedLeftCrop()
{
    m_iLeftCropNChar = m_pqleLeftCropNChar->text().toInt();
    SetingsChanged();
}


void IUIRenameName::SettingsChangedRightCrop()
{
    m_iRightCropNChar = m_pqleRightCropNChar->text().toInt();
    SetingsChanged();
}


void IUIRenameName::SetingsChanged()
{
    if (m_iRenameElement == Name)
        m_puifmFileList->GeneratePreviewName();
    else
        m_puifmFileList->GeneratePreviewNameAndExtension();
}


void IUIRenameName::ReReadTagCodes()
{
    ReadTagCodes(m_pqcbReplaceName, m_pqleReplaceName, m_qlstReplaceNameTags);
    ReadTagCodes(m_pqcbReplaceTheText, m_pqleReplaceTheTextWith, m_qlstReplaceTheTextWithTags);
    ReadTagCodes(m_pqcbInsertTheText, m_pqleInsertTheText, m_qlstInsertTheTextTags);
    ReadTagCodes(m_pqcbInsertAtStart, m_pqleInsertAtStart, m_qlstInsertAtStartTags);
    ReadTagCodes(m_pqcbInsertAtEnd, m_pqleInsertAtEnd, m_qlstInsertAtEndTags);
}


void IUIRenameName::DisableAllSettings()
{
    m_pqcbReplaceName->setChecked(false);
    m_pqcbReplaceTheText->setChecked(false);
    m_pqcbInsertTheText->setChecked(false);
    m_pqcbInsertAtStart->setChecked(false);
    m_pqcbInsertAtEnd->setChecked(false);
    m_pqcbCropAtPos->setChecked(false);
    m_pqcbLeftCropNChar->setChecked(false);
    m_pqcbRightCropNChar->setChecked(false);
    m_pqcboChangeCase->setCurrentIndex(0);
}


void IUIRenameName::ClearAll()
{
    DisableAllSettings();

    m_pqleReplaceName->clear();
    m_pqleReplaceTheText->clear();
    m_pqleReplaceTheTextWith->clear();
    m_pqleInsertTheText->clear();
    m_pqleInsertTheTextAtPos->clear();
    m_pqleInsertAtStart->clear();
    m_pqleInsertAtEnd->clear();
    m_pqleCropAtPos->clear();
    m_pqleCropAtPosNextNChar->clear();
    m_pqleLeftCropNChar->clear();
    m_pqleRightCropNChar->clear();
}


void IUIRenameName::CheckForMetaTags(bool & rbMusicTags, bool & rbExifTags)
{
    CheckListForMetaTags(m_qlstReplaceNameTags, rbMusicTags, rbExifTags);
    CheckListForMetaTags(m_qlstReplaceTheTextWithTags, rbMusicTags, rbExifTags);
    CheckListForMetaTags(m_qlstInsertTheTextTags, rbMusicTags, rbExifTags);
    CheckListForMetaTags(m_qlstInsertAtStartTags, rbMusicTags, rbExifTags);
    CheckListForMetaTags(m_qlstInsertAtEndTags, rbMusicTags, rbExifTags);
}


void IUIRenameName::GenerateName(QString & rqstrName, QTableWidgetItem* pqtwiFileItem)
{
    if (m_pqcbReplaceName->isChecked())
    {
        if (m_qlstReplaceNameTags.isEmpty() == false)
            rqstrName = m_rmtlMetaTagLookup.ReplaceTagCodesWithValues(m_pqleReplaceName->text(), m_qlstReplaceNameTags, pqtwiFileItem);
        else
            rqstrName = m_pqleReplaceName->text();
    }

    if (m_pqcbReplaceTheText->isChecked() && m_pqleReplaceTheText->text().isEmpty() == false)
    {
        if (m_qlstReplaceTheTextWithTags.isEmpty() == false)
            rqstrName.replace(m_pqleReplaceTheText->text(), m_rmtlMetaTagLookup.ReplaceTagCodesWithValues(m_pqleReplaceTheTextWith->text(), m_qlstReplaceTheTextWithTags, pqtwiFileItem), m_puirRenameUI->CaseSensitive() ? Qt::CaseSensitive : Qt::CaseInsensitive);
        else
            rqstrName.replace(m_pqleReplaceTheText->text(), m_pqleReplaceTheTextWith->text(), m_puirRenameUI->CaseSensitive() ? Qt::CaseSensitive : Qt::CaseInsensitive);
    }

    if (m_pqcbInsertTheText->isChecked() && m_pqleInsertTheText->text().isEmpty() == false && m_pqleInsertTheTextAtPos->text().isEmpty() == false)
    {
        if (m_iInsertTheTextAtPos <= rqstrName.length())
        {
            if (m_qlstInsertTheTextTags.isEmpty() == false)
                rqstrName.insert(m_iInsertTheTextAtPos, m_rmtlMetaTagLookup.ReplaceTagCodesWithValues(m_pqleInsertTheText->text(), m_qlstInsertTheTextTags, pqtwiFileItem));
            else
                rqstrName.insert(m_iInsertTheTextAtPos, m_pqleInsertTheText->text());
        }
    }

    if (m_pqcbInsertAtStart->isChecked() && m_pqleInsertAtStart->text().isEmpty() == false)
    {
        if (m_qlstInsertAtStartTags.isEmpty() == false)
            rqstrName.prepend(m_rmtlMetaTagLookup.ReplaceTagCodesWithValues(m_pqleInsertAtStart->text(), m_qlstInsertAtStartTags, pqtwiFileItem));
        else
            rqstrName.prepend(m_pqleInsertAtStart->text());
    }

    if (m_pqcbInsertAtEnd->isChecked() && m_pqleInsertAtEnd->text().isEmpty() == false)
    {
        if (m_qlstInsertAtEndTags.isEmpty() == false)
            rqstrName.append(m_rmtlMetaTagLookup.ReplaceTagCodesWithValues(m_pqleInsertAtEnd->text(), m_qlstInsertAtEndTags, pqtwiFileItem));
        else
            rqstrName.append(m_pqleInsertAtEnd->text());
    }

    if (m_pqcbCropAtPos->isChecked() && m_pqleCropAtPos->text().isEmpty() == false && m_pqleCropAtPosNextNChar->text().isEmpty() == false)
       rqstrName.remove(m_iCropAtPos, m_iCropAtPosNextNChar);

    if (m_pqcbLeftCropNChar->isChecked() && m_pqleLeftCropNChar->text().isEmpty() == false)
       rqstrName.remove(0, m_iLeftCropNChar);

    if (m_pqcbRightCropNChar->isChecked()  && m_pqleRightCropNChar->text().isEmpty() == false)
       rqstrName.truncate(rqstrName.length() - m_iRightCropNChar);

    switch (m_pqcboChangeCase->currentIndex())
    {
        case CaseNoChange  : break;

        case CaseTitle     : ConvertNameToTitleCase(rqstrName);
                             break;

        case CaseSentance  : rqstrName = rqstrName.toLower();
                             rqstrName[0] = rqstrName.at(0).toUpper();
                             break;

        case CaseLower     : rqstrName = rqstrName.toLower();
                             break;

        case CaseUpper     : rqstrName = rqstrName.toUpper();
    }
}


void IUIRenameName::ConvertNameToTitleCase(QString & rqstrName)
{
    QString qstrWordBreakChars = " -()[]{}.,;:/\\";
    rqstrName = rqstrName.toLower();
    rqstrName[0] = rqstrName[0].toUpper();

    int iLength = rqstrName.length();
    for (int iIndex = 1 ; iIndex < iLength ; ++iIndex)
    {
        if (qstrWordBreakChars.contains(rqstrName[iIndex-1]))
            rqstrName[iIndex] = rqstrName[iIndex].toUpper();
    }
}


void IUIRenameName::SaveSettings(const QString & krqstrSection, QStringList & rqstrlSettings)
{
    QStringList qstrlSettings;

    if (m_pqcbReplaceName->isChecked())
        qstrlSettings.push_back("<CB01ReplaceName>1</CB01ReplaceName>");
    if (m_pqcbReplaceTheText->isChecked())
        qstrlSettings.push_back("<CB02ReplaceTheText>1</CB02ReplaceTheText>");
    if (m_pqcbInsertTheText->isChecked())
        qstrlSettings.push_back("<CB03InsertTheText>1</CB03InsertTheText>");
    if (m_pqcbInsertAtStart->isChecked())
        qstrlSettings.push_back("<CB04InsertAtStart>1</CB04InsertAtStart>");
    if (m_pqcbInsertAtEnd->isChecked())
        qstrlSettings.push_back("<CB05InsertAtEnd>1</CB05InsertAtEnd>");
    if (m_pqcbCropAtPos->isChecked())
        qstrlSettings.push_back("<CB06CropAtPos>1</CB06CropAtPos>");
    if (m_pqcbLeftCropNChar->isChecked())
        qstrlSettings.push_back("<CB07LeftCropNChar>1</CB07LeftCropNChar>");
    if (m_pqcbRightCropNChar->isChecked())
        qstrlSettings.push_back("<CB08RightCropNChar>1</CB08RightCropNChar>");

    if (m_pqleReplaceName->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE01ReplaceName>%1</LE01ReplaceName>").arg(m_pqleReplaceName->text()));
    if (m_pqleReplaceTheText->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE02ReplaceTheText>%1</LE02ReplaceTheText>").arg(m_pqleReplaceTheText->text()));
    if (m_pqleReplaceTheTextWith->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE03ReplaceTheTextWith>%1</LE03ReplaceTheTextWith>").arg(m_pqleReplaceTheTextWith->text()));
    if (m_pqleInsertTheText->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE04InsertTheText>%1</LE04InsertTheText>").arg(m_pqleInsertTheText->text()));
    if (m_pqleInsertTheTextAtPos->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE05InsertTheTextAtPos>%1</LE05InsertTheTextAtPos>").arg(m_pqleInsertTheTextAtPos->text()));
    if (m_pqleInsertAtStart->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE06InsertAtStart>%1</LE06InsertAtStart>").arg(m_pqleInsertAtStart->text()));
    if (m_pqleInsertAtEnd->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE07InsertAtEnd>%1</LE07InsertAtEnd>").arg(m_pqleInsertAtEnd->text()));
    if (m_pqleCropAtPos->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE08CropAtPos>%1</LE08CropAtPos>").arg(m_pqleCropAtPos->text()));
    if (m_pqleCropAtPosNextNChar->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE09CropAtPosNextN>%1</LE09CropAtPosNextN>").arg(m_pqleCropAtPosNextNChar->text()));
    if (m_pqleLeftCropNChar->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE10LeftCropNChar>%1</LE10LeftCropNChar>").arg(m_pqleLeftCropNChar->text()));
    if (m_pqleRightCropNChar->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE11RightCropNChar>%1</LE11RightCropNChar>").arg(m_pqleRightCropNChar->text()));

    if (m_pqcboChangeCase->currentIndex() != 0)
        qstrlSettings.push_back(QString("<COM01ChangeCase>%1</COM01ChangeCase>").arg(m_pqcboChangeCase->currentIndex()));

    if (qstrlSettings.isEmpty() == false)
    {
        rqstrlSettings.push_back(QString("<%1>").arg(krqstrSection));
        rqstrlSettings.append(qstrlSettings);
        rqstrlSettings.push_back(QString("</%1>").arg(krqstrSection));
    }
}


void IUIRenameName::RestoreSettings(const QString & krqstrSettings, int & riIndex)
{
    QString qstrTag, qstrValue;
    while (m_puirRenameUI->ReadSettingsValue(krqstrSettings, riIndex, qstrTag, qstrValue) == true)
    {
        if      (qstrTag.startsWith("LE"))
            RestoreLineEdit(qstrTag, qstrValue);
        else if (qstrTag.startsWith("CB"))
            RestoreCheckBox(qstrTag);
        else if (qstrTag.startsWith("COM"))
            RestoreComboBox(qstrTag, qstrValue);
    }
}


void IUIRenameName::RestoreCheckBox(const QString & krqstrTag)
{
    int iCheckBoxID = krqstrTag.mid(2, 2).toInt();
    switch (iCheckBoxID)
    {
    case 1  :   m_pqcbReplaceName->setChecked(true);            // CB01ReplaceName
                break;
    case 2  :   m_pqcbReplaceTheText->setChecked(true);         // CB02ReplaceTheText
                break;
    case 3  :   m_pqcbInsertTheText->setChecked(true);          // CB03InsertTheText
                break;
    case 4  :   m_pqcbInsertAtStart->setChecked(true);          // CB04InsertAtStart
                break;
    case 5  :   m_pqcbInsertAtEnd->setChecked(true);            // CB05InsertAtEnd
                break;
    case 6  :   m_pqcbCropAtPos->setChecked(true);              // CB06CropAtPos
                break;
    case 7  :   m_pqcbLeftCropNChar->setChecked(true);          // CB07LeftCropNChar
                break;
    case 8  :   m_pqcbRightCropNChar->setChecked(true);         // CB08RightCropNChar
    }
}


void IUIRenameName::RestoreLineEdit(const QString & krqstrTag, const QString & krqstrValue)
{
    int iCheckBoxID = krqstrTag.mid(2, 2).toInt();
    switch (iCheckBoxID)
    {
    case 1  :   m_pqleReplaceName->setText(krqstrValue);        // LE01ReplaceName
                break;
    case 2  :   m_pqleReplaceTheText->setText(krqstrValue);     // LE02ReplaceTheText
                break;
    case 3  :   m_pqleReplaceTheTextWith->setText(krqstrValue); // LE03ReplaceTheTextWith
                break;
    case 4  :   m_pqleInsertTheText->setText(krqstrValue);      // LE04InsertTheText
                break;
    case 5  :   m_pqleInsertTheTextAtPos->setText(krqstrValue); // LE05InsertTheTextAtPos
                break;
    case 6  :   m_pqleInsertAtStart->setText(krqstrValue);      // LE06InsertAtStart
                break;
    case 7  :   m_pqleInsertAtEnd->setText(krqstrValue);        // LE07InsertAtEnd
                break;
    case 8  :   m_pqleCropAtPos->setText(krqstrValue);          // LE08CropAtPos
                break;
    case 9  :   m_pqleCropAtPosNextNChar->setText(krqstrValue); // LE09CropAtPosNextN
                break;
    case 10 :   m_pqleLeftCropNChar->setText(krqstrValue);      // LE10LeftCropNChar
                break;
    case 11 :   m_pqleRightCropNChar->setText(krqstrValue);     // LE11RightCropNChar
    }
}


void IUIRenameName::RestoreComboBox(const QString & krqstrTag, const QString & krqstrValue)
{
    int iCheckBoxID = krqstrTag.mid(3, 2).toInt();
    switch (iCheckBoxID)
    {
    case 1  :   m_pqcboChangeCase->setCurrentIndex(krqstrValue.toInt()); // COM01ChangeCase
    }
}
