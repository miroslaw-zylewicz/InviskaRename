#include <QtWidgets>
#include "IDlgRenameFile.h"
#include "IUIFileList.h"
#include "IUIMainWindow.h"


IDlgRenameFile::IDlgRenameFile(IUIFileList* puifmFileList, QDir & rqdirDirReader, const QString & krqstrFileName) : IDlgRenameBase(puifmFileList, tr("Rename File"), krqstrFileName),
                                                                                                                    m_rqdirDirReader(rqdirDirReader)
{
    m_pqleNewName->setValidator(IUIMainWindow::GetMainWindow()->GetInvalidCharValidator());

    int iExtenIndex = krqstrFileName.lastIndexOf('.');
    if (iExtenIndex != -1)
        m_pqleNewName->setSelection(0, iExtenIndex);
}


void IDlgRenameFile::accept()
{
    QString qstrNewFileName = m_pqleNewName->text();
    if (m_rqdirDirReader.exists(qstrNewFileName))
    {
        QMessageBox::warning(this, tr("File Already Exists"), tr("A file with the specified name already exists.\nPlease enter an alternative name."), QMessageBox::Ok);
        return;
    }

    if (m_rqdirDirReader.rename(m_qstrOriginalName, qstrNewFileName) == false)
    {
        if (m_rqdirDirReader.exists(m_qstrOriginalName) == false)
        {
           QMessageBox::critical(this, tr("File No Longer Exists"), tr("Rename failed because the original file no longer exists.\nIt may have been deleted, moved or renamed."), QMessageBox::Ok);
           QDialog::accept();
        }
        else
        {
            int iAnswer = QMessageBox::warning(this, tr("Rename Filure"), tr("File may be open in other application or you may not have permission to modify the file."), QMessageBox::Abort | QMessageBox::Retry);
            while (iAnswer == QMessageBox::Retry)
            {
                if (m_rqdirDirReader.rename(m_qstrOriginalName, qstrNewFileName) == false)
                    iAnswer = QMessageBox::warning(this, tr("Rename Filure"), tr("File may be open in other application or you may not have permission to modify the file."), QMessageBox::Abort | QMessageBox::Retry);
                else
                    break;
            }
        }
    }

    QDialog::accept();
}
