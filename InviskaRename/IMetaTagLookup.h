#ifndef IMetaTagLookup_h
#define IMetaTagLookup_h

#include <QHash>
#include <QString>
class QTableWidgetItem;


struct ITagInfo
{
    // Tag Categories
    enum TagCatagory                {Music, Exif, Attrib, Invalid = INT_MAX};

    // Index of the opening and closing bracket
    int                             m_iStartIndex;
    int                             m_iEndIndex;

    // Category of tag from above enum
    TagCatagory                     m_tcatCatagory;

    // Tag ID for the passed tag, specific to the Category
    int                             m_iTagID;
};


class IMetaTagLookup
{
public:
    // For looking up TagID from TagString
    QHash<QString, QString>         m_qhashTagData;

public:
    IMetaTagLookup();

    // Initialise Exif lookup hash depending whehter we're in basic or advaced mode
    void InitExifLookupHash(const bool kbExifAdvancedMode);

    // Sets the ITagInfo Category and TagID values, with the TagID being set to ITagInfo::Invalid if it's not valid
    void LookupTag(ITagInfo & rtagiTagInfo, const QString & krqstrCategory, const QString & krqstrTagCode);

    // Returns the value for the specified tag code
    QString GetValueForTagCode(const QTableWidgetItem* kpqtwiFileItem, const ITagInfo & krtagiTagInfo);

    // Replaces the tag codes in the passed string with the tag value and returns the resulting string
    QString ReplaceTagCodesWithValues(const QString & krqstrString, const QList<ITagInfo> & krqlstReplaceNameTags, const QTableWidgetItem* kpqtwiFileItem);
};

#endif // IMetaTagLookup_h
