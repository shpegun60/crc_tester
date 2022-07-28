#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"

extern "C" {
#include "crc_test.h"
#include "byte_order.h"

#include "rawparser_dma.h"

//#define CONVERT_TEST_DISABLE
#include "convert.h"
}

#include "smart_assert.h"
#include "templateTest.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	
    quint8 data[100];
    quint16 pos = 0;

    crc_test();
    //endiansTest();
    //__M_assert_test();
    M_Assert_disableExpr({
                             __M_SEND_DEBUG_INFO("debug: %d", 123);
                         });

    //templatetest();



    TEMPLATE(convertWriteMSB, f32)(data, &pos, 123.456);
    TEMPLATE(convertWriteMSB, u16)(data, &pos, 853);
    TEMPLATE(convertWriteLSB, u64)(data, &pos, 1122334455);
    pos = 0;
    f32 tmp = TEMPLATE(convertReadMSB, f32)(data, &pos);
    u16 tmp16 = TEMPLATE(convertReadMSB, u16)(data, &pos);
    u64 tmp64 = TEMPLATE(convertReadLSB, u64)(data, &pos);

    qDebug() << tmp;
    qDebug() << tmp16;
    qDebug() << tmp64;
    qDebug() << pos;
    qDebug() << "long double "<<sizeof(long double);


    qDebug() << "test: "<< convertTest();
}

MainWindow::~MainWindow()
{
    delete ui;
}

