#include "IMetaMusic.h"
#include "IComMetaMusic.h"
#include "IMetaTagLookup.h"
#include "IRenameInvalidCharSub.h"

QHash<QString, int> IMetaMusic::m_qhashTagLookup = QHash<QString, int>();


IMetaMusic::IMetaMusic() : IMetaBase()
{

}


IMetaMusic::IMetaMusic(IComMetaMusic* pmmuMusicMeta, const IRenameInvalidCharSub & kricsInvalidCharSub) : IMetaBase(NumTags)
{
    ReadTags(pmmuMusicMeta, kricsInvalidCharSub);
}


void IMetaMusic::InitTagLookupHash()
{
    m_qhashTagLookup["title"]       = Title;
    m_qhashTagLookup["artist"]      = Artist;
    m_qhashTagLookup["album"]       = Album;
    m_qhashTagLookup["track"]       = Track;
    m_qhashTagLookup["year"]        = Year;
    m_qhashTagLookup["genre"]       = Genre;
    m_qhashTagLookup["comment"]     = Comment;
    m_qhashTagLookup["runtime"]     = RunTime;
    m_qhashTagLookup["channels"]    = Channels;
    m_qhashTagLookup["samplerate"]  = SampleRate;
    m_qhashTagLookup["bitrate"]     = BitRate;
}


void IMetaMusic::ReadTags(IComMetaMusic* pmmuMusicMeta, const IRenameInvalidCharSub & kricsInvalidCharSub)
{
    m_prgqstrTagValues[Title]       = kricsInvalidCharSub.PerformSubstitution(pmmuMusicMeta->GetTitle());
    m_prgqstrTagValues[Artist]      = kricsInvalidCharSub.PerformSubstitution(pmmuMusicMeta->GetArtist());
    m_prgqstrTagValues[Album]       = kricsInvalidCharSub.PerformSubstitution(pmmuMusicMeta->GetAlbum());
    m_prgqstrTagValues[Track]       = pmmuMusicMeta->GetTrackTwoDigit();
    m_prgqstrTagValues[Year]        = pmmuMusicMeta->GetYear();
    m_prgqstrTagValues[Genre]       = kricsInvalidCharSub.PerformSubstitution(pmmuMusicMeta->GetGenre());
    m_prgqstrTagValues[Comment]     = kricsInvalidCharSub.PerformSubstitution(pmmuMusicMeta->GetComment());
    m_prgqstrTagValues[RunTime]     = pmmuMusicMeta->GetLengthMMSS(m_qchSeparatorTime);
    m_prgqstrTagValues[Channels]    = pmmuMusicMeta->GetChannels();
    m_prgqstrTagValues[SampleRate]  = pmmuMusicMeta->GetSampleRate();
    m_prgqstrTagValues[BitRate]     = pmmuMusicMeta->GetBitRate();
}


int IMetaMusic::GetTagID(const QString & krqstrTagCode)
{
    QHash<QString, int>::iterator itTagID = m_qhashTagLookup.find(krqstrTagCode);
    if (itTagID == m_qhashTagLookup.end())
        return ITagInfo::Invalid;
    return *itTagID;
}
