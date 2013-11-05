#include "mainwindow.h"

#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mainWidget(this)
{
    setupUi(this);
    SetupGui();
}

MainWindow::~MainWindow()
{
}

void MainWindow::SetupGui()
{
    setCentralWidget(&mainWidget);
    connect(actionSettings, SIGNAL(triggered()), &mainWidget, SLOT(SettingsPanel()));
    connect(actionError_Log, SIGNAL(triggered()), &mainWidget, SLOT(ShowErrLog()));

    connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
}
