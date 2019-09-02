#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Item.h"


namespace Ui {
class MainWindow;
}


#define CHESS_ROWS		9
#define CHESS_COLUMES	 16
#define RECT_WIDTH		44
#define RECT_HEIGHT		44

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);
private slots:
    void on_actionNew_triggered();

    void on_pushButton_clicked();

private:
	void DrawChessboard();
	void DrawItems();
	void DrawItemWithMouse();
    int x;

	void DrawChessAtPoint(QPainter& painter,QPoint& pt);

	int CountNearItem(Item item,QPoint ptDirection);	//统计某个方向(共8个方向)上的相连个数，用QPoint表示统计方向，如(1,1)表示右下方,（-1,0）表示向左
private:
    Ui::MainWindow *ui;

	QVector<Item> m_items;
    bool m_bIsBlackTurn;
};

#endif // MAINWINDOW_H
