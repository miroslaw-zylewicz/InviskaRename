#include <QtWidgets>
#include "IDlgRenameBase.h"
#include "IUIFileList.h"
#include "IUIMainWindow.h"


IDlgRenameBase::IDlgRenameBase(QWidget* pqwParent, const QString & krqstrTitle, const QString & krqstrName) : QDialog(pqwParent)
{
    m_qstrOriginalName = krqstrName;

    setWindowTitle(krqstrTitle);
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QLabel* pqlblNewName = new QLabel(tr("New Name:"));
    m_pqleNewName = new QLineEdit(krqstrName);
    connect(m_pqleNewName, SIGNAL(textChanged(const QString &)), this, SLOT(LineEditTextChanged()));

    QFontMetrics qfmFontMetrics(m_pqleNewName->font());
    m_iNamePadding = qfmFontMetrics.width('W');

    m_pqdbbCancelRename = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    m_pqdbbCancelRename->button(QDialogButtonBox::Ok)->setEnabled(false);
    m_pqdbbCancelRename->button(QDialogButtonBox::Ok)->setText(tr("Rename"));
    m_pqdbbCancelRename->setOrientation(Qt::Horizontal);
    connect(m_pqdbbCancelRename, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_pqdbbCancelRename, SIGNAL(rejected()), this, SLOT(reject()));

    QHBoxLayout* qhblNewNameLayout = new QHBoxLayout;
    qhblNewNameLayout->addWidget(pqlblNewName);
    qhblNewNameLayout->addWidget(m_pqleNewName);

    QVBoxLayout* qvblLayout = new QVBoxLayout;
    qvblLayout->addLayout(qhblNewNameLayout);
    qvblLayout->addWidget(m_pqdbbCancelRename);

    setLayout(qvblLayout);
    resize(sizeHint());
    show();

    LineEditTextChanged();
}


void IDlgRenameBase::LineEditTextChanged()
{
    QString qstrNewName = m_pqleNewName->text();

    if (qstrNewName != m_qstrOriginalName && qstrNewName.isEmpty() == false)
        m_pqdbbCancelRename->button(QDialogButtonBox::Ok)->setEnabled(true);
    else
        m_pqdbbCancelRename->button(QDialogButtonBox::Ok)->setEnabled(false);

    QFontMetrics qfmFontMetrics(m_pqleNewName->font());
    int iWidth = qfmFontMetrics.width(qstrNewName) + m_iNamePadding;
    m_pqleNewName->setMinimumWidth(iWidth);
}
