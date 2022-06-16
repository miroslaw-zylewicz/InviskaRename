#ifndef IDlgRenameFile_h
#define IDlgRenameFile_h

#include <QDir>
#include "IDlgRenameBase.h"
class IUIFileList;


class IDlgRenameFile : public IDlgRenameBase
{
    Q_OBJECT

private:
    // Reference to QDir from iUIFileList
    QDir &                      m_rqdirDirReader;

public:
    IDlgRenameFile(IUIFileList* puifmFileList, QDir & rqdirDirReader, const QString & krqstrFileName);

public slots:
    // Override accept() function to process settings when Save is pressed
    void accept();
};

#endif // IDlgRenameFile_h
