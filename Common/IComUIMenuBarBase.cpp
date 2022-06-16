#include <QtWidgets>
#include <QAction>
#include "IComUIMenuBarBase.h"
#include "IComDlgHelpAbout.h"
#include "IUIMainWindow.h"
#include "IDlgPreferences.h"


IComUIMenuBarBase::IComUIMenuBarBase(IUIMainWindow* pmwMainWindow) : QObject(pmwMainWindow)
{
    m_pmwMainWindow = pmwMainWindow;
}


void IComUIMenuBarBase::CreateActions()
{
    m_pqactPreferences = new QAction(tr("&Preferences..."), m_pmwMainWindow);
    m_pqactPreferences->setIcon(QIcon(":/Resources/Settings.png"));
    m_pqactPreferences->setMenuRole(QAction::PreferencesRole);
    m_pqactPreferences->setShortcut(QKeySequence::Preferences);
    m_pqactPreferences->setToolTip(tr("Change application settings"));
    QObject::connect(m_pqactPreferences, SIGNAL(triggered()), this, SLOT(ShowPreferencesDialog()));

    m_pqactExit = new QAction(tr("E&xit"), m_pmwMainWindow);
    m_pqactExit->setMenuRole(QAction::QuitRole);
    m_pqactExit->setShortcut(QKeySequence::Quit);
    QObject::connect(m_pqactExit, SIGNAL(triggered()), m_pmwMainWindow, SLOT(close()));


    // Help menu actions
    m_pqactInstructions = new QAction(tr("&Instructions"), m_pmwMainWindow);
    m_pqactInstructions->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_MessageBoxInformation));
    QObject::connect(m_pqactInstructions, SIGNAL(triggered()), this, SLOT(ShowInstructions()));

    m_pqactForum = new QAction(tr("&Forum"), m_pmwMainWindow);
    m_pqactForum->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_MessageBoxQuestion));
    QObject::connect(m_pqactForum, SIGNAL(triggered()), this, SLOT(ShowForum()));

    m_pqactWebsite = new QAction(tr("&Website"), m_pmwMainWindow);
    m_pqactWebsite->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_DesktopIcon));
    QObject::connect(m_pqactWebsite, SIGNAL(triggered()), this, SLOT(ShowWebsite()));

    m_pqactChangelog = new QAction(tr("&Changelog"), m_pmwMainWindow);
    m_pqactChangelog->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_DialogHelpButton));
    QObject::connect(m_pqactChangelog, SIGNAL(triggered()), this, SLOT(ShowChangelog()));

    m_pqactHelpAbout = new QAction(tr("&About"), m_pmwMainWindow);
    m_pqactHelpAbout->setMenuRole(QAction::AboutRole);
    QObject::connect(m_pqactHelpAbout, SIGNAL(triggered()), this, SLOT(ShowHelpAboutDialog()));
}


void IComUIMenuBarBase::InitialiseMenuBar()
{
    m_pqmenuHelp = m_pmwMainWindow->menuBar()->addMenu(tr("&Help"));
    m_pqmenuHelp->addAction(m_pqactInstructions);
    m_pqmenuHelp->addAction(m_pqactForum);
    m_pqmenuHelp->addAction(m_pqactWebsite);
    m_pqmenuHelp->addAction(m_pqactChangelog);
    m_pqmenuHelp->addSeparator();
    m_pqmenuHelp->addAction(m_pqactHelpAbout);
}


void IComUIMenuBarBase::ShowPreferencesDialog()
{
    new IDlgPreferences(m_pmwMainWindow);
}


void IComUIMenuBarBase::ShowInstructions()
{
    QDesktopServices::openUrl(QUrl(m_pmwMainWindow->GetInstructionsAddress()));
}


void IComUIMenuBarBase::ShowWebsite()
{
    QDesktopServices::openUrl(QUrl(m_pmwMainWindow->GetWebsite()));
}


void IComUIMenuBarBase::ShowForum()
{
    QDesktopServices::openUrl(QUrl(m_pmwMainWindow->GetForumAddress()));
}


void IComUIMenuBarBase::ShowChangelog()
{
    QDesktopServices::openUrl(QUrl(m_pmwMainWindow->GetChangelogAddress()));
}


void IComUIMenuBarBase::ShowHelpAboutDialog()
{
    new IComDlgHelpAbout(m_pmwMainWindow);
}
