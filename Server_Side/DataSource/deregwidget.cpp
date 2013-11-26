#include "deregwidget.h"

CDeregWidget::CDeregWidget(CDBConnection *dbconn, QWidget *parent)
    : QWidget(parent, Qt::Window)
    , m_dbconn(dbconn)
{
    setupUi(this);

    connect(deregBtn, SIGNAL(clicked()), listView, SLOT(Deregister()));
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(hide()));
    connect(listView, SIGNAL(DelCustomers(QStringList)), m_dbconn, SLOT(DeregCustomers(QStringList)));
}

void CDeregWidget::SetListView()
{
    if(m_dbconn->GetListModel(model) != 0)
        return;

    listView->setModel(&model);
}

void CDeregWidget::UpdateView()
{
    repaint();
}
