#include <QtWidgets>
#include "IUIFileList.h"
#include "IUIMainWindow.h"
#include "IUIMenuBar.h"
#include "IUIMenuTags.h"
#include "IUIMenuRenames.h"
#include "IUIToolBar.h"
#include "IUIRename.h"
#include "IUIRenameNumber.h"
#include "IUIRenameFilter.h"
#include "IDlgRenameErrorList.h"
#include "IDlgPreferences.h"
#include "IDlgRenameFile.h"
#include "IComDlgFileProperties.h"
#include "IComDlgProgress.h"
#include "IComUtilityFuncs.h"
#include "IComMetaMusic.h"
#include "IComMetaExif.h"
#include "IComSysSorts.h"
#include "IMetaBase.h"
#include "IMetaMusic.h"
#include "IMetaExif.h"
#include "ISysFileInfoSortClasses.h"
#include "IRenameLegacySave.h"


IUIFileList::IUIFileList(IUIMainWindow* pmwMainWindow) : QSplitter(Qt::Horizontal, pmwMainWindow),
                                                         m_ifisFileSort(this),
                                                         m_rpuimbMenuBar(pmwMainWindow->GetMenuBar()),
                                                         m_rpuitbToolBar(pmwMainWindow->GetToolBar()),
                                                         m_rpuirRenameUI(pmwMainWindow->GetRenameUI()),
                                                         m_rqsetSettings(pmwMainWindow->GetSettings()),
                                                         m_icsInvalidCharSub(pmwMainWindow->GetSettings())
{
    setChildrenCollapsible(false);
    m_bSyncSelection = true;
    m_qstrMyComputerPath = "Computer";
    m_pmwMainWindow = pmwMainWindow;
    m_bMetaTagsReadMusic = false;
    m_bMetaTagsReadExif = false;

    // Loading embeded executable and folder icons is slow, so use generic icons
    m_qicnExeIcon = m_qfipIconProvider.icon(QFileInfo("NonExistant.exe"));
    m_qfipIconProvider.setOptions(QFileIconProvider::DontUseCustomDirectoryIcons);

    m_rqsetSettings.beginGroup("FileList");
    m_bAutoRefresh = m_rqsetSettings.value("AutoRefreshDirectories", true).toBool();
    m_bOpenFileWhenDblClicked = m_rqsetSettings.value("OpenFileWhenDblClicked", false).toBool();
    m_bShowHiddenFiles = m_rqsetSettings.value("ShowHiddenFiles", false).toBool();
    m_bNameChangeColourText = m_rqsetSettings.value("NameChangeColourText", true).toBool();
    m_qcolNameChangeTextColour.setNamedColor(m_rqsetSettings.value("NameChangeTextColour", "#ff0000").toString());
    m_bNameChangeHighlightRow = m_rqsetSettings.value("NameChangeHighlightRow", false).toBool();
    m_qcolNameChangeHighlightColour.setNamedColor(m_rqsetSettings.value("NameChangeHighlightColour", "#ffff7f").toString());
    m_bUseUserDefinedRowHeight = m_rqsetSettings.value("UseUserDefinedRowHeight", false).toBool();
    m_iUserDefinedRowHeight = m_rqsetSettings.value("UserDefinedRowHeight", 0).toInt();
    m_bUseAlternativeFont = m_rqsetSettings.value("UseAlternativeFont", false).toBool();
    QString qstrFileListFont = m_rqsetSettings.value("FileListFont", "").toString();
    m_rqsetSettings.endGroup();

    m_rqsetSettings.beginGroup("Rename");
    m_bReSortFileListAfterRename = m_rqsetSettings.value("ReSortFileListAfterRename", false).toBool();
    m_rqsetSettings.endGroup();

    m_rqsetSettings.beginGroup("MetaTags");
    m_bExifAdvancedMode = m_rqsetSettings.value("ExifAdvancedMode", false).toBool();
    IMetaBase::SetSeparatorDate(m_rqsetSettings.value("SeparatorDate", "-").toString().at(0));
    IMetaBase::SetSeparatorTime(m_rqsetSettings.value("SeparatorTime", ".").toString().at(0));
    IMetaBase::SetSeparatorFraction(m_rqsetSettings.value("SeparatorFraction", "⁄").toString().at(0));
    m_rqsetSettings.endGroup();

    m_qdirDirReader.setPath("");
    m_qdirDirReader.setSorting(QDir::NoSort);
    SetHiddenFileFilter();

    ItitialiseTable(m_pqtwNameCurrent, tr("Current Name"));
    ItitialiseTable(m_pqtwNamePreview, tr("Preview"));
    m_pqtwNameCurrent->setItemDelegateForColumn(0, new CurrentTableHighlightDelegate(this));
    m_pqtwNamePreview->setItemDelegateForColumn(0, new PreviewTableHighlightDelegate(this));

    m_pqtwNameCurrent->setContextMenuPolicy(Qt::CustomContextMenu);
    CreateContextMenus();

    m_qfntDefaultFont = m_pqtwNameCurrent->font();
    if (m_bUseAlternativeFont && qstrFileListFont.isEmpty() == false)
    {
        QFont qfntFileListFont;
        qfntFileListFont.fromString(qstrFileListFont);
        SetTableFont(qfntFileListFont);
    }
    SetRowHeightAndIconSize();

    QScrollBar *pqsbScrollBarCurrent = m_pqtwNameCurrent->verticalScrollBar();
    QScrollBar *pqsbScrollBarPreview = m_pqtwNamePreview->verticalScrollBar();
    connect(pqsbScrollBarCurrent,   SIGNAL(valueChanged(int)),                  this, SLOT(SyncScrollPreviewToCurrent()));
    connect(pqsbScrollBarPreview,   SIGNAL(valueChanged(int)),                  this, SLOT(SyncScrollCurrentToPreview()));

    connect(m_pqtwNameCurrent,      SIGNAL(itemSelectionChanged()),             this, SLOT(SelectionChanged()));
    connect(m_pqtwNamePreview,      SIGNAL(itemSelectionChanged()),             this, SLOT(SelectionChanged()));

    connect(m_pqtwNameCurrent,      SIGNAL(cellDoubleClicked(int, int)),        this, SLOT(OpenItemAtRow(int)));
    connect(m_pqtwNamePreview,      SIGNAL(cellDoubleClicked(int, int)),        this, SLOT(OpenItemAtRow(int)));

    connect(m_pqtwNameCurrent,      SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextMenu(QPoint)));

    connect(&m_qfswFSWatcher,       SIGNAL(directoryChanged(const QString &)),  this, SLOT(DirectoryChanged(const QString &)));
    connect(&m_qfswFSWatcher,       SIGNAL(fileChanged(const QString &)),       this, SLOT(FileChanged(const QString &)));
}


void IUIFileList::ItitialiseTable(QTableWidget* & rpqtwTable, const QString & krqstrTitle)
{
    rpqtwTable = new QTableWidget(this);
    rpqtwTable->setColumnCount(1);

    QTableWidgetItem* pqtwiHeader = new QTableWidgetItem;
    pqtwiHeader->setText(krqstrTitle);
    rpqtwTable->setHorizontalHeaderItem(0, pqtwiHeader);
    rpqtwTable->setStyleSheet("QTableWidget::item { padding: 0px; }");
    rpqtwTable->verticalHeader()->setMinimumSectionSize(16);

    rpqtwTable->setEditTriggers(QAbstractItemView::NoEditTriggers);                     // Items not editable
    rpqtwTable->setWordWrap(false);                                                     // Disable word wrapping when contents won't fit in cell.
    rpqtwTable->verticalHeader()->hide();                                               // No row lables
    rpqtwTable->horizontalHeader()->setSectionsClickable(false);                        // Column headers not clickable
    rpqtwTable->horizontalHeader()->setStretchLastSection(true);                        // Expand last column to fill full space (there's only one column)

    addWidget(rpqtwTable);
}


void IUIFileList::InitAfterCreate()
{
    QStringList qstrlArguments = QCoreApplication::arguments();
    InnitStartDir(qstrlArguments);
    InnitRenameSettings(qstrlArguments);
}


void IUIFileList::InnitStartDir(const QStringList & krqstrlArguments)
{
    // Check for directories passed via command line before using default start directory
    int iNumArgs = krqstrlArguments.size();
    if (iNumArgs > 0)
    {
        QFileInfo qfinfFile;
        for (int iArgIndex = 1 ; iArgIndex < iNumArgs ; ++iArgIndex)
        {
            #ifdef Q_OS_LINUX
            // Nemo adds file:// to the start of files when running batch rename command
            if (krqstrlArguments.at(iArgIndex).startsWith("file://"))
                qfinfFile.setFile(krqstrlArguments.at(iArgIndex).mid(7));
            else
                qfinfFile.setFile(krqstrlArguments.at(iArgIndex));
            #else
            qfinfFile.setFile(krqstrlArguments.at(iArgIndex));
            #endif

            if (qfinfFile.exists())
            {
                if (qfinfFile.isDir())
                {
                    SetDirectory(qfinfFile.absoluteFilePath());
                    return;
                }
                else if (qfinfFile.isFile())
                {
                    SetDirectory(qfinfFile.absolutePath());
                    return;
                }
            }
        }
    }

    NavigateToStartDirectory(true);
}


void IUIFileList::InnitRenameSettings(const QStringList & krqstrlArguments)
{
    bool bRenameSettingsLoaded = false;

    int iNumArgs = krqstrlArguments.size();
    if (iNumArgs > 0)
    {
        for (int iArgIndex = 1 ; iArgIndex < iNumArgs ; ++iArgIndex)
        {
            if (krqstrlArguments.at(iArgIndex) == "--loadren" && iArgIndex+1 < iNumArgs)
                bRenameSettingsLoaded = m_pmwMainWindow->GetMenuBar()->RenamesMenu()->ExecuteActionCaseInsensitive(krqstrlArguments.at(iArgIndex+1));
        }
    }

    if (bRenameSettingsLoaded == false)
        SetInitialRenameSettings();
}


void IUIFileList::SetInitialRenameSettings()
{
    if      (m_rqsetSettings.value("Startup/StartSettingsRestoreLast").toBool())
    {
        //m_rpuirRenameUI->RestoreSettingsFromSaveString(m_rqsetSettings.value("LastSession/RenameSettings").toString());
        QString qstrSettingString = m_rqsetSettings.value("LastSession/RenameSettings").toString();
        if (qstrSettingString.startsWith('1'))
            IRenameLegacySave::ConvertLegacySave(qstrSettingString);
        m_rpuirRenameUI->RestoreSettingsFromSaveString(qstrSettingString);

    }
    else if (m_rqsetSettings.value("Startup/StartSettingsLoadProfile").toBool())
    {
        if (m_pmwMainWindow->GetMenuBar()->RenamesMenu()->ExecuteAction(m_rqsetSettings.value("Startup/StartSettingsProfileToLoad").toString()) == false)
        {
            m_rqsetSettings.setValue("Startup/StartSettingsLoadProfile", false);
            m_rqsetSettings.setValue("Startup/StartSettingsProfileToLoad", QString());
        }
    }
}


void IUIFileList::SaveSessionSettings()
{
    m_rqsetSettings.beginGroup("LastSession");

    if (m_bDisplayingMyComputer)
        m_rqsetSettings.setValue("Directory", m_qstrMyComputerPath);
    else
        m_rqsetSettings.setValue("Directory", m_qdirDirReader.absolutePath());

    m_rqsetSettings.setValue("RenameSettings", m_rpuirRenameUI->GetCurrentSettingsSaveString());

    m_rqsetSettings.endGroup();
}


QFileInfoList IUIFileList::GetDirectoryFileList(QDir::Filters m_qdirfFileFilter)
{
    m_qdirDirReader.setFilter(m_qdirfFileFilter | m_qdirfHiddenFileFilter);
    return m_qdirDirReader.entryInfoList();
}


void IUIFileList::OpenDirectory(QString qstrPath)
{
    QString qstrCurrentDir = m_bDisplayingMyComputer ? m_qstrMyComputerPath : m_qdirDirReader.path();
    if (SetDirectory(qstrPath))
    {
        m_qsqstrBackStack.push(qstrCurrentDir);
        m_qsqstrForwardStack.clear();
        EnableBackForwardActions();
    }
}


