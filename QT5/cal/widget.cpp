#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->lineEdit->setFont(QFont( "Timers" , 28 ,  QFont::Bold) );
    ui->textEdit->setFont(QFont( "Timers" , 14 ,  QFont::Normal) );
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_lineEdit_returnPressed()
{
    QString strInput = ui->lineEdit->text();
    if (strInput.isEmpty())
    {
        return ;
    }
    //如果最开始是减号,就在减号前加一个0，因为这个算法会忽略一开始就是运算符的情况，认为没有左操作数 比如导致-1+3=4忽略-号，加上0改成0-1+3=2就正常
    QString strNewInput = AddZeroBefore(strInput);
    QString strResult = AnlysisExpr(strNewInput);
    if (strResult.isEmpty())
    {
        return;
    }
    //去掉小数点后无效的0
    DeleteZero(strResult);

    QString strRecord = strInput;
    if (strRecord.right(1) != "=")
    {
        strRecord += "=";
    }
    strRecord += strResult;

    ui->lineEdit->setText(strResult);
    ui->textEdit->setText(strRecord + "\n" + ui->textEdit->toPlainText());
}
