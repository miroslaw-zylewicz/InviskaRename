#include <QtWidgets>
#include <QAction>
#include "IUIMenuBar.h"
#include "IUIMenuTags.h"
#include "IUIMenuRenames.h"
#include "IUIMenuBookmarks.h"
#include "IUIMainWindow.h"
#include "IUIFileList.h"
#include "IUIRename.h"
#include "IUISideBar.h"


IUIMenuBar::IUIMenuBar(IUIMainWindow* pmwMainWindow) : IComUIMenuBarBase(pmwMainWindow)
{
    m_pimenuTags = new IUIMenuTags(pmwMainWindow);
    m_pimenuRenames = new IUIMenuRenames(pmwMainWindow);
    m_pimenuBookmarks = new IUIMenuBookmarks(pmwMainWindow);

    CreateActions();
    InitialiseMenuBar();
}


void IUIMenuBar::CreateActions()
{
    CreateActionsFile();
    CreateActionsNavigate();
    m_pimenuTags->CreateActionsTags();
    m_pimenuRenames->CreateActionsRenames();
    m_pimenuBookmarks->CreateActionsBookmarks();
    IComUIMenuBarBase::CreateActions();
}


void IUIMenuBar::CreateActionsFile()
{
    IUIFileList* puifmFilelist = m_pmwMainWindow->GetFileListUI();
    IUIRename*   puirRenameUI = m_pmwMainWindow->GetRenameUI();

    m_pqactOpenDirectory = new QAction(tr("&Open Directory..."), m_pmwMainWindow);
    m_pqactOpenDirectory->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_DirOpenIcon));
    m_pqactOpenDirectory->setShortcut(QKeySequence::Open);
    QObject::connect(m_pqactOpenDirectory, SIGNAL(triggered()), puifmFilelist, SLOT(OpenDirectoryDialog()));

    m_pqactViewFileMetaTags = new QAction(tr("&View File Properties && Tags..."), m_pmwMainWindow);
    m_pqactViewFileMetaTags->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_FileDialogContentsView));
    QObject::connect(m_pqactViewFileMetaTags, SIGNAL(triggered()), puifmFilelist, SLOT(OpenFilePropertiesDialog()));

    m_pqactCaseSensitive = new QAction(tr("&Case Sensitive Comparisons"), m_pmwMainWindow);
    m_pqactCaseSensitive->setCheckable(true);
    m_pqactCaseSensitive->setChecked(puirRenameUI->CaseSensitive());
    m_pqactCaseSensitive->setIcon(QIcon(":/Resources/CaseSensitive.png"));
    m_pqactCaseSensitive->setToolTip(tr("Enable/disable case-sensitive comparisons.\nApplies to \"Replace The Text\", \"Files With Extensions\" and Regular Expressions."));
    QObject::connect(m_pqactCaseSensitive, SIGNAL(toggled(bool)), puirRenameUI, SLOT(SetCaseSensitivity(bool)));
}


void IUIMenuBar::CreateActionsNavigate()
{
    IUIFileList* puifmFilelist = m_pmwMainWindow->GetFileListUI();

    m_pqactNavigateBack = new QAction(tr("&Back"), m_pmwMainWindow);
    m_pqactNavigateBack->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_ArrowLeft));
    m_pqactNavigateBack->setShortcut(QKeySequence::Back);
    QObject::connect(m_pqactNavigateBack, SIGNAL(triggered()), puifmFilelist, SLOT(NavigateBack()));

    m_pqactNavigateForward = new QAction(tr("&Forward"), m_pmwMainWindow);
    m_pqactNavigateForward->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_ArrowRight));
    m_pqactNavigateForward->setShortcut(QKeySequence::Forward);
    QObject::connect(m_pqactNavigateForward, SIGNAL(triggered()), puifmFilelist, SLOT(NavigateForward()));

    m_pqactNavigateUp = new QAction(tr("&Up"), m_pmwMainWindow);
    m_pqactNavigateUp->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_ArrowUp));
    m_pqactNavigateUp->setShortcut(Qt::ALT + Qt::Key_Up);
    QObject::connect(m_pqactNavigateUp, SIGNAL(triggered()), puifmFilelist, SLOT(NavigateUp()));

    m_pqactNavigateRefresh = new QAction(tr("&Refresh"), m_pmwMainWindow);
    m_pqactNavigateRefresh->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_BrowserReload));
    m_pqactNavigateRefresh->setShortcut(QKeySequence::Refresh);
    QObject::connect(m_pqactNavigateRefresh, SIGNAL(triggered()), puifmFilelist, SLOT(NavigateRefresh()));

    m_pqactNavigateStartDir = new QAction(tr("Go To &Start Directory"), m_pmwMainWindow);
    m_pqactNavigateStartDir->setIcon(QIcon(":/Resources/Home.png"));
    m_pqactNavigateStartDir->setShortcut(Qt::ALT + Qt::Key_Home);
    QObject::connect(m_pqactNavigateStartDir, SIGNAL(triggered()), puifmFilelist, SLOT(NavigateToStartDirectory()));

    m_pqactShowHiddenFiles = new QAction(tr("Show &Hidden Files"), m_pmwMainWindow);
    m_pqactShowHiddenFiles->setCheckable(true);
    m_pqactShowHiddenFiles->setChecked(m_pmwMainWindow->GetSettings().value("FileList/ShowHiddenFiles", false).toBool());
    m_pqactShowHiddenFiles->setIcon(QIcon(":/Resources/ShowHidden.png"));
    m_pqactShowHiddenFiles->setShortcut(Qt::CTRL + Qt::Key_H); //tr("Ctrl+H"));
    m_pqactShowHiddenFiles->setToolTip(tr("Show/hide hidden files"));
    QObject::connect(m_pqactShowHiddenFiles, SIGNAL(triggered()), puifmFilelist, SLOT(SetHiddenFileState()));

    m_pqactMoveSelectedLinesUp = new QAction(tr("Move Selected Lines Up"), this);
    m_pqactMoveSelectedLinesUp->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_ArrowUp));
    m_pqactMoveSelectedLinesUp->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Up);
    QObject::connect(m_pqactMoveSelectedLinesUp, SIGNAL(triggered()), puifmFilelist, SLOT(MoveSelectionUp()));

    m_pqactMoveSelectedLinesDown = new QAction(tr("Move Selected Lines Down"), this);
    m_pqactMoveSelectedLinesDown->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_ArrowDown));
    m_pqactMoveSelectedLinesDown->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Down);
    QObject::connect(m_pqactMoveSelectedLinesDown, SIGNAL(triggered()), puifmFilelist, SLOT(MoveSelectionDown()));
}


