#include <QtWidgets>
#include "IUIRenameNumber.h"
#include "IUIRename.h"
#include "IUIFileList.h"
#include "IUIMainWindow.h"


IUIRenameNumber::IUIRenameNumber(IUIRename* puirRenameUI, const int kiRenameElement, const int kiTabID) : IUIRenameTabBase(puirRenameUI, kiRenameElement, kiTabID)
{
    m_qstrLineEditDefault = "1";

    setupUi(this);
    m_pqgbNumberingType->resize(m_pqgbNumberingType->width(), m_pqgbNumberingType->sizeHint().height());
    m_pqgbNumberingZeroFill->resize(m_pqgbNumberingZeroFill->width(), m_pqgbNumberingType->sizeHint().height());
    m_pqgbNumberingIncrement->resize(m_pqgbNumberingIncrement->width(), m_pqgbNumberingType->sizeHint().height());

    SetValidators();
    CreateConnections();

    QFontMetrics qfmFontMetrics(m_pqleNumberingZeroFill->font());
    int iLineEditWidth = qfmFontMetrics.width("000000000000");
    m_pqleNumberingAtPos->setMinimumWidth(iLineEditWidth);
    m_pqleNumberingStartNum->setMinimumWidth(iLineEditWidth);
    m_pqleNumberingIncrement->setMinimumWidth(iLineEditWidth);
    m_pqleNumberingZeroFill->setMinimumWidth(iLineEditWidth);
    m_pqleNumberingAtPos->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    m_pqleNumberingStartNum->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    m_pqleNumberingIncrement->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    m_pqleNumberingZeroFill->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    //m_pqgbNumberingType
    //m_pqgbNumberingIncrement
    //m_pqgbNumberingZeroFill

    ///m_pqgbNumberingType->sizeHint();
    //m_pqgbNumberingIncrement->sets(m_pqgbNumberingType->sizeHint());
    //m_pqgbNumberingZeroFill->setSizeHint(m_pqgbNumberingType->sizeHint());
}


void IUIRenameNumber::SetValidators()
{
    QIntValidator* pivalIntOnlyValidator = IUIMainWindow::GetMainWindow()->GetIntOnlyValidator();

    m_pqleNumberingAtPos->setValidator(pivalIntOnlyValidator);
    m_pqleNumberingStartNum->setValidator(pivalIntOnlyValidator);
    m_pqleNumberingIncrement->setValidator(pivalIntOnlyValidator);
    m_pqleNumberingZeroFill->setValidator(pivalIntOnlyValidator);
}


void IUIRenameNumber::CreateConnections()
{
    connect(m_pqrbNumberingNoNumber,        SIGNAL(toggled(bool)),                  this,               SLOT(RadioSettingsChanged(bool)));
    connect(m_pqrbNumberingBeforeName,      SIGNAL(toggled(bool)),                  this,               SLOT(RadioSettingsChanged(bool)));
    connect(m_pqrbNumberingAfterName,       SIGNAL(toggled(bool)),                  this,               SLOT(RadioSettingsChanged(bool)));
    connect(m_pqrbNumberingAtPos,           SIGNAL(toggled(bool)),                  this,               SLOT(RadioSettingsChanged(bool)));
    connect(m_pqleNumberingAtPos,           SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChanged()));

    connect(m_pqleNumberingStartNum,        SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChanged()));
    connect(m_pqleNumberingIncrement,       SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChanged()));
    connect(m_pqleNumberingZeroFill,        SIGNAL(textChanged(const QString &)),   this,               SLOT(SettingsChanged()));
    connect(m_pqrbNumberingZeroFillAuto,    SIGNAL(toggled(bool)),                  this,               SLOT(RadioSettingsChanged(bool)));
    connect(m_pqrbNumberingZeroFillSpec,    SIGNAL(toggled(bool)),                  this,               SLOT(RadioSettingsChanged(bool)));

    connect(m_pqrbNumberingNoNumber,        SIGNAL(toggled(bool)),                  this,               SLOT(EnableDisableZeroFillLineEdit()));
    connect(m_pqrbNumberingZeroFillSpec,    SIGNAL(toggled(bool)),                  this,               SLOT(EnableDisableZeroFillLineEdit()));

    connect(m_pqleNumberingAtPos,           SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(ClearLastActiveLineEdit()));
    connect(m_pqleNumberingStartNum,        SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(ClearLastActiveLineEdit()));
    connect(m_pqleNumberingIncrement,       SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(ClearLastActiveLineEdit()));
    connect(m_pqleNumberingZeroFill,        SIGNAL(GotFocus()),                     m_puirRenameUI,     SLOT(ClearLastActiveLineEdit()));
}


