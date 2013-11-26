#ifndef DEREGWIDGET_H
#define DEREGWIDGET_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QStringList>

#include "ui_dereg.h"
#include "dbconnection.h"

class CDeregWidget
        : public QWidget
        , public Ui::Dereg
{
    Q_OBJECT

public:
    explicit CDeregWidget(CDBConnection *dbconn, QWidget *parent = 0);

signals:

public slots:
    void SetListView();

    void UpdateView();

private:
    CDBConnection *m_dbconn;
    QSqlQueryModel model;
};

#endif // DEREGWIDGET_H
