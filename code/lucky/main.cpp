#include <QApplication>
#include <QTextCodec>

#include "user/account.h"
#include "public/Basic.h"
#include "widget/stockwindow.h"

//乱码处理
QTextCodec* codec = QTextCodec::codecForName("GBK");
//main thread
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //创建交易对象
    Account app;
    app.start();

    return a.exec();
}