void IUIFileList::RestoreLastSessionDirectory()
{
    QString qstrLastDir = m_rqsetSettings.value("LastSession/Directory").toString();
    if (m_qdirDirReader.exists(qstrLastDir))
    {
        SetDirectory(qstrLastDir);
    }
    else
    {
        #ifdef Q_OS_WIN
        SetDirectory(m_qstrMyComputerPath);
        #else
        SetDirectory(QDir::homePath());
        #endif
    }
}


bool IUIFileList::SetDirectory(QString qstrPath)
{
    qstrPath = QDir::fromNativeSeparators(qstrPath);

    #ifdef Q_OS_WIN
    if (qstrPath == m_qdirDirReader.absolutePath() && m_bDisplayingMyComputer == false)
        return false;

    if (qstrPath == m_qstrMyComputerPath)
    {
        PopulateTablesComputer();
        return true;
    }
    #else
    if (qstrPath == m_qdirDirReader.absolutePath())
        return false;
    #endif

    if (m_qdirDirReader.exists(qstrPath) == false)
    {
        QMessageBox::information(this, tr("Cannot Open Directory"), QString("%1:\n\n%2").arg(tr("The specified directory doesn't exist")).arg(qstrPath), QMessageBox::Ok);
        return false;
    }

    m_qdirDirReader.setPath(qstrPath);
    PopulateTablesDirectory();
    GeneratePreviewNameAndExtension();
    return true;
}


void IUIFileList::PopulateTablesDirectory()
{
    m_bDisplayingMyComputer = false;

    ClearTableContents();
    QFileInfoList qfilFileList = m_ifisFileSort.GetSortedFileList();
    m_pqtwNameCurrent->setRowCount(qfilFileList.size());
    m_pqtwNamePreview->setRowCount(qfilFileList.size());

    int iRow = 0;
    QIcon qicnFileIcon;
    QTableWidgetItem* pqtwiCurrentItem;
    QTableWidgetItem* pqtwiPreviewItem;
    QFileInfoList::const_iterator kitFile;
    for (kitFile = qfilFileList.constBegin() ; kitFile != qfilFileList.constEnd() ; ++kitFile)
    {
        pqtwiCurrentItem = new QTableWidgetItem;
        pqtwiCurrentItem->setText(kitFile->fileName());
        pqtwiCurrentItem->setData(FileInfo, QVariant::fromValue(*kitFile));
        m_pqtwNameCurrent->setItem(iRow, 0, pqtwiCurrentItem);

        pqtwiPreviewItem = new QTableWidgetItem;
        m_pqtwNamePreview->setItem(iRow, 0, pqtwiPreviewItem);

        #ifdef Q_OS_WIN
        qicnFileIcon = (kitFile->suffix().toLower() == "exe" ? m_qicnExeIcon : m_qfipIconProvider.icon(*kitFile));
        #else
        qicnFileIcon = m_qfipIconProvider.icon(*kitFile);
        #endif
        pqtwiCurrentItem->setIcon(qicnFileIcon);
        pqtwiPreviewItem->setIcon(qicnFileIcon);

        if (kitFile->isFile())
            m_qfswFSWatcher.addPath(kitFile->filePath());

        ++iRow;
    }

    const QString kqstrCurrentPath = m_qdirDirReader.path();
    m_qfswFSWatcher.addPath(kqstrCurrentPath);
    m_rpuitbToolBar->SetAddressBarText(QDir::toNativeSeparators(kqstrCurrentPath));
    #ifdef Q_OS_WIN
    m_rpuimbMenuBar->EnableUpAction(true);
    #else
    m_rpuimbMenuBar->EnableUpAction(!m_qdirDirReader.isRoot());
    #endif

    ReadMetaTags();
}


void IUIFileList::PopulateTablesComputer()
{
    m_bDisplayingMyComputer = true;

    ClearTableContents();
    QFileInfoList qfilFileList = m_qdirDirReader.drives();
    m_pqtwNameCurrent->setRowCount(qfilFileList.size());
    m_pqtwNamePreview->setRowCount(qfilFileList.size());

    int iRow = 0;
    QIcon qicnFileIcon;
    QString qstrDriveName;
    QStorageInfo qsiDriveInfo;
    QTableWidgetItem* pqtwiCurrentItem;
    QTableWidgetItem* pqtwiPreviewItem;
    QFileInfoList::const_iterator kitFile;

    for (kitFile = qfilFileList.constBegin() ; kitFile != qfilFileList.constEnd() ; ++kitFile)
    {
        qsiDriveInfo.setPath(kitFile->path());
        if (qsiDriveInfo.isValid() && qsiDriveInfo.isReady())
            qstrDriveName = qsiDriveInfo.displayName() + " (" + kitFile->path().at(0) + ":)";
        else
            qstrDriveName = tr("Removable Disk") + " (" + kitFile->path().at(0) + ":)";

        qicnFileIcon = m_qfipIconProvider.icon(*kitFile);

        pqtwiCurrentItem = new QTableWidgetItem;
        pqtwiCurrentItem->setText(qstrDriveName);
        pqtwiCurrentItem->setIcon(qicnFileIcon);
        pqtwiCurrentItem->setData(FileInfo, QVariant::fromValue(*kitFile));
        m_pqtwNameCurrent->setItem(iRow, 0, pqtwiCurrentItem);

        pqtwiPreviewItem = new QTableWidgetItem;
        pqtwiPreviewItem->setText(qstrDriveName);
        pqtwiPreviewItem->setIcon(qicnFileIcon);
        m_pqtwNamePreview->setItem(iRow, 0, pqtwiPreviewItem);

        ++iRow;
    }

    m_rpuitbToolBar->SetAddressBarText(m_qstrMyComputerPath);
    m_rpuimbMenuBar->EnableUpAction(false);
    m_rpuimbMenuBar->EnableBackAction(!m_qsqstrBackStack.isEmpty());
    m_rpuimbMenuBar->EnableForwardAction(!m_qsqstrForwardStack.isEmpty());

    // Here we should add the directory to the QFileSystemWatcher list, but you can't put a QFileSystemWatcher on Computer or  m_qdirDirReader.drives().
    // This means the Computer view won't update when drives are added/removed.  Not sure how to handle this.
}


void IUIFileList::ClearTableContents()
{
    m_bMetaTagsReadMusic = false;
    m_bMetaTagsReadExif = false;

    ClearFSWatcher();

    m_bSyncSelection = false;
    m_pqtwNameCurrent->clearContents();
    m_pqtwNamePreview->clearContents();
    m_bSyncSelection = true;
}


void IUIFileList::ClearFSWatcher()
{
    #ifdef QT_DEBUG
    if (m_qfswFSWatcher.directories().isEmpty() == false)
    {
        QStringList qstrlList = m_qfswFSWatcher.directories();
        QStringList::const_iterator itItem;
        for (itItem = qstrlList.constBegin() ; itItem != qstrlList.constEnd() ; ++itItem)
        {
            if (m_qfswFSWatcher.removePath(*itItem) == false)
                qDebug() << "Failed to remove directory: " << *itItem;
        }
    }
    if (m_qfswFSWatcher.files().isEmpty() == false)
    {
        QStringList qstrlList = m_qfswFSWatcher.files();
        QStringList::const_iterator itItem;
        for (itItem = qstrlList.constBegin() ; itItem != qstrlList.constEnd() ; ++itItem)
        {
            if (m_qfswFSWatcher.removePath(*itItem) == false)
                 qDebug() << "Failed to remove file: " << *itItem;
        }
    }
    #else
    if (m_qfswFSWatcher.directories().isEmpty() == false)
        m_qfswFSWatcher.removePaths(m_qfswFSWatcher.directories());
    if (m_qfswFSWatcher.files().isEmpty() == false)
        m_qfswFSWatcher.removePaths(m_qfswFSWatcher.files());
    #endif
}


void IUIFileList::RefreshDirectoryPostRename()
{
    m_qdirDirReader.refresh();
    QFileInfoList qfilFileList = GetDirectoryFileList(QDir::AllEntries);
    m_qfswFSWatcher.addPath(m_qdirDirReader.path());

    int iIndex;
    QList<int> qlstRowsToValidate;
    int iSize = m_pqtwNameCurrent->rowCount();
    qlstRowsToValidate.reserve(iSize);
    for (iIndex = 0 ; iIndex < iSize ; ++iIndex)
        qlstRowsToValidate.append(iIndex);

    QString qstrFileName;
    QFileInfo qfiFile;
    QTableWidgetItem* pqtwiRowItem;
    const int kiFileCount = qfilFileList.size();
    for (int iFileListIndex = 0 ; iFileListIndex < kiFileCount ; ++iFileListIndex)
    {
        qfiFile = qfilFileList.at(iFileListIndex);
        qstrFileName = qfiFile.fileName();
        if (qfiFile.isFile())
            m_qfswFSWatcher.addPath(qfiFile.filePath());

        iSize = qlstRowsToValidate.size();
        for (iIndex = 0 ; iIndex < iSize ; ++iIndex)
        {
            pqtwiRowItem = m_pqtwNameCurrent->item(qlstRowsToValidate.at(iIndex), 0);
            if (pqtwiRowItem->text() == qstrFileName)
            {
                if (qstrFileName != pqtwiRowItem->data(FileInfo).value<QFileInfo>().fileName())
                {
                    #ifdef QT_DEBUG
                    qDebug() << "File Renamed From:" << pqtwiRowItem->data(FileInfo).value<QFileInfo>().fileName() <<  "To:" << qstrFileName;
                    #endif

                    pqtwiRowItem->setData(FileInfo, QVariant::fromValue(qfiFile));
                    pqtwiRowItem->setIcon(m_qfipIconProvider.icon(qfiFile));
                    m_pqtwNamePreview->item(qlstRowsToValidate.at(iIndex), 0)->setIcon(m_qfipIconProvider.icon(qfiFile));
                }

                qlstRowsToValidate.removeAt(iIndex);
                break;
            }
        }
    }

    GeneratePreviewNameAndExtension();
}


void IUIFileList::RefreshDirectoryHard()
{
    if (m_bDisplayingMyComputer)
    {
        m_bSyncSelection = false;
        PopulateTablesComputer();
        m_bSyncSelection = true;
    }
    else
    {
        m_bSyncSelection = false;
        m_qdirDirReader.refresh();
        PopulateTablesDirectory();
        m_bSyncSelection = true;

        GeneratePreviewNameAndExtension();
    }
}


void IUIFileList::RefreshDirectorySoft()
{
    if (m_bDisplayingMyComputer)
    {
        RefreshDirectoryHard();
        return;
    }

    m_qdirDirReader.refresh();
    QFileInfoList qfilFileList = m_ifisFileSort.GetSortedFileList();

    QList<int> qlstRowsToRemove;
    QList<int> qlstFilesToAdd;

    int iIndex;
    int iSize = m_pqtwNameCurrent->rowCount();
    qlstRowsToRemove.reserve(iSize);
    for (iIndex = 0 ; iIndex < iSize ; ++iIndex)
        qlstRowsToRemove.append(iIndex);

    QString qstrFileName;
    QTableWidgetItem* pqtwiRowItem;
    const int kiFileCount = qfilFileList.size();
    for (int iFileListIndex = 0 ; iFileListIndex < kiFileCount ; ++iFileListIndex)
    {
        qstrFileName = qfilFileList.at(iFileListIndex).fileName();

        iSize = qlstRowsToRemove.size();
        for (iIndex = 0 ; iIndex < iSize ; ++iIndex)
        {
            pqtwiRowItem = m_pqtwNameCurrent->item(qlstRowsToRemove.at(iIndex), 0);
            if (pqtwiRowItem->text() == qstrFileName)
            {
                qlstRowsToRemove.removeAt(iIndex);
                break;
            }
        }
        if (iIndex == iSize)
            qlstFilesToAdd.push_back(iFileListIndex);
    }

    if (qlstRowsToRemove.isEmpty() == false)
        RemoveRows(qlstRowsToRemove);

    if (qlstFilesToAdd.isEmpty() == false)
        AddFiles(qlstFilesToAdd, qfilFileList);

    if (qlstFilesToAdd.isEmpty() == false || qlstRowsToRemove.isEmpty() == false)
    {
        #ifdef QT_DEBUG
        qDebug() << "Regenerating Names After Directory Change";
        #endif

        GeneratePreviewNameAndExtension();
    }
}


