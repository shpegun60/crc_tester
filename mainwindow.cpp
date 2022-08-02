#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "qdebug.h"

extern "C" {
#include "crc_test.h"
#include "byte_order.h"

#include "rawparser_dma.h"
#include "raw_parser_dma_test.h"

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
    reg pos = 0;

    //crc_test();
    //endiansTest();
    //__M_assert_test();
    M_Assert_disableExpr({
                             __M_SEND_DEBUG_INFO("debug: %d", 123);
                         });

    //templatetest();

    TEMPLATE(convertWrite_MSB, f32)(data, &pos, 123.456);
    TEMPLATE(convertWrite_MSB, u16)(data, &pos, 853);
    TEMPLATE(CAT_ENDIAN(convertWrite), u32)(data, &pos, 1122334455);

    u64 tmpuni = 1234567890ULL;
    TEMPLATE(CAT_ENDIAN(convertWrite), uni)(sizeof(u64), data, &pos, (u8*)&tmpuni);


    pos = 0;
    f32 tmp = TEMPLATE(convertRead_MSB, f32)(data, &pos);
    u16 tmp16 = TEMPLATE(convertRead_MSB, u16)(data, &pos);
    u64 tmp64 = TEMPLATE(CAT_ENDIAN(convertRead), u32)(data, &pos);

    u64 tmpunitest;
    TEMPLATE(CAT_ENDIAN(convertRead), uni)(sizeof(u64), data, &pos, (u8*)&tmpunitest);


//    qDebug() << "---------------------------------------------------";
//    qDebug() << tmp;
//    qDebug() << tmp16;
//    qDebug() << tmp64;
//    qDebug() << tmpunitest;
//    qDebug() << pos;
//    qDebug() << "long double "<<sizeof(long double);
//    qDebug() << "---------------------------------------------------";
//    qDebug() << "convert test exit with: "<< convertTest();

    rawParserDmaTest();
}

MainWindow::~MainWindow()
{
    delete ui;
}

