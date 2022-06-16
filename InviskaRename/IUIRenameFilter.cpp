#include <QtWidgets>
#include "IUIRenameFilter.h"
#include "IUIRename.h"
#include "IUIFileList.h"
#include "IUIMainWindow.h"
#include "IComQLineEdit.h"


IUIRenameFilter::IUIRenameFilter(IUIRename* puirRenameUI) : QGroupBox(puirRenameUI)
{
    m_puirRenameUI = puirRenameUI;
    m_puifmFileList = IUIMainWindow::GetMainWindow()->GetFileListUI();
    m_iRenameElements = RenameFilesOnly;
    setTitle(tr("Rename"));

    m_pqrbRenameFilesOnly           = new QRadioButton(tr("Files Only"));
    m_pqrbRenameFoldersOnly         = new QRadioButton(tr("Folders Only"));
    m_pqrbRenameFilesAndFolders     = new QRadioButton(tr("Files And Folders"));
    m_pqrbRenameSelectedItemsOnly   = new QRadioButton(tr("Selected Items Only"));
    m_pqrbRenameFilesWithExtension  = new QRadioButton(tr("Files With Extensions:"));
    m_pqleRenameFilesWithExtension  = new IComQLineEdit();
    QVBoxLayout* pvblLayout         = new QVBoxLayout;

    pvblLayout->addWidget(m_pqrbRenameFilesOnly);
    pvblLayout->addWidget(m_pqrbRenameFoldersOnly);
    pvblLayout->addWidget(m_pqrbRenameFilesAndFolders);
    pvblLayout->addWidget(m_pqrbRenameSelectedItemsOnly);
    pvblLayout->addWidget(m_pqrbRenameFilesWithExtension);
    pvblLayout->addWidget(m_pqleRenameFilesWithExtension);
    setLayout(pvblLayout);

    m_pqrbRenameFilesOnly->setChecked(true);
    m_pqleRenameFilesWithExtension->setEnabled(false);
    m_pqleRenameFilesWithExtension->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    m_pqleRenameFilesWithExtension->setToolTip(tr("Enter extensions of file types you wish to rename, e.g. jpg mkv.\n"
                                                  "You can enter multiple file types by seperating them with spaces.\n"
                                                  "For example, if you wish to rename only image files you could enter:\n\n"
                                                  "\t\tjpg jpeg png gif bmp\n"));

    CreateConnections();
}


void IUIRenameFilter::CreateConnections()
{
    connect(m_pqrbRenameFilesOnly,              SIGNAL(toggled(bool)),                  this,                               SLOT(RadioSettingsChanged(bool)));
    connect(m_pqrbRenameFoldersOnly,            SIGNAL(toggled(bool)),                  this,                               SLOT(RadioSettingsChanged(bool)));
    connect(m_pqrbRenameFilesAndFolders,        SIGNAL(toggled(bool)),                  this,                               SLOT(RadioSettingsChanged(bool)));
    connect(m_pqrbRenameSelectedItemsOnly,      SIGNAL(toggled(bool)),                  this,                               SLOT(RadioSettingsChanged(bool)));
    connect(m_pqrbRenameFilesWithExtension,     SIGNAL(toggled(bool)),                  this,                               SLOT(RadioSettingsChanged(bool)));
    connect(m_pqleRenameFilesWithExtension,     SIGNAL(textChanged(const QString &)),   this,                               SLOT(ExtensionListChanged()));

    connect(m_pqleRenameFilesWithExtension,     SIGNAL(GotFocus()),                     m_puirRenameUI,                     SLOT(ClearLastActiveLineEdit()));

    connect(m_pqrbRenameFilesWithExtension,     SIGNAL(toggled(bool)),                  m_pqleRenameFilesWithExtension,     SLOT(setEnabled(bool)));
    connect(m_pqrbRenameFilesWithExtension,     SIGNAL(toggled(bool)),                  m_pqleRenameFilesWithExtension,     SLOT(setFocus()));
}


void IUIRenameFilter::RadioSettingsChanged(const bool kbEnabled)
{
    if (kbEnabled == false)
        return;

    int iPrevSetting = m_iRenameElements;
    if (m_pqrbRenameFilesOnly->isChecked())
        m_iRenameElements = RenameFilesOnly;
    else if (m_pqrbRenameFoldersOnly->isChecked())
        m_iRenameElements = RenameFoldersOnly;
    else if (m_pqrbRenameFilesAndFolders->isChecked())
        m_iRenameElements = RenameFilesAndFolders;
    else if (m_pqrbRenameSelectedItemsOnly->isChecked())
        m_iRenameElements = RenameSelectedItems;
    else //if (m_pqrbRenameFilesWithExtension->isChecked())
        m_iRenameElements = RenameFilesWithExtension;

    if (m_iRenameElements != iPrevSetting)
        m_puifmFileList->RenameElementsSettingsChanged();
}


