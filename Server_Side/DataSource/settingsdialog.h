#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

#include "ui_settingsdialog.h"
#include "config.h"

class CSettingsDialog
        : public QDialog
        , public Ui::SettingsDialog
{
    Q_OBJECT

public:
    CSettingsDialog(TConnConfig *config, QWidget *parent = 0);

private slots:
    void OkSlot();
    void CancelSlot();

private:
    TConnConfig *m_pConfig;
};

#endif // SETTINGSDIALOG_H
