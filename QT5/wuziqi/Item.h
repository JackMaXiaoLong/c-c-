#pragma once
#include <QPoint>

class Item
{
public:
	Item(void);
	Item(QPoint pt,bool bBlack);
	~Item(void);

	bool operator==(const Item &t1)const
	{
        return ((m_pt == t1.m_pt) && (m_bBlack == t1.m_bBlack));
	}  

	QPoint m_pt;
	bool m_bBlack;
private:


};

