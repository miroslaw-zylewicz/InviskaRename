#include <QStringList>
#include "IRenameLegacySave.h"
#include "IUIRenameFilter.h"


void IRenameLegacySave::ConvertLegacySave(QString & rqstrSaveString)
{
    int iIndex = 0;
    QStringList qstrlSections = rqstrSaveString.split(SaveStringSectionSeparator());

    QStringList rqstrlSettings;
    Filter(qstrlSections.at(iIndex), "SETFilter", rqstrlSettings);
    Name(qstrlSections.at(++iIndex), "SETName", rqstrlSettings);
    Name(qstrlSections.at(++iIndex), "SETExten", rqstrlSettings);
    Number(qstrlSections.at(++iIndex), "SETNumber", rqstrlSettings);

    rqstrSaveString = rqstrlSettings.join("");
}


void IRenameLegacySave::Filter(const QString & krqstrSaveString, const QString & krqstrSection, QStringList & rqstrlSettings)
{
    int iIndex = 0;
    QStringList qstrlSettings;
    QStringList qstrlValues = krqstrSaveString.split(SaveStringSeparator());
    rqstrlSettings.push_back(QString("<Name>%1</Name>").arg(qstrlValues.at(++iIndex)));

    int iRenameElements = qstrlValues.at(++iIndex).toInt();
    switch (iRenameElements)
    {
    case IUIRenameFilter::RenameFoldersOnly         :   qstrlSettings.push_back("<RB02RenameFoldersOnly>1</RB02RenameFoldersOnly>");
                                                        break;

    case IUIRenameFilter::RenameFilesAndFolders     :   qstrlSettings.push_back("<RB03RenameFilesAndFolders>1</RB03RenameFilesAndFolders>");
                                                        break;

    case IUIRenameFilter::RenameSelectedItems       :   qstrlSettings.push_back("<RB04RenameSelectedItemsOnly>1</RB04RenameSelectedItemsOnly>");
                                                        break;

    case IUIRenameFilter::RenameFilesWithExtension  :   qstrlSettings.push_back("<RB05RenameFilesWithExtension>1</RB05RenameFilesWithExtension>");
    }


    if (qstrlValues.at(++iIndex).isEmpty() == false)
        qstrlSettings.push_back(QString("<LE01RenameFilesWithExtension>%1</LE01RenameFilesWithExtension>").arg(qstrlValues.at(iIndex)));

    if (qstrlSettings.isEmpty() == false)
    {
        rqstrlSettings.push_back(QString("<%1>").arg(krqstrSection));
        rqstrlSettings.append(qstrlSettings);
        rqstrlSettings.push_back(QString("</%1>").arg(krqstrSection));
    }
}


