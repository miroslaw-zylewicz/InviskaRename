#include <QtWidgets>
#include "IUIRename.h"
#include "IUIRenameFilter.h"
#include "IUIRenameName.h"
#include "IUIRenameNumber.h"
#include "IUIRenameRegEx.h"
#include "IUIMainWindow.h"
#include "IUIMenuBar.h"
#include "IUIMenuRenames.h"
#include "IUIFileList.h"



IUIRename::IUIRename(IUIMainWindow* pmwMainWindow) : QWidget(pmwMainWindow)
{
    m_pmwMainWindow = pmwMainWindow;
    m_puifmFileList = pmwMainWindow->GetFileListUI();
    m_pqleLastActiveLineEdit = nullptr;
    m_bChangingSettings = false;

    QSettings & rqsetSettings = m_pmwMainWindow->GetSettings();
    rqsetSettings.beginGroup("Rename");
    m_bCaseSensitive = rqsetSettings.value("CaseSensitive", false).toBool();
    m_bShowConfirmBeforeRename = rqsetSettings.value("ShowConfirmBeforeRename", true).toBool();
    m_bDeactivateSettingsAfterRename = rqsetSettings.value("DeactivateSettingsAfterRename", true).toBool();
    m_bSaveActiveTabWithRename = rqsetSettings.value("SaveActiveTabWithRename", true).toBool();
    m_bSaveCaseSensitivityWithRename = rqsetSettings.value("SaveCaseSensitivityWithRename", true).toBool();
    rqsetSettings.endGroup();

    m_mtlMetaTagLookup.InitExifLookupHash(m_puifmFileList->ExifAdvancedModeEnabled());

    m_purfFilter     = new IUIRenameFilter(this);
    m_purnName       = new IUIRenameName(this,  IUIRenameName::Name,      Name);
    m_purnExten      = new IUIRenameName(this,  IUIRenameName::Extension, Extension);
    m_purnNumber     = new IUIRenameNumber(this,IUIRenameName::Numbering, Numbering);
    m_purnRegExName1 = new IUIRenameRegEx(this, IUIRenameName::Name,      RegExName1, QString("RegEx %1 1").arg(tr("Name")));
    m_purnRegExName2 = new IUIRenameRegEx(this, IUIRenameName::Name,      RegExName2, QString("RegEx %1 2").arg(tr("Name")));
    m_purnRegExName3 = new IUIRenameRegEx(this, IUIRenameName::Name,      RegExName3, QString("RegEx %1 3").arg(tr("Name")));
    m_purnRegExExten = new IUIRenameRegEx(this, IUIRenameName::Extension, RegExExten, QString("RegEx %1").arg(tr("Extension")));

    m_pqtwRenameSettingsTab = new QTabWidget;
    m_pqtwRenameSettingsTab->addTab(m_purnName, tr("Name"));
    m_pqtwRenameSettingsTab->addTab(m_purnExten, tr("Extension"));
    m_pqtwRenameSettingsTab->addTab(m_purnNumber, tr("Numbering"));
    AddRegExTabs();

    m_pqpbRenameButton = new QPushButton(tr("Rename"));
    m_pqpbUndoButton = new QPushButton(tr("Undo"));
    m_pqpbRenameButton->setEnabled(false);
    m_pqpbUndoButton->setEnabled(false);

    QHBoxLayout* pqhblButtonLayout = new QHBoxLayout;
    pqhblButtonLayout->addWidget(m_pqpbRenameButton);
    pqhblButtonLayout->addWidget(m_pqpbUndoButton);

    QVBoxLayout* pqvblRightLayout = new QVBoxLayout;
    pqvblRightLayout->addWidget(m_purfFilter);
    pqvblRightLayout->addLayout(pqhblButtonLayout);

    QHBoxLayout* pqhblLayout = new QHBoxLayout;
    pqhblLayout->addWidget(m_pqtwRenameSettingsTab);
    pqhblLayout->addLayout(pqvblRightLayout);
    pqhblLayout->setContentsMargins(0, 3, 0, 5);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setLayout(pqhblLayout);

    connect(m_pqpbRenameButton,    SIGNAL(clicked()),      this,     SLOT(ConfirmRename()));
    connect(m_pqpbUndoButton,      SIGNAL(clicked()),      this,     SLOT(ConfirmUndo()));

    // On the Mac, if a line edit has focus and you change tab, the default item on the next tab gets keyboard focus.
    // This results in a glow around the default check boxes or radio boxes to indicate they have keybaord focus.
    // This glow doesn't go away, even if you click another contorl, which looks strange and is rather confusing.
    // To avoid this we give focus to some line edits, which results in them getting focus when the tab changes.
    #ifdef Q_OS_MACOS
    m_purnNumber->m_pqleNumberingAtPos->setEnabled(true);
    m_purnNumber->m_pqleNumberingAtPos->setFocus();
    m_purnNumber->m_pqleNumberingAtPos->setEnabled(false);
    m_purnExten->m_pqleReplaceName->setEnabled(true);
    m_purnExten->m_pqleReplaceName->setFocus();
    m_purnExten->m_pqleReplaceName->setEnabled(false);
    m_purnName->m_pqleReplaceName->setEnabled(true);
    m_purnName->m_pqleReplaceName->setFocus();
    m_purnName->m_pqleReplaceName->setEnabled(false);
    #endif
}