void IUIFileList::RemoveRows(const QList<int> & krqlstRemoveRows)
{
    #ifdef QT_DEBUG
    qDebug() << "-=Files To Remove=-";
    for (int iIndex = 0 ; iIndex < krqlstRemoveRows.size() ; ++iIndex)
         qDebug() << m_pqtwNameCurrent->item(krqlstRemoveRows.at(iIndex), 0)->text();
    #endif

    QList<int>::const_reverse_iterator ritRowNum;
    for (ritRowNum = krqlstRemoveRows.rbegin() ; ritRowNum != krqlstRemoveRows.rend() ; ++ritRowNum)
    {
        // Deleted files are automatically be removed, but this is still necessary for changing between show/hide hidden fils
        m_qfswFSWatcher.removePath(m_pqtwNameCurrent->item(*ritRowNum, 0)->data(FileInfo).value<QFileInfo>().filePath());
        m_pqtwNameCurrent->removeRow(*ritRowNum);
        m_pqtwNamePreview->removeRow(*ritRowNum);
    }
}


void IUIFileList::AddFiles(const QList<int> & krqlstAddIndexes, const QFileInfoList & krqfilFileList)
{
    #ifdef QT_DEBUG
    qDebug() << "-=Files To Add=-";
    for (int iIndex = 0 ; iIndex < krqlstAddIndexes.size() ; ++iIndex)
        qDebug() << krqfilFileList.at(krqlstAddIndexes.at(iIndex)).fileName();
    #endif

    int iNextToAddIndex = 0;
    const int kiNumFilesToAdd = krqlstAddIndexes.size();
    while (iNextToAddIndex < kiNumFilesToAdd)
    {
       AddFile(krqfilFileList.at(krqlstAddIndexes.at(iNextToAddIndex)), krqlstAddIndexes.at(iNextToAddIndex));
       ++iNextToAddIndex;
    }
}


void IUIFileList::AddFile(const QFileInfo & krqfiNewFile, const int kiRow)
{
    QTableWidgetItem* pqtwiCurrentItem = new QTableWidgetItem;
    pqtwiCurrentItem->setText(krqfiNewFile.fileName());
    pqtwiCurrentItem->setIcon(m_qfipIconProvider.icon(krqfiNewFile));
    m_pqtwNameCurrent->insertRow(kiRow);
    m_pqtwNameCurrent->setItem(kiRow, 0, pqtwiCurrentItem);

    QTableWidgetItem* pqtwiPreviewItem = new QTableWidgetItem;
    pqtwiPreviewItem->setIcon(m_qfipIconProvider.icon(krqfiNewFile));
    m_pqtwNamePreview->insertRow(kiRow);
    m_pqtwNamePreview->setItem(kiRow, 0, pqtwiPreviewItem);

    pqtwiCurrentItem->setData(FileInfo, QVariant::fromValue(krqfiNewFile));
    if (krqfiNewFile.isFile())
        m_qfswFSWatcher.addPath(krqfiNewFile.filePath());

    if (m_bMetaTagsReadMusic)
        ReadFileMetaTagsMusic(pqtwiCurrentItem);
    if (m_bMetaTagsReadExif)
        ReadFileMetaTagsExif(pqtwiCurrentItem);
}


void IUIFileList::FlagItemsForRenaming()
{
    int iRow = 0;
    int iNumRows = m_pqtwNameCurrent->rowCount();
    int iRenameElements = m_rpuirRenameUI->GetRenameUIFilter()->RenameElements();

    if (iRenameElements == IUIRenameFilter::RenameFilesOnly)
    {
        for (iRow = 0 ; iRow < iNumRows ; ++iRow)
            m_pqtwNameCurrent->item(iRow, 0)->setData(FlaggedForRenme, m_pqtwNameCurrent->item(iRow, 0)->data(FileInfo).value<QFileInfo>().isFile());
    }
    else if (iRenameElements == IUIRenameFilter::RenameFoldersOnly)
    {
        for (iRow = 0 ; iRow < iNumRows ; ++iRow)
            m_pqtwNameCurrent->item(iRow, 0)->setData(FlaggedForRenme, m_pqtwNameCurrent->item(iRow, 0)->data(FileInfo).value<QFileInfo>().isDir());
    }
    else if (iRenameElements == IUIRenameFilter::RenameFilesAndFolders)
    {
        for (iRow = 0 ; iRow < iNumRows ; ++iRow)
            m_pqtwNameCurrent->item(iRow, 0)->setData(FlaggedForRenme, true);
    }
    else if (iRenameElements == IUIRenameFilter::RenameSelectedItems)
    {
        FlagSelectedItemsForRenaming();
        return;
    }
    else //if (iRenameElements == IUIRenameFilter::RenameFilesWithExtension)
    {
        FlagItemsForRenamingByExtension();
    }

    UnflagItemsForRenamingIfNoMeta();

    m_iNumFilesToRename = 0;
    for (iRow = 0 ; iRow < iNumRows ; ++iRow)
    {
        if (m_pqtwNameCurrent->item(iRow, 0)->data(FlaggedForRenme).toBool())
            ++m_iNumFilesToRename;
    }
}


void IUIFileList::FlagSelectedItemsForRenaming()
{
    int iRow;
    const int kiNumRows = m_pqtwNameCurrent->rowCount();
    for (iRow = 0 ; iRow < kiNumRows ; ++iRow)
        m_pqtwNameCurrent->item(iRow, 0)->setData(FlaggedForRenme, false);

    int iRowBottom;
    m_iNumFilesToRename = 0;
    QList<QTableWidgetSelectionRange> qlqtwsrSelections = m_pqtwNameCurrent->selectedRanges();
    QList<QTableWidgetSelectionRange>::const_iterator kitSelection;
    for (kitSelection = qlqtwsrSelections.constBegin() ; kitSelection != qlqtwsrSelections.constEnd() ; ++kitSelection)
    {
        iRowBottom = kitSelection->bottomRow();
        for (iRow = kitSelection->topRow() ; iRow <= iRowBottom ; ++iRow)
        {
            ++m_iNumFilesToRename;
            m_pqtwNameCurrent->item(iRow, 0)->setData(FlaggedForRenme, true);
        }
    }
}


void IUIFileList::FlagItemsForRenamingByExtension()
{
    int iRow;
    const int kiNumRows = m_pqtwNameCurrent->rowCount();
    for (iRow = 0 ; iRow < kiNumRows ; ++iRow)
        m_pqtwNameCurrent->item(iRow, 0)->setData(FlaggedForRenme, false);

    QString qstrExtension;
    QStringList krqstrlExtensionList = m_rpuirRenameUI->GetRenameUIFilter()->GetRenameExtensions();
    QStringList::const_iterator kitExtension;
    for (iRow = 0 ; iRow < kiNumRows ; ++iRow)
    {
        if (m_pqtwNameCurrent->item(iRow, 0)->data(FileInfo).value<QFileInfo>().isFile())
        {
            qstrExtension = m_pqtwNameCurrent->item(iRow, 0)->text().mid(m_pqtwNameCurrent->item(iRow, 0)->text().lastIndexOf('.')+1);
            if (m_rpuirRenameUI->CaseSensitive() == false)
                qstrExtension = qstrExtension.toLower();

            for (kitExtension = krqstrlExtensionList.constBegin() ; kitExtension != krqstrlExtensionList.constEnd() ; ++kitExtension)
            {
                if (qstrExtension == *kitExtension)
                {
                    m_pqtwNameCurrent->item(iRow, 0)->setData(FlaggedForRenme, true);
                    break;
                }
            }
        }
    }
}


void IUIFileList::UnflagItemsForRenamingIfNoMeta()
{
    bool bMusicMetaReq = false;
    bool bExifMetaReq = false;
    m_rpuirRenameUI->CheckForMetaTags(bMusicMetaReq, bExifMetaReq);

    bMusicMetaReq = bMusicMetaReq && m_bMetaTagsReadMusic;
    bExifMetaReq  = bExifMetaReq  && m_bMetaTagsReadExif;

    if (bMusicMetaReq == false && bExifMetaReq == false)
        return;

    const int kiNumRows = m_pqtwNameCurrent->rowCount();
    for (int iRow = 0 ; iRow < kiNumRows ; ++iRow)
    {
        if (bMusicMetaReq && m_pqtwNameCurrent->item(iRow, 0)->data(IUIFileList::MusicMeta).isNull())
            m_pqtwNameCurrent->item(iRow, 0)->setData(FlaggedForRenme, false);
        if (bExifMetaReq && m_pqtwNameCurrent->item(iRow, 0)->data(IUIFileList::ExifMeta).isNull())
            m_pqtwNameCurrent->item(iRow, 0)->setData(FlaggedForRenme, false);
    }
}


void IUIFileList::ReadMetaTags()
{
    bool bMusicTags = false;
    bool bExifTags = false;
    m_rpuirRenameUI->CheckForMetaTags(bMusicTags, bExifTags);

    if (m_bMetaTagsReadMusic == false && bMusicTags == true)
        ReadMetaTagsMusic();

    if (m_bMetaTagsReadExif == false && bExifTags == true)
        ReadMetaTagsExif();
}


void IUIFileList::ReadMetaTagsMusic(const bool kbForceReRead)
{
    if (m_bMetaTagsReadMusic == true && kbForceReRead == false)
        return;

    #ifdef QT_DEBUG
    qDebug() << "Reading Music Meta For:" << QDir::toNativeSeparators(m_qdirDirReader.path());
    #endif

    int iRow = 0;
    const int kiNumRows = m_pqtwNameCurrent->rowCount();

    while (iRow < kiNumRows && m_pqtwNameCurrent->item(iRow, 0)->data(FileInfo).value<QFileInfo>().isDir())
        ++iRow;  
    const int kiStartRow = iRow;

    IComDlgProgress idprgRenameProgress(m_pmwMainWindow, tr("Reading Music Tags"), "Reading file: ", kiNumRows-iRow, false, true, 1000);

    QTableWidgetItem* pqtwiFileItem;
    const QString kqstrCurrentPath = m_qdirDirReader.path();
    while (iRow < kiNumRows)
    {
        pqtwiFileItem = m_pqtwNameCurrent->item(iRow, 0);
        idprgRenameProgress.UpdateMessage(tr("Reading file: %1").arg(pqtwiFileItem->text()));
        idprgRenameProgress.UpdateProgress(iRow-kiStartRow+1);

        ReadFileMetaTagsMusic(pqtwiFileItem);

        if (idprgRenameProgress.Aborted())
        {
            m_rpuirRenameUI->ClearAll();
            RefreshDirectoryHard();
            return;
        }

        ++iRow;
    }

    m_bMetaTagsReadMusic = true;
}


void IUIFileList::ReadFileMetaTagsMusic(QTableWidgetItem* pqtwiFileItem)
{
    IComMetaMusic mmuMusicMeta(QDir::toNativeSeparators(pqtwiFileItem->data(FileInfo).value<QFileInfo>().absoluteFilePath()));
    if (mmuMusicMeta.TagDataPresent())
        pqtwiFileItem->setData(MusicMeta, QVariant::fromValue(IMetaMusic(&mmuMusicMeta, m_icsInvalidCharSub)));
}


void IUIFileList::ReadMetaTagsExif(const bool kbForceReRead)
{
    if (m_bMetaTagsReadExif == true && kbForceReRead == false)
        return;

    #ifdef QT_DEBUG
    qDebug() << "Reading Exif For:" << QDir::toNativeSeparators(m_qdirDirReader.path());
    #endif

    int iRow = 0;
    const int kiNumRows = m_pqtwNameCurrent->rowCount();

    while (iRow < kiNumRows && m_pqtwNameCurrent->item(iRow, 0)->data(FileInfo).value<QFileInfo>().isDir())
        ++iRow;
    const int kiStartRow = iRow;

    IComDlgProgress idprgRenameProgress(m_pmwMainWindow, tr("Reading Exif Data"), "Reading file: ", kiNumRows-iRow, false, true, 1000);

    QTableWidgetItem* pqtwiFileItem;
    const QString kqstrCurrentPath = m_qdirDirReader.path();
    while (iRow < kiNumRows)
    {
        pqtwiFileItem = m_pqtwNameCurrent->item(iRow, 0);
        idprgRenameProgress.UpdateMessage(tr("Reading file: %1").arg(pqtwiFileItem->text()));
        idprgRenameProgress.UpdateProgress(iRow-kiStartRow+1);

        ReadFileMetaTagsExif(pqtwiFileItem);

        if (idprgRenameProgress.Aborted())
        {
            m_rpuirRenameUI->ClearAll();
            RefreshDirectoryHard();
            return;
        }

        ++iRow;
    }

    m_bMetaTagsReadExif = true;
}


