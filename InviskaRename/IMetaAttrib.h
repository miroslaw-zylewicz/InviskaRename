#ifndef IMetaAttrib_h
#define IMetaAttrib_h

#include <QHash>
class QTableWidgetItem;


class IMetaAttrib
{
public:
    // Index in array at which each tag value is stored
    enum FileAttributeTagsIDs           {CreatedDateTime, CreatedDate, CreatedTime, CreatedDateYYYY, CreatedDateYY, CreatedDateMM, CreatedDateDD, CreatedTimeHH, CreatedTimeMM, CreatedTimeSS,
                                         ModifiedDateTime, ModifiedDate, ModifiedTime, ModifiedDateYYYY, ModifiedDateYY, ModifiedDateMM, ModifiedDateDD, ModifiedTimeHH, ModifiedTimeMM, ModifiedTimeSS,
                                         NumTags};

    // Hash contining valid tag strings and their associated enum value
    static QHash<QString, int>          m_qhashTagLookup;

public:
    // Initialised the tag lookup hash with valid tags and associated tag IDs
    static void InitTagLookupHash();

    // Returns tag value for passed tag ID
    static QString GetTagValue(const QTableWidgetItem* kpqtwiFileItem, const int kiTagID);

    // Returns tag ID from MusicTagsIDs enum or ITagInfo::Invalid if passed tag string is invalid
    static int GetTagID(const QString & krqstrTagCode);

    // Replaces date and time seperators with user specified characters
    static QString ProcessDateTime(QString qstrDateTime);
};

#endif // IMetaAttrib_h
