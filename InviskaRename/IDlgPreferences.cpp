#include <QtWidgets>
#include "IDlgPreferences.h"
#include "IUIMainWindow.h"
#include "IUIFileList.h"
#include "IUIMenuBar.h"
#include "IUIMenuRenames.h"
#include "IUIRename.h"
#include "IRenameInvalidCharSub.h"
#include "IMetaBase.h"
#include "IComUIPrefGeneral.h"
#include "IComUtilityFuncs.h"
#include "IComQLineEdit.h"

int IDlgPreferences::m_iStartPage = 0;


IDlgPreferences::IDlgPreferences(IUIMainWindow* pmwMainWindow, const int kiStartPage) : QDialog(pmwMainWindow),
                                                                                        m_rqsetSettings(pmwMainWindow->GetSettings()),
                                                                                        m_ricsInvalidCharSub(pmwMainWindow->GetFileListUI()->GetInvCharSub())
{
    m_pmwMainWindow = pmwMainWindow;

    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setModal(true);

    m_pipgGeneralPreferences = new IComUIPrefGeneral(this, pmwMainWindow);
    m_pqswPageStack->insertWidget(0, m_pipgGeneralPreferences);

    if (kiStartPage != -1)
        m_iStartPage = kiStartPage;
    m_pqlwPageList->setCurrentRow(m_iStartPage);
    m_pqswPageStack->setCurrentIndex(m_iStartPage);

    SetStartDirectoryLabels();
    m_pqpbStartDirectorySelect->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_DirOpenIcon));

    m_pqsiInvCharTableSpacer->changeSize(0, m_pqcbReSortFileListAfterRename->sizeHint().height(), QSizePolicy::Fixed,  QSizePolicy::Fixed);
    m_pqpbCharSubRestoreDefaults->setFixedSize(m_pqpbCharSubSetDefaultsWin->sizeHint());

    m_pqleTagsSeparatorDate->setValidator(pmwMainWindow->GetInvalidCharValidator());
    m_pqleTagsSeparatorTime->setValidator(pmwMainWindow->GetInvalidCharValidator());
    m_pqleTagsSeparatorFraction->setValidator(pmwMainWindow->GetInvalidCharValidator());

    InitCharSubTable();
    SetWidgetStates();

    connect(m_pqpbNameChangeTextColour,         SIGNAL(clicked()),  this,   SLOT(SetColourChangesColour()));
    connect(m_pqpbNameChangeHighlightColour,    SIGNAL(clicked()),  this,   SLOT(SetHighlightChangesColour()));
    connect(m_pqpbFontSelectButton,             SIGNAL(clicked()),  this,   SLOT(SetFileListFont()));

    connect(m_pqpbStartDirectorySelect,         SIGNAL(clicked()),  this,   SLOT(SetCustomStartDirectory()));

    connect(m_pqpbCharSubRestoreDefaults,       SIGNAL(clicked()),  this,   SLOT(PopulateCharSubTableDefault()));
    connect(m_pqpbCharSubSetDefaultsWin,        SIGNAL(clicked()),  this,   SLOT(PopulateCharSubTableDefaultWindows()));

    connect(m_pqpbTagSeparatorsRestoreDefaults, SIGNAL(clicked()),  this,   SLOT(RestoreDefaultTagSeparators()));

    resize(sizeHint());
    show();
}


void IDlgPreferences::SetStartDirectoryLabels()
{   
    #if   defined(Q_OS_WIN)
    m_pqrbStartDirectoryRoot->setText(tr("Computer", "As in, 'My Computer' 'This PC' or whatever Microsft is calling it now"));
    m_pqrbStartDirectoryHome->setText(tr("User directory", "The directory where Documents, Downloads, etc is"));
    m_pqpbCharSubSetDefaultsWin->hide();
    #elif defined(Q_OS_MACOS)
    m_pqrbStartDirectoryRoot->setText(tr("Root", "The root directory"));
    m_pqrbStartDirectoryHome->setText(tr("User directory", "The directory where Documents, Downloads, etc is"));
    #else
    m_pqrbStartDirectoryRoot->setText(tr("Root", "The root directory"));
    m_pqrbStartDirectoryHome->setText(tr("Home", "The user's home directory"));
    #endif
}


