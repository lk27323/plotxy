#pragma once
/* * @filename: PlotText.h
* @brief: PlotText控件类
* @author: zhangdingwen
* @create time: 2022-10-10
*  */

#ifndef  _PLOT_TEXT_H_
#define  _PLOT_TEXT_H_ 

#include "PlotItemBase.h"
#include "BaseData.h"
#include <QTimer>
#include <QColor>
#include <QMap>
#include <QDebug>
#include <QVector>

class PlotText :public PlotItemBase
{
	Q_OBJECT
public:
	PlotText(QWidget* parent = Q_NULLPTR);
	~PlotText();
	//void onTimeout();

	bool m_flag;
	static int m_instanceCount;			//实体个数

public slots:
	void slot_getCurrentSeconds(double secs);
	//void slot_setValue(double);
	//void slot_getGridColor();
	//QColor slot_getGridColor();
protected:
	virtual void paintEvent(QPaintEvent* event);

private:
	//void updateItems(QPainter*);
	virtual void setGridColorWidth(QColor, uint);
	virtual void dataPairOrder();

	bool m_bHorizontal;
	bool m_started;
	

	QStringList m_xColumnList;
	QStringList m_yColumnList;

	QColor m_gridFillColor;
	QColor m_axisColor;
	
	QStringList m_entityTypeList;
	QStringList m_entityAttrList;

	int m_horiGridNum;
	int m_verGridNum;
	
	int m_leftPadding;
	int m_rightPadding;
	int m_interPadding;

	int m_currTimeIndex;
	
	QMap<QString, QMap<int, QColor>>m_thresholdColorMap;
	QVector<QList<double>> m_valueListVector;
	QColor m_defaultColor;
	QList<double> m_valueList;
	QList<QList<double>> m_temValueList;
	QList<QString> m_entityName, m_attriName;


	double m_secValue;

	void setTitle(QPainter& painter, QRect& rect);
	void drawXYTitle(QPainter& painter, int& horiGridWidth, int& verGridWidth , QList<QPair<QString, QString>> & dataList ,double &as);
	void drawNMCell(QPainter& painter, QSet<QString>& xset, QSet<QString>& yset, QList<QPair<QString, QString>> dataList,
		int& horiGridWidth, int& verGridWidth,double &as);
	void drawData(QSet<QString>& xset, QSet<QString>& yset, int& horiGridWidth, int& verGridWidth);

};

#endif // _PLOT_TEXT_H_ 