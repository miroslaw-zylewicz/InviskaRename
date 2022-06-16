#include <QtWidgets>
#include "IUIMenuSavesBase.h"
#include "IUIMainWindow.h"


IUIMenuSavesBase::IUIMenuSavesBase(const QString & krqstrName, IUIMainWindow* pmwMainWindow) : QMenu(krqstrName, pmwMainWindow)
{
    m_pmwMainWindow = pmwMainWindow;
}


void IUIMenuSavesBase::LoadActionsFromQSettings()
{
    QSettings & rqsetSettings = m_pmwMainWindow->GetSettings();

    QString qstrSaveString;
    rqsetSettings.beginGroup(m_qstrGroup);
    int iNumElements = rqsetSettings.beginReadArray(m_qstrArray);
    for (int iArrayIndex = 0 ; iArrayIndex < iNumElements ; ++iArrayIndex)
    {
        rqsetSettings.setArrayIndex(iArrayIndex);
        qstrSaveString = rqsetSettings.value(m_qstrEntry).toString();
        CreateAction(GetName(qstrSaveString), GetDataString(qstrSaveString));
    }
    rqsetSettings.endArray();
    rqsetSettings.endGroup();
}


void IUIMenuSavesBase::SaveActionsToQSettings()
{
    QSettings & rqsetSettings = m_pmwMainWindow->GetSettings();
    rqsetSettings.remove(m_qstrGroup);

    rqsetSettings.beginGroup(m_qstrGroup);
    rqsetSettings.beginWriteArray(m_qstrArray);
    int iArrayIndex = 0;
    QVector<QAction*>::ConstIterator kitAction;
    for (kitAction = m_qvecpqactActions.constBegin() ; kitAction < m_qvecpqactActions.constEnd() ; ++kitAction, ++iArrayIndex)
    {
        rqsetSettings.setArrayIndex(iArrayIndex);
        rqsetSettings.setValue(m_qstrEntry, GetActionSaveString(*kitAction));
    }
    rqsetSettings.endArray();
    rqsetSettings.endGroup();

    rqsetSettings.sync();
}


void IUIMenuSavesBase::CreateAction(const QString & krqstrName, const QString & krqstrData)
{
    QAction* pqactNewAction = new QAction(GenerateActionName(krqstrName), this);
    pqactNewAction->setData(krqstrData);
    pqactNewAction->setWhatsThis(krqstrName);
    if (m_bSetToolTipToDataPath)
        pqactNewAction->setToolTip(QDir::toNativeSeparators(krqstrData));

    QObject::connect(pqactNewAction, SIGNAL(triggered()), this, SLOT(LoadData()));

    m_qvecpqactActions.push_back(pqactNewAction);
    addAction(pqactNewAction);
}


QString IUIMenuSavesBase::GetName(const QString & krqstrSaveString)
{
    int iNameStart = krqstrSaveString.indexOf('>') + 1;
    int iNameEnd = krqstrSaveString.indexOf("</Name>", iNameStart);
    return krqstrSaveString.mid(iNameStart, iNameEnd-iNameStart);
}


QString IUIMenuSavesBase::GenerateActionName(const QString & krqstrName)
{
    QString qstrName = krqstrName;
    return qstrName.replace('&', "&&");
}


void IUIMenuSavesBase::UpdateActionName(QAction* pqactAction, const QString & krqstrName)
{
    pqactAction->setText(GenerateActionName(krqstrName));
    pqactAction->setWhatsThis(krqstrName);
}


void IUIMenuSavesBase::ReOrganiseMenu(const QListWidget* kpqlwList, QVector<QAction*> & rqvecpqactActionsToDelete)
{
    QVector<QAction*>::iterator itAction;
    for (itAction = m_qvecpqactActions.begin() ; itAction < m_qvecpqactActions.end() ; ++itAction)
        removeAction(*itAction);
    m_qvecpqactActions.clear();

    QAction* pactAction;
    int iNumRows = kpqlwList->count();
    for (int iRow = 0 ; iRow < iNumRows ; ++iRow)
    {
        // Removing an action from the menu breaks the connection so we have to recreate it
        pactAction = kpqlwList->item(iRow)->data(Qt::UserRole).value<QAction*>();
        QObject::connect(pactAction, SIGNAL(triggered()), this, SLOT(LoadData()));
        m_qvecpqactActions.push_back(pactAction);
        addAction(pactAction);
    }

    if (rqvecpqactActionsToDelete.empty() == false)
    {
        QVector<QAction*>::const_iterator kitAction;
        for (kitAction = rqvecpqactActionsToDelete.begin() ; kitAction < rqvecpqactActionsToDelete.end() ; ++kitAction)
            delete *kitAction;
    }
}


bool IUIMenuSavesBase::ExecuteAction(const QString & krqstrName)
{
    QVector<QAction*>::iterator itRenameAction;
    for (itRenameAction = m_qvecpqactActions.begin() ; itRenameAction < m_qvecpqactActions.end() ; ++itRenameAction)
    {
        if ((*itRenameAction)->whatsThis() == krqstrName)
        {
            (*itRenameAction)->trigger();
            return true;
        }
    }
    return false;
}


bool IUIMenuSavesBase::ExecuteActionCaseInsensitive(const QString & krqstrName)
{
    QString qstrNameLower = krqstrName.toLower();
    QVector<QAction*>::iterator itRenameAction;
    for (itRenameAction = m_qvecpqactActions.begin() ; itRenameAction < m_qvecpqactActions.end() ; ++itRenameAction)
    {
        if ((*itRenameAction)->whatsThis().toLower() == qstrNameLower)
        {
            (*itRenameAction)->trigger();
            return true;
        }
    }
    return false;
}


QAction* IUIMenuSavesBase::GetAction(const QString & krqstrName)
{
    QVector<QAction*>::iterator itRenameAction;
    for (itRenameAction = m_qvecpqactActions.begin() ; itRenameAction < m_qvecpqactActions.end() ; ++itRenameAction)
    {
        if ((*itRenameAction)->whatsThis() == krqstrName)
            return *itRenameAction;
    }
    return nullptr;
}


QAction* IUIMenuSavesBase::GetActionCaseInsensitive(const QString & krqstrName)
{
    QString qstrName = krqstrName.toLower();
    QVector<QAction*>::iterator itRenameAction;
    for (itRenameAction = m_qvecpqactActions.begin() ; itRenameAction < m_qvecpqactActions.end() ; ++itRenameAction)
    {
        if ((*itRenameAction)->whatsThis().toLower() == qstrName)
            return *itRenameAction;
    }
    return nullptr;
}
