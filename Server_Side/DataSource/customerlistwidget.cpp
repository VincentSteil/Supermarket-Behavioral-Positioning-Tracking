#include "customerlistwidget.h"

#include <QKeyEvent>
#include <QStringList>

CCustomerListWidget::CCustomerListWidget(QWidget *parent)
    : QListView(parent)
{
}

void CCustomerListWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
        Deregister();
}

void CCustomerListWidget::Deregister()
{
    QStringList idsList;
//    QAbstractItemModel *abmodel = this->model();
    QItemSelectionModel * model = this->selectionModel();
    QModelIndexList list = model->selectedIndexes();

    if(list.size() < 1)
        return;

//    QModelIndex previous = list.first();

    for(int i = 0; i < list.size(); i++){
        idsList.push_back(list[i].data().toString());
    }

    emit DelCustomers(idsList);
}
