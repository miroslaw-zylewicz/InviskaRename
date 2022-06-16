#ifndef IMetaMusic_h
#define IMetaMusic_h

#include <QObject>
#include <QHash>
#include "IMetaBase.h"
class IComMetaMusic;
class IRenameInvalidCharSub;


class IMetaMusic : public IMetaBase
{
public:
    // Index in array at which each tag value is stored
    enum MusicTagsIDs                   {Title, Artist, Album, Track, Year, Genre, Comment, RunTime, Channels, SampleRate, BitRate, NumTags};

private:
    // Hash contining valid tag strings and their associated enum value
    static QHash<QString, int>          m_qhashTagLookup;

public:
    IMetaMusic();
    IMetaMusic(IComMetaMusic* pmmuMusicMeta, const IRenameInvalidCharSub & kricsInvalidCharSub);

    // Initialised the tag lookup hash with valid tags and associated tag IDs
    static void InitTagLookupHash();

    // Read tags from passed MusicMeta object and store in string array
    void ReadTags(IComMetaMusic* pmmuMusicMeta, const IRenameInvalidCharSub & kricsInvalidCharSub);

    // Returns tag ID from MusicTagsIDs enum or ITagInfo::Invalid if passed tag string is invalid
    static int GetTagID(const QString & krqstrTagCode);
};

Q_DECLARE_METATYPE(IMetaMusic)
Q_DECLARE_METATYPE(IMetaMusic*)

#endif // IMetaMusic_h
