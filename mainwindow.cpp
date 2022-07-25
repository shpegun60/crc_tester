#include "mainwindow.h"
#include "ui_mainwindow.h"

extern "C" {
#include "crc_test.h"
#include "byte_order.h"

#include "rawparser_dma.h"
}

#include "smart_assert.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    crc_test();
    endiansTest();
    //__M_assert_test();
    M_Assert_disableExpr({
                             __M_SEND_DEBUG_INFO("debug: %d", 123);
                         });
}

MainWindow::~MainWindow()
{
    delete ui;
}