void IUIRenameFilter::ExtensionListChanged()
{
    ReadExtensions(m_puirRenameUI->CaseSensitive());
    m_puifmFileList->RenameElementsSettingsChanged();
}


void IUIRenameFilter::ReadExtensions(const bool kbCaseSensitive)
{
    QString qstrExtensions = m_pqleRenameFilesWithExtension->text();

    qstrExtensions.remove(QChar('*'));
    qstrExtensions.remove(QChar('.'));
    qstrExtensions.remove(QChar(','));

    if (kbCaseSensitive == false)
        qstrExtensions = qstrExtensions.toLower();

    m_qstrlRenameExtensions = qstrExtensions.split(QChar(' '), QString::SkipEmptyParts);
}


void IUIRenameFilter::DisableAllSettings()
{

}


void IUIRenameFilter::ClearAll()
{
    m_pqrbRenameFilesOnly->setChecked(true);
    m_pqleRenameFilesWithExtension->clear();
}


void IUIRenameFilter::SaveSettings(const QString & krqstrSection, QStringList & rqstrlSettings)
{
    QStringList qstrlSettings;

    /* if   (m_pqrbRenameFilesOnly->isChecked()) - Default Value - no need to save
        qstrlSettings.push_back("<RB01RenameFilesOnly>1</RB01RenameFilesOnly>");*/
    if      (m_pqrbRenameFoldersOnly->isChecked())
        qstrlSettings.push_back("<RB02RenameFoldersOnly>1</RB02RenameFoldersOnly>");
    else if (m_pqrbRenameFilesAndFolders->isChecked())
        qstrlSettings.push_back("<RB03RenameFilesAndFolders>1</RB03RenameFilesAndFolders>");
    else if (m_pqrbRenameSelectedItemsOnly->isChecked())
        qstrlSettings.push_back("<RB04RenameSelectedItemsOnly>1</RB04RenameSelectedItemsOnly>");
    else if (m_pqrbRenameFilesWithExtension->isChecked())
        qstrlSettings.push_back("<RB05RenameFilesWithExtension>1</RB05RenameFilesWithExtension>");

    if (m_pqleRenameFilesWithExtension->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE01RenameFilesWithExtension>%1</LE01RenameFilesWithExtension>").arg(m_pqleRenameFilesWithExtension->text()));

    if (qstrlSettings.isEmpty() == false)
    {
        rqstrlSettings.push_back(QString("<%1>").arg(krqstrSection));
        rqstrlSettings.append(qstrlSettings);
        rqstrlSettings.push_back(QString("</%1>").arg(krqstrSection));
    }
}


void IUIRenameFilter::RestoreSettings(const QString & krqstrSettings, int & riIndex)
{
    QString qstrTag, qstrValue;
    while (m_puirRenameUI->ReadSettingsValue(krqstrSettings, riIndex, qstrTag, qstrValue) == true)
    {
        if      (qstrTag.startsWith("RB"))
            RestoreRadioBox(qstrTag);
        else if (qstrTag.startsWith("LE"))
            RestoreLineEdit(qstrTag, qstrValue);
    }
}


void IUIRenameFilter::RestoreRadioBox(const QString & krqstrTag)
{
    int iCheckBoxID = krqstrTag.mid(2, 2).toInt();
    switch (iCheckBoxID)
    {
    case 1  :   m_pqrbRenameFilesOnly->setChecked(true);            // RB01RenameFilesOnly
                break;
    case 2  :   m_pqrbRenameFoldersOnly->setChecked(true);          // RB02RenameFoldersOnly
                break;
    case 3  :   m_pqrbRenameFilesAndFolders->setChecked(true);      // RB03RenameFilesAndFolders
                break;
    case 4  :   m_pqrbRenameSelectedItemsOnly->setChecked(true);    // RB04RenameSelectedItemsOnly
                break;
    case 5  :   m_pqrbRenameFilesWithExtension->setChecked(true);   // RB05RenameFilesWithExtension
    }
}


void IUIRenameFilter::RestoreLineEdit(const QString & krqstrTag, const QString & krqstrValue)
{
    int iCheckBoxID = krqstrTag.mid(2, 2).toInt();
    switch (iCheckBoxID)
    {
    case 1  :   m_pqleRenameFilesWithExtension->setText(krqstrValue); // LE01RenameFilesWithExtension
    }
}
