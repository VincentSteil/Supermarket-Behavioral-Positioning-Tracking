#ifndef CUSTOMERLISTWIDGET_H
#define CUSTOMERLISTWIDGET_H

#include <QListView>

class CCustomerListWidget
        : public QListView
{
    Q_OBJECT

public:
    CCustomerListWidget(QWidget *parent = 0);

public slots:
    // deregisters currently selected users
    void Deregister();

signals:
    void DelCustomers(QStringList idsList);

protected:
    void keyPressEvent(QKeyEvent *event);

private:

};

#endif // CUSTOMERLISTWIDGET_H
