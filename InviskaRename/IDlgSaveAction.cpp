#include <QtWidgets>
#include "IDlgSaveAction.h"
#include "IUIMenuSavesBase.h"


IDlgSaveAction::IDlgSaveAction(IUIMenuSavesBase* pimenuMenu, const QString & krstrTitle, const QString & krstrItemDescription, QString qstrDefaultText) : QDialog(pimenuMenu)
{
    m_pimenuMenu = pimenuMenu;
    m_strItemDescription = krstrItemDescription;

    setWindowTitle(krstrTitle);
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QLabel* pqlblName = new QLabel(tr("Name:"));
    m_pqleName = new QLineEdit;
    m_pqleName->setText(qstrDefaultText);
    m_pqleName->selectAll();
    connect(m_pqleName, SIGNAL(textChanged(const QString &)), this, SLOT(EnableDisableSaveButton()));

    m_pqdbbCancelSave = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Save);
    m_pqdbbCancelSave->button(QDialogButtonBox::Save)->setEnabled(!m_pqleName->text().isEmpty());
    m_pqdbbCancelSave->setOrientation(Qt::Horizontal);
    connect(m_pqdbbCancelSave, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_pqdbbCancelSave, SIGNAL(rejected()), this, SLOT(reject()));

    QHBoxLayout *qhblSaveLayout = new QHBoxLayout;
    qhblSaveLayout->addWidget(pqlblName);
    qhblSaveLayout->addWidget(m_pqleName);

    QVBoxLayout *qvblLayout = new QVBoxLayout;
    qvblLayout->addLayout(qhblSaveLayout);
    qvblLayout->addWidget(m_pqdbbCancelSave);

    setLayout(qvblLayout);
    resize(300, sizeHint().height());
    show();
}


void IDlgSaveAction::EnableDisableSaveButton()
{
    m_pqdbbCancelSave->button(QDialogButtonBox::Save)->setEnabled(!m_pqleName->text().isEmpty());
}


void IDlgSaveAction::accept()
{
    QString qstrName = m_pqleName->text();
    QAction* pqactExistingAction = m_pimenuMenu->GetActionCaseInsensitive(qstrName);
    if (pqactExistingAction != nullptr)
    {
        int iAnswer = QMessageBox::question(this,
                                           tr("Overwrite Entry?"),
                                           tr("There is already %1 called \"%2\".\nDo you wish to overwrite it?",
                                              "Example: There is alread a bookmark called \"Bookmark Name\"").arg(m_strItemDescription).arg(qstrName),
                                           QMessageBox::Yes | QMessageBox::No);
        if (iAnswer == QMessageBox::Yes)
             m_pimenuMenu->UpdateAction(pqactExistingAction, qstrName);
        else
            return;
    }
    else
    {
        m_pimenuMenu->SaveAction(qstrName);
    }

    QDialog::accept();
}
