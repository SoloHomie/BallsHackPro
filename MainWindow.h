#pragma once
#define NOMINMAX       // 禁用 Windows 的 min/max 宏
#define _NO_RPCNDR_H   // 禁用 Windows SDK 的 byte 定义

#include <QObject>
#include <QMainWindow>
#include "ui_MainWindow.h"
#include <QByteArray>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <QThread>
#include <QRunnable>
#include <QThreadPool>
#include <QMutex>
#include "Memory.h"
#include "BallsMerge.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();	

	void setTable(const QString mode); //放置数据到table中
	void loadjson();
	void setCheckboxState(QTreeWidget* treeWidget, const QString& parentText,
		const QString& itemText, bool checked);
	static LRESULT CALLBACK WheelHookProc(int nCode, WPARAM wParam, LPARAM lParam); //鼠标滚轮钩子
	static bool isUpdate;
	UINT virtualkey(QString key);


signals:
	void readFinished(); //读取完成信号

public slots:
	void treewidget_itemClicked(); //点击不同的items,索引stackedwidget

public:
	Ui::MainWindow ui;
	static HHOOK WheelHookID; //滚轮hook句柄
	static MainWindow* instance;  // 静态实例指针
	BallsMerge* ballsMerge;
	Memory* memory;
};

