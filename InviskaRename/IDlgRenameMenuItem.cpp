#include <QtWidgets>
#include "IDlgRenameMenuItem.h"
#include "IDlgOrganiseMenu.h"
#include "IUIMainWindow.h"


IDlgRenameMenuItem::IDlgRenameMenuItem(IDlgOrganiseMenu* pdlgOrganiseDialog, QListWidgetItem* pqlwiItem) : IDlgRenameBase(pdlgOrganiseDialog, tr("Rename Entry"), pqlwiItem->text())
{
    m_pdlgOrganiseDialog = pdlgOrganiseDialog;
    m_pqlwiItem = pqlwiItem;
    m_pqleNewName->selectAll();
}


void IDlgRenameMenuItem::accept()
{
    QString qstrNewName = m_pqleNewName->text();
    if (m_pdlgOrganiseDialog->EntryExists(qstrNewName))
    {
        QMessageBox::warning(this, tr("Entry Already Exists"), tr("An entry with the specified name already exists.\nPlease enter an alternative name."), QMessageBox::Ok);
        return;
    }

    m_pqlwiItem->setText(qstrNewName);
    m_pdlgOrganiseDialog->UpdateActionName(m_pqlwiItem->data(Qt::UserRole).value<QAction*>(), qstrNewName);

    QDialog::accept();
}
