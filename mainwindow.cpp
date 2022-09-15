#include "mainwindow.h"

#include "my_ctype_cast.h"
#include "ui_mainwindow.h"
#include "qdebug.h"

#include "protocol.h"
#include "protocol_test.h"

#include <time.h>       /* time */

extern "C" {

}

#include "smart_assert.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	



    protocolAllTest(time(NULL), 100, (TESTER_CRC | TESTER_ENDIAN | TESTER_CONVERT | TESTER_RAW_P_DMA | TESTER_REED_SOLOMON_ECC | TESTER_CALLBACK_MANAGER | TESTER_PULL_CONTAINER));
    //protocolAllTest(time(NULL), 500, (TESTER_PULL_CONTAINER));
    //__M_assert_test();
    M_Assert_disableExpr({
                             __M_SEND_DEBUG_INFO("debug: %d", 123);
                         });

    //callbackManagerTest();
    //qDebug() << "test: "<<pullContainerTest(123, 100);

    if(PREPROCESSOR_KEYWORD_EQ(return, case)) {
        qDebug() << "return equal to case";
    } else {
        qDebug() << "return not equal to case";
    }

    MY_CTYPE_GET_TYPE(UINT8_TYPE) my_val = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