void IUIFileList::ReadFileMetaTagsExif(QTableWidgetItem* pqtwiFileItem)
{
    QFileInfo qfiFileInfo = pqtwiFileItem->data(FileInfo).value<QFileInfo>();

    if (IComMetaExif::FileCanContainExif(qfiFileInfo.suffix()) == false)
        return;

    IComMetaExif mexExifMeta(QDir::toNativeSeparators(qfiFileInfo.absoluteFilePath()));
    if (mexExifMeta.ExifDataPresent())
        pqtwiFileItem->setData(ExifMeta, QVariant::fromValue(IMetaExif(&mexExifMeta, m_icsInvalidCharSub, m_bExifAdvancedMode)));
}


void IUIFileList::ReReadMetaTags()
{
    if (m_bMetaTagsReadMusic)
        ReadMetaTagsMusic(true);
    if (m_bMetaTagsReadExif)
        ReadMetaTagsExif(true);
}


void IUIFileList::ReReadMusicTags()
{
    if (m_bMetaTagsReadMusic)
        ReadMetaTagsMusic(true);
}


void IUIFileList::ReReadExifTags()
{
    if (m_bMetaTagsReadExif)
        ReadMetaTagsExif(true);
}


void IUIFileList::SetHiddenFileFilter()
{
    if (m_bShowHiddenFiles)
        m_qdirfHiddenFileFilter = QDir::NoDotAndDotDot | QDir::Hidden | QDir::System;
    else
        m_qdirfHiddenFileFilter = QDir::NoDotAndDotDot;
}


void IUIFileList::SetHiddenFileState()
{
    bool bShowHiddenFiles = m_pmwMainWindow->GetMenuBar()->ShowHiddenFiles();
    if (m_bShowHiddenFiles != bShowHiddenFiles)
    {
        m_bShowHiddenFiles = bShowHiddenFiles;
        m_pmwMainWindow->GetSettings().setValue("FileList/ShowHiddenFiles", m_bShowHiddenFiles);

        SetHiddenFileFilter();
        RefreshDirectorySoft();
    }
}



void IUIFileList::SelectionChanged()
{
    if (m_bSyncSelection == false)
            return;
    m_bSyncSelection = false;

    if (sender() == m_pqtwNameCurrent)
        SyncSelectionXToY(m_pqtwNameCurrent, m_pqtwNamePreview);
    else
        SyncSelectionXToY(m_pqtwNamePreview, m_pqtwNameCurrent);

    if (m_rpuirRenameUI->GetRenameUIFilter()->RenameElements() == IUIRenameFilter::RenameSelectedItems)
    {
        FlagSelectedItemsForRenaming();
        GeneratePreviewNameAndExtension();
    }

    m_bSyncSelection = true;
}


void IUIFileList::SyncSelectionXToY(QTableWidget* pqtwSyncFrom, QTableWidget* pqtwSyncTo)
{
    pqtwSyncTo->clearSelection();
    QList<QTableWidgetSelectionRange> qlqtwslSelections = pqtwSyncFrom->selectedRanges();
    QList<QTableWidgetSelectionRange>::const_iterator kitSelection;
    for (kitSelection = qlqtwslSelections.constBegin() ; kitSelection != qlqtwslSelections.constEnd() ; ++kitSelection)
        pqtwSyncTo->QTableWidget::setRangeSelected(*kitSelection, true);
}


void IUIFileList::SyncScrollPreviewToCurrent()
{
    QScrollBar *pqsbScrollBarCurrent = m_pqtwNameCurrent->verticalScrollBar();
    QScrollBar *pqsbScrollBarPreview = m_pqtwNamePreview->verticalScrollBar();

    pqsbScrollBarPreview->setValue(pqsbScrollBarCurrent->value());
}


void IUIFileList::SyncScrollCurrentToPreview()
{
    QScrollBar *pqsbScrollBarCurrent = m_pqtwNameCurrent->verticalScrollBar();
    QScrollBar *pqsbScrollBarPreview = m_pqtwNamePreview->verticalScrollBar();

    pqsbScrollBarCurrent->setValue(pqsbScrollBarPreview->value());
}


void IUIFileList::OpenFilePropertiesDialog()
{
    QFileDialog qfdFileDlg(this);
    qfdFileDlg.setFileMode(QFileDialog::ExistingFile);
    qfdFileDlg.setWindowTitle(tr("Select File to View Properties"));

    if (m_bDisplayingMyComputer)
        qfdFileDlg.setDirectoryUrl(IComUtilityFuncs::GetMyComputerURL());
    else
        qfdFileDlg.setDirectory(m_qdirDirReader.path());

    if (qfdFileDlg.exec())
        new IComDlgFileProperties(qfdFileDlg.selectedFiles().at(0));
}


void IUIFileList::OpenDirectoryDialog()
{
    QFileDialog qfdFileDlg(this);
    qfdFileDlg.setFileMode(QFileDialog::Directory);
    qfdFileDlg.setOptions(QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    qfdFileDlg.setWindowTitle(tr("Open Directory"));

    m_rqsetSettings.beginGroup("Startup");
    if (m_rqsetSettings.value("StartDirectoryHome", false).toBool())
    {
        qfdFileDlg.setDirectory(QDir::homePath());
    }
    else if (m_rqsetSettings.value("StartDirectorySpecified", false).toBool())
    {
        QString qstrStartDirectory = QDir::fromNativeSeparators(m_rqsetSettings.value("StartDirectorySpecifiedDirectory").toString());
        if (m_qdirDirReader.exists(qstrStartDirectory))
        {
            qfdFileDlg.setDirectory(qstrStartDirectory);
        }
        else
        {
            #ifdef Q_OS_WIN
            qfdFileDlg.setDirectoryUrl(IComUtilityFuncs::GetMyComputerURL());
            #else
            qfdFileDlg.setDirectory(QDir::homePath());
            #endif
        }
        qfdFileDlg.setDirectory(m_rqsetSettings.value("StartDirectorySpecifiedDirectory", QDir::rootPath()).toString());
    }
    else if (m_rqsetSettings.value("StartDirectoryRoot", false).toBool())
    {
        #ifdef Q_OS_WIN
        qfdFileDlg.setDirectoryUrl(IComUtilityFuncs::GetMyComputerURL());
        #else
        qfdFileDlg.setDirectory(QDir::rootPath());
        #endif
    }
    else
    {
        #ifdef Q_OS_WIN
        qfdFileDlg.setDirectoryUrl(IComUtilityFuncs::GetMyComputerURL());
        #else
        qfdFileDlg.setDirectory(QDir::homePath());
        #endif
    }
    m_rqsetSettings.endGroup();

    if (qfdFileDlg.exec())
        OpenDirectory(qfdFileDlg.selectedFiles().at(0));
}


void IUIFileList::SetDirectoryFromAddressBar()
{
    #ifdef QT_DEBUG
    if (m_rpuitbToolBar->GetAdddressBarText() == "test")
    {
        //PrintWatchList();

        const int kiNumRows = m_pqtwNameCurrent->rowCount();
        for (int iRow = 0 ; iRow < kiNumRows ; ++iRow)
        {
            qDebug() << m_pqtwNameCurrent->item(iRow, 0)->data(FileInfo).value<QFileInfo>().fileName() << m_pqtwNameCurrent->item(iRow, 0)->data(FileInfo).value<QFileInfo>().suffix();
        }

        return;
    }
    #endif

    OpenDirectory(m_rpuitbToolBar->GetAdddressBarText());
    m_rpuitbToolBar->AddressBarSelectAll();
}


void IUIFileList::NavigateBack()
{
    if (m_qsqstrBackStack.isEmpty())
        return;

    QString qstrCurrentDir = m_bDisplayingMyComputer ? m_qstrMyComputerPath : m_qdirDirReader.path();
    if (SetDirectory(m_qsqstrBackStack.pop()))
        m_qsqstrForwardStack.push(qstrCurrentDir);
    else
        m_qsqstrBackStack.clear();

    EnableBackForwardActions();
}


void IUIFileList::NavigateForward()
{
    if (m_qsqstrForwardStack.isEmpty())
        return;

    QString qstrCurrentDir = m_bDisplayingMyComputer ? m_qstrMyComputerPath : m_qdirDirReader.path();
    if (SetDirectory(m_qsqstrForwardStack.pop()))
        m_qsqstrBackStack.push(qstrCurrentDir);
    else
        m_qsqstrForwardStack.clear();

    EnableBackForwardActions();
}


void IUIFileList::NavigateUp()
{
    QString qstrNewDir;
    QString qstrCurrentPath = m_qdirDirReader.path();

    if (m_qdirDirReader.isRoot() == false)
    {
        qstrNewDir = qstrCurrentPath.left(qstrCurrentPath.lastIndexOf('/')+1);
    }
    else
    {
        #ifdef Q_OS_WIN
        qstrNewDir = m_qstrMyComputerPath;
        #else
        return;
        #endif
    }

    m_qsqstrForwardStack.clear();
    m_qsqstrBackStack.push(qstrCurrentPath);
    SetDirectory(qstrNewDir);
}


void IUIFileList::NavigateRefresh()
{
    RefreshDirectoryHard();
}


void IUIFileList::NavigateToStartDirectory(const bool kbStartupCall)
{
    QString qstrCurrentDir = m_bDisplayingMyComputer ? m_qstrMyComputerPath : m_qdirDirReader.path();

    #ifdef Q_OS_WIN
    QString qstrRootPath = m_qstrMyComputerPath;
    QString qstrDefaultPath = m_qstrMyComputerPath;
    #else
    QString qstrRootPath = QDir::rootPath();
    QString qstrDefaultPath = QDir::homePath();
    #endif

    bool bSuccess = false;
    QSettings & rqsetSettings = m_pmwMainWindow->GetSettings();
    if      (rqsetSettings.value("Startup/StartDirectoryRoot", IDlgPreferences::StartDirectoryRootDefaultVal()).toBool())
        bSuccess = SetDirectory(qstrRootPath);
    else if (rqsetSettings.value("Startup/StartDirectoryHome", IDlgPreferences::StartDirectoryHomeDefaultVal()).toBool())
        bSuccess = SetDirectory(QDir::homePath());
    else if (rqsetSettings.value("Startup/StartDirectoryRestoreLast", false).toBool() && kbStartupCall)
        RestoreLastSessionDirectory();
    else if (rqsetSettings.value("Startup/StartDirectorySpecified", false).toBool())
        bSuccess = SetDirectory(rqsetSettings.value("Startup/StartDirectorySpecifiedDirectory", qstrRootPath).toString());
    else
        bSuccess = SetDirectory(qstrDefaultPath);

    if (kbStartupCall == false && bSuccess == true)
    {
        m_qsqstrBackStack.push(qstrCurrentDir);
        m_qsqstrForwardStack.clear();
        EnableBackForwardActions();
    }
}


void IUIFileList::EnableBackForwardActions()
{
    m_rpuimbMenuBar->EnableBackAction(!m_qsqstrBackStack.isEmpty());
    m_rpuimbMenuBar->EnableForwardAction(!m_qsqstrForwardStack.isEmpty());
}


void IUIFileList::OpenItemAtRow(const int kiRow)
{
    if (m_pqtwNameCurrent->item(kiRow, 0)->data(FileInfo).value<QFileInfo>().isDir())
    {
        m_qsqstrForwardStack.clear();
        m_qsqstrBackStack.push(m_bDisplayingMyComputer ? m_qstrMyComputerPath : m_qdirDirReader.path());
        EnableBackForwardActions();
        SetDirectory(m_pqtwNameCurrent->item(kiRow, 0)->data(FileInfo).value<QFileInfo>().filePath());
    }
    else if (m_pqtwNameCurrent->item(kiRow, 0)->data(FileInfo).value<QFileInfo>().isFile())
    {
        if (m_bOpenFileWhenDblClicked)
        {
            // fromUserInput() is necessary for paths containing spaces as it replaces the space character with the %20 encoding required by QUrl
            QDesktopServices::openUrl(QUrl(QUrl::fromUserInput(m_pqtwNameCurrent->item(kiRow, 0)->data(FileInfo).value<QFileInfo>().absoluteFilePath())));
        }
    }
}


void IUIFileList::OpenFilePropertiesForRow(const int kiRow)
{
    if (m_pqtwNameCurrent->item(kiRow, 0)->data(FileInfo).value<QFileInfo>().isFile())
    {
        new IComDlgFileProperties(m_pqtwNameCurrent->item(kiRow, 0)->data(FileInfo).value<QFileInfo>().filePath());
    }
}


void IUIFileList::OpenRenameFileDlgForRow(const int kiRow)
{
    if (m_bDisplayingMyComputer)
    {
        QMessageBox::information(this, tr("Cannot Rename Drives"), tr("Computer drives cannot be renamed."), QMessageBox::Ok);
        return;
    }

    new IDlgRenameFile(this, m_qdirDirReader, m_pqtwNameCurrent->item(kiRow, 0)->text());
}


void IUIFileList::OpenRenameFileDlgForCurrentRow()
{
    OpenRenameFileDlgForRow(m_pqtwNameCurrent->selectedRanges().first().topRow());
}


void IUIFileList::ShowContextMenu(QPoint qpntClickPoint)
{
    #ifdef Q_OS_WIN
    if (m_bDisplayingMyComputer)
        m_pqacgSortGroup->setEnabled(false);
    else if (m_pqacgSortGroup->isEnabled() == false)
        m_pqacgSortGroup->setEnabled(true);
    #endif

    QModelIndex qmiIndex = m_pqtwNameCurrent->indexAt(qpntClickPoint);
    if (qmiIndex.isValid() == false)
    {
        m_pqmenuEmptyAreaContextMenu->popup(m_pqtwNameCurrent->viewport()->mapToGlobal(qpntClickPoint));
    }
    else if (m_pqtwNameCurrent->item(qmiIndex.row(), 0)->data(FileInfo).value<QFileInfo>().isDir())
    {
        m_iContextMenuRowClicked = qmiIndex.row();
        m_pqactRenameFolder->setEnabled(!m_bDisplayingMyComputer);
        m_pqmenuFolderContextMenu->popup(m_pqtwNameCurrent->viewport()->mapToGlobal(qpntClickPoint));
    }
    else
    {
        m_iContextMenuRowClicked = qmiIndex.row();
        m_pqmenuFileContextMenu->popup(m_pqtwNameCurrent->viewport()->mapToGlobal(qpntClickPoint));
    }
}


void IUIFileList::CreateContextMenus()
{
    m_pqactSortByName = new QAction(tr("Sort By Name"), this);
    m_pqactSortByName->setCheckable(true);
    m_pqactSortByName->setChecked(true);
    QObject::connect(m_pqactSortByName, SIGNAL(triggered()), this, SLOT(SortByName()));

    m_pqactSortByType = new QAction(tr("Sort By Type"), this);
    m_pqactSortByType->setCheckable(true);
    QObject::connect(m_pqactSortByType, SIGNAL(triggered()), this, SLOT(SortByType()));

    m_pqactSortByModified = new QAction(tr("Sort By Date Modified"), this);
    m_pqactSortByModified->setCheckable(true);
    QObject::connect(m_pqactSortByModified, SIGNAL(triggered()), this, SLOT(SortByModified()));

    m_pqacgSortGroup = new QActionGroup(this);
    m_pqacgSortGroup->addAction(m_pqactSortByName);
    m_pqacgSortGroup->addAction(m_pqactSortByType);
    m_pqacgSortGroup->addAction(m_pqactSortByModified);

    m_pqmenuEmptyAreaContextMenu = new QMenu(this);
    m_pqmenuEmptyAreaContextMenu->addAction(m_pqactSortByName);
    m_pqmenuEmptyAreaContextMenu->addAction(m_pqactSortByType);
    m_pqmenuEmptyAreaContextMenu->addAction(m_pqactSortByModified);


    m_pqactRenameFolder = new QAction(tr("&Rename Folder..."), this);
    QObject::connect(m_pqactRenameFolder, SIGNAL(triggered()), this, SLOT(OpenRenameFileDlgContext()));

    m_pqactOpenFolder = new QAction(tr("&Open Folder"), this);
    QObject::connect(m_pqactOpenFolder, SIGNAL(triggered()), this, SLOT(OpenDirectoryContext()));

    m_pqmenuFolderContextMenu = new QMenu(this);
    m_pqmenuFolderContextMenu->addAction(m_pqactSortByName);
    m_pqmenuFolderContextMenu->addAction(m_pqactSortByType);
    m_pqmenuFolderContextMenu->addAction(m_pqactSortByModified);
    m_pqmenuFolderContextMenu->addSeparator();
    m_pqmenuFolderContextMenu->addAction(m_pqactRenameFolder);
    m_pqmenuFolderContextMenu->addAction(m_pqactOpenFolder);


    m_pqactRenameFile = new QAction(tr("&Rename File..."), this);
    QObject::connect(m_pqactRenameFile, SIGNAL(triggered()), this, SLOT(OpenRenameFileDlgContext()));

    m_pqactViewProperties = new QAction(tr("&View File Properties && Tags..."), m_pmwMainWindow);
    m_pqactViewProperties->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_FileDialogContentsView));
    QObject::connect(m_pqactViewProperties, SIGNAL(triggered()), this, SLOT(OpenFilePropertiesContext()));

    m_pqmenuFileContextMenu = new QMenu(this);
    m_pqmenuFileContextMenu->addAction(m_pqactSortByName);
    m_pqmenuFileContextMenu->addAction(m_pqactSortByType);
    m_pqmenuFileContextMenu->addAction(m_pqactSortByModified);
    m_pqmenuFileContextMenu->addSeparator();
    m_pqmenuFileContextMenu->addAction(m_pqactRenameFile);
    m_pqmenuFileContextMenu->addAction(m_pqactViewProperties);
}


