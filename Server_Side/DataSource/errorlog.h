#ifndef ERRORLOG_H
#define ERRORLOG_H

#include <QWidget>

#include "ui_errorlog.h"

class CErrorLog
        : public QWidget
        , public Ui::ErrorLog
{
    Q_OBJECT

public:
    CErrorLog(QWidget *parent = 0);

public slots:
//    void ShowError(QString &errStr);
    void ShowError(QString errStr);

private slots:
    void CloseBtnSlot();
};

#endif // ERRORLOG_H
