#include "Item.h"


Item::Item(void)
{
}

Item::Item(QPoint pt,bool bBlack)
{
	m_pt = pt;
	m_bBlack = bBlack;
}


Item::~Item(void)
{
}