void IUIFileList::SortByName()
{
    if (m_ifisFileSort.GetSortOrder() == ISysFileInfoSort::Name)
        return;

    if (m_ifisFileSort.GetSortOrder() == ISysFileInfoSort::Modified)
        ResortTable(ISysFileInfoSort::Name, ISysFileInfoSort::Name);
    else
        ResortTableFilesOnly(ISysFileInfoSort::Name);

    m_pqactSortByName->setChecked(true);
    m_ifisFileSort.SetOrderName();

    if (m_rpuirRenameUI->GetRenameUINumber()->Numberingenabled())
        GeneratePreviewName();
}


void IUIFileList::SortByType()
{
    if (m_ifisFileSort.GetSortOrder() == ISysFileInfoSort::Type)
        return;

    if (m_ifisFileSort.GetSortOrder() == ISysFileInfoSort::Modified)
        ResortTable(ISysFileInfoSort::Name, ISysFileInfoSort::Type);
    else
        ResortTableFilesOnly(ISysFileInfoSort::Type);

    m_pqactSortByType->setChecked(true);
    m_ifisFileSort.SetOrderType();

    if (m_rpuirRenameUI->GetRenameUINumber()->Numberingenabled())
        GeneratePreviewName();
}


void IUIFileList::SortByModified()
{
    if (m_ifisFileSort.GetSortOrder() == ISysFileInfoSort::Modified)
        return;

    ResortTable(ISysFileInfoSort::Modified, ISysFileInfoSort::Modified);
    m_pqactSortByModified->setChecked(true);
    m_ifisFileSort.SetOrderModified();

    if (m_rpuirRenameUI->GetRenameUINumber()->Numberingenabled())
        GeneratePreviewName();
}


void IUIFileList::ResortTable(const int kiFolderSortOrder, const int kFileSortOrder)
{
    int iNumRows = m_pqtwNameCurrent->rowCount();
    int iNumFolders = GetNumFolders();
    int iNumFiles = iNumRows-iNumFolders;

    if (iNumFolders != 0 && kiFolderSortOrder != INT_MAX)
        ResortRows(0, iNumFolders-1, kiFolderSortOrder);

    if (iNumFiles != 0)
        ResortRows(iNumFolders, iNumRows-1, kFileSortOrder);
}


void IUIFileList::ResortRows(const int kiStart, const int kiEnd, const int kiSortOrder)
{
    QList<ITableRow*> qlstRowList;
    qlstRowList.reserve(kiEnd-kiStart);

    for (int iRow = kiStart ; iRow <= kiEnd ; ++iRow)
        qlstRowList.append(new ITableRow(m_pqtwNameCurrent->takeItem(iRow, 0), m_pqtwNamePreview->takeItem(iRow, 0)));

    m_ifisFileSort.ResortRows(qlstRowList, kiSortOrder);

    int iIndex = 0;
    for (int iRow = kiStart ; iRow <= kiEnd ; ++iRow, ++iIndex)
    {
        m_pqtwNameCurrent->setItem(iRow, 0, qlstRowList.at(iIndex)->m_pqtwiCurrent);
        m_pqtwNamePreview->setItem(iRow, 0, qlstRowList.at(iIndex)->m_pqtwiPreview);
        delete qlstRowList.at(iIndex);
    }
}


int IUIFileList::GetNumFolders()
{
    int iNumFolders = 0;
    int iNumRows = m_pqtwNameCurrent->rowCount();
    while (iNumFolders < iNumRows)
    {
        if (m_pqtwNameCurrent->item(iNumFolders, 0)->data(FileInfo).value<QFileInfo>().isDir() == false)
            return iNumFolders;
        ++iNumFolders;
    }
    return iNumFolders;
}


void IUIFileList::DirectoryChanged(const QString & krqstrDirectory)
{
    #ifdef QT_DEBUG
    qDebug() << "Directory Changed:" << krqstrDirectory;
    #endif

    if (m_bAutoRefresh == false || krqstrDirectory != m_qdirDirReader.path())
    {
        #ifdef QT_DEBUG
        qDebug() << "Ignoring Subdirectory Change:" << krqstrDirectory;
        #endif

        return;
    }

    RefreshDirectorySoft();
}


void IUIFileList::FileChanged(const QString & krqstrFile)
{
    #ifdef QT_DEBUG
    qDebug() << "File Changed:" << krqstrFile;
    #endif

    QFileInfo qfiModifiedFile(krqstrFile);
    if (qfiModifiedFile.exists())
    {
        const QString qstrFileName = qfiModifiedFile.fileName();
        const int kiNumRows = m_pqtwNameCurrent->rowCount();
        for (int iRow = 0 ; iRow < kiNumRows ; ++iRow)
        {
            if (m_pqtwNameCurrent->item(iRow, 0)->text() == qstrFileName)
            {
                QTableWidgetItem* pqtwiTableItem = m_pqtwNameCurrent->item(iRow, 0);

                // Sometimes it generates a signal twice for one modification, so we check it has actually been modified
                if (qfiModifiedFile.lastModified() != pqtwiTableItem->data(FileInfo).value<QFileInfo>().lastModified())
                {
                    #ifdef QT_DEBUG
                    qDebug() << "Processing Change:" << krqstrFile;
                    #endif

                    pqtwiTableItem->setData(FileInfo, QVariant::fromValue(qfiModifiedFile));

                    if (m_bMetaTagsReadMusic)
                        ReadFileMetaTagsMusic(pqtwiTableItem);
                    if (m_bMetaTagsReadExif)
                        ReadFileMetaTagsExif(pqtwiTableItem);
                }

                GeneratePreviewNameAndExtension();
                break;
            }
        }
    }
}


void IUIFileList::PrintWatchList()
{
    #ifdef QT_DEBUG
    qDebug() << "-=Watch List=-";
    QStringList qstrlList = m_qfswFSWatcher.directories();
    QStringList::const_iterator itItem;
    for (itItem = qstrlList.begin() ; itItem != qstrlList.end() ; ++itItem)
        qDebug() << "Dir: " << *itItem;

    qstrlList = m_qfswFSWatcher.files();
    for (itItem = qstrlList.begin() ; itItem != qstrlList.end() ; ++itItem)
        qDebug() << "File:" << *itItem;

    qDebug() << endl;
    #endif
}


