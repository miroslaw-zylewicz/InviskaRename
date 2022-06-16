#ifndef IRenameLegacySave_h
#define IRenameLegacySave_h

#include <QString>

class IRenameLegacySave
{
public:
    // Converts the passed legacy saved string to the new format save string
    static void ConvertLegacySave(QString & rqstrSaveString);

private:
    // Functions for saving the data to, and writing it from, a QString for strong in the settings file
    static void Filter(const QString & krqstrSaveString, const QString & krqstrSection, QStringList & rqstrlSettings);
    static void Name(const QString & krqstrSaveString, const QString & krqstrSection, QStringList & rqstrlSettings);
    static void Number(const QString & krqstrSaveString, const QString & krqstrSection, QStringList & rqstrlSettings);

    // Returns seperator for items in save string, and separator for sections e.g. name, extension, numbering
    static QChar   SaveStringSeparator()        {return '/';}
    static QString SaveStringSectionSeparator() {return "/|/";}
};


#endif // IRenameLegacySave_h