IUIRename::~IUIRename()
{
    QSettings & rqsetSettings = m_pmwMainWindow->GetSettings();
    rqsetSettings.beginGroup("RegEx");
    rqsetSettings.setValue("RegExName1", m_purnRegExName1->TabEnabled());
    rqsetSettings.setValue("RegExName2", m_purnRegExName2->TabEnabled());
    rqsetSettings.setValue("RegExName3", m_purnRegExName3->TabEnabled());
    rqsetSettings.setValue("RegExExten", m_purnRegExExten->TabEnabled());
    rqsetSettings.endGroup();
}


void IUIRename::AddRegExTabs()
{
    QSettings & rqsetSettings = m_pmwMainWindow->GetSettings();
    rqsetSettings.beginGroup("RegEx");
    m_purnRegExName1->InitTabEnabled(rqsetSettings.value("RegExName1", false).toBool());
    m_purnRegExName2->InitTabEnabled(rqsetSettings.value("RegExName2", false).toBool());
    m_purnRegExName3->InitTabEnabled(rqsetSettings.value("RegExName3", false).toBool());
    m_purnRegExExten->InitTabEnabled(rqsetSettings.value("RegExExten", false).toBool());
    rqsetSettings.endGroup();

    if (m_purnRegExName1->TabEnabled())
        m_pqtwRenameSettingsTab->addTab(m_purnRegExName1, m_purnRegExName1->GetTabName());
    if (m_purnRegExName2->TabEnabled())
        m_pqtwRenameSettingsTab->addTab(m_purnRegExName2, m_purnRegExName2->GetTabName());
    if (m_purnRegExName3->TabEnabled())
        m_pqtwRenameSettingsTab->addTab(m_purnRegExName3, m_purnRegExName3->GetTabName());
    if (m_purnRegExExten->TabEnabled())
        m_pqtwRenameSettingsTab->addTab(m_purnRegExExten, m_purnRegExExten->GetTabName());
}


void IUIRename::AddRegExTab(IUIRenameRegEx* purnRegExTab)
{
    purnRegExTab->SetTabEnabled(true);
    int iTabID = purnRegExTab->GetTabID();

    int iIndex = RegExName1;
    int iNumTabs = m_pqtwRenameSettingsTab->count();
    while (iIndex < iNumTabs)
    {
        if (iTabID < static_cast<IUIRenameTabBase*>(m_pqtwRenameSettingsTab->widget(iIndex))->GetTabID())
        {
            m_pqtwRenameSettingsTab->insertTab(iIndex, purnRegExTab, purnRegExTab->GetTabName());
            return;
        }
        ++iIndex;
    }
    m_pqtwRenameSettingsTab->addTab(purnRegExTab, purnRegExTab->GetTabName());
}


