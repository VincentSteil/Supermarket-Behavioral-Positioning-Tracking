#include "errorlog.h"

CErrorLog::CErrorLog(QWidget *parent)
    : QWidget(parent, Qt::Window)
{
    setupUi(this);

    connect(closeBtn, SIGNAL(clicked()), this, SLOT(CloseBtnSlot()));
}

void CErrorLog::CloseBtnSlot()
{
    hide();
}

//void CErrorLog::ShowError(QString &errStr)
//{
//    new QListWidgetItem(errStr, listWidget);
//}

void CErrorLog::ShowError(QString errStr)
{
    new QListWidgetItem(errStr, listWidget);
}
