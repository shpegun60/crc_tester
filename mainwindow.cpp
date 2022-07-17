#include "mainwindow.h"
#include "ui_mainwindow.h"

extern "C" {
#include "crc_test.h"
#include "byte_order.h"
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    crc_test();
    endiansTest();
}

MainWindow::~MainWindow()
{
    delete ui;
}

