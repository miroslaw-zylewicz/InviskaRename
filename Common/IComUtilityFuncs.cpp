#include <QtWidgets>
#include "IComUtilityFuncs.h"


int IComUtilityFuncs::GetTableRowHeightFitToFont(QTableWidget* pqtwTable)
{
    const int kiMinRowHeight = 17;

    QFontMetrics qfmFontMetrics(pqtwTable->font());
    int iRowHeight = qfmFontMetrics.height()+2;
    if (iRowHeight < kiMinRowHeight)
        iRowHeight = kiMinRowHeight;
    return iRowHeight;
}
