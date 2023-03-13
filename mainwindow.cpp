#include "mainwindow.h"


#include "my_ctype_cast.h"
#include "ui_mainwindow.h"
#include "qdebug.h"

#include "protocol.h"
#include "protocol_test.h"

#include <time.h>       /* time */

extern "C" {
    #include "raw_parser_it_test.h"
    #include "entity_manager.h"
    #include "convert.h"
}

#include "smart_assert.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	



    protocolAllTest(time(NULL), 100, (TESTER_CRC | TESTER_ENDIAN | TESTER_CONVERT | TESTER_RAW_P_DMA | TESTER_REED_SOLOMON_ECC | TESTER_CALLBACK_MANAGER | TESTER_PULL_CONTAINER | TESTER_RAW_P_IT | TESTER_RAW_P_GEN));
    //protocolAllTest(time(NULL), 10, (TESTER_ENDIAN | TESTER_CONVERT));
//    //protocolAllTest(time(NULL), 100, (TESTER_RAW_P_IT));

//    //__M_assert_test();
    M_Assert_disableExpr({
                             __M_DBG_FILE(stdout, "debug: %d", 123);
                         });


    int i = 0;
//    M_Assert_WarningElseSaveCheck((i != 0), tmpFuncTrue(), tmpFuncFalse(), M_EMPTY, "Msg: aaaaaaaaaaaaaaa");
//    M_Assert_Warning((i == 0), , , "Msg: aaaaaaaaaaaaaaa");
//    M_Assert_Break((i == 0), M_EMPTY, M_EMPTY, M_LIB_DATA_DEF "Msg: aaaaaaaaaaaaaaa, %d, %d", 1,"entityFramework", 456, 789);

//    //callbackManagerTest();
//    //qDebug() << "test: "<<pullContainerTest(123, 100);

//    //qDebug() << "test: "<<rawParserItTest(time(NULL), 100, 1);



//    if(PREPROCESSOR_KEYWORD_EQ(return, case)) {
//        qDebug() << "return equal to case";
//    } else {
//        qDebug() << "return not equal to case";
//    }

    //MY_CTYPE_GET_TYPE(UINT8_TYPE) my_val = 0;

//    int val = 1;
//    switch(val)
//    {

//        if(0) {
//            case 0:
//                qDebug() << "case 0";
//        }

//        if(0) {
//            case 1:
//                qDebug() << "case 1";
//        }

//        if(0) {
//            case 2:
//                qDebug() << "case 2";
//        }

//        if(0) {
//            case 3:
//                qDebug() << "case 3";
//        }

//    }

//    //int val1 = 55;
//    int val = 1;
//    int a = ({
//                int b = 123;
//                if(val == 1) {
//                    b = 345;
//                     // bla bla bla
//                }

//                b;
//            });

//    qDebug() << "!!!!!!!!!!!!!!!!!a " << a;
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tmpFuncTrue()
{
    int i = 0;
    return;
}

void MainWindow::tmpFuncFalse()
{
    int i = 0;
    return;
}

