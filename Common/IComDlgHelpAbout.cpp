#include <QtWidgets>
#include "IComDlgHelpAbout.h"
#include "IUIMainWindow.h"
#include "IUIMenuBar.h"


IComDlgHelpAbout::IComDlgHelpAbout(IUIMainWindow* pmwMainWindow) : QDialog(pmwMainWindow)
{
    m_pmwMainWindow = pmwMainWindow;

    setWindowTitle(QString("%1 %2").arg(tr("About")).arg(m_pmwMainWindow->GetAppName()));
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QPixmap qpixmTitleImage(":/Resources/HelpAboutBanner.png");
    QLabel* pqlblTitleBanner = new QLabel;
    pqlblTitleBanner->setPixmap(qpixmTitleImage);
    pqlblTitleBanner->setAlignment(Qt::AlignHCenter);

    m_pqtwTabWidget = new QTabWidget(this);
    m_pqtwTabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    CreateTabGeneral();
    CreateTab(tr("Translations"), tr("Translation Credits"), m_pmwMainWindow->GetMenuBar()->GetCreditsTranslations());
    CreateTab(tr("Contributions"), tr("Code Contribution Credits"), m_pmwMainWindow->GetMenuBar()->GetCreditsCodeContrib());
    CreateTab(tr("Libraries", "Software libraries"), tr("Library and Dependency Credits", "Credits for other software this program relies on to run"), m_pmwMainWindow->GetMenuBar()->GetCreditsDependency());
    //CreateTabLibraries();
    CreateTabLicence();

    m_pqdbbOK = new QDialogButtonBox(QDialogButtonBox::Ok);
    m_pqdbbOK->setOrientation(Qt::Horizontal);
    connect(m_pqdbbOK, SIGNAL(accepted()), this, SLOT(accept()));

    m_pqpbAboutQt = new QPushButton(tr("About Qt"));
    connect(m_pqpbAboutQt, SIGNAL(clicked()), this, SLOT(ShowAboutQt()));

    QHBoxLayout* phblButtonLayout = new QHBoxLayout;
    phblButtonLayout->addWidget(m_pqpbAboutQt);
    phblButtonLayout->addWidget(m_pqdbbOK);

    QVBoxLayout* pqvblLayout = new QVBoxLayout;
    pqvblLayout->addWidget(pqlblTitleBanner);
    pqvblLayout->addWidget(m_pqtwTabWidget);
    pqvblLayout->addLayout(phblButtonLayout);

    setLayout(pqvblLayout);
    resize(sizeHint());
    show();
}


void IComDlgHelpAbout::CreateTabGeneral()
{
    QString qstrCurrentVersion = tr("Current version:");
    QString qstrLatestVersion = tr("Latest version:");
    QString qstrLicence = tr("%1 is released under the %2 licence.", "Example: Inviska Rename is released under the GPLv2 licence.").arg(m_pmwMainWindow->GetAppName()).arg("<a href=\"http://www.gnu.org/licenses/gpl-2.0.html\">GPLv2</a>+");
    QString qstrDontSueMe = tr("It is provided as is with no warranty of any kind, including the <br>warranty of design, merchantability and fitness for a particular purpose.");
    QString qstrForum = tr("If you require assistance, or wish to report a bug, please visit the <a href='%1'>forum</a>.").arg(m_pmwMainWindow->GetForumAddress());

    QString qstrInfo = QString("<center><h2>%1</h2>"
                               "<p>%2 %3<br>%4 %5</p>"
                               "<a href='%6'>%6</a></p>"
                               "<p>%7</p>"
                               "<p>%8</p>"
                               "<p>%9</p>"
                               "<br></center>")
                               .arg(m_pmwMainWindow->GetAppName())
                               .arg(qstrCurrentVersion)
                               .arg(m_pmwMainWindow->GetVersion())
                               .arg(qstrLatestVersion)
                               .arg(m_pmwMainWindow->GetVersionInfo()->GetLatestVersion())
                               .arg(m_pmwMainWindow->GetWebsite())
                               .arg(qstrLicence)
                               .arg(qstrDontSueMe)
                               .arg(qstrForum);

    QLabel* pqlblInfo = new QLabel(qstrInfo);
    pqlblInfo->setTextFormat(Qt::RichText);
    pqlblInfo->setOpenExternalLinks(true);

    QLabel* pqlblMsg = nullptr;
    IComSysLatestVersion* pilvMsg = m_pmwMainWindow->GetVersionInfo();
    if (pilvMsg->MessageAvailable())
    {
        pqlblMsg = new QLabel("<center><h1>" + pilvMsg->GetMessageTitle() + "</h1>" + pilvMsg->GetMessageBody() + "<p></p></center>");
        pqlblMsg->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        pqlblMsg->setOpenExternalLinks(true);
    }

    QVBoxLayout* pqvblLayout = new QVBoxLayout;
    pqvblLayout->addWidget(pqlblInfo);
    if (pqlblMsg != nullptr)
        pqvblLayout->addWidget(pqlblMsg);
    pqvblLayout->addStretch();

    QWidget* pqwTab = new QWidget(m_pqtwTabWidget);
    pqwTab->setLayout(pqvblLayout);
    m_pqtwTabWidget->addTab(pqwTab, tr("About"));
}


