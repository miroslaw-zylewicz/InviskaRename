#ifndef IComDlgHelpAbout_h
#define IComDlgHelpAbout_h

#include <QDialog>
class QTabWidget;
class QDialogButtonBox;
class IUIMainWindow;


class IComDlgHelpAbout : public QDialog
{
    Q_OBJECT

private:
    // Main window
    IUIMainWindow*                  m_pmwMainWindow;

    // Tab widget for displaying general information and credits of various types
    QTabWidget*                     m_pqtwTabWidget;

    // About Qt button - mostly so the user can check what version of Qt is being used
    QPushButton*                    m_pqpbAboutQt;

    // OK button
    QDialogButtonBox*               m_pqdbbOK;

public:
    IComDlgHelpAbout(IUIMainWindow* pmwMainWindow);

    // Create tabs for tab widget
    void CreateTabGeneral();
    void CreateTab(const QString & krqstrTabTitle, const QString & krqstrTitle, const QString & krqstrCredits);
    void CreateTabLibraries();
    void CreateTabLicence();

private slots:
    // Displays About Qt messagebox
    void ShowAboutQt();
};

#endif // IComDlgHelpAbout_h