void IUIFileList::MoveSelectionUp()
{
    m_bSyncSelection = false;
    m_pqtwNameCurrent->setUpdatesEnabled(false);
    m_pqtwNamePreview->setUpdatesEnabled(false);

    IComTableWidgetSelectionSortAsc twslSelectionSortAsc;
    QList<QTableWidgetSelectionRange> qlitwirSelectedRange = m_pqtwNameCurrent->selectedRanges();
    std::sort(qlitwirSelectedRange.begin(), qlitwirSelectedRange.end(), twslSelectionSortAsc);

    int iTop = 0;
    QList<QTableWidgetSelectionRange>::const_iterator kitSelection;
    for (kitSelection = qlitwirSelectedRange.constBegin() ; kitSelection != qlitwirSelectedRange.constEnd() ; ++kitSelection)
    {
        MoveSelectionUp(m_pqtwNameCurrent, iTop, kitSelection->topRow(), kitSelection->bottomRow());
        iTop = MoveSelectionUp(m_pqtwNamePreview, iTop, kitSelection->topRow(), kitSelection->bottomRow());
    }

    m_pqtwNameCurrent->setUpdatesEnabled(true);
    m_pqtwNamePreview->setUpdatesEnabled(true);
    m_bSyncSelection = true;

    if (m_rpuirRenameUI->GetRenameUINumber()->Numberingenabled())
        GeneratePreviewName();
}


void IUIFileList::MoveSelectionDown()
{
    m_bSyncSelection = false;
    m_pqtwNameCurrent->setUpdatesEnabled(false);
    m_pqtwNamePreview->setUpdatesEnabled(false);

    IComTableWidgetSelectionSortDesc twslSelectionSortDesc;
    QList<QTableWidgetSelectionRange> qlitwirSelectedRange = m_pqtwNameCurrent->selectedRanges();
    std::sort(qlitwirSelectedRange.begin(), qlitwirSelectedRange.end(), twslSelectionSortDesc);

    int iBottom = m_pqtwNameCurrent->rowCount() - 1;
    QList<QTableWidgetSelectionRange>::const_iterator kitSelection;
    for (kitSelection = qlitwirSelectedRange.constBegin() ; kitSelection != qlitwirSelectedRange.constEnd() ; ++kitSelection)
    {
        MoveSelectionDown(m_pqtwNameCurrent, iBottom, kitSelection->topRow(), kitSelection->bottomRow());
        iBottom = MoveSelectionDown(m_pqtwNamePreview, iBottom, kitSelection->topRow(), kitSelection->bottomRow());
    }

    m_pqtwNameCurrent->setUpdatesEnabled(true);
    m_pqtwNamePreview->setUpdatesEnabled(true);
    m_bSyncSelection = true;

    if (m_rpuirRenameUI->GetRenameUINumber()->Numberingenabled())
        GeneratePreviewName();
}


int IUIFileList::MoveSelectionUp(QTableWidget * pqtwTable,  const int kiTop, const int kiSelectionTop, const int kiSelectionBottom)
{
    QTableWidgetItem* pqtwiItemAboveSelection;
    QTableWidgetItem* pqtwiItemShiftUp;
    if (kiSelectionTop != kiTop)
    {
        pqtwiItemAboveSelection = pqtwTable->takeItem(kiSelectionTop-1, 0);
        for (int iRow = kiSelectionTop ; iRow <= kiSelectionBottom ; ++iRow)
        {
            pqtwiItemShiftUp = pqtwTable->takeItem(iRow, 0);
            pqtwTable->setItem(iRow-1, 0, pqtwiItemShiftUp);
        }
        pqtwTable->setItem(kiSelectionBottom, 0, pqtwiItemAboveSelection);
        pqtwTable->setRangeSelected(QTableWidgetSelectionRange(kiSelectionTop,   0, kiSelectionBottom,   0), false);
        pqtwTable->setRangeSelected(QTableWidgetSelectionRange(kiSelectionTop-1, 0, kiSelectionBottom-1, 0), true);

        return kiSelectionBottom;
    }
    return kiSelectionBottom + 1;
}


int IUIFileList::MoveSelectionDown(QTableWidget * pqtwTable,  const int kiBottom, const int kiSelectionTop, const int kiSelectionBottom)
{
    QTableWidgetItem* pqtwiItemBelowSelection;
    QTableWidgetItem* pqtwiItemShiftDown;
    if (kiSelectionBottom != kiBottom)
    {
        pqtwiItemBelowSelection = pqtwTable->takeItem(kiSelectionBottom+1, 0);
        for (int iRow = kiSelectionBottom ; iRow >= kiSelectionTop ; --iRow)
        {
            pqtwiItemShiftDown = pqtwTable->takeItem(iRow, 0);
            pqtwTable->setItem(iRow+1, 0, pqtwiItemShiftDown);
        }
        pqtwTable->setItem(kiSelectionTop, 0, pqtwiItemBelowSelection);
        pqtwTable->setRangeSelected(QTableWidgetSelectionRange(kiSelectionTop,   0, kiSelectionBottom,   0), false);
        pqtwTable->setRangeSelected(QTableWidgetSelectionRange(kiSelectionTop+1, 0, kiSelectionBottom+1, 0), true);

        return kiSelectionTop;
    }
    return kiSelectionTop - 1;
}


void IUIFileList::GeneratePreviewName()
{
    if (m_rpuirRenameUI->ChangingSettings() || m_bDisplayingMyComputer)
        return;

    #ifdef QT_DEBUG
    qDebug() << "-=Generating Preview Name=-";
    #endif

    QString qstrFileName;
    QString qstrGeneratedName;
    QString qstrPreviewName;
    QString qstrPreviewExtension;
    QTableWidgetItem* pqtwiFileItem;
    int iExtensionIndexCurrent;
    int iExtensionIndexPreview;

    FlagItemsForRenaming();
    m_rpuirRenameUI->GetRenameUINumber()->InitNumberingVals(m_iNumFilesToRename);

    const int kiNumRows = m_pqtwNameCurrent->rowCount();
    for (int iRow = 0 ; iRow < kiNumRows ; ++iRow)
    {
        pqtwiFileItem = m_pqtwNameCurrent->item(iRow, 0);
        qstrFileName = pqtwiFileItem->text();
        if (pqtwiFileItem->data(FlaggedForRenme).toBool() == false)
        {
           m_pqtwNamePreview->item(iRow, 0)->setText(qstrFileName);
        }
        else if (pqtwiFileItem->data(FileInfo).value<QFileInfo>().isDir())
        {
            m_rpuirRenameUI->GenerateName(qstrFileName, pqtwiFileItem);
            m_pqtwNamePreview->item(iRow, 0)->setText(qstrFileName);
        }
        else
        {
            // left() returns entire string if n is less than zero, so this works even if there's no extension
            iExtensionIndexCurrent = qstrFileName.lastIndexOf('.');
            qstrGeneratedName = qstrFileName.left(iExtensionIndexCurrent);
            m_rpuirRenameUI->GenerateName(qstrGeneratedName, pqtwiFileItem);

            // Use extension from Preview table rather than generate it again since no changes have been made to the extension settings
            qstrPreviewName = m_pqtwNamePreview->item(iRow, 0)->text();
            iExtensionIndexPreview = qstrPreviewName.lastIndexOf('.');

            if (iExtensionIndexPreview == -1 || iExtensionIndexCurrent == -1)
            {
                m_pqtwNamePreview->item(iRow, 0)->setText(qstrGeneratedName);
            }
            else
            {
                qstrPreviewExtension = qstrPreviewName.mid(iExtensionIndexPreview);
                m_pqtwNamePreview->item(iRow, 0)->setText(qstrGeneratedName + qstrPreviewExtension);
            }
        }
    }
    HighlightRowsWithModifiedNames();
}


void IUIFileList::GeneratePreviewNameAndExtension()
{
    if (m_rpuirRenameUI->ChangingSettings() || m_bDisplayingMyComputer)
        return;

    #ifdef QT_DEBUG
    qDebug() << "-=Generating Preview Name & Extension=-";
    #endif

    QString qstrFileName;
    QString qstrGeneratedName;
    QString qstrGeneratedExtension;
    QTableWidgetItem* pqtwiFileItem;
    int iExtensionIndex;

    FlagItemsForRenaming();
    m_rpuirRenameUI->GetRenameUINumber()->InitNumberingVals(m_iNumFilesToRename);

    const int kiNumRows = m_pqtwNameCurrent->rowCount();
    for (int iRow = 0 ; iRow < kiNumRows ; ++iRow)
    {
        pqtwiFileItem = m_pqtwNameCurrent->item(iRow, 0);
        qstrFileName = pqtwiFileItem->text();
        if (pqtwiFileItem->data(FlaggedForRenme).toBool() == false)
        {
           m_pqtwNamePreview->item(iRow, 0)->setText(qstrFileName);
        }
        else if (pqtwiFileItem->data(FileInfo).value<QFileInfo>().isDir())
        {
            m_rpuirRenameUI->GenerateName(qstrFileName, pqtwiFileItem);
            m_pqtwNamePreview->item(iRow, 0)->setText(qstrFileName);
        }
        else
        {
            // left() returns entire string if n is less than zero, so this works even if there's no extension
            iExtensionIndex = qstrFileName.lastIndexOf('.');
            qstrGeneratedName = qstrFileName.left(iExtensionIndex);
            m_rpuirRenameUI->GenerateName(qstrGeneratedName, pqtwiFileItem);

            if (iExtensionIndex == -1)
            {
                m_pqtwNamePreview->item(iRow, 0)->setText(qstrGeneratedName);
            }
            else
            {
                qstrGeneratedExtension = qstrFileName.mid(iExtensionIndex+1);
                m_rpuirRenameUI->GenerateExtension(qstrGeneratedExtension, pqtwiFileItem);
                if (qstrGeneratedExtension.isEmpty())
                {
                    m_pqtwNamePreview->item(iRow, 0)->setText(qstrGeneratedName);
                }
                else if (qstrGeneratedExtension.startsWith('.'))
                {
                    int iIndex = 1;
                    int iLength = qstrGeneratedExtension.length();
                    while (iIndex < iLength && qstrGeneratedExtension.at(iIndex) == '.')
                        ++iIndex;

                    if (iIndex >= iLength)
                        m_pqtwNamePreview->item(iRow, 0)->setText(qstrGeneratedName);
                    else
                        m_pqtwNamePreview->item(iRow, 0)->setText(qstrGeneratedName + qstrGeneratedExtension.mid(iIndex-1));
                }
                else
                {
                    m_pqtwNamePreview->item(iRow, 0)->setText(qstrGeneratedName + '.' + qstrGeneratedExtension);
                }
            }
        }
    }
    HighlightRowsWithModifiedNames();
}


void IUIFileList::RenameElementsSettingsChanged()
{
    GeneratePreviewNameAndExtension();
}


void IUIFileList::HighlightRowsWithModifiedNames(const bool kbForceRedraw)
{
    bool bNameChangeColourText   = m_bNameChangeColourText || kbForceRedraw;
    bool bNameChangeHighlightRow = m_bNameChangeHighlightRow || kbForceRedraw;

    QColor qcolDefaultForeground    = (QApplication::palette().text().color());
    QColor qcolDefaultBackground    = (QApplication::palette().base().color());
    QColor qcolHighlightForeground = m_bNameChangeColourText ? m_qcolNameChangeTextColour : qcolDefaultForeground;
    QColor qcolHighlightBackground = m_bNameChangeHighlightRow ? m_qcolNameChangeHighlightColour : qcolDefaultBackground;

    bool bRowChanged;
    bool bFilesToRename = false;
    int iNumFiles = m_pqtwNameCurrent->rowCount();
    for (int iRow = 0 ; iRow < iNumFiles ; ++iRow)
    {
        bRowChanged = m_pqtwNameCurrent->item(iRow, 0)->text() != m_pqtwNamePreview->item(iRow, 0)->text();

        if (bNameChangeColourText)
            m_pqtwNamePreview->item(iRow, 0)->setForeground(bRowChanged ? qcolHighlightForeground : qcolDefaultForeground);

        if (bNameChangeHighlightRow)
            m_pqtwNamePreview->item(iRow, 0)->setBackground(bRowChanged ? qcolHighlightBackground : qcolDefaultBackground);

        if (bFilesToRename == false && bRowChanged == true)
            bFilesToRename = true;
    }

    m_rpuirRenameUI->EnableRenameButton(bFilesToRename);
}