void IDlgPreferences::InitCharSubTable()
{
    m_pqtwInvalidCharacterTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pqtwInvalidCharacterTable->horizontalHeader()->setSectionsClickable(false);
    m_pqtwInvalidCharacterTable->horizontalHeader()->setVisible(true);
    m_pqtwInvalidCharacterTable->verticalHeader()->setMinimumSectionSize(16);
    m_pqtwInvalidCharacterTable->verticalHeader()->setDefaultSectionSize(IComUtilityFuncs::GetTableRowHeightFitToFont(m_pqtwInvalidCharacterTable));
    m_pqtwInvalidCharacterTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_pqtwInvalidCharacterTable->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    const int kiNumInvalidChars = m_pmwMainWindow->GetFileListUI()->GetInvCharSub().GetNumInvalidChars();
    m_pqtwInvalidCharacterTable->setRowCount(kiNumInvalidChars);

    IComQLineEdit* pqleLineEdit;
    QTableWidgetItem* pqleInvCharItem;
    for (int iIndex = 0 ; iIndex < kiNumInvalidChars ; ++iIndex)
    {
        pqleInvCharItem = new QTableWidgetItem;
        pqleInvCharItem->setTextAlignment(Qt::AlignCenter);
        pqleInvCharItem->setFlags(pqleInvCharItem->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        m_pqtwInvalidCharacterTable->setItem(iIndex, 0, pqleInvCharItem);
        m_pqtwInvalidCharacterTable->setItem(iIndex, 1, new QTableWidgetItem);

        pqleLineEdit = new IComQLineEdit(this, true);
        pqleLineEdit->setProperty("Row", iIndex);
        pqleLineEdit->setFrame(false);
        pqleLineEdit->setValidator(m_pmwMainWindow->GetInvalidCharValidator());
        pqleLineEdit->setAlignment(Qt::AlignCenter);
        m_pqtwInvalidCharacterTable->setCellWidget(iIndex, 1, pqleLineEdit);

        #ifndef Q_OS_WIN
        connect(pqleLineEdit, SIGNAL(textChanged(QString)), this, SLOT(CharSubTableCellChanged()));
        #endif
    }

    m_ricsInvalidCharSub.PopulateTableInvalidChars(m_pqtwInvalidCharacterTable);
    m_ricsInvalidCharSub.PopulateTableReplacements(m_pqtwInvalidCharacterTable);
}


void IDlgPreferences::SetWidgetStates()
{
    m_pipgGeneralPreferences->SetWidgetStates();

    IUIFileList* puifmFileList = m_pmwMainWindow->GetFileListUI();
    IUIRename*   puirRenameUI = m_pmwMainWindow->GetRenameUI();

    m_rqsetSettings.beginGroup("Startup");
    m_pqrbStartDirectoryRoot->setChecked(m_rqsetSettings.value("StartDirectoryRoot", StartDirectoryRootDefaultVal()).toBool());
    m_pqrbStartDirectoryHome->setChecked(m_rqsetSettings.value("StartDirectoryHome", StartDirectoryHomeDefaultVal()).toBool());
    m_pqrbStartDirectoryRestore->setChecked(m_rqsetSettings.value("StartDirectoryRestoreLast", false).toBool());
    m_pqrbStartDirectoryCustom->setChecked(m_rqsetSettings.value("StartDirectorySpecified", false).toBool());
    if (m_pqrbStartDirectoryCustom->isChecked())
        m_pqleStartDirectory->setText(m_rqsetSettings.value("StartDirectorySpecifiedDirectory", "").toString());
    m_pqrbStartSettingsRestoreLast->setChecked(m_rqsetSettings.value("StartSettingsRestoreLast", false).toBool());
    m_pqrbStartSettingsLoadProfile->setChecked(m_rqsetSettings.value("StartSettingsLoadProfile", false).toBool());
    m_pmwMainWindow->GetMenuBar()->RenamesMenu()->PopulateRenameCombo(m_pqcobStartSettingsProfileSelector, (m_rqsetSettings.value("StartSettingsProfileToLoad").toString()));
    m_rqsetSettings.endGroup();

    m_pqcbShowSideBar->setChecked(m_pmwMainWindow->SideBarEnabled());
    m_pqcbAutoRefreshEnabled->setChecked(puifmFileList->AutoRefreshEnabled());
    m_pqcbSaveSortOrder->setChecked(puifmFileList->SaveSortOrder());
    m_pqcbOpenFileWhenDblClicked->setChecked(puifmFileList->OpenFileWhenDblClicked());
    m_qcolNameChangeTextColour = puifmFileList->GetNameChangeTextColour();
    m_qcolNameChangeHighlightColour = puifmFileList->GetNameChangeHighlightColour();
    m_pqcbNameChangeColourText->setChecked(puifmFileList->GetNameChangeColourText());
    m_pqpbNameChangeTextColour->setStyleSheet(QString("QPushButton { background-color : %1;}").arg(m_qcolNameChangeTextColour.name()));
    m_pqcbNameChangeHighlightRow->setChecked(puifmFileList->GetNameChangeHighlightRow());
    m_pqpbNameChangeHighlightColour->setStyleSheet(QString("QPushButton { background-color : %1;}").arg(m_qcolNameChangeHighlightColour.name()));
    m_pqcbUseCustomLineSpacing->setChecked(puifmFileList->GetUseUserDefinedRowHeight());
    m_pqleCustomLineSpacingPxl->setText(QString::number(puifmFileList->GetCurrentRowHeight()));
    m_pqcbUseAlternativeFont->setChecked(puifmFileList->GetUseAlternativeFont());
    SetCurrentFont(puifmFileList->GetCurrentFont());

    m_pqcbShowConfirmBeforeRename->setChecked(puirRenameUI->ShowConfirmBeforeRename());
    m_pqcbDeactivateSettingsAfterRename->setChecked(puirRenameUI->DeactivateSettingsAfterRename());
    m_pqcbReSortFileListAfterRename->setChecked(puifmFileList->ReSortFileListAfterRename());
    m_pqcbSaveActiveTabWithRename->setChecked(puirRenameUI->SaveActiveTabWithRename());
    m_pqcbSaveCaseSensitivityWithRename->setChecked(puirRenameUI->SaveCaseSensitivityWithRename());

    m_pqcbTagsLoadAllExif->setChecked(puifmFileList->ExifAdvancedModeEnabled());
    m_pqleTagsSeparatorDate->setText(IMetaBase::GetSeparatorDate());
    m_pqleTagsSeparatorTime->setText(IMetaBase::GetSeparatorTime());
    m_pqleTagsSeparatorFraction->setText(IMetaBase::GetSeparatorFraction());

    m_pqcbRegExName1->setChecked(puirRenameUI->RegExName1TabEnbled());
    m_pqcbRegExName2->setChecked(puirRenameUI->RegExName2TabEnbled());
    m_pqcbRegExName3->setChecked(puirRenameUI->RegExName3TabEnbled());
    m_pqcbRegExExten->setChecked(puirRenameUI->RegExExtenTabEnbled());

    if (m_pqcobStartSettingsProfileSelector->count() == 0)
        m_pqrbStartSettingsLoadProfile->setEnabled(false);
}


void IDlgPreferences::SetCurrentFont(const QFont & krqfntFont)
{
    m_qfntSelectedFont = krqfntFont;
    m_pqpbFontSelectButton->setFont(krqfntFont);
    m_pqpbFontSelectButton->setText(QString("%1 %2 %3 %4").arg(krqfntFont.family())
                                                          .arg(krqfntFont.pointSize())
                                                          .arg(krqfntFont.bold() ? tr("Bold") : "")
                                                          .arg(krqfntFont.italic() ? tr("Italic") : ""));
}


bool IDlgPreferences::SaveSettings()
{
    if (m_pipgGeneralPreferences->SaveSettings() == false)
        return false;

    m_rqsetSettings.beginGroup("Startup");
    m_rqsetSettings.setValue("StartDirectoryRoot", m_pqrbStartDirectoryRoot->isChecked());
    m_rqsetSettings.setValue("StartDirectoryHome", m_pqrbStartDirectoryHome->isChecked());
    m_rqsetSettings.setValue("StartDirectoryRestoreLast", m_pqrbStartDirectoryRestore->isChecked());
    m_rqsetSettings.setValue("StartDirectorySpecified", m_pqrbStartDirectoryCustom->isChecked());
    m_rqsetSettings.setValue("StartDirectorySpecifiedDirectory", m_pqleStartDirectory->text());
    m_rqsetSettings.setValue("StartSettingsRestoreLast", m_pqrbStartSettingsRestoreLast->isChecked());
    m_rqsetSettings.setValue("StartSettingsLoadProfile", m_pqrbStartSettingsLoadProfile->isChecked());
    m_rqsetSettings.setValue("StartSettingsProfileToLoad", m_pqcobStartSettingsProfileSelector->currentText());
    m_rqsetSettings.endGroup();

    m_rqsetSettings.beginGroup("FileList");
    m_rqsetSettings.setValue("ShowSideBar", m_pqcbShowSideBar->isChecked());
    m_rqsetSettings.setValue("AutoRefreshDirectories", m_pqcbAutoRefreshEnabled->isChecked());
    m_rqsetSettings.setValue("SaveSortOrder", m_pqcbSaveSortOrder->isChecked());
    m_rqsetSettings.setValue("OpenFileWhenDblClicked", m_pqcbOpenFileWhenDblClicked->isChecked());
    m_rqsetSettings.setValue("NameChangeColourText", m_pqcbNameChangeColourText->isChecked());
    m_rqsetSettings.setValue("NameChangeTextColour", m_qcolNameChangeTextColour.name());
    m_rqsetSettings.setValue("NameChangeHighlightRow", m_pqcbNameChangeHighlightRow->isChecked());
    m_rqsetSettings.setValue("NameChangeHighlightColour", m_qcolNameChangeHighlightColour.name());
    m_rqsetSettings.setValue("UseUserDefinedRowHeight", m_pqcbUseCustomLineSpacing->isChecked());
    m_rqsetSettings.setValue("UserDefinedRowHeight", m_pqleCustomLineSpacingPxl->text().toInt());
    m_rqsetSettings.setValue("UseAlternativeFont", m_pqcbUseAlternativeFont->isChecked());
    m_rqsetSettings.setValue("FileListFont", m_qfntSelectedFont.toString());
    m_rqsetSettings.endGroup();

    m_rqsetSettings.beginGroup("Rename");
    m_rqsetSettings.setValue("ShowConfirmBeforeRename", m_pqcbShowConfirmBeforeRename->isChecked());
    m_rqsetSettings.setValue("DeactivateSettingsAfterRename", m_pqcbDeactivateSettingsAfterRename->isChecked());
    m_rqsetSettings.setValue("ReSortFileListAfterRename", m_pqcbReSortFileListAfterRename->isChecked());
    m_rqsetSettings.setValue("SaveActiveTabWithRename", m_pqcbSaveActiveTabWithRename->isChecked());
    m_rqsetSettings.setValue("SaveCaseSensitivityWithRename", m_pqcbSaveCaseSensitivityWithRename->isChecked());
    m_rqsetSettings.endGroup();

    m_rqsetSettings.beginGroup("MetaTags");
    m_rqsetSettings.setValue("ExifAdvancedMode", m_pqcbTagsLoadAllExif->isChecked());
    m_rqsetSettings.setValue("SeparatorDate", m_pqleTagsSeparatorDate->text());
    m_rqsetSettings.setValue("SeparatorTime", m_pqleTagsSeparatorTime->text());
    m_rqsetSettings.setValue("SeparatorFraction", m_pqleTagsSeparatorFraction->text());
    m_rqsetSettings.endGroup();

    m_rqsetSettings.beginGroup("RegEx");
    m_rqsetSettings.setValue("RegExName1", m_pqcbRegExName1->isChecked());
    m_rqsetSettings.setValue("RegExName2", m_pqcbRegExName2->isChecked());
    m_rqsetSettings.setValue("RegExName3", m_pqcbRegExName3->isChecked());
    m_rqsetSettings.setValue("RegExExten", m_pqcbRegExExten->isChecked());
    m_rqsetSettings.endGroup();

    m_ricsInvalidCharSub.SavePreferencesChanges(m_pqtwInvalidCharacterTable);

    m_rqsetSettings.sync();
    ProcessChanges();

    return true;
}


void IDlgPreferences::ProcessChanges()
{
    m_pipgGeneralPreferences->ProcessChanges();

    IUIFileList* puifmFileList = m_pmwMainWindow->GetFileListUI();
    IUIRename*   puirRenameUI = m_pmwMainWindow->GetRenameUI();

    m_pmwMainWindow->SetSideBarEnabled(m_pqcbShowSideBar->isChecked());
    puifmFileList->SetAutoRefreshEnabled(m_pqcbAutoRefreshEnabled->isChecked());
    puifmFileList->SetSaveSortOrder(m_pqcbSaveSortOrder->isChecked());
    puifmFileList->SetOpenFileWhenDblClicked(m_pqcbOpenFileWhenDblClicked->isChecked());
    puifmFileList->SetHighlightSettings(m_pqcbNameChangeColourText->isChecked(), m_qcolNameChangeTextColour, m_pqcbNameChangeHighlightRow->isChecked(), m_qcolNameChangeHighlightColour);
    puifmFileList->SetUserDefinedRowHeight(m_pqcbUseCustomLineSpacing->isChecked(), m_pqleCustomLineSpacingPxl->text().toInt());
    puifmFileList->SetUseAlternativeFont(m_pqcbUseAlternativeFont->isChecked(), m_qfntSelectedFont);

    puirRenameUI->SetShowConfirmBeforeRename(m_pqcbShowConfirmBeforeRename->isChecked());
    puirRenameUI->SetDeactivateSettingsAfterRename(m_pqcbDeactivateSettingsAfterRename->isChecked());
    puifmFileList->SetReSortFilesAfterRename(m_pqcbReSortFileListAfterRename->isChecked());
    puirRenameUI->SetSaveActiveTabWithRename(m_pqcbSaveActiveTabWithRename->isChecked());
    puirRenameUI->SetSaveCaseSensitivityWithRename(m_pqcbSaveCaseSensitivityWithRename->isChecked());

    puirRenameUI->SetRegExTabVisibility(m_pqcbRegExName1->isChecked(), m_pqcbRegExName2->isChecked(), m_pqcbRegExName3->isChecked(), m_pqcbRegExExten->isChecked());

    bool bAdvancedExifEnabled = puifmFileList->SetExifAdvancedMode(m_pqcbTagsLoadAllExif->isChecked());
    bool bSeparatorsChanged = IMetaBase::SetSeparators(m_pqleTagsSeparatorDate->text().at(0), m_pqleTagsSeparatorTime->text().at(0), m_pqleTagsSeparatorFraction->text().at(0));

    if (m_ricsInvalidCharSub.ChangesMade() || bSeparatorsChanged)
        puifmFileList->ReReadMetaTags();
    else if (bAdvancedExifEnabled)
        puifmFileList->ReReadExifTags();
    puifmFileList->GeneratePreviewNameAndExtension();
}


bool IDlgPreferences::ValidateStartDirectory(const QString & krqstrPath)
{
    if (m_pqrbStartDirectoryCustom->isChecked() == false)
        return true;

    QFileInfo qfinfFile(krqstrPath);
    if (qfinfFile.exists() == false)
    {
        QMessageBox::warning(this, tr("Custom Start Directory Invalid"), tr("The specified start directory is not valid.\nPlease enter a valid directory."), QMessageBox::Ok);
        return false;
    }

    if (qfinfFile.isFile())
        m_pqleStartDirectory->setText(QDir::toNativeSeparators(qfinfFile.path()));

    return true;
}


void IDlgPreferences::accept()
{
    if (ValidateStartDirectory(m_pqleStartDirectory->text()) == false)
        return;

    if (SaveSettings() == false)
        return;

    QDialog::accept();
}


void IDlgPreferences::done(const int kiResult)
{
    m_iStartPage = m_pqlwPageList->currentRow();
    QDialog::done(kiResult);
}


void IDlgPreferences::SetCustomStartDirectory()
{
    QFileDialog qfdFileDlg(this);
    qfdFileDlg.setFileMode(QFileDialog::Directory);
    qfdFileDlg.setOptions(QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    qfdFileDlg.setWindowTitle(tr("Specify Start Directory"));

    if (m_pqleStartDirectory->text().isEmpty() == false && QFileInfo::exists(QDir::fromNativeSeparators(m_pqleStartDirectory->text())))
    {
        qfdFileDlg.setDirectory(QDir::fromNativeSeparators(m_pqleStartDirectory->text()));
    }
    else
    {
        #ifdef Q_OS_WIN
        qfdFileDlg.setDirectoryUrl(IComUtilityFuncs::GetMyComputerURL());
        #else
        qfdFileDlg.setDirectory(QDir::homePath());
        #endif
    }

    if (qfdFileDlg.exec())
    {
        m_pqleStartDirectory->setText(QDir::toNativeSeparators(qfdFileDlg.selectedFiles().at(0)));
        m_pqrbStartDirectoryCustom->setChecked(true);
    }
}


void IDlgPreferences::SetColourChangesColour()
{
    QColor qcolColour = QColorDialog::getColor(m_qcolNameChangeTextColour, this);
    if (qcolColour.isValid())
    {
        m_qcolNameChangeTextColour = qcolColour;
        m_pqpbNameChangeTextColour->setStyleSheet(QString("QPushButton { background-color : %1;}").arg(qcolColour.name()));
    }
}


void IDlgPreferences::SetHighlightChangesColour()
{
    QColor qcolColour = QColorDialog::getColor(m_qcolNameChangeHighlightColour, this);
    if (qcolColour.isValid())
    {
        m_qcolNameChangeHighlightColour = qcolColour;
        m_pqpbNameChangeHighlightColour->setStyleSheet(QString("QPushButton { background-color : %1;}").arg(qcolColour.name()));
    }
}


void IDlgPreferences::SetFileListFont()
{  
    bool bOK;
    QFont qfntFont = QFontDialog::getFont(&bOK, m_qfntSelectedFont, this);

    if (bOK)
        SetCurrentFont(qfntFont);
}


void IDlgPreferences::PopulateCharSubTableDefault()
{
    m_ricsInvalidCharSub.PopulateTableDefaultReplacements(m_pqtwInvalidCharacterTable);
}


void IDlgPreferences::PopulateCharSubTableDefaultWindows()
{
    m_ricsInvalidCharSub.PopulateTableDefaultReplacementsWindows(m_pqtwInvalidCharacterTable);
}


void IDlgPreferences::RestoreDefaultTagSeparators()
{
    m_pqleTagsSeparatorDate->setText("-");
    m_pqleTagsSeparatorTime->setText(".");
    m_pqleTagsSeparatorFraction->setText("⁄");
}


void IDlgPreferences::CharSubTableCellChanged()
{
    QLineEdit* pqleEditedItem = qobject_cast<QLineEdit*>(sender());
    const int kiRow = pqleEditedItem->property("Row").toInt();

    if (m_pqtwInvalidCharacterTable->item(kiRow, 0)->text() == pqleEditedItem->text())
        pqleEditedItem->setStyleSheet(QString("color: %1").arg(QApplication::palette().text().color().name()));
    else
        pqleEditedItem->setStyleSheet(QString("color: %1").arg(m_pmwMainWindow->GetFileListUI()->GetNameChangeTextColour().name()));
}


bool IDlgPreferences::StartDirectoryRootDefaultVal()
{
    #ifdef Q_OS_WIN
    return true;
    #else
    return false;
    #endif
}


bool IDlgPreferences::StartDirectoryHomeDefaultVal()
{
    #ifdef Q_OS_WIN
    return false;
    #else
    return true;
    #endif
}

