#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStack>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QString AnlysisExpr(QString sInputExpr)
    {
        // 获取逆波兰表达式
        QStack<QString> stackMain;
        QStack<QString> stackOper;
        for (int i = 0; i < sInputExpr.length(); i++)
        {
            QChar ch = sInputExpr[i];
            if (ch == ' ')
            { // 空格
                continue;
            }
            else if (ch == '(')
            { // 左括号
                stackOper.push(QString(ch));
            }
            else if (ch == ')')
            { // 右括号
                while (!stackOper.empty())
                {
                    QString chPop = stackOper.pop();
                    if (chPop != "(")
                    {
                        stackMain.push(chPop);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else if ((ch >= '0' && ch <= '9')
                || (ch >= 'a' && ch <= 'z')
                || (ch >= 'A' && ch <= 'Z')
                || (ch == '.'))
            { // 数字或字符
                stackMain.push(GetWholeWord(sInputExpr, i, i));
            }
            else if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
            { // 操作符
                if (stackOper.empty())
                { //
                    stackOper.push(QString(ch));
                }
                else
                { // 出栈直到遇到优先小于等于的
                    QString chTemp = stackOper.top();
                    while (((ch == '+' || ch == '-') && (chTemp == "*" || chTemp == "/"))
                        || ((ch == '+' || ch == '-') && (chTemp == "+" || chTemp == "-"))
                        || ((ch == '*' || ch == '/') && (chTemp == "*" || chTemp == "/")))
                    {
                        stackMain.push(stackOper.pop());
                        if (stackOper.size() > 0)
                            chTemp = stackOper.top();
                        else
                            break;
                    }
                    stackOper.push(QString(ch));
                }
            }
        }
        while (!stackOper.empty())
        {
            QString chPop = stackOper.pop();
            stackMain.push(chPop);
        }

        QString sText;
        QStack<QString> stackCalc;
        while (!stackMain.empty())
        {
            QString chPop = stackMain.pop();
            stackCalc.push(chPop);
            sText.push_back(chPop);
        }

        // 表达式还原计算
        QString sNewExpr;
        double dResultValue = sInputExpr.toDouble();
        {
            QString sOper;

            // 计算逆波兰表达式：重新生成表达式
            QStack<QString> stackTemp;
            QStack<double> stackTempValue;
            while (!stackCalc.empty())
            {
                QString sPop = stackCalc.pop();
                if (sPop.length() == 0)
                    continue;

                QChar ch = sPop.at(0);
                if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
                {
                    if (stackTemp.size() >= 2)
                    {
                        sOper = sPop;
                        QString s1 = stackTemp.pop();
                        QString s2 = stackTemp.pop();
                        if ((sOper == "*" || sOper == "/") && (s2.contains('+') || s2.contains('-')))
                        {
                            s2 = "(" + s2 + ")";
                        }
                        sNewExpr = QString("%1 %2 %3").arg(s2).arg(sOper).arg(s1);
                        stackTemp.push(sNewExpr);

                        double d1 = stackTempValue.pop();
                        double d2 = stackTempValue.pop();
                        switch (ch.cell())
                        {
                        case '+':dResultValue = d2 + d1; break;
                        case '-':dResultValue = d2 - d1; break;
                        case '*':dResultValue = d2 * d1; break;
                        case '/':dResultValue = d2 / d1; break;
                        }
                        stackTempValue.push(dResultValue);
                    }
                }
                else
                {
                    stackTemp.push(sPop);
                    stackTempValue.push(sPop.toDouble());
                }
            }
        }
        //return QString("%1").arg(dResultValue);		//使用QString的%1方式用浮点数来初始化时，会出现科学计数法的表示结果
        return QString::number(dResultValue,'f',10);	//最多保留小数点后10位
    }
    QString GetWholeWord(const QString& sInput, const int nStartPos, int& nOutEndPos)
    {
        QString sWord;
        nOutEndPos = nStartPos;
        for (int i = nStartPos; i < sInput.length(); i++)
        {
            QChar ch = sInput.at(i);
            if ((ch >= '0' && ch <= '9')
                || (ch >= 'a' && ch <= 'z')
                || (ch >= 'A' && ch <= 'Z')
                || (ch == '.'))
            {
                sWord += ch;
                nOutEndPos = i;
            }
            else
            {
                break;
            }
        }
        return sWord;
    };

private slots:
    void on_lineEdit_returnPressed();

private:
    Ui::Widget *ui;
    QString AddZeroBefore(QString str)
    {
        QString s = str;
        if (s.isEmpty())
        {
            return s;
        }
        if (s[0] == '-')
        {
            s = QString("0") + s;
        }
        return s;
    }
     void  DeleteZero(QString& s)
    {
        int nPos = s.indexOf(".");
        if (nPos != -1)
        {
            int nNum = 0;
            for (int i=s.length()-1; i>=nPos; i--)
            {
                if (s[i] == '0' || s[i] == '.')
                {
                    nNum++;
                }
                else
                {
                    break;
                }
            }
            if (nNum>0)
            {
                s = s.left(s.length() - nNum);
            }
        }
    }

};

#endif // WIDGET_H
