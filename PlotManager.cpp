#include "PlotManager.h"
#include "addplotpair.h"
#include <QColorDialog>
#include <QTreeWidget>
#include <QTableWidgetItem>
#include <QDebug>
#include <QAction>
#include <QString>
#include <QStyleFactory>
#include "PlotItemBase.h"
#include "PlotManagerData.h"

//#include "PlotBar.h"

PlotManager::PlotManager(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton_close, &QPushButton::clicked, this, &PlotManager::onBtnCloseClicked);
	this->setWindowTitle(QString::fromLocal8Bit("图表管理器"));
	init();

	connect(PlotManagerData::getInstance(), SIGNAL(sgnUpdatePlotManager()), this, SLOT(onUpdatePlotManager()));
	connect(this, SIGNAL(sigChangePlotName()), PlotManagerData::getInstance(), SLOT(slotChangePlotName()));

	ui.treeWidget_selectedPlots->setStyle(QStyleFactory::create("windows"));
	ui.treeWidget_selectedPlots->setHeaderHidden(true);
	ui.treeWidget_selectedPlots->expandAll();

	ui.stackedWidget->setCurrentIndex(0);

	connect(ui.treeWidget_selectedPlots, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onTWSPclicked(QTreeWidgetItem*, int)));

	connect(ui.pushButton_addNew, SIGNAL(clicked()), this, SLOT(onAddNewClicked()));

	connect(ui.spinBox_between, QOverload<int>::of(&QSpinBox::valueChanged), this, &PlotManager::spinboxBetweenChanged);
	connect(ui.spinBox_left, QOverload<int>::of(&QSpinBox::valueChanged), this, &PlotManager::spinboxLeftChanged);
	connect(ui.spinBox_right, QOverload<int>::of(&QSpinBox::valueChanged), this, &PlotManager::spinboxRightChanged);

	connect(ui.lineEdit_hrozGrids, &QLineEdit::textChanged, this, [=]() {
		m_hrozGrids = ui.lineEdit_hrozGrids->text().toInt();
	});
	connect(ui.lineEdit_vertGrids, &QLineEdit::textChanged, this, [=]() {
		m_vertGrids = ui.lineEdit_vertGrids->text().toInt();
	});

	// 	QFontDatabase FontDb;
	// 	foreach(int size, FontDb.standardSizes()) {
	// 		ui.comboBox_AxisGrid_FontSize->addItem(QString::number(size));
	// 	}
}

PlotManager::~PlotManager()
{

}


void PlotManager::init()
{
	initTreeWidgetSettings();
	initGeneralUI();
	initAxisGridUI();
	initTextLightUI();
}

void PlotManager::addPlot(const QString& tabName, PlotItemBase* plotItem)
{
	m_plotManager = PlotManagerData::getInstance()->getPlotManagerData();

	//显示层更新
	if (m_plotManager.contains(tabName))
	{
		QList<QTreeWidgetItem*> topWidget = ui.treeWidget_selectedPlots->findItems(tabName, Qt::MatchCaseSensitive, 0);
		if (topWidget.size() != 0)
		{
			QTreeWidgetItem* itemselPlotI = new QTreeWidgetItem(QStringList() << plotItem->currName());
			topWidget[0]->addChild(itemselPlotI);
		}
	}
	else
	{
		QTreeWidgetItem* itemselPlotH = new QTreeWidgetItem(QStringList() << tabName);
		ui.treeWidget_selectedPlots->addTopLevelItem(itemselPlotH);
		ui.treeWidget_selectedPlots->expandAll();

		QTreeWidgetItem* itemselPlotI = new QTreeWidgetItem(QStringList() << plotItem->currName());
		itemselPlotH->addChild(itemselPlotI);

		//comboBox_tabName
		ui.comboBox_tabName->addItem(tabName);
	}

	//数据层更新
//	m_plotManager[tabName].append(plotItem);
}