void IUIFileList::PerformRename()
{
    if (RenameEndResultValid() == false)
        return;

    QList<int> qlstiRows;
    QStringList qstrlCurrentName, qstrlNewName;
    const int kiNumRows = m_pqtwNameCurrent->rowCount();
    for (int iRow = 0 ; iRow < kiNumRows ; ++iRow)
    {
        if (m_pqtwNameCurrent->item(iRow, 0)->data(FlaggedForRenme).toBool() && m_pqtwNameCurrent->item(iRow, 0)->text() != m_pqtwNamePreview->item(iRow, 0)->text())
        {
            qstrlCurrentName.push_back(m_pqtwNameCurrent->item(iRow, 0)->text());
            qstrlNewName.push_back(m_pqtwNamePreview->item(iRow, 0)->text());
            qlstiRows.push_back(iRow);
        }
    }

    m_qstrUndoDirectory = m_qdirDirReader.path();
    m_qstrlUndoRenameFrom.clear();
    m_qstrlUndoRenameTo.clear();

    RenameFiles(qstrlCurrentName, qstrlNewName, &qlstiRows);

    if (m_qstrlUndoRenameFrom.isEmpty() == false)
        m_rpuirRenameUI->EnableUndoButton(true);
}


void IUIFileList::UndoRename()
{
    if (m_qdirDirReader.path() != m_qstrUndoDirectory)
    {
        if (SetDirectory(m_qstrUndoDirectory))
        {
            QString qstrCurrentDir = m_bDisplayingMyComputer ? m_qstrMyComputerPath : m_qdirDirReader.path();
            m_qsqstrBackStack.push(qstrCurrentDir);
            m_qsqstrForwardStack.clear();
            EnableBackForwardActions();
        }
    }

    RenameFiles(m_qstrlUndoRenameFrom, m_qstrlUndoRenameTo);

    m_rpuirRenameUI->EnableUndoButton(false);
    m_qstrlUndoRenameFrom.clear();
    m_qstrlUndoRenameTo.clear();
}


void IUIFileList::RenameFiles(QStringList & rqstrlCurrentName, QStringList & rqstrlNewName, QList<int>* pqlstiRows)
{
    bool bRenameBackwards = false;
    if (RenameProccessValidForward(rqstrlCurrentName, rqstrlNewName) == false)
    {
        bRenameBackwards = true;
        if (RenameProccessValidBackward(rqstrlCurrentName, rqstrlNewName) == false)
        {
            RenameFilesIntermediate(rqstrlCurrentName, rqstrlNewName, pqlstiRows);
            return;
        }
    }

    bool bUndoOperation = (pqlstiRows == nullptr);
    QString qstrCurrentName, qstrNewName;
    IDlgRenameErrorList* preldRenameErrorsDialog = nullptr;

    ClearFSWatcher();

    const int kiNumFiles = rqstrlCurrentName.size();
    IComDlgProgress idprgRenameProgress(m_pmwMainWindow, tr("Renaming Files"), "Renaming: \nTo: ", kiNumFiles, false, false, kiNumFiles > m_kiShowRenameProgressFileNum ? 0 : m_kiShowRenameProgressAfterMS);

    int iIndex;
    for (int iCount = 0 ; iCount < kiNumFiles ; ++iCount)
    {
        iIndex          = bRenameBackwards ? kiNumFiles-(iCount+1) : iCount;
        qstrCurrentName = rqstrlCurrentName.at(iIndex);
        qstrNewName     = rqstrlNewName.at(iIndex);
        idprgRenameProgress.UpdateMessage(tr("Renaming: %1\nTo: %2").arg(qstrCurrentName).arg(qstrNewName));

        if (bUndoOperation == false)
            RemoveInvalidTrailingCharacters(qstrNewName);

        if (m_qdirDirReader.rename(qstrCurrentName, qstrNewName) == false)
        {
            if (preldRenameErrorsDialog == nullptr)
                preldRenameErrorsDialog = new IDlgRenameErrorList(this, true);

            QFileInfo qfiFileInfo;
            if (bUndoOperation == false)
                qfiFileInfo = m_pqtwNameCurrent->item(pqlstiRows->at(iIndex), 0)->data(FileInfo).value<QFileInfo>();
            preldRenameErrorsDialog->AddToErrorList(qstrCurrentName, qstrNewName, DetermineReasonForFailure(qstrCurrentName, qstrNewName, qfiFileInfo));
        }
        else
        {
            if (bUndoOperation == false)
            {
                m_pqtwNameCurrent->item(pqlstiRows->at(iIndex), 0)->setText(qstrNewName);
                m_qstrlUndoRenameFrom.push_back(qstrNewName);
                m_qstrlUndoRenameTo.push_back(qstrCurrentName);
            }
        }

        idprgRenameProgress.IncrementAndUpdateProgress();
        if (idprgRenameProgress.Aborted())
            break;
    }

    if (preldRenameErrorsDialog != nullptr)
        preldRenameErrorsDialog->ResizeColumnsAndShow();

    if (bUndoOperation || m_bReSortFileListAfterRename)
        RefreshDirectoryHard();
    else
        RefreshDirectoryPostRename();
}


void IUIFileList::RenameFilesIntermediate(QStringList & rqstrlCurrentName, QStringList & rqstrlNewName, QList<int>* pqlstiRows)
{
    bool bUndoOperation = (pqlstiRows == nullptr);
    QString qstrCurrentName, qstrNewName, qstrIntermedName;
    QStringList qstrlIntermediateNames;
    unsigned int uiIntermedNum = UINT_MAX;
    IDlgRenameErrorList* preldRenameErrorsDialog = nullptr;

    ClearFSWatcher();

    const int kiNumFiles = rqstrlCurrentName.size();
    IComDlgProgress idprgRenameProgress(m_pmwMainWindow, tr("Renaming Files"), QString(), kiNumFiles, false, false, kiNumFiles > m_kiShowRenameProgressFileNum ? 0 : m_kiShowRenameProgressAfterMS);
    idprgRenameProgress.HideAbort();

    int iIndex;
    for (iIndex = 0 ; iIndex < kiNumFiles ; ++iIndex)
    {
        qstrCurrentName = rqstrlCurrentName.at(iIndex);
        qstrNewName     = rqstrlNewName.at(iIndex);

        if (bUndoOperation == false)
            RemoveInvalidTrailingCharacters(qstrNewName);

        qstrIntermedName = QString("INV#%1#.%2").arg(uiIntermedNum--, 8, 16, QChar('0')).arg(qstrNewName);
        if (m_qdirDirReader.rename(qstrCurrentName, qstrIntermedName) == false)
        {
            if (preldRenameErrorsDialog == nullptr)
                preldRenameErrorsDialog = new IDlgRenameErrorList(this, true);

            QFileInfo qfiFileInfo;
            if (bUndoOperation == false)
                qfiFileInfo = m_pqtwNameCurrent->item(pqlstiRows->at(iIndex), 0)->data(FileInfo).value<QFileInfo>();
            preldRenameErrorsDialog->AddToErrorList(qstrCurrentName, qstrNewName, DetermineReasonForFailure(qstrCurrentName, qstrNewName, qfiFileInfo));
        }
        else
        {
            if (bUndoOperation == false)
            {
                m_pqtwNameCurrent->item(pqlstiRows->at(iIndex), 0)->setText(qstrNewName);
                m_qstrlUndoRenameFrom.push_back(qstrIntermedName);
                m_qstrlUndoRenameTo.push_back(qstrCurrentName);
            }
            else
            {
                qstrlIntermediateNames.push_back(qstrIntermedName);
            }
        }

        if (iIndex % 2 == 0)
            idprgRenameProgress.IncrementAndUpdateProgress();
    }


    QStringList::iterator itName;
    QStringList & rqstrlIntermediateList = bUndoOperation ? qstrlIntermediateNames : m_qstrlUndoRenameFrom;
    for (itName = rqstrlIntermediateList.begin() ; itName != rqstrlIntermediateList.end() ; ++itName)
    {
        qstrNewName = (*itName).mid(14);
        if (m_qdirDirReader.rename(*itName, qstrNewName))
        {
            if (bUndoOperation == false)
                *itName = qstrNewName;
        }

        if (iIndex++ % 2 == 0)
            idprgRenameProgress.IncrementAndUpdateProgress();
    }

    if (preldRenameErrorsDialog != nullptr)
        preldRenameErrorsDialog->ResizeColumnsAndShow();

    if (bUndoOperation || m_bReSortFileListAfterRename)
        RefreshDirectoryHard();
    else
        RefreshDirectoryPostRename();
}


void IUIFileList::RenameFilesValidated(QStringList & rqstrlCurrentName, QStringList & rqstrlNewName, QList<int>* pqlstiRows)
{
    bool bRenameBackwards = false;
    bool bItermediateRename = false;
    if (RenameProccessValidForward(rqstrlCurrentName, rqstrlNewName) == false)
    {
        if (RenameProccessValidBackward(rqstrlCurrentName, rqstrlNewName))
            bRenameBackwards = true;
        else
            bItermediateRename = true;
    }

    bool bUndoOperation = (pqlstiRows == nullptr);
    QString qstrCurrentName, qstrNewName, qstrIntermedName;
    QString & rqstrRenameName = bItermediateRename ? qstrIntermedName : qstrNewName;
    QStringList qstrlIntermediateNames;
    unsigned int uiIntermedNum = UINT_MAX;
    IDlgRenameErrorList* preldRenameErrorsDialog = nullptr;

    ClearFSWatcher();

    const int kiNumFiles = rqstrlCurrentName.size();
    IComDlgProgress idprgRenameProgress(m_pmwMainWindow, tr("Renaming Files"), QString(), kiNumFiles, false, false, kiNumFiles > m_kiShowRenameProgressFileNum ? 0 : m_kiShowRenameProgressAfterMS);
    idprgRenameProgress.HideAbort();

    int iCount, iIndex;
    for (iCount = 0 ; iCount < kiNumFiles ; ++iCount)
    {
        iIndex          = bRenameBackwards ? kiNumFiles-(iCount+1) : iCount;
        qstrCurrentName = rqstrlCurrentName.at(iIndex);
        qstrNewName     = rqstrlNewName.at(iIndex);

        if (bUndoOperation == false)
            RemoveInvalidTrailingCharacters(qstrNewName);

        if (bItermediateRename)
        {
            qstrIntermedName = QString("INV#%1#.%2").arg(uiIntermedNum, 8, 16, QChar('0')).arg(qstrNewName);
            --uiIntermedNum;
        }

        if (m_qdirDirReader.rename(qstrCurrentName, rqstrRenameName) == false)
        {
            if (preldRenameErrorsDialog == nullptr)
                preldRenameErrorsDialog = new IDlgRenameErrorList(this, true);

            QFileInfo qfiFileInfo;
            if (bUndoOperation == false)
                qfiFileInfo = m_pqtwNameCurrent->item(pqlstiRows->at(iIndex), 0)->data(FileInfo).value<QFileInfo>();
            preldRenameErrorsDialog->AddToErrorList(qstrCurrentName, qstrNewName, DetermineReasonForFailure(qstrCurrentName, qstrNewName, qfiFileInfo));
        }
        else
        {
            if (bUndoOperation == false)
            {
                m_pqtwNameCurrent->item(pqlstiRows->at(iIndex), 0)->setText(qstrNewName);
                m_qstrlUndoRenameFrom.push_back(rqstrRenameName);
                m_qstrlUndoRenameTo.push_back(qstrCurrentName);
            }
            else if (bItermediateRename)
            {
                qstrlIntermediateNames.push_back(rqstrRenameName);
            }
        }

        if (bItermediateRename == false)
            idprgRenameProgress.IncrementAndUpdateProgress();
        else if (iCount % 2 == 0)
            idprgRenameProgress.IncrementAndUpdateProgress();
    }

    if (bItermediateRename)
    {
        QStringList::iterator itName;
        QStringList & rqstrlIntermediateList = bUndoOperation ? qstrlIntermediateNames : m_qstrlUndoRenameFrom;
        for (itName = rqstrlIntermediateList.begin() ; itName != rqstrlIntermediateList.end() ; ++itName)
        {
            qstrNewName = (*itName).mid(14);
            if (m_qdirDirReader.rename(*itName, qstrNewName))
            {
                if (bUndoOperation == false)
                    *itName = qstrNewName;
            }

            if (iCount++ % 2 == 0)
                idprgRenameProgress.IncrementAndUpdateProgress();
        }
    }

    if (preldRenameErrorsDialog != nullptr)
        preldRenameErrorsDialog->ResizeColumnsAndShow();

    if (bUndoOperation || m_bReSortFileListAfterRename)
        RefreshDirectoryHard();
    else
        RefreshDirectoryPostRename();
}


