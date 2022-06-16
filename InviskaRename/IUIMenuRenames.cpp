#include <QtWidgets>
#include "IUIMenuRenames.h"
#include "IUIMainWindow.h"
#include "IUIRename.h"
#include "IDlgSaveAction.h"
#include "IDlgOrganiseMenu.h"
#include "IRenameLegacySave.h"


IUIMenuRenames::IUIMenuRenames(IUIMainWindow* pmwMainWindow) : IUIMenuSavesBase(tr("&Renames"), pmwMainWindow)
{
    m_bSetToolTipToDataPath = false;
    m_puirRenameUI = pmwMainWindow->GetRenameUI();

    m_qstrGroup = "Renames";
    m_qstrArray = "SavedRenames";
    m_qstrEntry = "Rename";
}


IUIMenuRenames::~IUIMenuRenames()
{
    SaveActionsToQSettings();
}


void IUIMenuRenames::CreateActionsRenames()
{
    m_pqactSaveCurrentSettings = new QAction(tr("&Save Rename Settings..."), this);
    m_pqactSaveCurrentSettings->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_DialogSaveButton));
    m_pqactSaveCurrentSettings->setShortcut(QKeySequence::Save);
    QObject::connect(m_pqactSaveCurrentSettings, SIGNAL(triggered()), this, SLOT(ShowSaveRenameSettingsDialog()));

    m_pqactOrganiseSavedRenames = new QAction(tr("&Organise Saved Renames..."), this);
    m_pqactOrganiseSavedRenames->setIcon(m_pmwMainWindow->style()->standardIcon(QStyle::SP_DirHomeIcon));
    m_pqactOrganiseSavedRenames->setShortcut(Qt::CTRL + Qt::Key_R); //tr("Ctrl+R"));
    QObject::connect(m_pqactOrganiseSavedRenames, SIGNAL(triggered()), this, SLOT(ShowOrganiseSavedRenamesDialog()));

    m_pqactClearRenameSettings = new QAction(tr("&Clear Current Settings"), this);
    m_pqactClearRenameSettings->setShortcut(Qt::CTRL + Qt::Key_X); //tr("Ctrl+X"));
    QObject::connect(m_pqactClearRenameSettings, SIGNAL(triggered()), m_puirRenameUI, SLOT(ClearAll()));

    InitialiseMenu();
    LoadActionsFromQSettings();
    LoadLegacyRenames();
}


void IUIMenuRenames::InitialiseMenu()
{
    addAction(m_pqactSaveCurrentSettings);
    addAction(m_pqactOrganiseSavedRenames);
    addSeparator();
    addAction(m_pqactClearRenameSettings);
    addSeparator();
}


QString IUIMenuRenames::GetDataString(const QString & krqstrSaveString)
{
    int iDataStart = krqstrSaveString.indexOf("</Name>") + 7;
    return krqstrSaveString.mid(iDataStart);
}


QString IUIMenuRenames::GetActionSaveString(const QAction* const kpactAction)
{
    return QString("<Name>%1</Name>%2").arg(kpactAction->whatsThis()).arg(kpactAction->data().toString());
}


void IUIMenuRenames::SaveAction(const QString & krqstrName)
{
    CreateAction(krqstrName, m_puirRenameUI->GetCurrentSettingsSaveString());
}


void IUIMenuRenames::UpdateAction(QAction* pqactAction, const QString & krqstrName)
{
    UpdateActionName(pqactAction, krqstrName);
    pqactAction->setData(m_puirRenameUI->GetCurrentSettingsSaveString());
}


void IUIMenuRenames::PopulateRenameCombo(QComboBox* pqcomRenames, const QString & krqstrSetValue)
{
    QVector<QAction*>::iterator itRenameAction;
    for (itRenameAction = m_qvecpqactActions.begin() ; itRenameAction < m_qvecpqactActions.end() ; ++itRenameAction)
    {
        pqcomRenames->addItem((*itRenameAction)->whatsThis());
        if ((*itRenameAction)->whatsThis() == krqstrSetValue)
            pqcomRenames->setCurrentIndex(pqcomRenames->count()-1);
    }
}


void IUIMenuRenames::LoadData()
{
    m_puirRenameUI->RestoreSettingsFromSaveString(qobject_cast<QAction*>(sender())->data().toString());
}


void IUIMenuRenames::ShowSaveRenameSettingsDialog()
{
    new IDlgSaveAction(this, tr("Save Rename Settings"), tr("saved rename"));
}


void IUIMenuRenames::ShowOrganiseSavedRenamesDialog()
{
    new IDlgOrganiseMenu(this, tr("Organise Saved Renames"), IDlgOrganiseMenu::Renames);
}


void IUIMenuRenames::LoadLegacyRenames()
{
    QSettings & rqsetSettings = m_pmwMainWindow->GetSettings();

    QString qstrSaveString;
    rqsetSettings.beginGroup("SavedRenames");
    int iNumElements = rqsetSettings.beginReadArray("RenameSaves");
    for (int iArrayIndex = 0 ; iArrayIndex < iNumElements ; ++iArrayIndex)
    {
        rqsetSettings.setArrayIndex(iArrayIndex);
        qstrSaveString = rqsetSettings.value("RenameSettings").toString();
        IRenameLegacySave::ConvertLegacySave(qstrSaveString);
        CreateAction(GetName(qstrSaveString), GetDataString(qstrSaveString));
    }
    rqsetSettings.endArray();
    rqsetSettings.endGroup();

    rqsetSettings.remove("SavedRenames");
    SaveActionsToQSettings();
}