void PlotManager::initTreeWidgetSettings()
{
	ui.treeWidget_settings->setHeaderHidden(false);
	ui.treeWidget_settings->setHeaderLabel(QString::fromLocal8Bit("设置"));
	ui.treeWidget_settings->setIndentation(15);

	connect(ui.treeWidget_settings, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onTWSclicked(QTreeWidgetItem*, int)));

	m_itemGeneral = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("总体设置")));
	m_itemAxis = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("坐标轴和网格设置")));
	m_itemPlotData = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("数据设置")));
	m_itemText = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("文本信息")));
	m_itemGOG = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("GOG曲线")));
	m_itemLinkedAxis = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("相关的轴")));
	m_itemScatterPlot = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Scatter设置")));
	m_itemAScope = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("A-Scope设置")));
	m_itemRTI = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("RTI设置")));
	m_itemTextLight = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Text/Light设置")));
	m_itemBar = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Bar设置")));
	m_itemDial = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Dials设置")));
	m_itemAttitude = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Attitude设置")));
	m_itemTrackStatus = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Track Status设置")));
	m_itemRangeDoppler = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Range Doppler设置")));

	m_itemLimits = new QTreeWidgetItem(m_itemScatterPlot, QStringList(QString::fromLocal8Bit("限制")));
	m_itemPlotMarkers = new QTreeWidgetItem(m_itemScatterPlot, QStringList(QString::fromLocal8Bit("标记")));
	m_itemTimeLine = new QTreeWidgetItem(m_itemScatterPlot, QStringList("Time Line"));
	m_itemHandsOff = new QTreeWidgetItem(m_itemScatterPlot, QStringList("Hands-Off"));

	ui.treeWidget_settings->setCurrentItem(m_itemGeneral);
	m_itemScatterPlot->setExpanded(true);
}

void PlotManager::initGeneralUI()
{
	ui.pushButton_flipHeight->setVisible(false);
	ui.pushButton_flipWidth->setVisible(false);
	ui.radioButton_percent->setChecked(false);
	ui.radioButton_pixel->setChecked(true);
	m_radioPixelChecked = true;
	connect(ui.radioButton_percent, &QRadioButton::clicked, this, &PlotManager::onRadioPercentClicked);
	connect(ui.radioButton_pixel, &QRadioButton::clicked, this, &PlotManager::onRadioPixelClicked);
	connect(ui.lineEdit_plotPositionX, &QLineEdit::editingFinished, this, &PlotManager::onPlotRectEditFinished);
	connect(ui.lineEdit_plotPositionY, &QLineEdit::editingFinished, this, &PlotManager::onPlotRectEditFinished);
	connect(ui.lineEdit_plotWidth, &QLineEdit::editingFinished, this, &PlotManager::onPlotRectEditFinished);
	connect(ui.lineEdit_plotHeight, &QLineEdit::editingFinished, this, &PlotManager::onPlotRectEditFinished);
	connect(ui.checkBox_draw, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_drawStateChanged);
	connect(ui.lineEdit_plotName, &QLineEdit::editingFinished, this, &PlotManager::onLineEditPlotNameEditingFinished);
}

