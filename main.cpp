// main.cpp
#include "mainwindow.h"
#include <QApplication>

// 程序的入口函数
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();  // 显示主窗口
    return a.exec();
}