void IUIRename::SetRegExTabVisibility(const bool kbRegExName1, const bool kbRegExName2, const bool kbRegExName3, const bool kbRegExExten)
{
    int iIndex = RegExName1;
    SetRegExTabEnabled(m_purnRegExName1, kbRegExName1, iIndex);
    SetRegExTabEnabled(m_purnRegExName2, kbRegExName2, iIndex);
    SetRegExTabEnabled(m_purnRegExName3, kbRegExName3, iIndex);
    SetRegExTabEnabled(m_purnRegExExten, kbRegExExten, iIndex);
}


void IUIRename::SetRegExTabEnabled(IUIRenameRegEx* purnRegExTab, const bool kbEnabled, int & riIndex)
{
    if (kbEnabled != purnRegExTab->TabEnabled())
    {
        purnRegExTab->SetTabEnabled(kbEnabled);
        if (kbEnabled)
            m_pqtwRenameSettingsTab->insertTab(riIndex, purnRegExTab, purnRegExTab->GetTabName());
        else
            m_pqtwRenameSettingsTab->removeTab(riIndex);
    }

    if (purnRegExTab->TabEnabled())
        ++riIndex;
}


void IUIRename::SetCaseSensitivity(const bool kbCaseSensitive)
{
    if (kbCaseSensitive != m_bCaseSensitive)
    {
        m_bCaseSensitive = kbCaseSensitive;
        m_purnRegExName1->SetCaseSensitivity(m_bCaseSensitive);
        m_purnRegExName2->SetCaseSensitivity(m_bCaseSensitive);
        m_purnRegExName3->SetCaseSensitivity(m_bCaseSensitive);
        m_purnRegExExten->SetCaseSensitivity(m_bCaseSensitive);
        m_pmwMainWindow->GetSettings().setValue("Rename/CaseSensitive", m_bCaseSensitive);

        m_purfFilter->ReadExtensions(m_bCaseSensitive);
        m_puifmFileList->GeneratePreviewNameAndExtension();
    }
}


void IUIRename::DisableAllSettings()
{
    m_bChangingSettings = true;

    m_purfFilter->DisableAllSettings();
    m_purnName->DisableAllSettings();
    m_purnExten->DisableAllSettings();
    m_purnNumber->DisableAllSettings();
    m_purnRegExName1->DisableAllSettings();
    m_purnRegExName2->DisableAllSettings();
    m_purnRegExName3->DisableAllSettings();
    m_purnRegExExten->DisableAllSettings();

    m_bChangingSettings = false;
    m_puifmFileList->GeneratePreviewNameAndExtension();
}


void IUIRename::ClearAll(const bool kbRegenerateName)
{
    m_bChangingSettings = true;

    m_purfFilter->ClearAll();
    m_purnName->ClearAll();
    m_purnExten->ClearAll();
    m_purnNumber->ClearAll();
    m_purnRegExName1->ClearAll();
    m_purnRegExName2->ClearAll();
    m_purnRegExName3->ClearAll();
    m_purnRegExExten->ClearAll();

    m_bChangingSettings = false;

    if (kbRegenerateName)
        m_puifmFileList->GeneratePreviewNameAndExtension();
}


void IUIRename::ClearTab(IUIRenameTabBase* purnTab)
{
    m_bChangingSettings = true;
    purnTab->ClearAll();
    m_bChangingSettings = false;
}


void IUIRename::StoreLastActiveLineEdit()
{
    m_pqleLastActiveLineEdit = qobject_cast<QLineEdit*>(sender());
}


void IUIRename::ClearLastActiveLineEdit()
{
    m_pqleLastActiveLineEdit = nullptr;
}


void IUIRename::CheckForMetaTags(bool & rbMusicTags, bool & rbExifTags)
{
    m_purnName->CheckForMetaTags(rbMusicTags, rbExifTags);
    m_purnExten->CheckForMetaTags(rbMusicTags, rbExifTags);
    m_purnRegExName1->CheckForMetaTags(rbMusicTags, rbExifTags);
    m_purnRegExName2->CheckForMetaTags(rbMusicTags, rbExifTags);
    m_purnRegExName3->CheckForMetaTags(rbMusicTags, rbExifTags);
    m_purnRegExExten->CheckForMetaTags(rbMusicTags, rbExifTags);
}


