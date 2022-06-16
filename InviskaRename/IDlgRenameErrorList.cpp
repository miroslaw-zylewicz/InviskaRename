#include <QtWidgets>
#include "IDlgRenameErrorList.h"
#include "IUIFileList.h"
#include "IComUtilityFuncs.h"


IDlgRenameErrorList::IDlgRenameErrorList(IUIFileList* puifmFileList, const bool kbErrorList) : QDialog(puifmFileList)
{
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QLabel* pqlblTitleMessage = new QLabel;
    QLabel* pqlblMessage = new QLabel;
    pqlblTitleMessage->setAlignment(Qt::AlignHCenter);

    m_pqpbCloseButton = new QPushButton(tr("Close"));
    m_pqpbCloseButton->setDefault(true);
    connect(m_pqpbCloseButton, SIGNAL(clicked()), this, SLOT(close()));

    m_pqtwErrorList = new QTableWidget(this);
    m_pqtwErrorList->setColumnCount(3);

    QTableWidgetItem* pqtwiHeader = new QTableWidgetItem;
    pqtwiHeader->setText(tr("Current Name"));
    m_pqtwErrorList->setHorizontalHeaderItem(0, pqtwiHeader);
    pqtwiHeader = new QTableWidgetItem;
    m_pqtwErrorList->setHorizontalHeaderItem(1, pqtwiHeader);
    pqtwiHeader = new QTableWidgetItem;
    m_pqtwErrorList->setHorizontalHeaderItem(2, pqtwiHeader);

    if (kbErrorList)
    {
        setWindowTitle(tr("Error Renaming Errors"));
        pqlblTitleMessage->setText(QString("<h2>%1</h2>").arg(tr("Error Renaming Files")));
        pqlblMessage->setText(tr("The following files could not be renamed:"));
        m_pqtwErrorList->horizontalHeaderItem(1)->setText(tr("Failed Rename"));
        m_pqtwErrorList->horizontalHeaderItem(2)->setText(tr("Probable Failure Reason"));
    }
    else
    {
        setWindowTitle(tr("Invalid Rename Operation"));
        pqlblTitleMessage->setText(QString("<h2>%1</h2>").arg(tr("Invalid Rename Operation")));
        pqlblMessage->setText(tr("This rename operation cannot proceed because it would produce the following conflicts:"));
        m_pqtwErrorList->horizontalHeaderItem(1)->setText(tr("New Name"));
        m_pqtwErrorList->horizontalHeaderItem(2)->setText(tr("Conflict"));
    }

    m_pqtwErrorList->setEditTriggers(QAbstractItemView::NoEditTriggers);                 // Items not editable
    m_pqtwErrorList->setWordWrap(false);                                                 // Disable word wrapping when contents won't fit in cell.
    m_pqtwErrorList->verticalHeader()->hide();                                           // No row lables
    m_pqtwErrorList->horizontalHeader()->setSectionsClickable(false);                    // Column headers not clicable
    //m_pqtwErrorList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);   // Streatch columns proportionally
    m_pqtwErrorList->horizontalHeader()->setStretchLastSection(true);

    m_pqtwErrorList->setFont(puifmFileList->GetCurrentFont());
    m_pqtwErrorList->verticalHeader()->setFont(puifmFileList->GetCurrentFont());
    m_pqtwErrorList->verticalHeader()->setMinimumSectionSize(16);
    m_pqtwErrorList->verticalHeader()->setDefaultSectionSize(IComUtilityFuncs::GetTableRowHeightFitToFont(m_pqtwErrorList));

    QHBoxLayout* pqhblCloseLayout = new QHBoxLayout;
    pqhblCloseLayout->addStretch();
    pqhblCloseLayout->addWidget(m_pqpbCloseButton);

    QVBoxLayout* pqvblLayout = new QVBoxLayout;
    pqvblLayout->addWidget(pqlblTitleMessage);
    pqvblLayout->addWidget(pqlblMessage);
    pqvblLayout->addWidget(m_pqtwErrorList);
    pqvblLayout->addLayout(pqhblCloseLayout);

    setLayout(pqvblLayout);
}


void IDlgRenameErrorList::AddToErrorList(const QString & krqstrCurrentName, const QString & krqstrFailedRename, const QString & krqstrFailureReason)
{
    int iRow = m_pqtwErrorList->rowCount();
    m_pqtwErrorList->setRowCount(iRow+1);

    QTableWidgetItem* pqtwiItem = new QTableWidgetItem;
    pqtwiItem->setText(krqstrCurrentName);
    m_pqtwErrorList->setItem(iRow, 0, pqtwiItem);

    pqtwiItem = new QTableWidgetItem;
    pqtwiItem->setText(krqstrFailedRename);
    m_pqtwErrorList->setItem(iRow, 1, pqtwiItem);

    pqtwiItem = new QTableWidgetItem;
    pqtwiItem->setText(krqstrFailureReason);
    m_pqtwErrorList->setItem(iRow, 2, pqtwiItem);
}


void IDlgRenameErrorList::ResizeColumnsAndShow()
{
    m_pqtwErrorList->resizeColumnsToContents();
    m_pqtwErrorList->setColumnWidth(0, m_pqtwErrorList->columnWidth(0)+20);
    m_pqtwErrorList->setColumnWidth(1, m_pqtwErrorList->columnWidth(1)+20);

    int iWidth = m_pqtwErrorList->columnWidth(0) + m_pqtwErrorList->columnWidth(1) + m_pqtwErrorList->columnWidth(2);
    resize(iWidth+50, 400);
    show();
}