void IUIRenameNumber::RadioSettingsChanged(const bool kbEnabled)
{
    if (kbEnabled == true)
        SettingsChanged();
}


void IUIRenameNumber::EnableDisableZeroFillLineEdit()
{
    if (m_pqrbNumberingNoNumber->isChecked() == false && m_pqrbNumberingZeroFillSpec->isChecked() == true)
    {
        m_pqleNumberingZeroFill->setEnabled(true);
        if (qobject_cast<QRadioButton*>(sender()) == m_pqrbNumberingZeroFillSpec)
            m_pqleNumberingZeroFill->setFocus();
    }
    else
    {
        m_pqleNumberingZeroFill->setEnabled(false);
    }
}


void IUIRenameNumber::SettingsChanged()
{
    IUIMainWindow::GetMainWindow()->GetFileListUI()->GeneratePreviewName();
}


void IUIRenameNumber::DisableAllSettings()
{
    m_pqrbNumberingNoNumber->setChecked(true);
}


void IUIRenameNumber::ClearAll()
{
    m_pqrbNumberingNoNumber->setChecked(true);
    m_pqleNumberingAtPos->clear();
    m_pqleNumberingStartNum->setText(m_qstrLineEditDefault);
    m_pqleNumberingIncrement->setText(m_qstrLineEditDefault);

    m_pqrbNumberingZeroFillAuto->setChecked(true);
    m_pqleNumberingZeroFill->setText(m_qstrLineEditDefault);
}


void IUIRenameNumber::InitNumberingVals(const int kiNumFilesToRename)
{
    if (m_pqrbNumberingNoNumber->isChecked())
        return;

    m_iCurrentNumber    = m_pqleNumberingStartNum->text().toInt();
    m_iIncrement        = m_pqleNumberingIncrement->text().toInt();
    m_iNumberingAtPos   = m_pqleNumberingAtPos->text().isEmpty() ? INT_MAX : m_pqleNumberingAtPos->text().toInt();;

    if (m_pqrbNumberingZeroFillAuto->isChecked())
    {
        int iMaxNumber = m_iCurrentNumber + (m_iIncrement * (kiNumFilesToRename-1));
        iMaxNumber = abs(iMaxNumber);
        m_iNumberCharWidth =   (iMaxNumber < 10 ? 1 :
                               (iMaxNumber < 100 ? 2 :
                               (iMaxNumber < 1000 ? 3 :
                               (iMaxNumber < 10000 ? 4 :
                               (iMaxNumber < 100000 ? 5 :
                               (iMaxNumber < 1000000 ? 6 :
                               (iMaxNumber < 10000000 ? 7 :
                               (iMaxNumber < 100000000 ? 8 :
                               (iMaxNumber < 1000000000 ? 9 :
                               10)))))))));
    }
    else
    {
        m_iNumberCharWidth  = m_pqleNumberingZeroFill->text().toInt() + 1;
    }
}


void IUIRenameNumber::GenerateName(QString & rqstrName)
{
    if (m_pqrbNumberingNoNumber->isChecked())
        return;

    QString qstrNumber = QString("%1").arg(m_iCurrentNumber, m_iNumberCharWidth, 10, QChar('0'));

    if (m_pqrbNumberingAfterName->isChecked())
    {
        rqstrName.insert(rqstrName.length(), qstrNumber);
    }
    else if (m_pqrbNumberingBeforeName->isChecked())
    {
        rqstrName.insert(0, qstrNumber);
    }
    else //if (m_pqrbNumberingAtPos->isChecked())
    {
        if (m_iNumberingAtPos <= rqstrName.length())
            rqstrName.insert(m_iNumberingAtPos, qstrNumber);
    }

    m_iCurrentNumber += m_iIncrement;
}