void IUIRename::AddActionLookupMusic(QAction* qactAction, const QString & qstrTagCode)
{
    m_qhashActionLookupMusic[qactAction] = qstrTagCode;
    QObject::connect(qactAction, SIGNAL(triggered()), this, SLOT(InsertTagCodeMusic()));
}


void IUIRename::AddActionLookupExif(QAction* qactAction, const QString & qstrTagCode)
{
    m_qhashActionLookupExif[qactAction] = qstrTagCode;
    QObject::connect(qactAction, SIGNAL(triggered()), this, SLOT(InsertTagCodeExif()));
}


void IUIRename::AddActionLookupFiAt(QAction* qactAction, const QString & qstrTagCode)
{
    m_qhashActionLookupFiAt[qactAction] = qstrTagCode;
    QObject::connect(qactAction, SIGNAL(triggered()), this, SLOT(InsertTagCodeFiAt()));
}


void IUIRename::InsertTagCodeMusic()
{
    if (m_pqleLastActiveLineEdit != nullptr && m_pqleLastActiveLineEdit->isEnabled())
        InsertTagCode(m_qhashActionLookupMusic[qobject_cast<QAction*>(sender())]);
}


void IUIRename::InsertTagCodeExif()
{
    if (m_pqleLastActiveLineEdit != nullptr && m_pqleLastActiveLineEdit->isEnabled())
        InsertTagCode(m_qhashActionLookupExif[qobject_cast<QAction*>(sender())]);
}


void IUIRename::InsertTagCodeFiAt()
{
    if (m_pqleLastActiveLineEdit != nullptr && m_pqleLastActiveLineEdit->isEnabled())        
        InsertTagCode(m_qhashActionLookupFiAt[qobject_cast<QAction*>(sender())]);
}


void IUIRename::InsertTagCode(const QString & krqstrTagCode)
{
    int iCursorPos = m_pqleLastActiveLineEdit->cursorPosition();
    QString qstrText = m_pqleLastActiveLineEdit->text();
    qstrText.insert(iCursorPos, krqstrTagCode);
    m_pqleLastActiveLineEdit->setText(qstrText);
    m_pqleLastActiveLineEdit->setCursorPosition(iCursorPos + krqstrTagCode.length());
}


void IUIRename::ReReadTagCodes()
{
    m_purnName->ReReadTagCodes();
    m_purnExten->ReReadTagCodes();
    m_purnRegExName1->ReReadTagCodes();
    m_purnRegExName2->ReReadTagCodes();
    m_purnRegExName3->ReReadTagCodes();
    m_purnRegExExten->ReReadTagCodes();
}


void IUIRename::ConfirmRename()
{
    int iResponse = QMessageBox::Yes;
    if (m_bShowConfirmBeforeRename)
        iResponse = QMessageBox::question(this, tr("Continue With Rename?"),tr("Do you wish to proceed with the rename operation?"), QMessageBox::Yes | QMessageBox::No);

    if (iResponse == QMessageBox::Yes)
    {
        m_puifmFileList->PerformRename();

        if (m_bDeactivateSettingsAfterRename)
            DisableAllSettings();
    }
}


void IUIRename::ConfirmUndo()
{
    int iResponse = QMessageBox::Yes;
    if (m_bShowConfirmBeforeRename)
        iResponse = QMessageBox::question(this, tr("Continue With Undo?"),tr("Do you wish to undo the last rename operation?"), QMessageBox::Yes | QMessageBox::No);

    if (iResponse == QMessageBox::Yes)
    {
        m_puifmFileList->UndoRename();

        if (m_bDeactivateSettingsAfterRename)
            DisableAllSettings();
    }
}


void IUIRename::EnableRenameButton(const bool kbEnabled)
{
    m_pqpbRenameButton->setEnabled(kbEnabled);
}


