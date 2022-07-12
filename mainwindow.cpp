#include "mainwindow.h"
#include "ui_mainwindow.h"

extern "C" {
#include "crc/crc_test.h"
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    crc_test();
}

MainWindow::~MainWindow()
{
    delete ui;
}