void IUIRenameNumber::SaveSettings(const QString & krqstrSection, QStringList & rqstrlSettings)
{
    QStringList qstrlSettings;

    /* if   (m_pqrbNumberingNoNumber->isChecked())      - Default Value - no need to save
        qstrlSettings.push_back("<RB01NumberingNoNumber>1</RB01NumberingNoNumber>");*/
    if      (m_pqrbNumberingAfterName->isChecked())
        qstrlSettings.push_back("<RB02NumberingAfterName>1</RB02NumberingAfterName>");
    else if (m_pqrbNumberingBeforeName->isChecked())
        qstrlSettings.push_back("<RB03NumberingBeforeName>1</RB03NumberingBeforeName>");
    else if (m_pqrbNumberingAtPos->isChecked())
        qstrlSettings.push_back("<RB04NumberingAtPos>1</RB04NumberingAtPos>");

    /* if   (m_pqrbNumberingZeroFillAuto->isChecked())  - Default Value - no need to save
        qstrlSettings.push_back("<RB05NumberingZeroFillAuto>1</RB05NumberingZeroFillAuto>");*/
    if      (m_pqrbNumberingZeroFillSpec->isChecked())
        qstrlSettings.push_back("<RB06NumberingZeroFillSpec>1</RB06NumberingZeroFillSpec>");

    if (m_pqleNumberingAtPos->text().isEmpty() == false)
        qstrlSettings.push_back(QString("<LE01NumberingAtPos>%1</LE01NumberingAtPos>").arg(m_pqleNumberingAtPos->text()));
    if (m_pqleNumberingStartNum->text() != m_qstrLineEditDefault)
        qstrlSettings.push_back(QString("<LE02NumberingStartNum>%1</LE02NumberingStartNum>").arg(m_pqleNumberingStartNum->text()));
    if (m_pqleNumberingIncrement->text() != m_qstrLineEditDefault)
        qstrlSettings.push_back(QString("<LE03NumberingIncrement>%1</LE03NumberingIncrement>").arg(m_pqleNumberingIncrement->text()));
    if (m_pqleNumberingZeroFill->text() != m_qstrLineEditDefault)
        qstrlSettings.push_back(QString("<LE04NumberingZeroFill>%1</LE04NumberingZeroFill>").arg(m_pqleNumberingZeroFill->text()));

    if (qstrlSettings.isEmpty() == false)
    {
        rqstrlSettings.push_back(QString("<%1>").arg(krqstrSection));
        rqstrlSettings.append(qstrlSettings);
        rqstrlSettings.push_back(QString("</%1>").arg(krqstrSection));
    }
}


void IUIRenameNumber::RestoreSettings(const QString & krqstrSettings, int & riIndex)
{
    QString qstrTag, qstrValue;
    while (m_puirRenameUI->ReadSettingsValue(krqstrSettings, riIndex, qstrTag, qstrValue) == true)
    {
        if      (qstrTag.startsWith("RB"))
            RestoreRadioBox(qstrTag);
        else if (qstrTag.startsWith("LE"))
            RestoreLineEdit(qstrTag, qstrValue);
    }
}


void IUIRenameNumber::RestoreRadioBox(const QString & krqstrTag)
{
    int iCheckBoxID = krqstrTag.mid(2, 2).toInt();
    switch (iCheckBoxID)
    {
    case 1  :   m_pqrbNumberingNoNumber->setChecked(true);      // RB01NumberingNoNumber
                break;
    case 2  :   m_pqrbNumberingAfterName->setChecked(true);     // RB02NumberingAfterName
                break;
    case 3  :   m_pqrbNumberingBeforeName->setChecked(true);    // RB03NumberingBeforeName
                break;
    case 4  :   m_pqrbNumberingAtPos->setChecked(true);         // RB04NumberingAtPos
                break;
    case 5  :   m_pqrbNumberingZeroFillAuto->setChecked(true);  // RB05NumberingZeroFillAuto
                break;
    case 6  :   m_pqrbNumberingZeroFillSpec->setChecked(true);  // RB06NumberingZeroFillSpec
    }
}


void IUIRenameNumber::RestoreLineEdit(const QString & krqstrTag, const QString & krqstrValue)
{
    int iCheckBoxID = krqstrTag.mid(2, 2).toInt();
    switch (iCheckBoxID)
    {
    case 1  :   m_pqleNumberingAtPos->setText(krqstrValue);     // LE01NumberingAtPos
                break;
    case 2  :   m_pqleNumberingStartNum->setText(krqstrValue);  // LE02NumberingStartNum
                break;
    case 3  :   m_pqleNumberingIncrement->setText(krqstrValue); // LE03NumberingIncrement
                break;
    case 4  :   m_pqleNumberingZeroFill->setText(krqstrValue);  // LE04NumberingZeroFill
                break;
    }
}