void IComDlgHelpAbout::CreateTab(const QString & krqstrTabTitle, const QString & krqstrTitle, const QString & krqstrCredits)
{
    if (krqstrCredits.isEmpty())
        return;

    QLabel* pqlblCredits = new QLabel(QString("<center><h2>%1</h2><p>%2</p></center>").arg(krqstrTitle).arg(krqstrCredits));
    pqlblCredits->setTextFormat(Qt::RichText);
    pqlblCredits->setOpenExternalLinks(true);

    QVBoxLayout* pqvblLayout = new QVBoxLayout;
    pqvblLayout->addWidget(pqlblCredits);
    pqvblLayout->addStretch();

    QWidget* pqwTab = new QWidget(m_pqtwTabWidget);
    pqwTab->setLayout(pqvblLayout);
    m_pqtwTabWidget->addTab(pqwTab, krqstrTabTitle);
}


void IComDlgHelpAbout::CreateTabLibraries()
{
    QString qstrQt = QString("<a href=\"https://www.qt.io/\">Qt %1</a>").arg(QT_VERSION_STR);
    QString qstrCredits = m_pmwMainWindow->GetMenuBar()->GetCreditsDependency();

    if (qstrCredits.isEmpty())
        qstrCredits = qstrQt;
    else
        qstrCredits.append(QString("<br><br>%1").arg(qstrQt));

    CreateTab(tr("Libraries", "Software libraries"), tr("Library and Dependency Credits", "Credits for other software this program relies on to run"), qstrCredits);
}


void IComDlgHelpAbout::CreateTabLicence()
{
    QFile qfilLicence(":/Resources/Licence.txt");
    if (!qfilLicence.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream qtsrmIn(&qfilLicence);

    const QFont qfntMonoFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    QTextEdit* pqtedTextEdit =  new QTextEdit(m_pqtwTabWidget);
    pqtedTextEdit->setReadOnly(true);
    pqtedTextEdit->setFont(qfntMonoFont);
    pqtedTextEdit->setLineWrapMode(QTextEdit::NoWrap);

    QString strLine = qtsrmIn.readLine();
    while (!strLine.isNull())
    {
        pqtedTextEdit->append(strLine);
        strLine = qtsrmIn.readLine();
    }
    qfilLicence.close();

    pqtedTextEdit->moveCursor(QTextCursor::Start);
    pqtedTextEdit->ensureCursorVisible();

    QVBoxLayout* pqvblLayout = new QVBoxLayout;
    pqvblLayout->addWidget(pqtedTextEdit);
    pqvblLayout->setContentsMargins(0, 0, 0, 0);

    QWidget* pqwTab = new QWidget(m_pqtwTabWidget);
    pqwTab->setLayout(pqvblLayout);
    m_pqtwTabWidget->addTab(pqwTab, tr("Licence", "Licence the software is released under"));
}


void IComDlgHelpAbout::ShowAboutQt()
{
    QMessageBox::aboutQt(this);
}