void IUIRename::EnableUndoButton(const bool kbEnabled)
{
    // The next control in the tab order after Undo is the address bar, which gets activated when the Undo button is deactivated.
    // This felt a bit odd, particularly when all the text was selected in the address bar, so we set focus to Name tab before deactivating Undo.
    if (kbEnabled == false)
        m_purnName->setFocus();

    m_pqpbUndoButton->setEnabled(kbEnabled);
}


void IUIRename::GenerateName(QString & rqstrName, QTableWidgetItem* pqtwiFileItem)
{
    m_purnName->GenerateName(rqstrName, pqtwiFileItem);
    m_purnNumber->GenerateName(rqstrName);
    m_purnRegExName1->GenerateName(rqstrName, pqtwiFileItem);
    m_purnRegExName2->GenerateName(rqstrName, pqtwiFileItem);
    m_purnRegExName3->GenerateName(rqstrName, pqtwiFileItem);
}


void IUIRename::GenerateExtension(QString & rqstrExtension, QTableWidgetItem* pqtwiFileItem)
{
    m_purnExten->GenerateName(rqstrExtension, pqtwiFileItem);
    m_purnRegExExten->GenerateName(rqstrExtension, pqtwiFileItem);
}


void IUIRename::SaveSettings(const QString & krqstrSection, QStringList & rqstrlSettings)
{
    QStringList qstrlSettings;

    if (m_bSaveCaseSensitivityWithRename)
        qstrlSettings.push_back(QString("<CaseSensitivity>%1</CaseSensitivity>").arg(m_bCaseSensitive));
    if (m_bSaveActiveTabWithRename)
        qstrlSettings.push_back(QString("<ActiveTab>%1</ActiveTab>").arg(static_cast<IUIRenameTabBase*>(m_pqtwRenameSettingsTab->currentWidget())->GetTabID()));

    if (qstrlSettings.isEmpty() == false)
    {
        rqstrlSettings.push_back(QString("<%1>").arg(krqstrSection));
        rqstrlSettings.append(qstrlSettings);
        rqstrlSettings.push_back(QString("</%1>").arg(krqstrSection));
    }
}


void IUIRename::RestoreSettings(const QString & krqstrSettings, int & riIndex)
{
    QString qstrTag, qstrValue;
    while (ReadSettingsValue(krqstrSettings, riIndex, qstrTag, qstrValue) == true)
    {
        if      (qstrTag == "CaseSensitivity")
        {
            m_pmwMainWindow->GetMenuBar()->SetCaseSensitive(qstrValue.toInt());
        }
        else if (qstrTag == "ActiveTab")
        {
            const int kiSetTab = qstrValue.toInt();
            switch (kiSetTab)
            {
            case Name           :   m_pqtwRenameSettingsTab->setCurrentIndex(m_pqtwRenameSettingsTab->indexOf(m_purnName)); break;
            case Extension      :   m_pqtwRenameSettingsTab->setCurrentIndex(m_pqtwRenameSettingsTab->indexOf(m_purnExten)); break;
            case Numbering      :   m_pqtwRenameSettingsTab->setCurrentIndex(m_pqtwRenameSettingsTab->indexOf(m_purnNumber)); break;
            case RegExName1     :   if (m_purnRegExName1->TabEnabled()) {m_pqtwRenameSettingsTab->setCurrentIndex(m_pqtwRenameSettingsTab->indexOf(m_purnRegExName1));} break;
            case RegExName2     :   if (m_purnRegExName2->TabEnabled()) {m_pqtwRenameSettingsTab->setCurrentIndex(m_pqtwRenameSettingsTab->indexOf(m_purnRegExName2));} break;
            case RegExName3     :   if (m_purnRegExName3->TabEnabled()) {m_pqtwRenameSettingsTab->setCurrentIndex(m_pqtwRenameSettingsTab->indexOf(m_purnRegExName3));} break;
            case RegExExten     :   if (m_purnRegExExten->TabEnabled()) {m_pqtwRenameSettingsTab->setCurrentIndex(m_pqtwRenameSettingsTab->indexOf(m_purnRegExExten));} break;
            }
        }
    }
}


