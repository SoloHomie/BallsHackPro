#pragma once
#include "ui_LogIn.h"
#include <QMainWindow>
#include <QObject>
#include <string>

using namespace std;

class Log : public QMainWindow
{
	Q_OBJECT
public:
	Log();
	~Log();

	void checkNum();
	static bool cardState;
	static string cardcode;
public:
	Ui::LogIn* ui;
};