void IUIMenuBar::InitialiseMenuBar()
{  
    m_pqmenuFile = m_pmwMainWindow->menuBar()->addMenu(tr("&File"));
    m_pqmenuFile->addAction(m_pqactOpenDirectory);
    m_pqmenuFile->addAction(m_pqactViewFileMetaTags);
    m_pqmenuFile->addAction(m_pqactPreferences);
    m_pqmenuFile->addAction(m_pqactCaseSensitive);
    m_pqmenuFile->addSeparator();
    m_pqmenuFile->addAction(m_pqactExit);

    m_pqmenuNavigate = m_pmwMainWindow->menuBar()->addMenu(tr("&Navigation"));
    m_pqmenuNavigate->addAction(m_pqactNavigateStartDir);
    m_pqmenuNavigate->addAction(m_pqactNavigateBack);
    m_pqmenuNavigate->addAction(m_pqactNavigateForward);
    m_pqmenuNavigate->addAction(m_pqactNavigateUp);
    m_pqmenuNavigate->addAction(m_pqactNavigateRefresh);
    m_pqmenuNavigate->addSeparator();
    m_pqmenuNavigate->addAction(m_pqactShowHiddenFiles);
    m_pqmenuNavigate->addSeparator();
    m_pqmenuNavigate->addAction(m_pqactMoveSelectedLinesUp);
    m_pqmenuNavigate->addAction(m_pqactMoveSelectedLinesDown);

    m_pmwMainWindow->menuBar()->addMenu(m_pimenuTags);
    m_pmwMainWindow->menuBar()->addMenu(m_pimenuBookmarks);
    m_pmwMainWindow->menuBar()->addMenu(m_pimenuRenames);

    IComUIMenuBarBase::InitialiseMenuBar();
}


void IUIMenuBar::AddNavigationActions(QToolBar* pqtpNavigationToolBar)
{
    pqtpNavigationToolBar->addAction(m_pqactNavigateBack);
    pqtpNavigationToolBar->addAction(m_pqactNavigateForward);
    pqtpNavigationToolBar->addAction(m_pqactNavigateUp);
    pqtpNavigationToolBar->addAction(m_pqactNavigateStartDir);
    pqtpNavigationToolBar->addAction(m_pqactNavigateRefresh);

}


void IUIMenuBar::AddPreferencesActions(QToolBar* pqtpNavigationToolBar)
{   
    pqtpNavigationToolBar->addAction(m_pqactShowHiddenFiles);
    pqtpNavigationToolBar->addAction(m_pqactCaseSensitive);
    pqtpNavigationToolBar->addAction(m_pqactPreferences);
}


void IUIMenuBar::AddSideBarActions(IUISideBar* puisbSideBar)
{
    puisbSideBar->addAction(m_pqactMoveSelectedLinesUp);
    puisbSideBar->addAction(m_pqactMoveSelectedLinesDown);
}


void IUIMenuBar::EnableBackAction(const bool kbEnable)
{
    m_pqactNavigateBack->setEnabled(kbEnable);
}


void IUIMenuBar::EnableForwardAction(const bool kbEnable)
{
    m_pqactNavigateForward->setEnabled(kbEnable);
}


void IUIMenuBar::EnableUpAction(const bool kbEnable)
{
    m_pqactNavigateUp->setEnabled(kbEnable);
}


bool IUIMenuBar::ShowHiddenFiles()
{
    return m_pqactShowHiddenFiles->isChecked();
}


bool IUIMenuBar::CaseSensitive()
{
    return m_pqactCaseSensitive->isChecked();
}


void IUIMenuBar::SetCaseSensitive(const bool kbCaseSensitive)
{
    if (kbCaseSensitive != m_pqactCaseSensitive->isChecked())
        m_pqactCaseSensitive->setChecked(kbCaseSensitive);
}


QString IUIMenuBar::GetCreditsTranslations()
{
    return "Polski - Mirosław Żylewicz";
    return "Português Brasileiro - Claudio Mantovani Vieira";
}


QString IUIMenuBar::GetCreditsDependency()
{
    QString qstrTagLib = QString("<a href=\"https://taglib.org/\">TagLib</a> - %1").arg(tr("Used for reading music tags."));
    QString qstrLibExif = QString("<a href=\"https://libexif.github.io/\">LibExif</a> - %1").arg(tr("Used for reading Exif tags."));

    return QString("%1<br>%2").arg(qstrTagLib).arg(qstrLibExif);
}
