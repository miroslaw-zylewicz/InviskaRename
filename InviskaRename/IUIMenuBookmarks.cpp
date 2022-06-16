#include <QtWidgets>
#include "IUIMenuBookmarks.h"
#include "IUIMainWindow.h"
#include "IUIFileList.h"
#include "IDlgSaveAction.h"
#include "IDlgOrganiseMenu.h"


IUIMenuBookmarks::IUIMenuBookmarks(IUIMainWindow* pmwMainWindow) : IUIMenuSavesBase(tr("&Bookmarks"), pmwMainWindow)
{
    m_bSetToolTipToDataPath = true;
    setToolTipsVisible(true);

    m_qstrGroup = "Bookmarks";
    m_qstrArray = "SavedBookmarks";
    m_qstrEntry = "Bookmark";
}


IUIMenuBookmarks::~IUIMenuBookmarks()
{
    SaveActionsToQSettings();
}


void IUIMenuBookmarks::CreateActionsBookmarks()
{
    m_pqactBookmarkFolder = new QAction(tr("Book&mark Folder..."), this);
    m_pqactBookmarkFolder->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_DialogSaveButton));
    m_pqactBookmarkFolder->setShortcut(Qt::CTRL + Qt::Key_D); //tr("Ctrl+D"));
    QObject::connect(m_pqactBookmarkFolder, SIGNAL(triggered()), this, SLOT(ShowSaveBookmarkDialog()));

    m_pqactOrganiseBookmars = new QAction(tr("&Organise Bookmarks..."), this);
    m_pqactOrganiseBookmars->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_DirHomeIcon));
    m_pqactOrganiseBookmars->setShortcut(Qt::CTRL + Qt::Key_B); //tr("Ctrl+B"));
    QObject::connect(m_pqactOrganiseBookmars, SIGNAL(triggered()), this, SLOT(ShowOrganiseBookmarksDialog()));

    InitialiseMenu();
    LoadActionsFromQSettings();
}


void IUIMenuBookmarks::InitialiseMenu()
{
    addAction(m_pqactBookmarkFolder);
    addAction(m_pqactOrganiseBookmars);
    addSeparator();
}


QString IUIMenuBookmarks::GetDataString(const QString & krqstrSaveString)
{
    int iPathStart = krqstrSaveString.indexOf("<Path>") + 6;
    int iPathEnd = krqstrSaveString.indexOf("</Path>", iPathStart);
    return krqstrSaveString.mid(iPathStart, iPathEnd-iPathStart);
}


QString IUIMenuBookmarks::GetActionSaveString(const QAction* const kpactAction)
{
    return QString("<Name>%1</Name><Path>%2</Path>").arg(kpactAction->whatsThis()).arg(kpactAction->data().toString());
}


void IUIMenuBookmarks::SaveAction(const QString & krqstrName)
{
    CreateAction(krqstrName, m_pmwMainWindow->GetFileListUI()->CurrentPath());
}


void IUIMenuBookmarks::UpdateAction(QAction* pqactAction, const QString & krqstrName)
{
    UpdateActionName(pqactAction, krqstrName);

    QString qstrPath = m_pmwMainWindow->GetFileListUI()->CurrentPath();
    pqactAction->setData(qstrPath);
    pqactAction->setToolTip(QDir::toNativeSeparators(qstrPath));
}


void IUIMenuBookmarks::LoadData()
{
    m_pmwMainWindow->GetFileListUI()->OpenDirectory(qobject_cast<QAction*>(sender())->data().toString());
}


void IUIMenuBookmarks::ShowSaveBookmarkDialog()
{
    QString qstrDirectory = m_pmwMainWindow->GetFileListUI()->CurrentDirectory();
    if (qstrDirectory.isEmpty())
        qstrDirectory = QDir::toNativeSeparators(m_pmwMainWindow->GetFileListUI()->CurrentPath());

    new IDlgSaveAction(this, tr("Bookmark Folder"), tr("bookmark"), qstrDirectory);
}


void IUIMenuBookmarks::ShowOrganiseBookmarksDialog()
{
    new IDlgOrganiseMenu(this, tr("Organise Bookmarks"), IDlgOrganiseMenu::Bookmarks);
}