void PlotManager::initAxisGridUI()
{
	ui.pushButton_flipXValues->setVisible(false);
	ui.pushButton_flipYValues->setVisible(false);

	ui.lineEdit_10->setEnabled(false);
	ui.lineEdit_11->setEnabled(false);
	ui.lineEdit_12->setEnabled(false);
	ui.lineEdit_13->setEnabled(false);
	ui.tableWidget->setEnabled(false);
	ui.tableWidget_2->setEnabled(false);
	connect(ui.checkBox_4, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_4StateChanged);
	connect(ui.checkBox_5, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_5StateChanged);
	connect(ui.lineEdit_limitBgnX, &QLineEdit::editingFinished, this, &PlotManager::onLineEdit_limitXEditingFinished);
	connect(ui.lineEdit_LimitBgnY, &QLineEdit::editingFinished, this, &PlotManager::onLineEdit_limitYEditingFinished);
	connect(ui.lineEdit_limitEndX, &QLineEdit::editingFinished, this, &PlotManager::onLineEdit_limitXEditingFinished);
	connect(ui.lineEdit_limitEndY, &QLineEdit::editingFinished, this, &PlotManager::onLineEdit_limitYEditingFinished);
	connect(ui.pushButton_flipXValues, &QPushButton::clicked, this, &PlotManager::onPushButton_flipXValuesClicked);
	connect(ui.pushButton_flipYValues, &QPushButton::clicked, this, &PlotManager::onPushButton_flipYValuesClicked);
	connect(ui.lineEdit_hrozGrids, &QLineEdit::editingFinished, this, &PlotManager::onLineEdit_horzGridsEditingFinished);
	connect(ui.lineEdit_vertGrids, &QLineEdit::editingFinished, this, &PlotManager::onLineEdit_vertGridsEditingFinished);
	connect(ui.lineEdit_21, &QLineEdit::editingFinished, this, &PlotManager::onSetAxisColorWidth);
	connect(ui.lineEdit_23, &QLineEdit::editingFinished, this, &PlotManager::onSetGridColorWidth);
	connect(ui.pushButton_axisColor, &QPushButton::clicked, this, &PlotManager::onSetAxisColorWidth);
	connect(ui.pushButton_gridColor, &QPushButton::clicked, this, &PlotManager::onSetGridColorWidth);
}

void PlotManager::initTextLightUI()
{
	ui.groupBox_29->setVisible(false);
	ui.radioButton_3->setEnabled(false);
	ui.radioButton_4->setEnabled(false);
	ui.spinBox_14->setEnabled(false);
	ui.spinBox_15->setEnabled(false);
	ui.spinBox_16->setEnabled(false);
	ui.spinBox_17->setEnabled(false);
	connect(ui.pushButton_71, &QPushButton::clicked, this, &PlotManager::onPushButton_71Clicked);
	connect(ui.spinBox_10, &QSpinBox::editingFinished, this, &PlotManager::onPushButton_71Clicked);
}

void PlotManager::refreshTreeWidgetSettingEnabled(PlotItemBase * plot)
{
	QString name = plot->metaObject()->className();
	if (name.compare("PlotScatter") == 0)
	{
		enableItem_Scatter();
	}
	else if (name.compare("PlotAScope") == 0)
	{
		enableItem_AScope();
	}
	else if (name.compare("PlotRTI") == 0)
	{
		enableItem_RTI();
	}
	else if (name.compare("PlotText") == 0 || name.compare("PlotLight") == 0)
	{
		enableItem_Text_Light();
	}
	else if (name.compare("PlotBar") == 0)
	{
		enableItem_Bar();
	}
	else if (name.compare("PlotDial") == 0)
	{
		enableItem_Dial();
	}
	else if (name.compare("PlotAttitude") == 0)
	{
		enableItem_Attitude();
	}
	else if (name.compare("PlotPolar") == 0)
	{
		enableItem_Polar();
	}
	else if (name.compare("PlotTrack") == 0)
	{
		enableItem_Track();
	}
	else if (name.compare("PlotDoppler") == 0)
	{
		enableItem_Doppler();
	}
}

void PlotManager::refreshGeneralUI(PlotItemBase * plot)
{
	emit sigGetTabRect();

	ui.lineEdit_plotName->setText(plot->currName());
	ui.comboBox_tabName->setCurrentText(plot->currTabName());
	if (ui.radioButton_pixel->isChecked())
	{
		ui.lineEdit_plotPositionX->setText(QString("%1").arg(plot->currPosition().x()));
		ui.lineEdit_plotPositionY->setText(QString("%1").arg(plot->currPosition().y()));
		ui.lineEdit_plotWidth->setText(QString("%1").arg(plot->currWidth()));
		ui.lineEdit_plotHeight->setText(QString("%1").arg(plot->currHeight()));

	}
	else if (ui.radioButton_percent->isChecked())
	{
		ui.lineEdit_plotPositionX->setText(QString("%1").arg((float)plot->currPosition().x() / m_tabWidgetRect.width()));
		ui.lineEdit_plotPositionY->setText(QString("%1").arg((float)plot->currPosition().y() / m_tabWidgetRect.height()));
		ui.lineEdit_plotWidth->setText(QString("%1").arg((float)plot->currWidth() / m_tabWidgetRect.width()));
		ui.lineEdit_plotHeight->setText(QString("%1").arg((float)plot->currHeight() / m_tabWidgetRect.height()));
	}
}

