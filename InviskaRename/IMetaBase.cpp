#include "IMetaBase.h"
#include "IComMetaMusic.h"
#include "IMetaTagLookup.h"
#include "IRenameInvalidCharSub.h"

QChar IMetaBase::m_qchSeparatorDate = QChar();
QChar IMetaBase::m_qchSeparatorTime = QChar();
QChar IMetaBase::m_qchSeparatorFraction = QChar();


IMetaBase::IMetaBase()
{
    m_prgqstrTagValues = nullptr;
    m_piReferenceCount = nullptr;
}


IMetaBase::IMetaBase(const int kiNumElements)
{
    m_prgqstrTagValues = new QString[static_cast<unsigned long long>(kiNumElements)];
    m_piReferenceCount = new int;
    *m_piReferenceCount = 1;
}


IMetaBase::IMetaBase(const IMetaBase & krmbaCopyMe)
{
    m_piReferenceCount = krmbaCopyMe.m_piReferenceCount;
    m_prgqstrTagValues = krmbaCopyMe.m_prgqstrTagValues;
    ++(*m_piReferenceCount);
}


IMetaBase::~IMetaBase()
{
    if (m_piReferenceCount != nullptr)
    {
        --(*m_piReferenceCount);
        if (*m_piReferenceCount == 0)
        {
            delete m_piReferenceCount;
            delete[] m_prgqstrTagValues;
        }
    }
}


QString IMetaBase::GetTagValue(const int kiTagID) const
{
    return m_prgqstrTagValues[kiTagID];
}



bool IMetaBase::SetSeparators(const QChar & krqchDate, const QChar & krqchTime, const QChar & krqchFraction)
{
    if (krqchDate != m_qchSeparatorDate || krqchTime != m_qchSeparatorTime || krqchFraction != m_qchSeparatorFraction)
    {
        m_qchSeparatorDate = krqchDate;
        m_qchSeparatorTime = krqchTime;
        m_qchSeparatorFraction = krqchFraction;
        return true;
    }
    return false;
}
