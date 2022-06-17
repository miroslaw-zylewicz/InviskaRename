#include <QtWidgets>
#include <QSettings>
#include "IUIMainWindow.h"
#include "IUIFileList.h"
#include "IUIMenuBar.h"
#include "IUIToolBar.h"
#include "IUISideBar.h"
#include "IUIRename.h"

IUIMainWindow* IUIMainWindow::m_spmwMainWindow = nullptr;


IUIMainWindow::IUIMainWindow(QSettings & rqsetSettings, IComSysSingleInstance & rsnglSingleInstance) : IComUIMainWinBase(rqsetSettings, rsnglSingleInstance)
{
    IUIMainWindow::m_spmwMainWindow = this;
    CreateRegExpValidtors();

    QWidget* pqwCentralWidget = new QWidget(this);
    m_puifmFileList = new IUIFileList(this);
    m_puirRenameUI = new IUIRename(this);

    m_puimbMenuBar = new IUIMenuBar(this);
    m_puitbToolBar = new IUIToolBar(this);
    m_puisbSideBar = new IUISideBar(this);

    QHBoxLayout* pqhblTopLayout = new QHBoxLayout;
    pqhblTopLayout->setContentsMargins(0, 0, 0, 0);
    pqhblTopLayout->setSpacing(0);
    pqhblTopLayout->addWidget(m_puisbSideBar);
    pqhblTopLayout->addWidget(m_puifmFileList);
    m_puisbSideBar->setVisible(rqsetSettings.value("FileList/ShowSideBar", false).toBool());

    QVBoxLayout* pvblMainWinLayout = new QVBoxLayout(pqwCentralWidget);
    pvblMainWinLayout->setContentsMargins(0, 0, 0, 0);
    pvblMainWinLayout->addLayout(pqhblTopLayout);
    pvblMainWinLayout->addWidget(m_puirRenameUI);
    pqwCentralWidget->setLayout(pvblMainWinLayout);
    setCentralWidget(pqwCentralWidget);

    m_puifmFileList->InitAfterCreate();

    QTimer::singleShot(0, this, SLOT(FocusAddressBar()));
}


void IUIMainWindow::CreateRegExpValidtors()
{
    #ifdef Q_OS_WIN
    m_pqrevInvalidCharValidator = new QRegExpValidator(QRegExp("^[^\\\\/:*?\"<>|]*$"), this);     // Windows    \/:*?"<>|
    #else
    m_pqrevInvalidCharValidator = new QRegExpValidator(QRegExp("^[^/]*$"), this);                 // Linux+Mac  /
    #endif

    m_pivalIntOnlyValidator = new QIntValidator(this);
}



void IUIMainWindow::SaveSettings()
{
    m_puifmFileList->SaveSessionSettings();

    IComUIMainWinBase::SaveSettings();
}


void IUIMainWindow::FocusAddressBar()
{
    m_puitbToolBar->FocusAddressBar();
}


void IUIMainWindow::SetSideBarEnabled(const bool kbEnabled)
{
    if (m_puisbSideBar->isVisible() != kbEnabled)
        m_puisbSideBar->setVisible(kbEnabled);
}


bool IUIMainWindow::SideBarEnabled()
{
    return m_puisbSideBar->isVisible();
}


void IUIMainWindow::PopulateLanguageList(QComboBox* pqcboLanguages)
{
    pqcboLanguages->addItem(tr("Default"), "");
    pqcboLanguages->addItem("English", "en_GB");
    pqcboLanguages->addItem("Polski", "pl_PL");
    pqcboLanguages->addItem("Português Brasileiro", "pt_BR");
}