QString IUIRename::GetCurrentSettingsSaveString()
{
    QStringList rqstrlSettings;
    m_purfFilter->SaveSettings("SETFilter", rqstrlSettings);
    m_purnName->SaveSettings("SETName", rqstrlSettings);
    m_purnExten->SaveSettings("SETExten", rqstrlSettings);
    m_purnNumber->SaveSettings("SETNumber", rqstrlSettings);
    m_purnRegExName1->SaveSettings("SETRegExName1", rqstrlSettings);
    m_purnRegExName2->SaveSettings("SETRegExName2", rqstrlSettings);
    m_purnRegExName3->SaveSettings("SETRegExName3", rqstrlSettings);
    m_purnRegExExten->SaveSettings("SETRegExExten", rqstrlSettings);
    SaveSettings("SETGeneral", rqstrlSettings);

    return rqstrlSettings.join("");
}


void IUIRename::RestoreSettingsFromSaveString(const QString & krqstrSaveString)
{
    // Must be outside the m_bChangingSettings flag changes because this too changes the m_bChangingSettings flag
    ClearAll(false);

    m_bChangingSettings = true;

    int iIndex = 0;
    QString qstrSection;
    while (ReadSetting(krqstrSaveString, iIndex, qstrSection))
    {
        if      (qstrSection == "SETFilter")
            m_purfFilter->RestoreSettings(krqstrSaveString, iIndex);
        else if (qstrSection == "SETName")
            m_purnName->RestoreSettings(krqstrSaveString, iIndex);
        else if (qstrSection == "SETExten")
            m_purnExten->RestoreSettings(krqstrSaveString, iIndex);
        else if (qstrSection == "SETNumber")
            m_purnNumber->RestoreSettings(krqstrSaveString, iIndex);
        else if (qstrSection == "SETRegExName1")
            m_purnRegExName1->RestoreSettings(krqstrSaveString, iIndex);
        else if (qstrSection == "SETRegExName2")
            m_purnRegExName2->RestoreSettings(krqstrSaveString, iIndex);
        else if (qstrSection == "SETRegExName3")
            m_purnRegExName3->RestoreSettings(krqstrSaveString, iIndex);
        else if (qstrSection == "SETRegExExten")
            m_purnRegExExten->RestoreSettings(krqstrSaveString, iIndex);
        else if (qstrSection == "SETGeneral")
            RestoreSettings(krqstrSaveString, iIndex);
    }

    m_bChangingSettings = false;
    m_puifmFileList->ReadMetaTags();
    m_puifmFileList->GeneratePreviewNameAndExtension();
}


bool IUIRename::ReadSettingsValue(const QString & krqstrSettings, int & riIndex, QString & rqstrTag, QString & rqstrValue)
{
    riIndex = krqstrSettings.indexOf('<', riIndex) + 1;
    if (riIndex == 0)
        return false;

    if (krqstrSettings.at(riIndex) == '/')
        return false;

    int iIDEnd = krqstrSettings.indexOf('>', riIndex);
    rqstrTag = krqstrSettings.mid(riIndex, iIDEnd-riIndex);

    QString qstrTagClose = QString("</%1>").arg(rqstrTag);
    int iValueEnd = krqstrSettings.indexOf(qstrTagClose, iIDEnd) - 1;

    rqstrValue = krqstrSettings.mid(iIDEnd+1, iValueEnd-iIDEnd);
    riIndex = iValueEnd + qstrTagClose.length() + 1;

    return true;
}


bool IUIRename::ReadSetting(const QString & krqstrSettings, int & riIndex, QString & rqstrTag)
{
    int iIDStart = krqstrSettings.indexOf('<', riIndex) + 1;
    if (iIDStart == 0)
        return false;

    riIndex = krqstrSettings.indexOf('>', iIDStart);
    rqstrTag = krqstrSettings.mid(iIDStart, riIndex-iIDStart);
    ++riIndex;
    return true;
}