void IUIFileList::RemoveInvalidTrailingCharacters(QString & rqstrFileName)
{
    // Trailing spaces can make files inaccessible on Windows.  On Linux and Mac traling spaces are allowed, but can cause problems.
    // On macOS "file.txt" is considered a text file, while "file.txt " is treated as an executable, so it's best to remove trailing spaes on all platforms.
    // Trailing dots are are also invlaid on Windows, and they again change the MIME type on Mac so we strip them off as well.
    int iPos = rqstrFileName.length() - 1;
    QChar qchLastChar = rqstrFileName.at(iPos);

    if (qchLastChar == ' ' || qchLastChar == '.')
    {
        do
            qchLastChar = rqstrFileName.at(--iPos);
        while (qchLastChar == ' ' || qchLastChar == '.');
        rqstrFileName = rqstrFileName.left(iPos+1);
    }
}


bool IUIFileList::RenameEndResultValid()
{
    int iRow;
    int iNumRows = m_pqtwNamePreview->rowCount();
    IDlgRenameErrorList* preldRenameErrorsDialog = nullptr;

    QHash<QString, int> qhshHash;
    qhshHash.reserve(static_cast<int>(iNumRows*1.1));

    QString qstrCompareName;
    for (iRow = 0 ; iRow < iNumRows ; ++iRow)
    {
        #ifdef Q_OS_WIN
        qstrCompareName = m_pqtwNamePreview->item(iRow, 0)->text().toLower();
        #else
        qstrCompareName = m_pqtwNamePreview->item(iRow, 0)->text();
        #endif

        if (qhshHash.contains(qstrCompareName))
        {
            if (preldRenameErrorsDialog == nullptr)
                preldRenameErrorsDialog = new IDlgRenameErrorList(IUIMainWindow::GetMainWindow()->GetFileListUI(), false);
            preldRenameErrorsDialog->AddToErrorList(m_pqtwNameCurrent->item(iRow, 0)->text(), m_pqtwNamePreview->item(iRow, 0)->text(), tr("Duplicate filename"));
        }
        qhshHash.insert(qstrCompareName, 0);
    }

    if (preldRenameErrorsDialog != nullptr)
    {
        preldRenameErrorsDialog->ResizeColumnsAndShow();
        return false;
    }
    return true;
}


bool IUIFileList::RenameProccessValid(QStringList & rqstrlCurrentName, QStringList & rqstrlNewName, const bool kbValidateBackwards)
{
    #ifdef Q_OS_WIN
    QString qstrCompareName;
    #endif

    QHash<QString, int> qhshHash;
    const int kiNumRows = m_pqtwNameCurrent->rowCount();
    qhshHash.reserve(static_cast<int>(kiNumRows*1.1));

    for (int iRow = 0 ; iRow < kiNumRows ; ++iRow)
    {
        #ifdef Q_OS_WIN
        qhshHash.insert(m_pqtwNameCurrent->item(iRow, 0)->text().toLower(), 0);
        #else
        qhshHash.insert(m_pqtwNameCurrent->item(iRow, 0)->text(), 0);
        #endif
    }

    int iIndex;
    const int kiStringCount = rqstrlCurrentName.size();
    for (int iCount = 0 ; iCount < kiStringCount ; ++iCount)
    {
        iIndex = kbValidateBackwards ? kiStringCount-(iCount+1) : iCount;

        #ifdef Q_OS_WIN
        qhshHash.remove(rqstrlCurrentName.at(iIndex).toLower());
        qstrCompareName = rqstrlNewName.at(iIndex).toLower();
        if (qhshHash.contains(qstrCompareName))
            return false;
        qhshHash.insert(qstrCompareName, 0);
        #else
        qhshHash.remove(rqstrlCurrentName.at(iIndex));
        if (qhshHash.contains(rqstrlNewName.at(iIndex)))
            return false;
        qhshHash.insert(rqstrlNewName.at(iIndex), 0);
        #endif
    }
    return true;
}


QString IUIFileList::DetermineReasonForFailure(const QString & krqstrCurrentName, const QString & krqstrFailedRename, const QFileInfo & krqfiFileInfo)
{
    if (m_qdirDirReader.exists(krqstrFailedRename))
    {
        return tr("File already exits");
    }
    else if (m_qdirDirReader.exists(krqstrCurrentName) == false)
    {
        return tr("File not found - May have been deleted/moved/renamed");
    }
    #ifndef Q_OS_WIN // Permissions check doesn't work on Windows and it says you have permission to write everything
    else if (krqfiFileInfo.exists() && krqfiFileInfo.permission(QFile::WriteUser) == false)
    {
        return tr("Insufficient privileges");
    }
    else
    {
        return tr("File open in another application?");
    }
    #else
    else
    {
        krqfiFileInfo.permission(QFile::WriteUser);
        return tr("File open in another application or insufficient privelages");
    }
    #endif
}


void IUIFileList::SetAutoRefreshEnabled(const bool kbAutoRefresh)
{
    if (m_bAutoRefresh != kbAutoRefresh)
    {
        m_bAutoRefresh = kbAutoRefresh;
        if (m_bAutoRefresh)
            RefreshDirectoryHard();
    }
}


int IUIFileList::GetCurrentRowHeight()
{
    return m_pqtwNameCurrent->verticalHeader()->defaultSectionSize();
}


void IUIFileList::SetUserDefinedRowHeight(const bool kbUseUserDefinedRowHeight, const int kiRowHeight)
{
    // To explain why this is implemented in a somewhat unusual way
    // The row height in the Preferences dialog is always initialised to the current row height so if the font has changed the row height will change
    // We therefore have to assess the changes as below to determine if the row height settings were changed by the user or if they changed becuase the font changed

    bool bChange = false;

    if (kbUseUserDefinedRowHeight != m_bUseUserDefinedRowHeight)
    {
        m_bUseUserDefinedRowHeight = kbUseUserDefinedRowHeight;
        bChange = true;
    }

    if (kbUseUserDefinedRowHeight && kiRowHeight != m_iUserDefinedRowHeight)
    {
        m_iUserDefinedRowHeight = kiRowHeight;
        bChange = true;
    }

    if (bChange)
        SetRowHeightAndIconSize();
}


void IUIFileList::SetUseAlternativeFont(const bool kbUseAlternativeFont, const QFont & krqfntSelectedFont)
{
    if (kbUseAlternativeFont != m_bUseAlternativeFont || krqfntSelectedFont != m_pqtwNameCurrent->font())
    {
        m_bUseAlternativeFont = kbUseAlternativeFont;
        if (kbUseAlternativeFont == false)
            RestoreDefaultFonts();
        else
            SetTableFont(krqfntSelectedFont);
    }
}


/* There's a QTableWidget::resizeColumnsToContents() function, but it leaves a lot of padding.
 * I've tried to remove the padding with the below, but it doesn't reduce the padding.
 *      rpqtwTable->setStyleSheet("QTableWidget::item { padding: 0px; }");
 *      rpqtwTable->setStyleSheet("QTableWidget::item { padding: 0px; border: 0px; margin: 0px; }");
 * Using table->verticalHeader()->setDefaultSectionSize() seems to be the only way to remove the excessive white space in each row.
 *
 * There are similar problem with excessive padding between the icon and the file name that makes it look like there's a space at the start of the file name.
 * I tried the below, but once again it doesn't reduce the padding.
 *      rpqtwTable->setStyleSheet("QTableWidget::icon { padding: 0px; }");
 *      rpqtwTable->setStyleSheet("QTableWidget::icon { padding: 0px; border: 0px; margin: 0px; }");
 * I hate excessive padding and white space, but Qt doesn't give you much control over it. */
void  IUIFileList::SetRowHeightAndIconSize()
{
    int iRowHeight;
    if (m_bUseUserDefinedRowHeight)
        iRowHeight = m_iUserDefinedRowHeight;
    else
        iRowHeight = IComUtilityFuncs::GetTableRowHeightFitToFont(m_pqtwNameCurrent);

    if (iRowHeight == m_pqtwNameCurrent->verticalHeader()->defaultSectionSize())
        return;

    m_pqtwNameCurrent->verticalHeader()->setDefaultSectionSize(iRowHeight);
    m_pqtwNamePreview->verticalHeader()->setDefaultSectionSize(iRowHeight);

    int iIconSize = iRowHeight - (iRowHeight%16);
    QSize qsizIconSize(iIconSize, iIconSize);
    m_pqtwNameCurrent->setIconSize(qsizIconSize);
    m_pqtwNamePreview->setIconSize(qsizIconSize);
}


void IUIFileList::SetHighlightSettings(const bool kbNameChangeColourText, const QColor & krqcolNameChangeTextColour, const bool kbNameChangeHighlightRow, const QColor & krqcolNameChangeHighlightColour)
{
    if (kbNameChangeColourText   != m_bNameChangeColourText   || krqcolNameChangeTextColour      != m_qcolNameChangeTextColour ||
        kbNameChangeHighlightRow != m_bNameChangeHighlightRow || krqcolNameChangeHighlightColour != m_qcolNameChangeHighlightColour)
    {
        m_bNameChangeColourText         = kbNameChangeColourText;
        m_qcolNameChangeTextColour      = krqcolNameChangeTextColour;
        m_bNameChangeHighlightRow       = kbNameChangeHighlightRow;
        m_qcolNameChangeHighlightColour = krqcolNameChangeHighlightColour;

        HighlightRowsWithModifiedNames(true);
    }
}


const QFont & IUIFileList::GetCurrentFont()
{
    return m_pqtwNameCurrent->font();
}


void IUIFileList::RestoreDefaultFonts()
{
    SetTableFont(m_qfntDefaultFont);
}


void IUIFileList::SetTableFont(const QFont & krqfntNewFont)
{
    if (krqfntNewFont != m_pqtwNameCurrent->font())
    {
        m_pqtwNameCurrent->setFont(krqfntNewFont);
        m_pqtwNamePreview->setFont(krqfntNewFont);
        if (m_bUseUserDefinedRowHeight == false)
            SetRowHeightAndIconSize();
    }
}


bool IUIFileList::SetExifAdvancedMode(const bool kbExifAdvancedMode)
{
    if (kbExifAdvancedMode != m_bExifAdvancedMode)
    {
        m_bExifAdvancedMode = kbExifAdvancedMode;
        m_pmwMainWindow->GetMenuBar()->TagsMenu()->SetExifMode(kbExifAdvancedMode);
        m_rpuirRenameUI->SetExifAdvancedMode(m_bExifAdvancedMode);
        m_rpuirRenameUI->ReReadTagCodes();
        return true;
    }
    return false;
}


void IUIFileList::keyPressEvent(QKeyEvent* pqkeEvent)
{
    if (pqkeEvent->key() == Qt::Key_F2)
        OpenRenameFileDlgForCurrentRow();
    else
        pqkeEvent->ignore();
}


void CurrentTableHighlightDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if(option.state & QStyle::State_Selected)
    {
        // Always use ACTIVE widget colour on the Current Name table, even if it doesn't have focus
        QStyleOptionViewItem opt = option;
        opt.palette.setColor(QPalette::HighlightedText, qApp->palette().color(QPalette::HighlightedText));
        opt.palette.setColor(QPalette::Highlight,       qApp->palette().color(QPalette::Highlight));
        QStyledItemDelegate::paint(painter, opt, index);
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}


void PreviewTableHighlightDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if(option.state & QStyle::State_Selected)
    {
        // Always use INACTIVE widget colour on the Preview table, even if it has focus, and override with name changed highlight colour when necessary
        QStyleOptionViewItem opt = option;
        bool bNameChanged = m_puifmFileList->m_pqtwNameCurrent->item(index.row(), 0)->text() != m_puifmFileList->m_pqtwNamePreview->item(index.row(), 0)->text();

        if (bNameChanged && m_puifmFileList->m_bNameChangeColourText)
            opt.palette.setColor(QPalette::HighlightedText, m_puifmFileList->m_qcolNameChangeTextColour);
        else
            opt.palette.setColor(QPalette::HighlightedText, qApp->palette().color(QPalette::Inactive, QPalette::HighlightedText));

        if (bNameChanged && m_puifmFileList->m_bNameChangeHighlightRow)
            opt.palette.setColor(QPalette::Highlight, m_puifmFileList->m_qcolNameChangeHighlightColour);
        else
            opt.palette.setColor(QPalette::Highlight, qApp->palette().color(QPalette::Inactive, QPalette::Highlight));

        QStyledItemDelegate::paint(painter, opt, index);
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}