void PlotManager::refreshAxisGridUI(PlotItemBase * plot)
{
	double x0, x1, y0, y1;
	plot->getCoordRangeX(x0, x1);
	plot->getCoordRangeY(y0, y1);
	ui.lineEdit_limitBgnX->setText(QString("%1").arg(x0));
	ui.lineEdit_limitEndX->setText(QString("%1").arg(x1));
	ui.lineEdit_LimitBgnY->setText(QString("%1").arg(y0));
	ui.lineEdit_limitEndY->setText(QString("%1").arg(y1));
	ui.lineEdit_hrozGrids->setText(QString("%1").arg(plot->getHorzGrids()));
	ui.lineEdit_vertGrids->setText(QString("%1").arg(plot->getVertGrids()));
	ui.lineEdit_21->setText(QString("%1").arg(plot->getAxisWidth()));
	ui.lineEdit_23->setText(QString("%1").arg(plot->getGridWidth()));
	ui.pushButton_axisColor->setColor(plot->getAxisColor());
	ui.pushButton_gridColor->setColor(plot->getGridColor());

}

void PlotManager::refreshPlotDataUI(PlotItemBase * plot)
{
	ui.treeWidget_4->clear();
	QList<QPair<QString, QString>> plotPairData = plot->getPlotPairData();
	for (int k = 0; k < plotPairData.size(); ++k)
	{
		//界面更新
		QTreeWidgetItem* addplotItem = new QTreeWidgetItem;
		addplotItem->setText(0, plotPairData[k].first);
		addplotItem->setText(1, plotPairData[k].second);

		ui.treeWidget_4->addTopLevelItem(addplotItem);
	}
}
void PlotManager::refreshLightTextUI(PlotItemBase * plot)
{
	if (!(plot == nullptr))
	{
		if (plot->currName().startsWith("Text"))
			ui.stackedWidget_LightTextDataSort->setCurrentIndex(0);
		else
			ui.stackedWidget_LightTextDataSort->setCurrentIndex(1);

		for (int i = ui.tableWidget_TextDataSort->rowCount(); i >0 ; i--)
		{
			ui.tableWidget_TextDataSort->removeRow(ui.tableWidget_TextDataSort->rowCount()-1);
		}

		for (int i = 0; i < plot->getPlotPairData().size(); i++)
		{
			QString temFirst = plot->getPlotPairData().at(i).first;
			QString temEntityString = temFirst.split("+").front();
			QString temAttriString = temFirst.split("+").back();
			QTableWidgetItem *temEntity = new QTableWidgetItem(temEntityString);
			QTableWidgetItem *temAttri = new QTableWidgetItem(temAttriString);
			int temRow = ui.tableWidget_TextDataSort->rowCount();
			ui.tableWidget_TextDataSort->insertRow(temRow);
			ui.tableWidget_TextDataSort->setItem(temRow, 0, temEntity);
			ui.tableWidget_TextDataSort->setItem(temRow, 1, temAttri);
		}

	}

}


