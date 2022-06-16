#ifndef IDlgRenameMenuItem_h
#define IDlgRenameMenuItem_h

#include <QDir>
#include "IDlgRenameBase.h"
class QListWidgetItem;
class IDlgOrganiseMenu;


class IDlgRenameMenuItem : public IDlgRenameBase
{
    Q_OBJECT

private:
    // Organise menu dialog containing list of existing entries
    IDlgOrganiseMenu*           m_pdlgOrganiseDialog;

    // List widget item to rename (menu QAction is in the data variant)
    QListWidgetItem*            m_pqlwiItem;

public:
    IDlgRenameMenuItem(IDlgOrganiseMenu* pdlgOrganiseDialog, QListWidgetItem* pqlwiItem);

public slots:
    // Override accept() function to process settings when Save is pressed
    void accept();
};

#endif // IDlgRenameMenuItem_h
