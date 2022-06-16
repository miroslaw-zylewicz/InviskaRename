#include <QtWidgets>
#include "IDlgOrganiseMenu.h"
#include "IDlgRenameMenuItem.h"
#include "IUIMenuSavesBase.h"


IDlgOrganiseMenu::IDlgOrganiseMenu(IUIMenuSavesBase* pimenuMenu, const QString & krqstrTitle, const MenuType iMenuType) : QDialog(pimenuMenu)
{
    m_pimenuMenu = pimenuMenu;
    m_bChangesMade = false;

    setWindowTitle(krqstrTitle);
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    m_pqlwMenuItems = new QListWidget(this);
    PopulateActionList();

    m_qpbMoveUp   = new QPushButton(tr("Move Up"), this);
    m_qpbMoveDown = new QPushButton(tr("Move Down"), this);
    m_qpbRename = new QPushButton(tr("Rename"), this);
    m_qpbDelete   = new QPushButton(tr("Delete"), this);
    connect(m_qpbMoveUp,   SIGNAL(clicked()), this, SLOT(MoveUp()));
    connect(m_qpbMoveDown, SIGNAL(clicked()), this, SLOT(MoveDown()));
    connect(m_qpbRename,   SIGNAL(clicked()), this, SLOT(Rename()));
    connect(m_qpbDelete,   SIGNAL(clicked()), this, SLOT(Delete()));

    m_qdbbOKCancel = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    m_qdbbOKCancel->setOrientation(Qt::Vertical);
    connect(m_qdbbOKCancel, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_qdbbOKCancel, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout* qvblListLayout = new QVBoxLayout;
    qvblListLayout->addWidget(m_pqlwMenuItems);

    if (iMenuType == Bookmarks)
    {
        QLabel* pqlblDetailsLabel = new QLabel(tr("Folder:"));
        m_pqleDetails = new QLineEdit(this);
        m_pqleDetails->setReadOnly(true);
        connect(m_pqlwMenuItems, SIGNAL(itemSelectionChanged()), this, SLOT(ShowSelectedItemDetails()));

        QHBoxLayout* qhblDetailsLayout = new QHBoxLayout;
        qhblDetailsLayout->addWidget(pqlblDetailsLabel);
        qhblDetailsLayout->addWidget(m_pqleDetails);
        qvblListLayout->addLayout(qhblDetailsLayout);
    }

    QVBoxLayout* qvblButtonLayout = new QVBoxLayout;
    qvblButtonLayout->addWidget(m_qpbMoveUp);
    qvblButtonLayout->addWidget(m_qpbMoveDown);
    qvblButtonLayout->addWidget(m_qpbRename);
    qvblButtonLayout->addWidget(m_qpbDelete);
    qvblButtonLayout->addStretch(1);
    qvblButtonLayout->addWidget(m_qdbbOKCancel);

    QHBoxLayout* qhblLayout = new QHBoxLayout;
    qhblLayout->addLayout(qvblListLayout);
    qhblLayout->addLayout(qvblButtonLayout);

    setLayout(qhblLayout);
    resize(sizeHint());
    show();
}


void IDlgOrganiseMenu::PopulateActionList()
{
    QString qstrName;
    QListWidgetItem* pqlwiNewItem;
    const QVector<QAction*> & m_qvecpqactActions = m_pimenuMenu->GetActions();
    QVector<QAction*>::const_iterator kitAction;
    for (kitAction = m_qvecpqactActions.constBegin() ; kitAction < m_qvecpqactActions.constEnd() ; ++kitAction)
    {
        pqlwiNewItem = new QListWidgetItem((*kitAction)->whatsThis());
        pqlwiNewItem->setData(Qt::UserRole, QVariant::fromValue(*kitAction));
        m_pqlwMenuItems->addItem(pqlwiNewItem);
    }
}


void IDlgOrganiseMenu::MoveUp()
{
    int iSelectedRow = m_pqlwMenuItems->currentRow();
    if (m_pqlwMenuItems->count() == 0 || iSelectedRow == 0)
        return;

    QListWidgetItem* pqlwiItem = m_pqlwMenuItems->takeItem(iSelectedRow);
    m_pqlwMenuItems->insertItem(--iSelectedRow, pqlwiItem);
    m_pqlwMenuItems->setCurrentRow(iSelectedRow);
    m_bChangesMade = true;
}


void IDlgOrganiseMenu::MoveDown()
{
    int iSelectedRow = m_pqlwMenuItems->currentRow();
    if (m_pqlwMenuItems->count() == 0 || iSelectedRow == m_pqlwMenuItems->count()-1)
        return;

    QListWidgetItem* pqlwiItem = m_pqlwMenuItems->takeItem(iSelectedRow);
    m_pqlwMenuItems->insertItem(++iSelectedRow, pqlwiItem);
    m_pqlwMenuItems->setCurrentRow(iSelectedRow);
    m_bChangesMade = true;
}


void IDlgOrganiseMenu::Rename()
{
    if (m_pqlwMenuItems->currentItem() == nullptr)
        return;

    new IDlgRenameMenuItem(this, m_pqlwMenuItems->currentItem());
}


void IDlgOrganiseMenu::Delete()
{
    if (m_pqlwMenuItems->count() == 0)
        return;

    QListWidgetItem* pqlwiItem = m_pqlwMenuItems->takeItem(m_pqlwMenuItems->currentRow());
    m_qvecpqactActionsToDelete.push_back(pqlwiItem->data(Qt::UserRole).value<QAction*>());
    delete pqlwiItem;
    m_bChangesMade = true;
}


bool IDlgOrganiseMenu::EntryExists(const QString & krqstrEntryName)
{
    const int kiItemCount = m_pqlwMenuItems->count();
    for (int iIndex = 0 ; iIndex < kiItemCount ; ++iIndex)
    {
        if (krqstrEntryName == m_pqlwMenuItems->item(iIndex)->text())
            return true;
    }
    return false;
}

void IDlgOrganiseMenu::UpdateActionName(QAction* pqactAction, const QString & krqstrName)
{
    m_pimenuMenu->UpdateActionName(pqactAction, krqstrName);
}


void IDlgOrganiseMenu::ShowSelectedItemDetails()
{
    m_pqleDetails->setText(QDir::toNativeSeparators(m_pqlwMenuItems->currentItem()->data(Qt::UserRole).value<QAction*>()->data().toString()));
    m_pqleDetails->setCursorPosition(0);
}


void IDlgOrganiseMenu::accept()
{
    if (m_bChangesMade)
        m_pimenuMenu->ReOrganiseMenu(m_pqlwMenuItems, m_qvecpqactActionsToDelete);

    QDialog::accept();
}
