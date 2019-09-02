#include "mainwindow.h"
#include <QApplication>

/*
 * 五子棋
 * 作者:马小虫
*/


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
	w.show();

	return a.exec();
}
