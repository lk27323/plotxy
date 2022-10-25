#ifndef _ADD_PLOT_PAIR_H_
#define _ADD_PLOT_PAIR_H_

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPair>
#include <QMap>
#include <QMenu>
#include <QWidgetAction>
#include "ui_AddPlotPair.h"
#include "constdef.h"
#include "PlotItemBase.h"

class AddPlotPair : public QWidget
{
    Q_OBJECT

public:
	static AddPlotPair* m_getInstance();
	
    ~AddPlotPair();

    void init(PlotType index = PlotType::Type_PlotScatter);
	int textRowCount();
	int textCloumnCount();

	QSet<QString> m_temSet1;
	QSet<QString> m_temSet2;

	void onChangeStackIndex(PlotType index);
	void setPlotBaseInfo(BaseInfo);

private:
	void initTreePlot();
	void updatePlotTrees();
	PlotType getPlotType(PlotItemBase*);
	
signals:
    void sigAddPlotPair(QString,QString);
	void sgn_addPlotPair(QString, QString, QString, QString);

public slots:
    void onBtnAddClicked();
	void onBtnUpdateClicked();
	void onBtnRemoveClicked();
    void onBtnCloseClicked();
	void onTableWidgetItemClicked(QTableWidgetItem*);
	void onTableWidgetItemClicked_2(QTableWidgetItem*);
	void onTableWidgetItemClicked_3(QTableWidgetItem*);
	void onTableWidgetItemClicked_4(QTableWidgetItem*);
	void onTableWidgetItemClicked_Attitude1(QTableWidgetItem*);
	void onTableWidgetItemClicked_Attitude2(QTableWidgetItem*);
	void onUpdateData();
	void onAddPlot(const QString&, PlotItemBase*);
	void onDoubleClickedTreeWidgetItem(QTreeWidgetItem *item, int column);
	
private:
    Ui::AddPlotPair ui;
	static AddPlotPair* thispoint;
	explicit AddPlotPair(QWidget *parent = 0);
	BaseInfo m_curPlotInfo;
	QMap<QString, QList<PlotItemBase*>> m_plotManager; //tabName

	QTreeWidget* m_treePlot;
	QMenu* m_menuPlot;
	QWidgetAction* m_widgetActionPlot;
};

#endif // _ADD_PLOT_PAIR_H_