void IRenameLegacySave::Name(const QString & krqstrSaveString, const QString & krqstrSection, QStringList & rqstrlSettings)
{
    int iIndex = 0;
    QStringList qstrlSettings;
    QStringList qstrlValues = krqstrSaveString.split(SaveStringSeparator());

    if (qstrlValues.at(++iIndex).toInt())
        qstrlSettings.push_back("<CB01ReplaceName>1</CB01ReplaceName>");
    if (qstrlValues.at(++iIndex).isEmpty() == false)
        qstrlSettings.push_back(QString("<LE01ReplaceName>%1</LE01ReplaceName>").arg(qstrlValues.at(iIndex)));

    if (qstrlValues.at(++iIndex).toInt())
        qstrlSettings.push_back("<CB02ReplaceTheText>1</CB02ReplaceTheText>");
    if (qstrlValues.at(++iIndex).isEmpty() == false)
        qstrlSettings.push_back(QString("<LE02ReplaceTheText>%1</LE02ReplaceTheText>").arg(qstrlValues.at(iIndex)));
    if (qstrlValues.at(++iIndex).isEmpty() == false)
        qstrlSettings.push_back(QString("<LE03ReplaceTheTextWith>%1</LE03ReplaceTheTextWith>").arg(qstrlValues.at(iIndex)));

    if (qstrlValues.at(++iIndex).toInt())
        qstrlSettings.push_back("<CB03InsertTheText>1</CB03InsertTheText>");
    if (qstrlValues.at(++iIndex).isEmpty() == false)
        qstrlSettings.push_back(QString("<LE04InsertTheText>%1</LE04InsertTheText>").arg(qstrlValues.at(iIndex)));
    if (qstrlValues.at(++iIndex).toInt() != -1)
        qstrlSettings.push_back(QString("<LE05InsertTheTextAtPos>%1</LE05InsertTheTextAtPos>").arg(qstrlValues.at(iIndex)));

    if (qstrlValues.at(++iIndex).toInt())
        qstrlSettings.push_back("<CB04InsertAtStart>1</CB04InsertAtStart>");
    if (qstrlValues.at(++iIndex).isEmpty() == false)
        qstrlSettings.push_back(QString("<LE06InsertAtStart>%1</LE06InsertAtStart>").arg(qstrlValues.at(iIndex)));

    if (qstrlValues.at(++iIndex).toInt())
        qstrlSettings.push_back("<CB05InsertAtEnd>1</CB05InsertAtEnd>");
    if (qstrlValues.at(++iIndex).isEmpty() == false)
        qstrlSettings.push_back(QString("<LE07InsertAtEnd>%1</LE07InsertAtEnd>").arg(qstrlValues.at(iIndex)));

    if (qstrlValues.at(++iIndex).toInt())
        qstrlSettings.push_back("<CB06CropAtPos>1</CB06CropAtPos>");
    if (qstrlValues.at(++iIndex).toInt() != -1)
        qstrlSettings.push_back(QString("<LE08CropAtPos>%1</LE08CropAtPos>").arg(qstrlValues.at(iIndex)));
    if (qstrlValues.at(++iIndex).toInt() != -1)
        qstrlSettings.push_back(QString("<LE09CropAtPosNextN>%1</LE09CropAtPosNextN>").arg(qstrlValues.at(iIndex)));

    if (qstrlValues.at(++iIndex).toInt())
        qstrlSettings.push_back("<CB07LeftCropNChar>1</CB07LeftCropNChar>");
    if (qstrlValues.at(++iIndex).toInt() != -1)
        qstrlSettings.push_back(QString("<LE10LeftCropNChar>%1</LE10LeftCropNChar>").arg(qstrlValues.at(iIndex)));

    if (qstrlValues.at(++iIndex).toInt())
        qstrlSettings.push_back("<CB08RightCropNChar>1</CB08RightCropNChar>");
    if (qstrlValues.at(++iIndex).toInt() != -1)
        qstrlSettings.push_back(QString("<LE11RightCropNChar>%1</LE11RightCropNChar>").arg(qstrlValues.at(iIndex)));

    if (qstrlValues.at(++iIndex).toInt() != 0)
        qstrlSettings.push_back(QString("<COM01ChangeCase>%1</COM01ChangeCase>").arg(qstrlValues.at(iIndex)));

    if (qstrlSettings.isEmpty() == false)
    {
        rqstrlSettings.push_back(QString("<%1>").arg(krqstrSection));
        rqstrlSettings.append(qstrlSettings);
        rqstrlSettings.push_back(QString("</%1>").arg(krqstrSection));
    }
}


void IRenameLegacySave::Number(const QString & krqstrSaveString, const QString & krqstrSection, QStringList & rqstrlSettings)
{
    int iIndex = 0;
    QStringList qstrlSettings;
    QStringList qstrlValues = krqstrSaveString.split(SaveStringSeparator());

    int iNumberingType  = qstrlValues.at(++iIndex).toInt();
    switch (iNumberingType)
    {
    case 1  :   qstrlSettings.push_back("<RB02NumberingAfterName>1</RB02NumberingAfterName>");
                break;

    case 2  :   qstrlSettings.push_back("<RB03NumberingBeforeName>1</RB03NumberingBeforeName>");
                break;

    case 3  :   qstrlSettings.push_back("<RB04NumberingAtPos>1</RB04NumberingAtPos>");
    }

    if (qstrlValues.at(++iIndex).toInt() != -1)
        qstrlSettings.push_back(QString("<LE01NumberingAtPos>%1</LE01NumberingAtPos>").arg(qstrlValues.at(iIndex)));
    if (qstrlValues.at(++iIndex).toInt() != 1)
        qstrlSettings.push_back(QString("<LE02NumberingStartNum>%1</LE02NumberingStartNum>").arg(qstrlValues.at(iIndex)));
    if (qstrlValues.at(++iIndex).toInt() != 1)
        qstrlSettings.push_back(QString("<LE03NumberingIncrement>%1</LE03NumberingIncrement>").arg(qstrlValues.at(iIndex)));

    if (qstrlValues.at(++iIndex).toInt() == 0)
        qstrlSettings.push_back(QString("<RB06NumberingZeroFillSpec>1</RB06NumberingZeroFillSpec>"));
    if (qstrlValues.at(++iIndex).toInt() != 1)
        qstrlSettings.push_back(QString("<LE04NumberingZeroFill>%1</LE04NumberingZeroFill>").arg(qstrlValues.at(iIndex)));

    if (qstrlSettings.isEmpty() == false)
    {
        rqstrlSettings.push_back(QString("<%1>").arg(krqstrSection));
        rqstrlSettings.append(qstrlSettings);
        rqstrlSettings.push_back(QString("</%1>").arg(krqstrSection));
    }
}
