#ifndef IDlgRenameBase_h
#define IDlgRenameBase_h

#include <QDialog>
class QLineEdit;
class QDialogButtonBox;


class IDlgRenameBase : public QDialog
{
    Q_OBJECT

protected:
    // Line edit to enter new name
    QLineEdit*                  m_pqleNewName;

    // Cancel and Rename buttonbox
    QDialogButtonBox*           m_pqdbbCancelRename;

    // Copy of original name so we can confirm it has changed and enable/disable the Rename button
    QString                     m_qstrOriginalName;

    // Padding to use in addition to string length when resizing QLineEdit to fit text
    int                         m_iNamePadding;          

public:
    IDlgRenameBase(QWidget* pqwParent, const QString & krqstrTitle, const QString & krqstrName);

private slots:
    // Resizes window to fit name and enables/disables the Rename button depending on whether the name has changed
    void LineEditTextChanged();
};

#endif // IDlgRenameBase_h
