#include "mainwindow.h"

#include "my_ctype_cast.h"
#include "ui_mainwindow.h"
#include "qdebug.h"

#include "protocol.h"
#include "protocol_test.h"

#include <time.h>       /* time */

#include "smart_assert.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	



    //protocolAllTest(time(NULL), 500, (TESTER_CRC | TESTER_ENDIAN | TESTER_CONVERT | TESTER_RAW_P_DMA | TESTER_REED_SOLOMON_ECC));
    protocolAllTest(time(NULL), 500, (TESTER_RAW_P_DMA));
    //__M_assert_test();
    M_Assert_disableExpr({
                             __M_SEND_DEBUG_INFO("debug: %d", 123);
                         });
}

MainWindow::~MainWindow()
{
    delete ui;
}

