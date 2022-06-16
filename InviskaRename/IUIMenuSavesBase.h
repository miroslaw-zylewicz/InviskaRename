#ifndef IUIMenuSavesBase_h
#define IUIMenuSavesBase_h

#include <QMenu>
class QListWidget;
class IUIMainWindow;


class IUIMenuSavesBase : public QMenu
{
    Q_OBJECT

protected:
    // Main window
    IUIMainWindow*          m_pmwMainWindow;

    // List of actions that execute user's saved renames/bookmarks
    // The whatsThis variable contains the action name, but with & characters stored as a single &&
    // The whatsThis is therefore used as the lookup for the action name
    QVector<QAction*>       m_qvecpqactActions;

    // Indicates if tooltip should be set to data variant path
    bool                    m_bSetToolTipToDataPath;

    // Keys for loading and saving actions to QSettings
    QString                 m_qstrGroup;
    QString                 m_qstrArray;
    QString                 m_qstrEntry;

public:
    IUIMenuSavesBase(const QString & krqstrName, IUIMainWindow* pmwMainWindow);

protected:
    // Loads and saves actions to QSettings
    void LoadActionsFromQSettings();
    void SaveActionsToQSettings();

    // Creates a QAction from the passed name and data strings
    void CreateAction(const QString & krqstrName, const QString & krqstrData);

    // Returns the action name with & replaced by && so the character displays properly
    QString GenerateActionName(const QString & krqstrName);

    // Reads name from save string
    QString GetName(const QString & krqstrSaveString);

    // Returns the data string for storing in the QAction data variant
    virtual QString GetDataString(const QString & krqstrSaveString) = 0;

    // Returns action save string for saving to QSettings
    virtual QString GetActionSaveString(const QAction* const kpactAction) = 0;

public:
    // Saves new action with passed name
    virtual void SaveAction(const QString & krqstrName) = 0;

    // Updates passed action
    virtual void UpdateAction(QAction* pqactAction, const QString & krqstrName) = 0;

    // Updates name of the action
    void UpdateActionName(QAction* pqactAction, const QString & krqstrName);

    // Rebuilds menu with the actions from the past list widget
    void ReOrganiseMenu(const QListWidget* kpqlwList, QVector<QAction*> & rqvecpqactActionsToDelete);

    // Execute the action with the passed name, returning true if executed successfully
    bool ExecuteAction(const QString & krqstrName);
    bool ExecuteActionCaseInsensitive(const QString & krqstrName);

    // Returns the action with the passed name or nullptr
    QAction* GetAction(const QString & krqstrName);
    QAction* GetActionCaseInsensitive(const QString & krqstrName);

    // Returns list of actions
    QVector<QAction*> & GetActions() {return m_qvecpqactActions;}

private slots:
    // Loads the data variant in the sending QAction
    virtual void LoadData() = 0;
};

#endif // IUIMenuSavesBase_h
