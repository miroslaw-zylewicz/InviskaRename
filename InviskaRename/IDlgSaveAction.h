#ifndef IDlgSaveAction_h
#define IDlgSaveAction_h

#include <QDialog>
class QLineEdit;
class QDialogButtonBox;
class IUIMenuSavesBase;


class IDlgSaveAction : public QDialog
{
    Q_OBJECT

protected:
    // Pointer to menu to save action to
    IUIMenuSavesBase*       m_pimenuMenu;

    // Descriptions of item being saved for overwrite string
    QString                 m_strItemDescription;

    // Line edit to enter name
    QLineEdit*              m_pqleName;

    // Cancel and Save buttonbox
    QDialogButtonBox*       m_pqdbbCancelSave;

public:
    IDlgSaveAction(IUIMenuSavesBase* pimenuMenu, const QString & krstrTitle, const QString & krstrItemDescription, QString qstrDefaultText = QString());

private slots:
    // Enables/disbales save button depending on whether the Name line edit contains text
    void EnableDisableSaveButton();

    // Override accept() function to process settings when Save is pressed
    void accept();
};

#endif // IDlgSaveAction_h
