#ifndef  _PLOT_MANAGER_H_
#define  _PLOT_MANAGER_H_

#include <QtWidgets/QMainWindow>
#include "ui_PlotManager.h"
#include "constdef.h"

class PlotItemBase;

class PlotManager : public QWidget
{
    Q_OBJECT

public:
    explicit PlotManager(QWidget* parent = Q_NULLPTR);
    ~PlotManager();

    void init();								//初始化函数，连接信号槽
	void addPlot(const QString&, PlotItemBase*);			//添加Plot

	QColor m_axisColor;
	QColor m_gridColor;
	QColor m_girdFill;

	int m_spinBoxBetween;
	int m_spinBoxLeft;
	int m_spinBoxRight;

 	int m_hrozGrids;
 	int m_vertGrids;
private:
	void initTreeWidgetSettings();
	void initGeneralUI();
	void initAxisGridUI();

	void initTextLightUI();

	void initPlotDataUI();
	void initTextEditUI();

	void refreshTreeWidgetSettingEnabled(PlotItemBase* plot);
	void refreshGeneralUI(PlotItemBase* plot);
	void refreshAxisGridUI(PlotItemBase* plot);
	void refreshPlotDataUI(PlotItemBase* plot);

	void refreshLightTextUI(PlotItemBase* plot);

	void refreshTextEditUI(PlotItemBase* plot);


	void enableItem_Scatter();
	void enableItem_AScope();
	void enableItem_RTI();
	void enableItem_Text_Light();
	void enableItem_Bar();
	void enableItem_Dial();
	void enableItem_Attitude();
	void enableItem_Polar();
	void enableItem_Track();
	void enableItem_Doppler();

public slots:
    void onTWSclicked(QTreeWidgetItem* item, int i);
    void onTWSPclicked(QTreeWidgetItem* item, int i);
    void onAddNewClicked();
	void onSelectedPlot(QString tabName, QString plotName);	//用于默认点击的Item

	void onUpdatePlotManager();
	void onBtnCloseClicked();

	void spinboxBetweenChanged();
	void spinboxLeftChanged();
	void spinboxRightChanged();

// 	void onAddPlotPair(QString,QString);
// 	void onAddPlotPair(QString tabName, QString plotName, QString xColumn, QString yColumn);

	//General
	void onRadioPixelClicked();
	void onRadioPercentClicked();
	void onGetTabWidgetRect(QRect);
	void onPlotRectEditFinished();
	void onLineEditPlotNameEditingFinished();
	void onCheckBox_drawStateChanged();

	//Axis and Grid
	void onCheckBox_4StateChanged();
	void onCheckBox_5StateChanged();
	void onLineEdit_limitXEditingFinished();
	void onLineEdit_limitYEditingFinished();
	void onPushButton_flipXValuesClicked();
	void onPushButton_flipYValuesClicked();
	void onLineEdit_horzGridsEditingFinished();
	void onLineEdit_vertGridsEditingFinished();
	void onSetAxisColorWidth();
	void onSetGridColorWidth();
	void onCheckBox_6StateChanged();
	void onPushButton_10Clicked();
	void onfontComboBox_3CurrentFontChanged(const QFont& font);
	void onComboBox_AxisGrid_FontSizeCurrentTextChanged(const QString& text);
	void onComboBox_2CurrentIndexChanged(int index);
	void onComboBox_3CurrentIndexChanged(int index);


	//Light/Text
	void onPushButton_71Clicked();

	//Text Edit
	void onCheckBox_12StateChanged();
	void onCheckBox_13StateChanged();
	void onCheckBox_14StateChanged();
	void onLineEdit_26EditingFinished();
	void onPushButton_22Clicked();
	void onPushButton_23Clicked();
	void onfontComboBox_2CurrentFontChanged(const QFont& font);
	void onComboBox_Text_fontSizeCurrentTextChanged(const QString& text);


signals:
	void sigAddPlotPair();
	void sigRectChanged(QRect);
	void sigGetTabRect();
	void sigSetPlotVisible(bool);
	void sigChangePlotName();
	void sigOnTextGridColorChanged();


private:
    Ui::PlotManager ui;

	QTreeWidgetItem* m_itemGeneral;
	QTreeWidgetItem* m_itemAxis;
	QTreeWidgetItem* m_itemPlotData;
	QTreeWidgetItem* m_itemText;
	QTreeWidgetItem* m_itemGOG;
	QTreeWidgetItem* m_itemLinkedAxis;
	QTreeWidgetItem* m_itemScatterPlot;
	QTreeWidgetItem* m_itemLimits;
	QTreeWidgetItem* m_itemPlotMarkers;
	QTreeWidgetItem* m_itemTimeLine;
	QTreeWidgetItem* m_itemHandsOff;
	QTreeWidgetItem* m_itemAScope;
	QTreeWidgetItem* m_itemRTI;
	QTreeWidgetItem* m_itemTextLight;
	QTreeWidgetItem* m_itemBar;
	QTreeWidgetItem* m_itemDial;
	QTreeWidgetItem* m_itemAttitude;
	QTreeWidgetItem* m_itemTrackStatus;
	QTreeWidgetItem* m_itemRangeDoppler;

    QMap<QString/*int*/, QList<PlotItemBase*>> m_plotManager;	//tabName
	bool m_radioPixelChecked;
	QRect m_tabWidgetRect;
	PlotItemBase* m_curSelectPlot = nullptr;;
};

#endif // ! 