void PlotManager::enableItem_Scatter()
{
	m_itemGOG->setDisabled(false);
	m_itemLinkedAxis->setDisabled(true);
	m_itemScatterPlot->setDisabled(false);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_AScope()
{
	m_itemGOG->setDisabled(true);
	m_itemLinkedAxis->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(false);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_RTI()
{
	m_itemGOG->setDisabled(true);
	m_itemLinkedAxis->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(false);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Text_Light()
{
	m_itemGOG->setDisabled(true);
	m_itemLinkedAxis->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(false);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Bar()
{
	m_itemGOG->setDisabled(true);
	m_itemLinkedAxis->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(false);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Dial()
{
	m_itemGOG->setDisabled(true);
	m_itemLinkedAxis->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(false);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Attitude()
{
	m_itemGOG->setDisabled(true);
	m_itemLinkedAxis->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(false);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Polar()
{
	m_itemGOG->setDisabled(true);
	m_itemLinkedAxis->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Track()
{
	m_itemGOG->setDisabled(true);
	m_itemLinkedAxis->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(false);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Doppler()
{
	m_itemGOG->setDisabled(true);
	m_itemLinkedAxis->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(false);
}

void PlotManager::onTWSPclicked(QTreeWidgetItem* item, int column)
{
	QTreeWidgetItem *parent = item->parent();
	if (NULL == parent)
		return;

	QString parent_text = parent->text(0);
	QString child_text = item->text(column);

	if (m_plotManager.contains(parent_text))
	{
		for (int i = 0; i < m_plotManager[parent_text].size(); ++i)
		{
			PlotItemBase *tempPlot = m_plotManager[parent_text].at(i);
			if (child_text == tempPlot->currName())
			{
				m_curSelectPlot = tempPlot;
				//刷新treeWidgetSetting的使能状态
				refreshTreeWidgetSettingEnabled(m_curSelectPlot);
				//general界面
				refreshGeneralUI(m_curSelectPlot);
				//Axis&Grid界面
				refreshAxisGridUI(m_curSelectPlot);
				//plotPair界面
				refreshPlotDataUI(m_curSelectPlot);
				//Text&Light界面
				refreshLightTextUI(m_curSelectPlot);

				//
			//	tempPlot->deleteLater();
				break;
			}
			m_curSelectPlot = nullptr;
		}
	}
	else
		m_curSelectPlot = nullptr;

	/*if (child_text == "Bar")
	{
		m_itemBar->setDisabled(false);
	}*/
}

void PlotManager::onAddNewClicked()
{
	/*AddPlotPair* addPlotPair = new AddPlotPair();
	addPlotPair->show();*/

	emit sigAddPlotPair();
}

void PlotManager::onUpdatePlotManager()
{
	m_plotManager = PlotManagerData::getInstance()->getPlotManagerData();
	if (m_plotManager.isEmpty())
	{
		return;
	}

	ui.treeWidget_selectedPlots->clear();

	for (int i = 0; i < m_plotManager.size(); ++i)
	{
		QString tabName = m_plotManager.keys().at(i);
		QTreeWidgetItem* itemselPlotH = new QTreeWidgetItem(QStringList() << tabName);
		ui.treeWidget_selectedPlots->addTopLevelItem(itemselPlotH);
		ui.treeWidget_selectedPlots->expandAll();
		for (int k = 0; k < m_plotManager[tabName].size(); ++k)
		{
			QTreeWidgetItem* itemselPlotI = new QTreeWidgetItem(QStringList() << m_plotManager[tabName].at(k)->currName());
			itemselPlotH->addChild(itemselPlotI);
		}

		//comboBox_tabName
		ui.comboBox_tabName->addItem(tabName);
	}
}

void PlotManager::onTWSclicked(QTreeWidgetItem* item, int column)
{
	QString compare;
	compare = item->text(column);

	if (item->isDisabled() == true)
	{
		return;
	}
	else
	{
		if (compare == QString::fromLocal8Bit("总体设置"))
		{
			ui.stackedWidget->setCurrentIndex(0);
		}
		else if (compare == QString::fromLocal8Bit("坐标轴和网格设置"))
		{
			ui.stackedWidget->setCurrentIndex(1);
		}
		else if (compare == QString::fromLocal8Bit("数据设置"))
		{
			ui.stackedWidget->setCurrentIndex(2);
		}
		else if (compare == QString::fromLocal8Bit("文本信息"))
		{
			ui.stackedWidget->setCurrentIndex(3);
		}
		else if (compare == QString::fromLocal8Bit("GOG曲线"))
		{
			ui.stackedWidget->setCurrentIndex(4);
		}
		else if (compare == QString::fromLocal8Bit("相关的轴"))
		{
			ui.stackedWidget->setCurrentIndex(5);
		}
		else if (compare == QString::fromLocal8Bit("Scatter设置"))
		{
			ui.stackedWidget->setCurrentIndex(6);
		}
		else if (compare == QString::fromLocal8Bit("限制"))
		{
			ui.stackedWidget->setCurrentIndex(6);
		}
		else if (compare == QString::fromLocal8Bit("标记"))
		{
			ui.stackedWidget->setCurrentIndex(7);
		}
		else if (compare == QString::fromLocal8Bit("Time Line"))
		{
			ui.stackedWidget->setCurrentIndex(8);
		}
		else if (compare == QString::fromLocal8Bit("Hands-Off"))
		{
			ui.stackedWidget->setCurrentIndex(9);
		}
		else if (compare == QString::fromLocal8Bit("A-Scope设置"))
		{
			ui.stackedWidget->setCurrentIndex(10);
		}
		else if (compare == QString::fromLocal8Bit("RTI设置"))
		{
			ui.stackedWidget->setCurrentIndex(11);
		}
		else if (compare == QString::fromLocal8Bit("Text/Light设置"))
		{
			ui.stackedWidget->setCurrentIndex(12);
		}
		else if (compare == QString::fromLocal8Bit("Bar设置"))
		{
			ui.stackedWidget->setCurrentIndex(13);
		}
		else if (compare == QString::fromLocal8Bit("Dials设置"))
		{
			ui.stackedWidget->setCurrentIndex(14);
		}
		else if (compare == QString::fromLocal8Bit("Attitude设置"))
		{
			ui.stackedWidget->setCurrentIndex(15);
		}
		else if (compare == QString::fromLocal8Bit("Track Status设置"))
		{
			ui.stackedWidget->setCurrentIndex(16);
		}
		else if (compare == QString::fromLocal8Bit("Range Doppler设置"))
		{
			ui.stackedWidget->setCurrentIndex(17);
		}
	}
}

void PlotManager::spinboxBetweenChanged()
{
	m_spinBoxBetween = ui.spinBox_between->value();
}
void PlotManager::spinboxLeftChanged()
{
	m_spinBoxLeft = ui.spinBox_left->value();
}
void PlotManager::spinboxRightChanged()
{
	m_spinBoxRight = ui.spinBox_right->value();
}

void PlotManager::onRadioPixelClicked()
{
	ui.label_xPos->setText("pixel");
	ui.label_yPos->setText("pixel");
	ui.label_widthPos->setText("pixel");
	ui.label_heightPos->setText("pixel");

	if (ui.lineEdit_plotPositionX->text() == nullptr || ui.lineEdit_plotPositionY->text() == nullptr ||
		ui.lineEdit_plotWidth->text() == nullptr || ui.lineEdit_plotHeight->text() == nullptr)
	{
		m_radioPixelChecked = true;
		return;
	}
	if (!m_radioPixelChecked)
	{
		float percent = ui.lineEdit_plotPositionX->text().toFloat();
		int pixel = (int)(percent * m_tabWidgetRect.width());
		ui.lineEdit_plotPositionX->setText(QString("%1").arg(pixel));

		percent = ui.lineEdit_plotPositionY->text().toFloat();
		pixel = (int)(percent * m_tabWidgetRect.height());
		ui.lineEdit_plotPositionY->setText(QString("%1").arg(pixel));

		percent = ui.lineEdit_plotWidth->text().toFloat();
		pixel = (int)(percent * m_tabWidgetRect.width());
		ui.lineEdit_plotWidth->setText(QString("%1").arg(pixel));

		percent = ui.lineEdit_plotHeight->text().toFloat();
		pixel = (int)(percent * m_tabWidgetRect.height());
		ui.lineEdit_plotHeight->setText(QString("%1").arg(pixel));
	}
	m_radioPixelChecked = true;
}

void PlotManager::onRadioPercentClicked()
{
	ui.label_xPos->setText("%");
	ui.label_yPos->setText("%");
	ui.label_widthPos->setText("%");
	ui.label_heightPos->setText("%");

	if (ui.lineEdit_plotPositionX->text() == nullptr || ui.lineEdit_plotPositionY->text() == nullptr ||
		ui.lineEdit_plotWidth->text() == nullptr || ui.lineEdit_plotHeight->text() == nullptr)
	{
		m_radioPixelChecked = false;
		return;
	}
	if (m_radioPixelChecked)
	{
		int pixel = ui.lineEdit_plotPositionX->text().toInt();
		float percent = (float)pixel / m_tabWidgetRect.width();
		ui.lineEdit_plotPositionX->setText(QString("%1").arg(percent));

		pixel = ui.lineEdit_plotPositionY->text().toInt();
		percent = (float)pixel / m_tabWidgetRect.height();
		ui.lineEdit_plotPositionY->setText(QString("%1").arg(percent));

		pixel = ui.lineEdit_plotWidth->text().toInt();
		percent = (float)pixel / m_tabWidgetRect.width();
		ui.lineEdit_plotWidth->setText(QString("%1").arg(percent));

		pixel = ui.lineEdit_plotHeight->text().toInt();
		percent = (float)pixel / m_tabWidgetRect.height();
		ui.lineEdit_plotHeight->setText(QString("%1").arg(percent));
	}
	m_radioPixelChecked = false;
}

void PlotManager::onGetTabWidgetRect(QRect rect)
{
	m_tabWidgetRect = rect;
}

void PlotManager::onBtnCloseClicked()
{
	close();
}

void PlotManager::onPushButton_71Clicked()
{
	bool on;
	uint width = ui.spinBox_10->text().toInt(&on);
	QColor color = ui.pushButton_71->color();
	if (on)
		m_curSelectPlot->setGridColorWidth(color, width);
}

void PlotManager::onPlotRectEditFinished()
{
	if (ui.lineEdit_plotPositionX->text() == nullptr || ui.lineEdit_plotPositionY->text() == nullptr ||
		ui.lineEdit_plotWidth->text() == nullptr || ui.lineEdit_plotHeight->text() == nullptr)
	{
		return;
	}

	bool bX, bY, bW, bH;
	int x, y, w, h;
	if (m_radioPixelChecked)
	{
		x = ui.lineEdit_plotPositionX->text().toInt(&bX);
		y = ui.lineEdit_plotPositionY->text().toInt(&bY);
		w = ui.lineEdit_plotWidth->text().toInt(&bW);
		h = ui.lineEdit_plotHeight->text().toInt(&bH);
	}
	else
	{
		x = (int)(ui.lineEdit_plotPositionX->text().toFloat(&bX) * m_tabWidgetRect.width());
		y = (int)(ui.lineEdit_plotPositionY->text().toFloat(&bY) * m_tabWidgetRect.height());
		w = (int)(ui.lineEdit_plotWidth->text().toFloat(&bW) * m_tabWidgetRect.width());
		h = (int)(ui.lineEdit_plotHeight->text().toFloat(&bH) * m_tabWidgetRect.height());
	}

	if (bX && bY && bW && bH && (m_curSelectPlot != nullptr))
	{
		//m_curSelectPlot->setRect(QRect(x, y, w, h));
		connect(this, SIGNAL(sigRectChanged(QRect)), m_curSelectPlot, SLOT(slot_updateRect(QRect)));
		emit sigRectChanged(QRect(x, y, w, h));
		disconnect(this, SIGNAL(sigRectChanged(QRect)), m_curSelectPlot, SLOT(slot_updateRect(QRect)));
	}

}

void PlotManager::onLineEditPlotNameEditingFinished()
{
	if (m_curSelectPlot != nullptr)
	{
		QString oldName = m_curSelectPlot->currName();
		QString newName = ui.lineEdit_plotName->text();
		if (newName.compare(oldName) != 0)
		{
			m_curSelectPlot->setName(newName);
			emit sigChangePlotName();
		}
	}
}

void PlotManager::onCheckBox_drawStateChanged()
{
	if (m_curSelectPlot != nullptr)
	{
		connect(this, SIGNAL(sigSetPlotVisible(bool)), m_curSelectPlot, SLOT(slot_setVisible(bool)));
		emit sigSetPlotVisible(ui.checkBox_draw->isChecked());
		disconnect(this, SIGNAL(sigSetPlotVisible(bool)), m_curSelectPlot, SLOT(slot_setVisible(bool)));
	}
}

void PlotManager::onCheckBox_4StateChanged()
{
	ui.lineEdit_10->setEnabled(ui.checkBox_4->isChecked());
	ui.lineEdit_11->setEnabled(ui.checkBox_4->isChecked());
	ui.tableWidget->setEnabled(ui.checkBox_4->isChecked());
}

void PlotManager::onCheckBox_5StateChanged()
{
	ui.lineEdit_12->setEnabled(ui.checkBox_5->isChecked());
	ui.lineEdit_13->setEnabled(ui.checkBox_5->isChecked());
	ui.tableWidget_2->setEnabled(ui.checkBox_5->isChecked());
}

void PlotManager::onLineEdit_limitXEditingFinished()
{
	if (m_curSelectPlot == nullptr)
		return;

	bool bx0, bx1;
	double x0 = ui.lineEdit_limitBgnX->text().toDouble(&bx0);
	double x1 = ui.lineEdit_limitEndX->text().toDouble(&bx1);
	if (bx0 && bx1)
		m_curSelectPlot->setCoordRangeX(x0, x1);
}

void PlotManager::onLineEdit_limitYEditingFinished()
{
	if (m_curSelectPlot == nullptr)
		return;

	bool by0, by1;
	double y0 = ui.lineEdit_LimitBgnY->text().toDouble(&by0);
	double y1 = ui.lineEdit_limitEndY->text().toDouble(&by1);
	if (by0 && by1)
		m_curSelectPlot->setCoordRangeY(y0, y1);
}

void PlotManager::onPushButton_flipXValuesClicked()
{
	if (m_curSelectPlot == nullptr)
		return;

	bool bx0, bx1;
	double x0 = ui.lineEdit_limitBgnX->text().toDouble(&bx0);
	double x1 = ui.lineEdit_limitEndX->text().toDouble(&bx1);
	if (bx0 && bx1)
	{
		m_curSelectPlot->setCoordRangeX(x1, x0);
		ui.lineEdit_limitBgnX->setText(QString("%1").arg(x1));
		ui.lineEdit_limitEndX->setText(QString("%1").arg(x0));
	}
}

void PlotManager::onPushButton_flipYValuesClicked()
{
	if (m_curSelectPlot == nullptr)
		return;

	bool by0, by1;
	double y0 = ui.lineEdit_LimitBgnY->text().toDouble(&by0);
	double y1 = ui.lineEdit_limitEndY->text().toDouble(&by1);
	if (by0 && by1)
	{
		m_curSelectPlot->setCoordRangeX(y1, y0);
		ui.lineEdit_LimitBgnY->setText(QString("%1").arg(y1));
		ui.lineEdit_limitEndY->setText(QString("%1").arg(y0));
	}
}

void PlotManager::onLineEdit_horzGridsEditingFinished()
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}
	bool on;
	uint count = ui.lineEdit_hrozGrids->text().toUInt(&on);
	if (!on || count < 0)
	{
		return;
	}

	m_curSelectPlot->setHorzGrids(count);
}

void PlotManager::onLineEdit_vertGridsEditingFinished()
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}
	bool on;
	uint count = ui.lineEdit_vertGrids->text().toUInt(&on);
	if (!on || count < 0)
	{
		return;
	}

	m_curSelectPlot->setVertGrids(count);
}

void PlotManager::onSetAxisColorWidth()
{
	bool on;
	uint width = ui.lineEdit_21->text().toInt(&on);
	QColor color = ui.pushButton_axisColor->color();
	if (on)
		m_curSelectPlot->setAxisColorWidth(color, width);
}

void PlotManager::onSetGridColorWidth()
{
	bool on;
	uint width = ui.lineEdit_23->text().toInt(&on);
	QColor color = ui.pushButton_gridColor->color();
	if (on)
		m_curSelectPlot->setGridColorWidth(color, width);
}
