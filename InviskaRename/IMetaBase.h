#ifndef IMetaBase_h
#define IMetaBase_h

#include <QObject>


class IMetaBase
{
protected:
    // Array of tag values indexed with enum from inheriting class
    QString*                            m_prgqstrTagValues;

    // Number of objects referencing the tag value array so it's not deleted until it's unreferenced
    int*                                m_piReferenceCount;

    // Separators to use in substituted tag values
    static QChar                        m_qchSeparatorDate;
    static QChar                        m_qchSeparatorTime;
    static QChar                        m_qchSeparatorFraction;

public:
    IMetaBase();
    IMetaBase(const int kiNumElements);
    IMetaBase(const IMetaBase & krmbaCopyMe);
    virtual ~IMetaBase();

    // Returns tag value for passed tag ID
    virtual QString GetTagValue(const int kiTagID) const;

    // Separators accessors
    static QChar &  GetSeparatorDate()                                          {return m_qchSeparatorDate;}
    static QChar &  GetSeparatorTime()                                          {return m_qchSeparatorTime;}
    static QChar &  GetSeparatorFraction()                                      {return m_qchSeparatorFraction;}
    static void     SetSeparatorDate(const QChar & krqchSeparatorDate)          {m_qchSeparatorDate = krqchSeparatorDate;}
    static void     SetSeparatorTime(const QChar & krqchSeparatorTime)          {m_qchSeparatorTime = krqchSeparatorTime;}
    static void     SetSeparatorFraction(const QChar & krqchSeparatorFraction)  {m_qchSeparatorFraction = krqchSeparatorFraction;}
    static bool     SetSeparators(const QChar & krqchDate, const QChar & krqchTime, const QChar & krqchFraction);
};

Q_DECLARE_METATYPE(IMetaBase)
Q_DECLARE_METATYPE(IMetaBase*)

#endif // IMetaBase_h
