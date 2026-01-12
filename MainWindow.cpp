#include <Windows.h>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMainWindow>
#include <QMessageBox>
#include <cstddef>
#include <winuser.h>
#include <Tlhelp32.h>
#include <algorithm>
#include <atomic>
#include <vector>
#include <cmath>
#include <mutex>
#include <QThreadPool>
#include <QRunnable>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QProcess>
#include <QCoreApplication>
#include <QtConcurrent>
#include "Memory.h"
#include "BallsMerge.h"

HHOOK MainWindow::WheelHookID = nullptr;
MainWindow* MainWindow::instance = nullptr;
bool MainWindow::isUpdate = false;
HANDLE Memory::hProcess = nullptr;
HWND Memory::hWnd = nullptr;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowIcon(QIcon(":/icon(1).ico"));
	setWindowTitle("BallsHackPro  |  V1.0.0  |  Release");
	ui.treeWidget->expandAll();

	instance = this;

	loadjson();
	memory = new Memory(); 
	ballsMerge = new BallsMerge();

	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &MainWindow::treewidget_itemClicked);
	connect(ui.pushButton, &QPushButton::clicked, [=] {
		WheelHookID = SetWindowsHookEx(WH_MOUSE_LL, WheelHookProc,
			GetModuleHandle(NULL), 0);
		});
	
	connect(ui.pushButton, &QPushButton::clicked, [=] {
		ui.pushButton->setEnabled(false);
		ui.pushButton->setText("内存初始化中...");
		
		//内存初始化
		Memory::hProcess = memory->getProcessHandle();
		Memory::hWnd = memory->getRectHwnd();
		memory->SundaySearcher();
		printf("内存初始化中");

		if (Memory::patternAddresses[0] != 0)
		{
			memory->shiyeAdress = Memory::patternAddresses[0] + 0x414;
			memory->tuqiuAdress = Memory::patternAddresses[0] + 0xA0;
			memory->ncygAdress = Memory::patternAddresses[0] + 0x40;
			memory->tijiAdress = Memory::patternAddresses[0] + 0x3A0;
			memory->zhanjuzhiAdress = Memory::patternAddresses[0] + 0x4A0;
			memory->ncygjsxAdress = Memory::patternAddresses[0] + 0x68;
			memory->ncygjsyAdress = Memory::patternAddresses[0] + 0x6C;
			memory->xmin = Memory::patternAddresses[0] + 0x408;
			memory->xmax = Memory::patternAddresses[0] + 0x40C;
			memory->ymin = Memory::patternAddresses[0] + 0x404;
			memory->ymax = Memory::patternAddresses[0] + 0x400;
			memory->selfsize = Memory::patternAddresses[0] + 0x410;
		}

		if (Memory::patternAddresses[1] != 0)
		{
			memory->biansuAdress = memory->patternAddresses[1] + 0x0;
		}
		if (Memory::patternAddresses[2] != 0)
		{
			memory->nianheAdress = memory->patternAddresses[2] + 0x10;
		}
		if (Memory::patternAddresses[3] != 0)
		{
			memory->fsjiexianAdress = memory->patternAddresses[3] + 0x9;
		}
		if (Memory::patternAddresses[4] != 0)
		{
			memory->yaoganjiexianAdress = memory->patternAddresses[4] - 0x24;
		}
		if (Memory::patternAddresses[5] != 0)
		{
			DWORD_PTR baseAddr = memory->patternAddresses[5];
			memory->yaoganhuitanAdress_1 = baseAddr + 0x20;
			memory->yaoganhuitanAdress_2 = baseAddr + 0x20 - 0x04;
			memory->yaoganrongchaAdress = baseAddr + 0x1C;
		}
		if (Memory::patternAddresses[6] != 0)
		{
			memory->yaogan10000Adress_2 = memory->patternAddresses[6] - 0x51 + 0x04;
		}
		if (Memory::patternAddresses[7] != 0)
		{
			memory->juneiqupiAdress = memory->patternAddresses[7] - 0x02;
		}
		if (Memory::patternAddresses[8] != 0)
		{
			DWORD_PTR baseAddr = memory->patternAddresses[8];
			memory->ygycAdress_1 = baseAddr + 0x30;
			memory->ygycAdress_2 = baseAddr;
		}
		if (Memory::patternAddresses[9] != 0)
		{
			memory->ncdxAdress = memory->patternAddresses[9] - 0x08;
		}
		printf("内存初始化完成");
		//启动循环线程ff
		QtConcurrent::run([=]() {
				while (true)
				{
					if (Memory::tuqiuFlag)
					{
						float tuqiuValue = 0.0f;
						WriteProcessMemory(Memory::hProcess, (LPVOID)Memory::tuqiuAdress, &tuqiuValue, sizeof(float), NULL);
						Sleep(1);
					}
					else
					{
						Sleep(1000);
					}
				}
			});
		QtConcurrent::run([=]() {
				while (true)
				{
					if (Memory::fsjiexianFlag)
					{
						memory->writefsjiexian(Memory::hProcess, 0);
						Sleep(1);
					}
					else
					{
						Sleep(1000);
					}
				}
			});
		QtConcurrent::run([=]() {
				while (true)
				{
					if (Memory::yaoganjiexianFlag)
					{
						memory->writeyaoganjiexian(Memory::hProcess, 0);
						Sleep(1);
					}
					else
					{
						Sleep(1000);
					}
				}
			});
		QtConcurrent::run([=]() {
				while (true)
				{
					if (Memory::yaoganhuitanFlag)
					{
						memory->writeyaoganhuitan(Memory::hProcess, Memory::huitanValue);
						Sleep(1);
					}
					else
					{
						Sleep(1000);
					}
				}
			});
		QtConcurrent::run([=]() {
				while (true)
				{
					if (Memory::yaogan10000Flag)
					{
						memory->writeyaogan10000(Memory::hProcess, 99999999999999999);
						Sleep(1);
					}
					else
					{
						Sleep(1000);
					}
				}
			});
		QtConcurrent::run([=]() {
				while (true)
				{
					if (Memory::yaoganyouhuaFlag)
					{
						memory->writeyaoganyouhua(Memory::hProcess, 0.0f);
						Sleep(1);
					}
					else
					{
						Sleep(1000);
					}
				}
			});
		QtConcurrent::run([=]() {
				while (true)
				{
					if (Memory::yaoganycFlag)
					{
						memory->writeyaoganyanchi(Memory::hProcess, Memory::yaoganycValue);
						Sleep(1);
					}
					else
					{
						Sleep(1000);
					}
				}
			});

		
		

		});

	connect(ui.pushButton_4, &QPushButton::clicked, [=] {
		ui.pushButton_4->setEnabled(false);
		ui.pushButton_4->setText("合球初始化完成");
		QtConcurrent::run([=]() {
			while (true)
			{
				if (GetKeyState(VK_LBUTTON) < 0)
				{
					GetCursorPos(&ballsMerge->cursorPos_1);
					while (true)
					{
						Sleep(5);
						if (GetKeyState(ballsMerge->sanjiaojian1) < 0 && ballsMerge->sanjiao1Flog) break;
						if (GetKeyState(ballsMerge->sanjiaojian2) < 0 && ballsMerge->sanjiao2Flog) break;
						if (GetKeyState(ballsMerge->sanjiaojian3) < 0 && ballsMerge->sanjiao3Flog) break;
						if (GetKeyState(ballsMerge->chongqiujian) < 0 && ballsMerge->chongqiuFlog) break;
						if (GetKeyState(ballsMerge->sifenjian) < 0 && ballsMerge->sifenFlog) break;
						if (GetKeyState(ballsMerge->zhongfenjian) < 0 && ballsMerge->zhongfenFlog) break;
						if (GetKeyState(ballsMerge->xuanzhuanjian1) < 0 && ballsMerge->xuanzhuan1Flog) break;
						if (GetKeyState(ballsMerge->xuanzhuanjian2) < 0 && ballsMerge->xuanzhuan2Flog) break;
						if (GetKeyState(ballsMerge->banxuanjian1) < 0 && ballsMerge->banxuan1Flog) break;
						if (GetKeyState(ballsMerge->banxuanjian2) < 0 && ballsMerge->banxuan2Flog) break;
						if (GetKeyState(ballsMerge->sheshoujian) < 0 && ballsMerge->sheshouFlog) break;
						if (GetKeyState(ballsMerge->houyangjian) < 0 && ballsMerge->houyangFlog) break;
						if (GetKeyState(ballsMerge->qupijian) < 0 && Memory::juneiqupiFlag) break;
					}
					GetCursorPos(&ballsMerge->cursorPos_2);

					if (GetKeyState(ballsMerge->sanjiaojian1) < 0 && ballsMerge->sanjiao1Flog)
					{
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						ballsMerge->sanjiao1(Memory::hProcess);
					}

					if (GetAsyncKeyState(ballsMerge->sanjiaojian2) < 0 && ballsMerge->sanjiao2Flog)
					{
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						ballsMerge->sanjiao2(Memory::hProcess);
					}

					if (GetAsyncKeyState(ballsMerge->sanjiaojian3) < 0 && ballsMerge->sanjiao3Flog)
					{
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						ballsMerge->sanjiao3(Memory::hProcess);
					}

					if (GetAsyncKeyState(ballsMerge->chongqiujian) < 0 && ballsMerge->chongqiuFlog)
					{
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						ballsMerge->chongqiu(Memory::hProcess);
					}

					if (GetAsyncKeyState(ballsMerge->sifenjian) < 0 && ballsMerge->sifenFlog)
					{
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						ballsMerge->sifen(Memory::hProcess);
					}

					if (GetAsyncKeyState(ballsMerge->zhongfenjian) < 0 && ballsMerge->zhongfenFlog)
					{
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						ballsMerge->zhongfen(Memory::hProcess);
					}

					if (GetAsyncKeyState(ballsMerge->xuanzhuanjian1) < 0 && ballsMerge->xuanzhuan1Flog)
					{
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						ballsMerge->xuanzhuan1(Memory::hProcess);
					}

					if (GetAsyncKeyState(ballsMerge->xuanzhuanjian2) < 0 && ballsMerge->xuanzhuan2Flog)
					{
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						ballsMerge->xuanzhuan2(Memory::hProcess);
					}

					if (GetAsyncKeyState(ballsMerge->banxuanjian1) < 0 && ballsMerge->banxuan1Flog)
					{
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						ballsMerge->banxuan1(Memory::hProcess);
					}

					if (GetAsyncKeyState(ballsMerge->banxuanjian2) < 0 && ballsMerge->banxuan2Flog)
					{
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						ballsMerge->banxuan2(Memory::hProcess);
					}

					if (GetAsyncKeyState(ballsMerge->sheshoujian) < 0 && ballsMerge->sheshouFlog)
					{
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						ballsMerge->sheshou(Memory::hProcess);
					}

					if (GetAsyncKeyState(ballsMerge->houyangjian) < 0 && ballsMerge->houyangFlog)
					{
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						ballsMerge->houyang(Memory::hProcess);
					}
				}
				Sleep(200);
			}

			});
		});
	

	connect(ui.doubleSpinBox, &QDoubleSpinBox::valueChanged, [=]() {
		memory->writenianhe(Memory::hProcess, ui.doubleSpinBox->value());
		});
	connect(ui.doubleSpinBox_2, &QDoubleSpinBox::valueChanged, [=]() {
		memory->writeshiye(Memory::hProcess, ui.doubleSpinBox_2->value());
		});
	//connect(ui.doubleSpinBox_2, &QDoubleSpinBox::editingFinished, this, &memory->writeshiye);
	connect(ui.doubleSpinBox_8, &QDoubleSpinBox::valueChanged, [=]() {
		memory->writencdx(Memory::hProcess, ui.doubleSpinBox_8->value());
		});
	connect(ui.doubleSpinBox_7, &QDoubleSpinBox::valueChanged, [=]() {
		memory->writebiansu(Memory::hProcess, ui.doubleSpinBox_7->value());
		});

	connect(ui.doubleSpinBox_6, &QDoubleSpinBox::valueChanged, [=] {
		memory->huitanValue = ui.doubleSpinBox_6->value();
		});
	//connect(ui.checkBox_5,&QCheckBox::toggled,this,&MainWindow::writeyaoganhuitan);
	connect(ui.checkBox_4, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			memory->yaogan10000Value = 99999999999999.0f;
		}
		else
		{
			memory->yaogan10000Value = 100;
		}
		});
		
	connect(ui.checkBox_6, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			memory->yaoganyouhuaFlag = true;
		}
		else
		{
			memory->yaoganyouhuaFlag = false;
		}
		});
	connect(ui.checkBox_5, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			memory->yaoganhuitanFlag = true;
		}
		else
		{
			memory->yaoganhuitanFlag = false;
		}
		});
	connect(ui.checkBox_7, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
			{
			memory->yaoganjiexianFlag = true;
		}
		else
		{
			memory->yaoganjiexianFlag = false;
		}
		});
	connect(ui.checkBox_8, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			memory->fsjiexianFlag = true;
		}
		else
		{
			memory->fsjiexianFlag = false;
		}
		});
	connect(ui.checkBox_4, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			memory->yaogan10000Flag = true;
		}
		else
		{
			memory->yaogan10000Flag = false;
		}
		});

	connect(ui.checkBox, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			memory->tuqiuFlag = true;
		}
		else
		{
			memory->tuqiuFlag = false;
		}
		});
	connect(ui.checkBox_3, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			memory->gunlunshiyeFlag = true;
		}
		else
		{
			memory->gunlunshiyeFlag = false;
		}
		});
	connect(ui.doubleSpinBox_5, &QDoubleSpinBox::valueChanged, [=] {
		memory->step = ui.doubleSpinBox_5->value();
		});
	connect(ui.lineEdit_2, &QLineEdit::textChanged, [=] {
		memory->yaoganycValue = ui.lineEdit_2->text().toFloat();
		});

	connect(ui.menu_2, &QMenu::triggered, [=](QAction* action) {
		if (action->text() == "技战室-240帧-快")
		{
			QMessageBox::information(this, "提示", "当前以及切换到技战室参数");
		}
		else if (action->text() == "逃杀双冠-240帧-快")
		{
			QMessageBox::information(this, "提示", "当前已经切换到双冠逃杀模式\n若有合球失误，请联系群主，等待下个版本修复");
		}
		else if (action->text() == "自建房-240帧-快")
		{
			QMessageBox::information(this, "提示", "当前已经切换到自建房模式");
		}
		else if (action->text() == "巨行星-240帧-快")
		{
			QMessageBox::information(this, "提示", "当前已经切换到巨行星模式，暂时使用逃杀参数");
		}
		else if (action->text() == "团战-240帧")
		{
			QMessageBox::information(this, "提示", "当前已经切换到团战模式");
		}
		else if (action->text() == "通用延迟-240帧-慢")
		{
			QMessageBox::information(this, "提示", "当前已经切换到通用延迟");
		}
		});

	//涓夎1淇″彿涓庢Ы
	connect(ui.comboBox_5, &QComboBox::currentTextChanged, [=] {
		BallsMerge::sanjiaojian1 = virtualkey(ui.comboBox_5->currentText());
		});
	connect(ui.comboBox_6, &QComboBox::currentTextChanged, [=] {
		BallsMerge::sj1jd = ui.comboBox_6->currentText().toInt();
		});
	connect(ui.doubleSpinBox_14, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::sj1hqfd = ui.doubleSpinBox_14->value();
		});
	connect(ui.doubleSpinBox_15, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::sj1zyfd = ui.doubleSpinBox_15->value();
		});
	connect(ui.checkBox_15, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::sj1tqFlog = true;
		}
		else
		{
			BallsMerge::sj1tqFlog = false;
		}
		});
	connect(ui.checkBox_16, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::autosanjiao_1_Flog = true;
		}
		else
		{
			BallsMerge::autosanjiao_1_Flog = false;
		}
		});
	//涓夎2淇″彿涓庢Ы
	connect(ui.comboBox_7, &QComboBox::currentTextChanged, [=] {
		BallsMerge::sanjiaojian2 = virtualkey(ui.comboBox_7->currentText());
		});
	connect(ui.comboBox_8, &QComboBox::currentTextChanged, [=] {
		BallsMerge::sj2jd = ui.comboBox_8->currentText().toInt();
		});
	connect(ui.doubleSpinBox_18, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::sj2hqfd = ui.doubleSpinBox_18->value();
		});
	connect(ui.doubleSpinBox_16, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::sj2zyfd = ui.doubleSpinBox_16->value();
		});
	connect(ui.checkBox_21, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::sj2tqFlog = true;
		}
		else
		{
			BallsMerge::sj2tqFlog = false;
		}
		});
	connect(ui.checkBox_24, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::autosanjiao_2_Flog = true;
		}
		else
		{
			BallsMerge::autosanjiao_2_Flog = false;
		}
		});

	//涓夎3淇″彿涓庢Ы
	connect(ui.comboBox_9, &QComboBox::currentTextChanged, [=] {
		BallsMerge::sanjiaojian3 = virtualkey(ui.comboBox_9->currentText());
		});
	connect(ui.comboBox_10, &QComboBox::currentTextChanged, [=] {
		BallsMerge::sj1jd = ui.comboBox_10->currentText().toInt();
		});
	connect(ui.doubleSpinBox_20, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::sj1hqfd = ui.doubleSpinBox_20->value();
		});
	connect(ui.doubleSpinBox_21, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::sj1zyfd = ui.doubleSpinBox_21->value();
		});
	connect(ui.checkBox_26, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::sj3tqFlog = true;
		}
		else
		{
			BallsMerge::sj3tqFlog = false;
		}
		});
	connect(ui.checkBox_25, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::autosanjiao_3_Flog = true;
		}
		else
		{
			BallsMerge::autosanjiao_3_Flog = false;
		}
		});

	//鍐茬悆淇″彿涓庢Ы
	connect(ui.comboBox_11, &QComboBox::currentTextChanged, [=] {
		BallsMerge::chongqiujian = virtualkey(ui.comboBox_11->currentText());
		});
	connect(ui.checkBox_30, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::chongqiutqFlog = true;
		}
		else
		{
			BallsMerge::chongqiutqFlog = false;
		}
		});
	connect(ui.checkBox_32, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::chongqiujiantouFlog = true;
		}
		else
		{
			BallsMerge::chongqiujiantouFlog = false;
		}
		});
	//鍥涘垎淇″彿涓庢Ы
	connect(ui.comboBox_12, &QComboBox::currentTextChanged, [=] {
		BallsMerge::sifenjian = virtualkey(ui.comboBox_12->currentText());
		});
	connect(ui.checkBox_35, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::sifentqFlog = true;
		}
		else
		{
			BallsMerge::sifentqFlog = false;
		}
		});
	connect(ui.checkBox_33, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::autosifenFlog = true;
		}
		else
		{
			BallsMerge::autosifenFlog = false;
		}
		});
	connect(ui.doubleSpinBox_32, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::sfjcfd = ui.doubleSpinBox_32->value();
		});
	//涓垎淇″彿涓庢Ы
	connect(ui.comboBox_13, &QComboBox::currentTextChanged, [=] {
		BallsMerge::zhongfenjian = virtualkey(ui.comboBox_13->currentText());
		});
	connect(ui.checkBox_38, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::zhongfentqFlog = true;
		}
		else
		{
			BallsMerge::zhongfentqFlog = false;
		}
		});
	connect(ui.checkBox_36, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::autozhongfenFlog = true;
		}
		else
		{
			BallsMerge::autozhongfenFlog = false;
		}
		});
	connect(ui.doubleSpinBox_31, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::zfjcfd = ui.doubleSpinBox_31->value();
		});


	//鏃嬭浆1淇″彿涓庢Ы
	connect(ui.comboBox_14, &QComboBox::currentTextChanged, [=] {
		BallsMerge::xuanzhuanjian1 = virtualkey(ui.comboBox_14->currentText());
		});
	connect(ui.checkBox_41, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::xuanzhuan1tqFlog = true;
		}
		else
		{
			BallsMerge::xuanzhuan1tqFlog = false;
		}
		});
	connect(ui.doubleSpinBox_33, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::xuanzhuan1jd1 = ui.doubleSpinBox_33->value();
		});
	connect(ui.doubleSpinBox_39, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::xuanzhuan1jd2 = ui.doubleSpinBox_39->value();
		});
	connect(ui.doubleSpinBox_34, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::xuanzhuan1jd3 = ui.doubleSpinBox_34->value();
		});
	connect(ui.doubleSpinBox_40, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::xuanzhuan1jd4 = ui.doubleSpinBox_40->value();
		});
	connect(ui.doubleSpinBox_35, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::xuanzhuan1fd1 = ui.doubleSpinBox_35->value();
		});
	connect(ui.doubleSpinBox_37, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::xuanzhuan1fd2 = ui.doubleSpinBox_37->value();
		});
	connect(ui.doubleSpinBox_36, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::xuanzhuan1fd3 = ui.doubleSpinBox_36->value();
		});
	connect(ui.doubleSpinBox_38, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::xuanzhuan1fd4 = ui.doubleSpinBox_38->value();
		});
	
	//鍗婃棆1淇″彿涓庢Ы
	connect(ui.comboBox_15, &QComboBox::currentTextChanged, [=] {
		BallsMerge::banxuanjian1 = virtualkey(ui.comboBox_15->currentText());
		});
	connect(ui.checkBox_44, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::banxuan1tqFlog = true;
		}
		else
		{
			BallsMerge::banxuan1tqFlog = false;
		}
		});
	connect(ui.doubleSpinBox_41, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::banxuan1jd1 = ui.doubleSpinBox_41->value();
		});
	connect(ui.doubleSpinBox_42, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::banxuan1jd2 = ui.doubleSpinBox_42->value();
		});
	connect(ui.doubleSpinBox_44, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::banxuan1jd3 = ui.doubleSpinBox_44->value();
		});
	connect(ui.doubleSpinBox_46, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::banxuan1jd4 = ui.doubleSpinBox_46->value();
		});
	connect(ui.doubleSpinBox_43, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::banxuan1fd1 = ui.doubleSpinBox_43->value();
		});
	connect(ui.doubleSpinBox_45, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::banxuan1fd2 = ui.doubleSpinBox_45->value();
		});
	connect(ui.doubleSpinBox_47, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::banxuan1fd3 = ui.doubleSpinBox_47->value();
		});
	connect(ui.doubleSpinBox_48, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::banxuan1fd4 = ui.doubleSpinBox_48->value();
		});

	//鏃嬭浆2淇″彿涓庢Ы
	connect(ui.comboBox_16, &QComboBox::currentTextChanged, [=] {
		BallsMerge::xuanzhuanjian2 = virtualkey(ui.comboBox_16->currentText());
		});
	connect(ui.checkBox_47, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::xuanzhuan2tqFlog = true;
		}
		else
		{
			BallsMerge::xuanzhuan2tqFlog = false;
		}
		});
	connect(ui.doubleSpinBox_49, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::xuanzhuan2jd1 = ui.doubleSpinBox_49->value();
		});
	connect(ui.doubleSpinBox_50, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::xuanzhuan2jd2 = ui.doubleSpinBox_50->value();
		});
	connect(ui.doubleSpinBox_52, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::xuanzhuan2jd3 = ui.doubleSpinBox_52->value();
		});
	connect(ui.doubleSpinBox_54, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::xuanzhuan2jd4 = ui.doubleSpinBox_54->value();
		});
	connect(ui.doubleSpinBox_51, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::xuanzhuan2fd1 = ui.doubleSpinBox_51->value();
		});
	connect(ui.doubleSpinBox_53, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::xuanzhuan2fd2 = ui.doubleSpinBox_53->value();
		});
	connect(ui.doubleSpinBox_55, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::xuanzhuan2fd3 = ui.doubleSpinBox_55->value();
		});
	connect(ui.doubleSpinBox_56, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::xuanzhuan2fd4 = ui.doubleSpinBox_56->value();
		});

	//鍗婃棆2淇″彿涓庢Ы
	connect(ui.comboBox_17, &QComboBox::currentTextChanged, [=] {
		BallsMerge::banxuanjian2 = virtualkey(ui.comboBox_17->currentText());
		});
	connect(ui.checkBox_50, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::banxuan2tqFlog = true;
		}
		else
		{
			BallsMerge::banxuan2tqFlog = false;
		}
		});
	connect(ui.doubleSpinBox_57, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::banxuan2jd1 = ui.doubleSpinBox_57->value();
		});
	connect(ui.doubleSpinBox_58, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::banxuan2jd2 = ui.doubleSpinBox_58->value();
		});
	connect(ui.doubleSpinBox_60, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::banxuan2jd3 = ui.doubleSpinBox_60->value();
		});
	connect(ui.doubleSpinBox_62, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::banxuan2jd4 = ui.doubleSpinBox_62->value();
		});
	connect(ui.doubleSpinBox_59, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::banxuan2fd1 = ui.doubleSpinBox_59->value();
		});
	connect(ui.doubleSpinBox_61, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::banxuan2fd2 = ui.doubleSpinBox_61->value();
		});
	connect(ui.doubleSpinBox_63, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::banxuan2fd3 = ui.doubleSpinBox_63->value();
		});
	connect(ui.doubleSpinBox_64, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::banxuan2fd4 = ui.doubleSpinBox_64->value();
		});

	//铔囨墜淇″彿涓庢Ы
	connect(ui.comboBox_18, &QComboBox::currentTextChanged, [=] {
		BallsMerge::sheshoujian = virtualkey(ui.comboBox_18->currentText());
		});
	connect(ui.checkBox_53, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::sheshoutqFlog = true;
		}
		else
		{
			BallsMerge::sheshoutqFlog = false;
		}
		});
	connect(ui.doubleSpinBox_65, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::sheshoujd1 = ui.doubleSpinBox_65->value();
		});
	connect(ui.doubleSpinBox_66, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::sheshoujd2 = ui.doubleSpinBox_66->value();
		});
	connect(ui.doubleSpinBox_68, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::sheshoujd3 = ui.doubleSpinBox_68->value();
		});
	connect(ui.doubleSpinBox_70, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::sheshoujd4 = ui.doubleSpinBox_70->value();
		});
	connect(ui.doubleSpinBox_67, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::sheshoufd1 = ui.doubleSpinBox_67->value();
		});
	connect(ui.doubleSpinBox_69, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::sheshoufd2 = ui.doubleSpinBox_69->value();
		});
	connect(ui.doubleSpinBox_71, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::sheshoufd3 = ui.doubleSpinBox_71->value();
		});
	connect(ui.doubleSpinBox_72, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::sheshoufd4 = ui.doubleSpinBox_72->value();
		});



	//鍚庝话淇″彿涓庢Ы
	connect(ui.comboBox_19, &QComboBox::currentTextChanged, [=] {
		BallsMerge::houyangjian = virtualkey(ui.comboBox_19->currentText());
		});
	connect(ui.checkBox_56, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::houyangtqFlog = true;
		}
		else
		{
			BallsMerge::houyangtqFlog = false;
		}
		});
	connect(ui.checkBox_54, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			BallsMerge::autohouyangFlog = true;
		}
		else
		{
			BallsMerge::autohouyangFlog = false;
		}
		});
	connect(ui.doubleSpinBox_73, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::hyjcfd1 = ui.doubleSpinBox_73->value();
		});
	connect(ui.doubleSpinBox_74, &QDoubleSpinBox::valueChanged, [=] {
		BallsMerge::hyjcfd2 = ui.doubleSpinBox_74->value();
		});

	connect(ui.comboBox_2, &QComboBox::currentTextChanged, [=] {
		BallsMerge::tuqiujian = virtualkey(ui.comboBox_2->currentText());
		});
	connect(ui.comboBox_3, &QComboBox::currentTextChanged, [=] {
		BallsMerge::fenshenjian = virtualkey(ui.comboBox_3->currentText());
		});
	connect(ui.comboBox_4, &QComboBox::currentTextChanged, [=] {
		BallsMerge::qupijian = virtualkey(ui.comboBox_4->currentText());
		});

	connect(ui.checkBox_11, &QCheckBox::toggled, [=](bool checked) {
		if (checked)
		{
			memory->juneiqupiFlag = true;
		}
		else
		{
			memory->juneiqupiFlag = false;
		}
		});
	
	



	connect(ui.treeWidget, &QTreeWidget::itemChanged, [=](QTreeWidgetItem* item, int column) {
		// 鍙鐞嗗閫夋鎵€鍦ㄧ殑鍒楋紙閫氬父鏄0鍒楋級
		if (column != 0) return;

		// 鑾峰彇椤圭殑鏂囨湰鍜屽閫夋鐘舵€?
		QString text = item->text(column);
		bool isChecked = (item->checkState(column) == Qt::Checked);

		// 鏍规嵁鏂囨湰璁剧疆鐩稿簲鐨勬爣蹇?
		if (text == "一键三角1") {
			BallsMerge::sanjiao1Flog = isChecked;
		}
		else if (text == "一键三角2") {
			BallsMerge::sanjiao2Flog = isChecked;
		}
		else if (text == "一键三角3") {
			BallsMerge::sanjiao3Flog = isChecked;
		}
		else if (text == "冲球-直线") {
			BallsMerge::chongqiuFlog = isChecked;
		}
		else if (text == "侧合-四分") {
			BallsMerge::sifenFlog = isChecked;
		}
		else if (text == "侧合-中分") {
			BallsMerge::zhongfenFlog = isChecked;
		}
		else if (text == "旋转-初级") {
			BallsMerge::xuanzhuan1Flog = isChecked;
		}
		else if (text == "半旋-初级") {
			BallsMerge::banxuan1Flog = isChecked;
		}
		else if (text == "旋转-高级") {
			BallsMerge::xuanzhuan2Flog = isChecked;
		}
		else if (text == "半旋-高级") {
			BallsMerge::banxuan2Flog = isChecked;
		}
		else if (text == "蛇手") {
			BallsMerge::sheshouFlog = isChecked;
		}
		else if (text == "后仰") {
			BallsMerge::houyangFlog = isChecked;
		}
		});

	//table琛ㄧ浉鍏崇殑淇″彿涓庢Ы
	//涓夎1
	connect(ui.tableWidget, &QTableWidget::cellChanged, [=] {
		if(isUpdate)
		{
			if (auto item = ui.tableWidget->item(0, 1)) BallsMerge::sj1yc1 = item->text().toInt();
			if (auto item = ui.tableWidget->item(1, 1)) BallsMerge::sj1yc2 = item->text().toInt();
			if (auto item = ui.tableWidget->item(2, 1)) BallsMerge::sj1yc3 = item->text().toInt();
			if (auto item = ui.tableWidget->item(3, 1)) BallsMerge::sj1yc4 = item->text().toInt();
			if (auto item = ui.tableWidget->item(4, 1)) BallsMerge::sj1yc5 = item->text().toInt();
			if (auto item = ui.tableWidget->item(5, 1)) BallsMerge::sj1yc6 = item->text().toInt();
			if (auto item = ui.tableWidget->item(6, 1)) BallsMerge::sj1cs = item->text().toInt();
			if (auto item = ui.tableWidget->item(7, 1)) BallsMerge::sj1fsjg = item->text().toInt();
		}
		});

	//涓夎2
	connect(ui.tableWidget_2, &QTableWidget::cellChanged, [=] {
		if (isUpdate)
		{
			if (auto item = ui.tableWidget_2->item(0, 1)) BallsMerge::sj2yc1 = item->text().toInt();
			if (auto item = ui.tableWidget_2->item(1, 1)) BallsMerge::sj2yc2 = item->text().toInt();
			if (auto item = ui.tableWidget_2->item(2, 1)) BallsMerge::sj2yc3 = item->text().toInt();
			if (auto item = ui.tableWidget_2->item(3, 1)) BallsMerge::sj2yc4 = item->text().toInt();
			if (auto item = ui.tableWidget_2->item(4, 1)) BallsMerge::sj2yc5 = item->text().toInt();
			if (auto item = ui.tableWidget_2->item(5, 1)) BallsMerge::sj2yc6 = item->text().toInt();
			if (auto item = ui.tableWidget_2->item(6, 1)) BallsMerge::sj2cs = item->text().toInt();
			if (auto item = ui.tableWidget_2->item(7, 1)) BallsMerge::sj2fsjg = item->text().toInt();
		}
		});
	//涓夎3
	connect(ui.tableWidget_3, &QTableWidget::cellChanged, [=] {
		if (isUpdate)
		{
			if (auto item = ui.tableWidget_3->item(0, 1)) BallsMerge::sj3yc1 = item->text().toInt();
			if (auto item = ui.tableWidget_3->item(1, 1)) BallsMerge::sj3yc2 = item->text().toInt();
			if (auto item = ui.tableWidget_3->item(2, 1)) BallsMerge::sj3yc3 = item->text().toInt();
			if (auto item = ui.tableWidget_3->item(3, 1)) BallsMerge::sj3yc4 = item->text().toInt();
			if (auto item = ui.tableWidget_3->item(4, 1)) BallsMerge::sj3yc5 = item->text().toInt();
			if (auto item = ui.tableWidget_3->item(5, 1)) BallsMerge::sj3yc6 = item->text().toInt();
			if (auto item = ui.tableWidget_3->item(6, 1)) BallsMerge::sj3cs = item->text().toInt();
			if (auto item = ui.tableWidget_3->item(7, 1)) BallsMerge::sj3fsjg = item->text().toInt();
		}
		});
	//鍐茬悆
	connect(ui.tableWidget_4, &QTableWidget::cellChanged, [=] {
		if (isUpdate)
		{
			if (auto item = ui.tableWidget_4->item(0, 1)) BallsMerge::cqyc1 = item->text().toInt();
			if (auto item = ui.tableWidget_4->item(1, 1)) BallsMerge::cqyc2 = item->text().toInt();
			if (auto item = ui.tableWidget_4->item(2, 1)) BallsMerge::cqcs = item->text().toInt();
			if (auto item = ui.tableWidget_4->item(3, 1)) BallsMerge::cqfsjg = item->text().toInt();
		}
		});
	//鍥涘垎
	connect(ui.tableWidget_5, &QTableWidget::cellChanged, [=] {
		if (isUpdate)
		{
			if (auto item = ui.tableWidget_5->item(0, 1)) BallsMerge::sfyc1 = item->text().toInt();
			if (auto item = ui.tableWidget_5->item(1, 1)) BallsMerge::sfyc2 = item->text().toInt();
			if (auto item = ui.tableWidget_5->item(2, 1)) BallsMerge::sfyc3 = item->text().toInt();
			if (auto item = ui.tableWidget_5->item(3, 1)) BallsMerge::sfyc4 = item->text().toInt();
			if (auto item = ui.tableWidget_5->item(4, 1)) BallsMerge::sfyc5 = item->text().toInt();
			if (auto item = ui.tableWidget_5->item(5, 1)) BallsMerge::sfcs = item->text().toInt();
			if (auto item = ui.tableWidget_5->item(6, 1)) BallsMerge::sffsjg = item->text().toInt();
		}
		});
	//涓垎
	connect(ui.tableWidget_6, &QTableWidget::cellChanged, [=] {
		if (isUpdate)
		{
			if (auto item = ui.tableWidget_6->item(0, 1)) BallsMerge::zfyc1 = item->text().toInt();
			if (auto item = ui.tableWidget_6->item(1, 1)) BallsMerge::zfyc2 = item->text().toInt();
			if (auto item = ui.tableWidget_6->item(2, 1)) BallsMerge::zfyc3 = item->text().toInt();
			if (auto item = ui.tableWidget_6->item(3, 1)) BallsMerge::zfyc4 = item->text().toInt();
			if (auto item = ui.tableWidget_6->item(4, 1)) BallsMerge::zfyc5 = item->text().toInt();
			if (auto item = ui.tableWidget_6->item(5, 1)) BallsMerge::zfcs = item->text().toInt();
			if (auto item = ui.tableWidget_6->item(6, 1)) BallsMerge::zffsjg = item->text().toInt();
		}
		});
	//鍚庝话
	connect(ui.tableWidget_12, &QTableWidget::cellChanged, [=] {
		if (isUpdate)
		{
			if (auto item = ui.tableWidget_12->item(0, 1)) BallsMerge::hyyc1 = item->text().toInt();
			if (auto item = ui.tableWidget_12->item(1, 1)) BallsMerge::hyyc2 = item->text().toInt();
			if (auto item = ui.tableWidget_12->item(2, 1)) BallsMerge::hyyc3 = item->text().toInt();
			if (auto item = ui.tableWidget_12->item(3, 1)) BallsMerge::hyyc4 = item->text().toInt();
			if (auto item = ui.tableWidget_12->item(4, 1)) BallsMerge::hyyc5 = item->text().toInt();
			if (auto item = ui.tableWidget_12->item(5, 1)) BallsMerge::hycs = item->text().toInt();
			if (auto item = ui.tableWidget_12->item(6, 1)) BallsMerge::hyfsjg = item->text().toInt();
		}
		});
	//鏃嬭浆1
	connect(ui.tableWidget_7, &QTableWidget::cellChanged, [=] {
		if (isUpdate)
		{
			if (auto item = ui.tableWidget_7->item(0, 1)) BallsMerge::xz1yc1 = item->text().toInt();
			if (auto item = ui.tableWidget_7->item(1, 1)) BallsMerge::xz1yc2 = item->text().toInt();
			if (auto item = ui.tableWidget_7->item(2, 1)) BallsMerge::xz1yc3 = item->text().toInt();
			if (auto item = ui.tableWidget_7->item(3, 1)) BallsMerge::xz1yc4 = item->text().toInt();
			if (auto item = ui.tableWidget_7->item(4, 1)) BallsMerge::xz1yc5 = item->text().toInt();
			if (auto item = ui.tableWidget_7->item(5, 1)) BallsMerge::xz1yc6 = item->text().toInt();
			if (auto item = ui.tableWidget_7->item(6, 1)) BallsMerge::xz1yc7 = item->text().toInt();
			if (auto item = ui.tableWidget_7->item(7, 1)) BallsMerge::xz1yc8 = item->text().toInt();
			if (auto item = ui.tableWidget_7->item(8, 1)) BallsMerge::xz1yc9 = item->text().toInt();
			if (auto item = ui.tableWidget_7->item(9, 1)) BallsMerge::xz1cs = item->text().toInt();
			if (auto item = ui.tableWidget_7->item(10, 1)) BallsMerge::xz1fsjg = item->text().toInt();
		}
		});
	//鏃嬭浆2
	connect(ui.tableWidget_9, &QTableWidget::cellChanged, [=] {
		if (isUpdate)
		{
			if (auto item = ui.tableWidget_9->item(0, 1)) BallsMerge::xz2yc1 = item->text().toInt();
			if (auto item = ui.tableWidget_9->item(1, 1)) BallsMerge::xz2yc2 = item->text().toInt();
			if (auto item = ui.tableWidget_9->item(2, 1)) BallsMerge::xz2yc3 = item->text().toInt();
			if (auto item = ui.tableWidget_9->item(3, 1)) BallsMerge::xz2yc4 = item->text().toInt();
			if (auto item = ui.tableWidget_9->item(4, 1)) BallsMerge::xz2yc5 = item->text().toInt();
			if (auto item = ui.tableWidget_9->item(5, 1)) BallsMerge::xz2yc6 = item->text().toInt();
			if (auto item = ui.tableWidget_9->item(6, 1)) BallsMerge::xz2yc7 = item->text().toInt();
			if (auto item = ui.tableWidget_9->item(7, 1)) BallsMerge::xz2yc8 = item->text().toInt();
			if (auto item = ui.tableWidget_9->item(8, 1)) BallsMerge::xz2yc9 = item->text().toInt();
			if (auto item = ui.tableWidget_9->item(9, 1)) BallsMerge::xz2cs = item->text().toInt();
			if (auto item = ui.tableWidget_9->item(10, 1)) BallsMerge::xz2fsjg = item->text().toInt();
		}
		});
	//鍗婃棆1
	connect(ui.tableWidget_8, &QTableWidget::cellChanged, [=] {
		if (isUpdate)
		{
			if (auto item = ui.tableWidget_8->item(0, 1)) BallsMerge::bx1yc1 = item->text().toInt();
			if (auto item = ui.tableWidget_8->item(1, 1)) BallsMerge::bx1yc2 = item->text().toInt();
			if (auto item = ui.tableWidget_8->item(2, 1)) BallsMerge::bx1yc3 = item->text().toInt();
			if (auto item = ui.tableWidget_8->item(3, 1)) BallsMerge::bx1yc4 = item->text().toInt();
			if (auto item = ui.tableWidget_8->item(4, 1)) BallsMerge::bx1yc5 = item->text().toInt();
			if (auto item = ui.tableWidget_8->item(5, 1)) BallsMerge::bx1yc6 = item->text().toInt();
			if (auto item = ui.tableWidget_8->item(6, 1)) BallsMerge::bx1yc7 = item->text().toInt();
			if (auto item = ui.tableWidget_8->item(7, 1)) BallsMerge::bx1yc8 = item->text().toInt();
			if (auto item = ui.tableWidget_8->item(8, 1)) BallsMerge::bx1yc9 = item->text().toInt();
			if (auto item = ui.tableWidget_8->item(9, 1)) BallsMerge::bx1cs = item->text().toInt();
			if (auto item = ui.tableWidget_8->item(10, 1)) BallsMerge::bx1fsjg = item->text().toInt();
		}
		});
	//鍗婃棆2
	connect(ui.tableWidget_10, &QTableWidget::cellChanged, [=] {
		if (isUpdate)
		{
			if (auto item = ui.tableWidget_10->item(0, 1)) BallsMerge::bx2yc1 = item->text().toInt();
			if (auto item = ui.tableWidget_10->item(1, 1)) BallsMerge::bx2yc2 = item->text().toInt();
			if (auto item = ui.tableWidget_10->item(2, 1)) BallsMerge::bx2yc3 = item->text().toInt();
			if (auto item = ui.tableWidget_10->item(3, 1)) BallsMerge::bx2yc4 = item->text().toInt();
			if (auto item = ui.tableWidget_10->item(4, 1)) BallsMerge::bx2yc5 = item->text().toInt();
			if (auto item = ui.tableWidget_10->item(5, 1)) BallsMerge::bx2yc6 = item->text().toInt();
			if (auto item = ui.tableWidget_10->item(6, 1)) BallsMerge::bx2yc7 = item->text().toInt();
			if (auto item = ui.tableWidget_10->item(7, 1)) BallsMerge::bx2yc8 = item->text().toInt();
			if (auto item = ui.tableWidget_10->item(8, 1)) BallsMerge::bx2yc9 = item->text().toInt();
			if (auto item = ui.tableWidget_10->item(9, 1)) BallsMerge::bx2cs = item->text().toInt();
			if (auto item = ui.tableWidget_10->item(10, 1)) BallsMerge::bx2fsjg = item->text().toInt();
		}
		});
	//铔囨墜
	connect(ui.tableWidget_11, &QTableWidget::cellChanged, [=] {
		if (isUpdate)
		{
			if (auto item = ui.tableWidget_11->item(0, 1)) BallsMerge::ssyc1 = item->text().toInt();
			if (auto item = ui.tableWidget_11->item(1, 1)) BallsMerge::ssyc2 = item->text().toInt();
			if (auto item = ui.tableWidget_11->item(2, 1)) BallsMerge::ssyc3 = item->text().toInt();
			if (auto item = ui.tableWidget_11->item(3, 1)) BallsMerge::ssyc4 = item->text().toInt();
			if (auto item = ui.tableWidget_11->item(4, 1)) BallsMerge::ssyc5 = item->text().toInt();
			if (auto item = ui.tableWidget_11->item(5, 1)) BallsMerge::ssyc6 = item->text().toInt();
			if (auto item = ui.tableWidget_11->item(6, 1)) BallsMerge::ssyc7 = item->text().toInt();
			if (auto item = ui.tableWidget_11->item(7, 1)) BallsMerge::ssyc8 = item->text().toInt();
			if (auto item = ui.tableWidget_11->item(8, 1)) BallsMerge::ssyc9 = item->text().toInt();
			if (auto item = ui.tableWidget_11->item(9, 1)) BallsMerge::sscs = item->text().toInt();
			if (auto item = ui.tableWidget_11->item(10, 1)) BallsMerge::ssfsjg = item->text().toInt();
		}
		});

	//鐐瑰嚮鑿滃崟鏍廰ction,缁欐瘡寮犺〃璧嬪€?
	connect(ui.action_10, &QAction::triggered, [=] {
		//璇诲彇json鏂囦欢鐨?jzs"閿搴旂殑鍊?
		setTable(QString("jzs"));
		});
	connect(ui.action_11, &QAction::triggered, [=] {
		setTable(QString("sgts"));
		});
	connect(ui.action_12, &QAction::triggered, [=] {
		setTable(QString("zjf"));
		});
	connect(ui.action_13, &QAction::triggered, [=] {
		setTable(QString("jxx"));
		});
	connect(ui.action_14, &QAction::triggered, [=] {
		setTable(QString("tongyong"));
		});
	connect(ui.action_15, &QAction::triggered, [=] {
		setTable(QString("tongyong"));
		});

	connect(this, &MainWindow::readFinished, [=] {

		//鎻掑叆鏁版嵁
		ui.tableWidget->item(0, 1)->setText(QString::number(BallsMerge::sj1yc1));
		ui.tableWidget->item(1, 1)->setText(QString::number(BallsMerge::sj1yc2));
		ui.tableWidget->item(2, 1)->setText(QString::number(BallsMerge::sj1yc3));
		ui.tableWidget->item(3, 1)->setText(QString::number(BallsMerge::sj1yc4));
		ui.tableWidget->item(4, 1)->setText(QString::number(BallsMerge::sj1yc5));
		ui.tableWidget->item(5, 1)->setText(QString::number(BallsMerge::sj1yc6));
		ui.tableWidget->item(6, 1)->setText(QString::number(BallsMerge::sj1cs));
		ui.tableWidget->item(7, 1)->setText(QString::number(BallsMerge::sj1fsjg));

		ui.tableWidget_2->item(0, 1)->setText(QString::number(BallsMerge::sj2yc1));
		ui.tableWidget_2->item(1, 1)->setText(QString::number(BallsMerge::sj2yc2));
		ui.tableWidget_2->item(2, 1)->setText(QString::number(BallsMerge::sj2yc3));
		ui.tableWidget_2->item(3, 1)->setText(QString::number(BallsMerge::sj2yc4));
		ui.tableWidget_2->item(4, 1)->setText(QString::number(BallsMerge::sj2yc5));
		ui.tableWidget_2->item(5, 1)->setText(QString::number(BallsMerge::sj2yc6));
		ui.tableWidget_2->item(6, 1)->setText(QString::number(BallsMerge::sj2cs));
		ui.tableWidget_2->item(7, 1)->setText(QString::number(BallsMerge::sj2fsjg));
		ui.tableWidget_3->item(0, 1)->setText(QString::number(BallsMerge::sj3yc1));
		ui.tableWidget_3->item(1, 1)->setText(QString::number(BallsMerge::sj3yc2));
		ui.tableWidget_3->item(2, 1)->setText(QString::number(BallsMerge::sj3yc3));
		ui.tableWidget_3->item(3, 1)->setText(QString::number(BallsMerge::sj3yc4));
		ui.tableWidget_3->item(4, 1)->setText(QString::number(BallsMerge::sj3yc5));
		ui.tableWidget_3->item(5, 1)->setText(QString::number(BallsMerge::sj3yc6));
		ui.tableWidget_3->item(6, 1)->setText(QString::number(BallsMerge::sj3cs));
		ui.tableWidget_3->item(7, 1)->setText(QString::number(BallsMerge::sj3fsjg));

		ui.tableWidget_11->item(0, 1)->setText(QString::number(BallsMerge::ssyc1));
		ui.tableWidget_11->item(1, 1)->setText(QString::number(BallsMerge::ssyc2));
		ui.tableWidget_11->item(2, 1)->setText(QString::number(BallsMerge::ssyc3));
		ui.tableWidget_11->item(3, 1)->setText(QString::number(BallsMerge::ssyc4));
		ui.tableWidget_11->item(4, 1)->setText(QString::number(BallsMerge::ssyc5));
		ui.tableWidget_11->item(5, 1)->setText(QString::number(BallsMerge::ssyc6));
		ui.tableWidget_11->item(6, 1)->setText(QString::number(BallsMerge::ssyc7));
		ui.tableWidget_11->item(7, 1)->setText(QString::number(BallsMerge::ssyc8));
		ui.tableWidget_11->item(8, 1)->setText(QString::number(BallsMerge::ssyc9));
		ui.tableWidget_11->item(9, 1)->setText(QString::number(BallsMerge::sscs));
		ui.tableWidget_11->item(10, 1)->setText(QString::number(BallsMerge::ssfsjg));

		ui.tableWidget_7->item(0, 1)->setText(QString::number(BallsMerge::xz1yc1));
		ui.tableWidget_7->item(1, 1)->setText(QString::number(BallsMerge::xz1yc2));
		ui.tableWidget_7->item(2, 1)->setText(QString::number(BallsMerge::xz1yc3));
		ui.tableWidget_7->item(3, 1)->setText(QString::number(BallsMerge::xz1yc4));
		ui.tableWidget_7->item(4, 1)->setText(QString::number(BallsMerge::xz1yc5));
		ui.tableWidget_7->item(5, 1)->setText(QString::number(BallsMerge::xz1yc6));
		ui.tableWidget_7->item(6, 1)->setText(QString::number(BallsMerge::xz1yc7));
		ui.tableWidget_7->item(7, 1)->setText(QString::number(BallsMerge::xz1yc8));
		ui.tableWidget_7->item(8, 1)->setText(QString::number(BallsMerge::xz1yc9));
		ui.tableWidget_7->item(9, 1)->setText(QString::number(BallsMerge::xz1cs));
		ui.tableWidget_7->item(10, 1)->setText(QString::number(BallsMerge::xz1fsjg));

		ui.tableWidget_9->item(0, 1)->setText(QString::number(BallsMerge::xz2yc1));
		ui.tableWidget_9->item(1, 1)->setText(QString::number(BallsMerge::xz2yc2));
		ui.tableWidget_9->item(2, 1)->setText(QString::number(BallsMerge::xz2yc3));
		ui.tableWidget_9->item(3, 1)->setText(QString::number(BallsMerge::xz2yc4));
		ui.tableWidget_9->item(4, 1)->setText(QString::number(BallsMerge::xz2yc5));
		ui.tableWidget_9->item(5, 1)->setText(QString::number(BallsMerge::xz2yc6));
		ui.tableWidget_9->item(6, 1)->setText(QString::number(BallsMerge::xz2yc7));
		ui.tableWidget_9->item(7, 1)->setText(QString::number(BallsMerge::xz2yc8));
		ui.tableWidget_9->item(8, 1)->setText(QString::number(BallsMerge::xz2yc9));
		ui.tableWidget_9->item(9, 1)->setText(QString::number(BallsMerge::xz2cs));
		ui.tableWidget_9->item(10, 1)->setText(QString::number(BallsMerge::xz2fsjg));

		ui.tableWidget_6->item(0, 1)->setText(QString::number(BallsMerge::zfyc1));
		ui.tableWidget_6->item(1, 1)->setText(QString::number(BallsMerge::zfyc2));
		ui.tableWidget_6->item(2, 1)->setText(QString::number(BallsMerge::zfyc3));
		ui.tableWidget_6->item(3, 1)->setText(QString::number(BallsMerge::zfyc4));
		ui.tableWidget_6->item(4, 1)->setText(QString::number(BallsMerge::zfyc5));
		ui.tableWidget_6->item(5, 1)->setText(QString::number(BallsMerge::zfcs));
		ui.tableWidget_6->item(6, 1)->setText(QString::number(BallsMerge::zffsjg));

		ui.tableWidget_12->item(0, 1)->setText(QString::number(BallsMerge::hyyc1));
		ui.tableWidget_12->item(1, 1)->setText(QString::number(BallsMerge::hyyc2));
		ui.tableWidget_12->item(2, 1)->setText(QString::number(BallsMerge::hyyc3));
		ui.tableWidget_12->item(3, 1)->setText(QString::number(BallsMerge::hyyc4));
		ui.tableWidget_12->item(4, 1)->setText(QString::number(BallsMerge::hyyc5));
		ui.tableWidget_12->item(5, 1)->setText(QString::number(BallsMerge::hycs));
		ui.tableWidget_12->item(6, 1)->setText(QString::number(BallsMerge::hyfsjg));

		ui.tableWidget_8->item(0, 1)->setText(QString::number(BallsMerge::bx1yc1));
		ui.tableWidget_8->item(1, 1)->setText(QString::number(BallsMerge::bx1yc2));
		ui.tableWidget_8->item(2, 1)->setText(QString::number(BallsMerge::bx1yc3));
		ui.tableWidget_8->item(3, 1)->setText(QString::number(BallsMerge::bx1yc4));
		ui.tableWidget_8->item(4, 1)->setText(QString::number(BallsMerge::bx1yc5));
		ui.tableWidget_8->item(5, 1)->setText(QString::number(BallsMerge::bx1yc6));
		ui.tableWidget_8->item(6, 1)->setText(QString::number(BallsMerge::bx1yc7));
		ui.tableWidget_8->item(7, 1)->setText(QString::number(BallsMerge::bx1yc8));
		ui.tableWidget_8->item(8, 1)->setText(QString::number(BallsMerge::bx1yc9));
		ui.tableWidget_8->item(9, 1)->setText(QString::number(BallsMerge::bx1cs));
		ui.tableWidget_8->item(10, 1)->setText(QString::number(BallsMerge::bx1fsjg));

		ui.tableWidget_10->item(0, 1)->setText(QString::number(BallsMerge::bx2yc1));
		ui.tableWidget_10->item(1, 1)->setText(QString::number(BallsMerge::bx2yc2));
		ui.tableWidget_10->item(2, 1)->setText(QString::number(BallsMerge::bx2yc3));
		ui.tableWidget_10->item(3, 1)->setText(QString::number(BallsMerge::bx2yc4));
		ui.tableWidget_10->item(4, 1)->setText(QString::number(BallsMerge::bx2yc5));
		ui.tableWidget_10->item(5, 1)->setText(QString::number(BallsMerge::bx2yc6));
		ui.tableWidget_10->item(6, 1)->setText(QString::number(BallsMerge::bx2yc7));
		ui.tableWidget_10->item(7, 1)->setText(QString::number(BallsMerge::bx2yc8));
		ui.tableWidget_10->item(8, 1)->setText(QString::number(BallsMerge::bx2yc9));
		ui.tableWidget_10->item(9, 1)->setText(QString::number(BallsMerge::bx2cs));
		ui.tableWidget_10->item(10, 1)->setText(QString::number(BallsMerge::bx2fsjg));

		ui.tableWidget_5->item(0, 1)->setText(QString::number(BallsMerge::sfyc1));
		ui.tableWidget_5->item(1, 1)->setText(QString::number(BallsMerge::sfyc2));
		ui.tableWidget_5->item(2, 1)->setText(QString::number(BallsMerge::sfyc3));
		ui.tableWidget_5->item(3, 1)->setText(QString::number(BallsMerge::sfyc4));
		ui.tableWidget_5->item(4, 1)->setText(QString::number(BallsMerge::sfyc5));
		ui.tableWidget_5->item(5, 1)->setText(QString::number(BallsMerge::sfcs));
		ui.tableWidget_5->item(6, 1)->setText(QString::number(BallsMerge::sffsjg));
		isUpdate = true;
		});
}

MainWindow::~MainWindow()
{
	QFile file("config.json");
	if (file.open(QIODevice::ReadWrite))
	{
		QByteArray data = file.readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
		QJsonObject jsonObj = jsonDoc.object();

		jsonObj["checkBox"] = ui.checkBox->isChecked();
		jsonObj["checkBox_2"] = ui.checkBox_2->isChecked();
		jsonObj["doubleSpinBox"] = ui.doubleSpinBox->value();
		jsonObj["doubleSpinBox_2"] = ui.doubleSpinBox_2->value();
		jsonObj["checkBox_3"] = ui.checkBox_3->isChecked();
		jsonObj["checkBox_4"] = ui.checkBox_4->isChecked();
		jsonObj["doubleSpinBox_3"] = ui.doubleSpinBox_3->value();
		jsonObj["doubleSpinBox_4"] = ui.doubleSpinBox_4->value();
		jsonObj["doubleSpinBox_5"] = ui.doubleSpinBox_5->value();
		jsonObj["checkBox_5"] = ui.checkBox_5->isChecked();
		jsonObj["doubleSpinBox_6"] = ui.doubleSpinBox_6->value();
		jsonObj["checkBox_6"] = ui.checkBox_6->isChecked();
		jsonObj["checkBox_7"] = ui.checkBox_7->isChecked();
		jsonObj["checkBox_8"] = ui.checkBox_8->isChecked();
		jsonObj["doubleSpinBox_7"] = ui.doubleSpinBox_7->value();
		jsonObj["doubleSpinBox_8"] = ui.doubleSpinBox_8->value();
		jsonObj["checkBox_11"] = ui.checkBox_11->isChecked();
		jsonObj["comboBox_4"] = ui.comboBox_4->currentText();
		jsonObj["checkBox_4"] = ui.checkBox_4->isChecked();
		jsonObj["comboBox"] = ui.comboBox->currentText();
		jsonObj["comboBox_2"] = ui.comboBox_2->currentText();
		jsonObj["comboBox_3"] = ui.comboBox_3->currentText();
		jsonObj["checkBox_15"] = ui.checkBox_15->isChecked();
		jsonObj["checkBox_16"] = ui.checkBox_16->isChecked();
		//jsonObj["checkBox_18"] = ui.checkBox_18->isChecked();
		jsonObj["checkBox_21"] = ui.checkBox_21->isChecked();
		//jsonObj["checkBox_22"] = ui.checkBox_22->isChecked();
		jsonObj["checkBox_24"] = ui.checkBox_24->isChecked();
		jsonObj["checkBox_25"] = ui.checkBox_25->isChecked();
		jsonObj["checkBox_26"] = ui.checkBox_26->isChecked();
		//jsonObj["checkBox_28"] = ui.checkBox_28->isChecked();
		jsonObj["checkBox_30"] = ui.checkBox_30->isChecked();
		jsonObj["checkBox_31"] = ui.checkBox_31->isChecked();
		jsonObj["checkBox_32"] = ui.checkBox_32->isChecked();
		jsonObj["checkBox_33"] = ui.checkBox_33->isChecked();
		jsonObj["checkBox_34"] = ui.checkBox_34->isChecked();
		jsonObj["checkBox_35"] = ui.checkBox_35->isChecked();
		jsonObj["checkBox_36"] = ui.checkBox_36->isChecked();
		jsonObj["checkBox_37"] = ui.checkBox_37->isChecked();
		jsonObj["checkBox_38"] = ui.checkBox_38->isChecked();
		jsonObj["checkBox_39"] = ui.checkBox_39->isChecked();
		jsonObj["checkBox_40"] = ui.checkBox_40->isChecked();
		jsonObj["checkBox_41"] = ui.checkBox_41->isChecked();
		jsonObj["checkBox_42"] = ui.checkBox_42->isChecked();
		jsonObj["checkBox_43"] = ui.checkBox_43->isChecked();
		jsonObj["checkBox_44"] = ui.checkBox_44->isChecked();
		jsonObj["checkBox_45"] = ui.checkBox_45->isChecked();
		jsonObj["checkBox_46"] = ui.checkBox_46->isChecked();
		jsonObj["checkBox_47"] = ui.checkBox_47->isChecked();
		jsonObj["checkBox_48"] = ui.checkBox_48->isChecked();
		jsonObj["checkBox_49"] = ui.checkBox_49->isChecked();
		jsonObj["checkBox_50"] = ui.checkBox_50->isChecked();
		jsonObj["checkBox_51"] = ui.checkBox_51->isChecked();
		//jsonObj["checkBox_52"] = ui.checkBox_52->isChecked();
		jsonObj["checkBox_53"] = ui.checkBox_53->isChecked();
		jsonObj["checkBox_54"] = ui.checkBox_54->isChecked();
		jsonObj["checkBox_56"] = ui.checkBox_56->isChecked();
		jsonObj["comboBox_5"] = ui.comboBox_5->currentText();
		jsonObj["comboBox_6"] = ui.comboBox_6->currentText();
		jsonObj["comboBox_7"] = ui.comboBox_7->currentText();
		jsonObj["comboBox_8"] = ui.comboBox_8->currentText();
		jsonObj["comboBox_9"] = ui.comboBox_9->currentText();
		jsonObj["comboBox_10"] = ui.comboBox_10->currentText();
		jsonObj["comboBox_11"] = ui.comboBox_11->currentText();
		jsonObj["comboBox_12"] = ui.comboBox_12->currentText();
		jsonObj["comboBox_13"] = ui.comboBox_13->currentText();
		jsonObj["comboBox_14"] = ui.comboBox_14->currentText();
		jsonObj["comboBox_15"] = ui.comboBox_15->currentText();
		jsonObj["comboBox_16"] = ui.comboBox_16->currentText();
		jsonObj["comboBox_17"] = ui.comboBox_17->currentText();
		jsonObj["comboBox_18"] = ui.comboBox_18->currentText();
		jsonObj["comboBox_19"] = ui.comboBox_19->currentText();
		jsonObj["comboBox_20"] = ui.comboBox_20->currentText();
		jsonObj["comboBox_21"] = ui.comboBox_21->currentText();
		jsonObj["comboBox_22"] = ui.comboBox_22->currentText();
		jsonObj["comboBox_23"] = ui.comboBox_23->currentText();
		jsonObj["comboBox_24"] = ui.comboBox_24->currentText();
		jsonObj["comboBox_25"] = ui.comboBox_25->currentText();
		//jsonObj["comboBox_26"] = ui.comboBox_26->currentText();
		//jsonObj["comboBox_27"] = ui.comboBox_27->currentText();
		jsonObj["doubleSpinBox_14"] = ui.doubleSpinBox_14->value();
		jsonObj["doubleSpinBox_15"] = ui.doubleSpinBox_15->value();
		jsonObj["doubleSpinBox_16"] = ui.doubleSpinBox_16->value();
		jsonObj["doubleSpinBox_18"] = ui.doubleSpinBox_18->value();
		jsonObj["doubleSpinBox_20"] = ui.doubleSpinBox_20->value();
		jsonObj["doubleSpinBox_21"] = ui.doubleSpinBox_21->value();
		jsonObj["doubleSpinBox_31"] = ui.doubleSpinBox_31->value();
		jsonObj["doubleSpinBox_32"] = ui.doubleSpinBox_32->value();
		jsonObj["doubleSpinBox_33"] = ui.doubleSpinBox_33->value();
		jsonObj["doubleSpinBox_34"] = ui.doubleSpinBox_34->value();
		jsonObj["doubleSpinBox_35"] = ui.doubleSpinBox_35->value();
		jsonObj["doubleSpinBox_36"] = ui.doubleSpinBox_36->value();
		jsonObj["doubleSpinBox_37"] = ui.doubleSpinBox_37->value();
		jsonObj["doubleSpinBox_38"] = ui.doubleSpinBox_38->value();
		jsonObj["doubleSpinBox_39"] = ui.doubleSpinBox_39->value();
		jsonObj["doubleSpinBox_40"] = ui.doubleSpinBox_40->value();
		jsonObj["doubleSpinBox_41"] = ui.doubleSpinBox_41->value();
		jsonObj["doubleSpinBox_42"] = ui.doubleSpinBox_42->value();
		jsonObj["doubleSpinBox_43"] = ui.doubleSpinBox_43->value();
		jsonObj["doubleSpinBox_44"] = ui.doubleSpinBox_44->value();
		jsonObj["doubleSpinBox_45"] = ui.doubleSpinBox_45->value();
		jsonObj["doubleSpinBox_46"] = ui.doubleSpinBox_46->value();
		jsonObj["doubleSpinBox_47"] = ui.doubleSpinBox_47->value();
		jsonObj["doubleSpinBox_48"] = ui.doubleSpinBox_48->value();
		jsonObj["doubleSpinBox_49"] = ui.doubleSpinBox_49->value();
		jsonObj["doubleSpinBox_50"] = ui.doubleSpinBox_50->value();
		jsonObj["doubleSpinBox_51"] = ui.doubleSpinBox_51->value();
		jsonObj["doubleSpinBox_52"] = ui.doubleSpinBox_52->value();
		jsonObj["doubleSpinBox_53"] = ui.doubleSpinBox_53->value();
		jsonObj["doubleSpinBox_54"] = ui.doubleSpinBox_54->value();
		jsonObj["doubleSpinBox_55"] = ui.doubleSpinBox_55->value();
		jsonObj["doubleSpinBox_56"] = ui.doubleSpinBox_56->value();
		jsonObj["doubleSpinBox_57"] = ui.doubleSpinBox_57->value();
		jsonObj["doubleSpinBox_58"] = ui.doubleSpinBox_58->value();
		jsonObj["doubleSpinBox_59"] = ui.doubleSpinBox_59->value();
		jsonObj["doubleSpinBox_60"] = ui.doubleSpinBox_60->value();
		jsonObj["doubleSpinBox_61"] = ui.doubleSpinBox_61->value();
		jsonObj["doubleSpinBox_62"] = ui.doubleSpinBox_62->value();
		jsonObj["doubleSpinBox_63"] = ui.doubleSpinBox_63->value();
		jsonObj["doubleSpinBox_64"] = ui.doubleSpinBox_64->value();
		jsonObj["doubleSpinBox_65"] = ui.doubleSpinBox_65->value();
		jsonObj["doubleSpinBox_66"] = ui.doubleSpinBox_66->value();
		jsonObj["doubleSpinBox_67"] = ui.doubleSpinBox_67->value();
		jsonObj["doubleSpinBox_68"] = ui.doubleSpinBox_68->value();
		jsonObj["doubleSpinBox_69"] = ui.doubleSpinBox_69->value();
		jsonObj["doubleSpinBox_70"] = ui.doubleSpinBox_70->value();
		jsonObj["doubleSpinBox_71"] = ui.doubleSpinBox_71->value();
		jsonObj["doubleSpinBox_72"] = ui.doubleSpinBox_72->value();
		jsonObj["doubleSpinBox_73"] = ui.doubleSpinBox_73->value();
		jsonObj["doubleSpinBox_74"] = ui.doubleSpinBox_74->value();
		jsonObj["doubleSpinBox_75"] = ui.doubleSpinBox_75->value();
		jsonObj["doubleSpinBox_76"] = ui.doubleSpinBox_76->value();
		jsonObj["doubleSpinBox_77"] = ui.doubleSpinBox_77->value();
		jsonObj["doubleSpinBox_78"] = ui.doubleSpinBox_78->value();
		jsonObj["lineEdit_2"] = ui.lineEdit_2->text();


		//鍌ㄥ瓨涓夎1table
		jsonObj["sj1yc1"] = BallsMerge::sj1yc1;
		jsonObj["sj1yc2"] = BallsMerge::sj1yc2;
		jsonObj["sj1yc3"] = BallsMerge::sj1yc3;
		jsonObj["sj1yc4"] = BallsMerge::sj1yc4;
		jsonObj["sj1yc5"] = BallsMerge::sj1yc5;
		jsonObj["sj1yc6"] = BallsMerge::sj1yc6;
		jsonObj["sj1cs"] = BallsMerge::sj1cs;
		jsonObj["sj1fsjg"] = BallsMerge::sj1fsjg;
	
		//鍌ㄥ瓨涓夎2table
		jsonObj["sj2yc1"] = BallsMerge::sj2yc1;
		jsonObj["sj2yc2"] = BallsMerge::sj2yc2;
		jsonObj["sj2yc3"] = BallsMerge::sj2yc3;
		jsonObj["sj2yc4"] = BallsMerge::sj2yc4;
		jsonObj["sj2yc5"] = BallsMerge::sj2yc5;
		jsonObj["sj2yc6"] = BallsMerge::sj2yc6;
		jsonObj["sj2cs"] = BallsMerge::sj2cs;
		jsonObj["sj2fsjg"] = BallsMerge::sj2fsjg;
		//鍌ㄥ瓨涓夎3table
		jsonObj["sj3yc1"] = BallsMerge::sj3yc1;
		jsonObj["sj3yc2"] = BallsMerge::sj3yc2;
		jsonObj["sj3yc3"] = BallsMerge::sj3yc3;
		jsonObj["sj3yc4"] = BallsMerge::sj3yc4;
		jsonObj["sj3yc5"] = BallsMerge::sj3yc5;
		jsonObj["sj3yc6"] = BallsMerge::sj3yc6;
		jsonObj["sj3cs"] = BallsMerge::sj3cs;
		jsonObj["sj3fsjg"] = BallsMerge::sj3fsjg;
		//鍌ㄥ瓨鍐茬悆table
		jsonObj["cqyc1"] = BallsMerge::cqyc1;
		jsonObj["cqyc2"] = BallsMerge::cqyc2;
		jsonObj["cqcs"] = BallsMerge::cqcs;
		jsonObj["cqfsjg"] = BallsMerge::cqfsjg;
		//鍌ㄥ瓨鍥涘垎table
		jsonObj["sfyc1"] = BallsMerge::sfyc1;
		jsonObj["sfyc2"] = BallsMerge::sfyc2;
		jsonObj["sfyc3"] = BallsMerge::sfyc3;
		jsonObj["sfyc4"] = BallsMerge::sfyc4;
		jsonObj["sfyc5"] = BallsMerge::sfyc5;
		jsonObj["sfcs"] = BallsMerge::sfcs;
		jsonObj["sffsjg"] = BallsMerge::sffsjg;
		//鍌ㄥ瓨涓垎table
		jsonObj["zfyc1"] = BallsMerge::zfyc1;
		jsonObj["zfyc2"] = BallsMerge::zfyc2;
		jsonObj["zfyc3"] = BallsMerge::zfyc3;
		jsonObj["zfyc4"] = BallsMerge::zfyc4;
		jsonObj["zfyc5"] = BallsMerge::zfyc5;
		jsonObj["zfcs"] = BallsMerge::zfcs;
		jsonObj["zffsjg"] = BallsMerge::zffsjg;
		//鍌ㄥ瓨鍚庝话table
		jsonObj["hyyc1"] = BallsMerge::hyyc1;
		jsonObj["hyyc2"] = BallsMerge::hyyc2;
		jsonObj["hyyc3"] = BallsMerge::hyyc3;
		jsonObj["hyyc4"] = BallsMerge::hyyc4;
		jsonObj["hyyc5"] = BallsMerge::hyyc5;
		jsonObj["hycs"] = BallsMerge::hycs;
		jsonObj["hyfsjg"] = BallsMerge::hyfsjg;
		//鍌ㄥ瓨鏃嬭浆1table
		jsonObj["xz1yc1"] = BallsMerge::xz1yc1;
		jsonObj["xz1yc2"] = BallsMerge::xz1yc2;
		jsonObj["xz1yc3"] = BallsMerge::xz1yc3;
		jsonObj["xz1yc4"] = BallsMerge::xz1yc4;
		jsonObj["xz1yc5"] = BallsMerge::xz1yc5;
		jsonObj["xz1yc6"] = BallsMerge::xz1yc6;
		jsonObj["xz1yc7"] = BallsMerge::xz1yc7;
		jsonObj["xz1yc8"] = BallsMerge::xz1yc8;
		jsonObj["xz1yc9"] = BallsMerge::xz1yc9;
		jsonObj["xz1cs"] = BallsMerge::xz1cs;
		jsonObj["xz1fsjg"] = BallsMerge::xz1fsjg;
		//鍌ㄥ瓨鏃嬭浆2table
		jsonObj["xz2yc1"] = BallsMerge::xz2yc1;
		jsonObj["xz2yc2"] = BallsMerge::xz2yc2;
		jsonObj["xz2yc3"] = BallsMerge::xz2yc3;
		jsonObj["xz2yc4"] = BallsMerge::xz2yc4;
		jsonObj["xz2yc5"] = BallsMerge::xz2yc5;
		jsonObj["xz2yc6"] = BallsMerge::xz2yc6;
		jsonObj["xz2yc7"] = BallsMerge::xz2yc7;
		jsonObj["xz2yc8"] = BallsMerge::xz2yc8;
		jsonObj["xz2yc9"] = BallsMerge::xz2yc9;
		jsonObj["xz2cs"] = BallsMerge::xz2cs;
		jsonObj["xz2fsjg"] = BallsMerge::xz2fsjg;
		//鍌ㄥ瓨鍗婃棆1table
		jsonObj["bx1yc1"] = BallsMerge::bx1yc1;
		jsonObj["bx1yc2"] = BallsMerge::bx1yc2;
		jsonObj["bx1yc3"] = BallsMerge::bx1yc3;
		jsonObj["bx1yc4"] = BallsMerge::bx1yc4;
		jsonObj["bx1yc5"] = BallsMerge::bx1yc5;
		jsonObj["bx1yc6"] = BallsMerge::bx1yc6;
		jsonObj["bx1yc7"] = BallsMerge::bx1yc7;
		jsonObj["bx1yc8"] = BallsMerge::bx1yc8;
		jsonObj["bx1yc9"] = BallsMerge::bx1yc9;
		jsonObj["bx1cs"] = BallsMerge::bx1cs;
		jsonObj["bx1fsjg"] = BallsMerge::bx1fsjg;
		//鍌ㄥ瓨鍗婃棆2table
		jsonObj["bx2yc1"] = BallsMerge::bx2yc1;
		jsonObj["bx2yc2"] = BallsMerge::bx2yc2;
		jsonObj["bx2yc3"] = BallsMerge::bx2yc3;
		jsonObj["bx2yc4"] = BallsMerge::bx2yc4;
		jsonObj["bx2yc5"] = BallsMerge::bx2yc5;
		jsonObj["bx2yc6"] = BallsMerge::bx2yc6;
		jsonObj["bx2yc7"] = BallsMerge::bx2yc7;
		jsonObj["bx2yc8"] = BallsMerge::bx2yc8;
		jsonObj["bx2yc9"] = BallsMerge::bx2yc9;
		jsonObj["bx2cs"] = BallsMerge::bx2cs;
		jsonObj["bx2fsjg"] = BallsMerge::bx2fsjg;
		//鍌ㄥ瓨铔囨墜table
		jsonObj["ssyc1"] = BallsMerge::ssyc1;
		jsonObj["ssyc2"] = BallsMerge::ssyc2;
		jsonObj["ssyc3"] = BallsMerge::ssyc3;
		jsonObj["ssyc4"] = BallsMerge::ssyc4;
		jsonObj["ssyc5"] = BallsMerge::ssyc5;
		jsonObj["ssyc6"] = BallsMerge::ssyc6;
		jsonObj["ssyc7"] = BallsMerge::ssyc7;
		jsonObj["ssyc8"] = BallsMerge::ssyc8;
		jsonObj["ssyc9"] = BallsMerge::ssyc9;
		jsonObj["sscs"] = BallsMerge::sscs;
		jsonObj["ssfsjg"] = BallsMerge::ssfsjg;

		jsonObj["sanjiao1Flog"] = BallsMerge::sanjiao1Flog;
		jsonObj["sanjiao2Flog"] = BallsMerge::sanjiao2Flog;
		jsonObj["sanjiao3Flog"] = BallsMerge::sanjiao3Flog;
		jsonObj["chongqiuFlog"] = BallsMerge::chongqiuFlog;
		jsonObj["sifenFlog"] = BallsMerge::sifenFlog;
		jsonObj["zhongfenFlog"] = BallsMerge::zhongfenFlog;
		jsonObj["xuanzhuan1Flog"] = BallsMerge::xuanzhuan1Flog;
		jsonObj["banxuan1Flog"] = BallsMerge::banxuan1Flog;
		jsonObj["xuanzhuan2Flog"] = BallsMerge::xuanzhuan2Flog;
		jsonObj["banxuan2Flog"] = BallsMerge::banxuan2Flog;
		jsonObj["sheshouFlog"] = BallsMerge::sheshouFlog;
		jsonObj["houyangFlog"] = BallsMerge::houyangFlog;

		QJsonDocument updatedJsonDoc(jsonObj); 
		file.resize(0);
		file.write(updatedJsonDoc.toJson());

		if (WheelHookID != nullptr)
		{
			UnhookWindowsHookEx(WheelHookID);
			WheelHookID = nullptr;
		}

		if (Memory::hProcess != NULL)
		{
			CloseHandle(Memory::hProcess);
			//Memory::hProcess = NULL; // 灏嗗彞鏌勮缃负NULL锛岄伩鍏嶆偓鎸傛寚閽?
		}
	}
}

void MainWindow::setTable(const QString mode)
{
	QString filePath = "config.json";
	if (QFile::exists(filePath))
	{
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly))
		{
			QMessageBox::warning(this, "警告", "找不到config.json文件");
		}
		else
		{
			//璇诲彇鍚嶅彨jzs杩欎釜閿噷闈㈢殑閿€煎
			QByteArray data = file.readAll();
			QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
			QJsonObject jsonObj = jsonDoc.object();
			QJsonObject Obj = jsonObj.value(mode).toObject();
			BallsMerge::sj1cs = Obj.value("sj1cs").toInt();
			BallsMerge::sj1fsjg = Obj.value("sj1fsjg").toInt();
			BallsMerge::sj1yc1 = Obj.value("sj1yc1").toInt();
			BallsMerge::sj1yc2 = Obj.value("sj1yc2").toInt();
			BallsMerge::sj1yc3 = Obj.value("sj1yc3").toInt();
			BallsMerge::sj1yc4 = Obj.value("sj1yc4").toInt();
			BallsMerge::sj1yc5 = Obj.value("sj1yc5").toInt();
			BallsMerge::sj1yc6 = Obj.value("sj1yc6").toInt();
			BallsMerge::sj2cs = Obj.value("sj2cs").toInt();
			BallsMerge::sj2fsjg = Obj.value("sj2fsjg").toInt();
			BallsMerge::sj2yc1 = Obj.value("sj2yc1").toInt();
			BallsMerge::sj2yc2 = Obj.value("sj2yc2").toInt();
			BallsMerge::sj2yc3 = Obj.value("sj2yc3").toInt();
			BallsMerge::sj2yc4 = Obj.value("sj2yc4").toInt();
			BallsMerge::sj2yc5 = Obj.value("sj2yc5").toInt();
			BallsMerge::sj2yc6 = Obj.value("sj2yc6").toInt();
			BallsMerge::sj3cs = Obj.value("sj3cs").toInt();
			BallsMerge::sj3fsjg = Obj.value("sj3fsjg").toInt();
			BallsMerge::sj3yc1 = Obj.value("sj3yc1").toInt();
			BallsMerge::sj3yc2 = Obj.value("sj3yc2").toInt();
			BallsMerge::sj3yc3 = Obj.value("sj3yc3").toInt();
			BallsMerge::sj3yc4 = Obj.value("sj3yc4").toInt();
			BallsMerge::sj3yc5 = Obj.value("sj3yc5").toInt();
			BallsMerge::sj3yc6 = Obj.value("sj3yc6").toInt();
			BallsMerge::sscs = Obj.value("sscs").toInt();
			BallsMerge::ssfsjg = Obj.value("ssfsjg").toInt();
			BallsMerge::ssyc1 = Obj.value("ssyc1").toInt();
			BallsMerge::ssyc2 = Obj.value("ssyc2").toInt();
			BallsMerge::ssyc3 = Obj.value("ssyc3").toInt();
			BallsMerge::ssyc4 = Obj.value("ssyc4").toInt();
			BallsMerge::ssyc5 = Obj.value("ssyc5").toInt();
			BallsMerge::ssyc6 = Obj.value("ssyc6").toInt();
			BallsMerge::ssyc7 = Obj.value("ssyc7").toInt();
			BallsMerge::ssyc8 = Obj.value("ssyc8").toInt();
			BallsMerge::ssyc9 = Obj.value("ssyc9").toInt();
			BallsMerge::xz1cs = Obj.value("xz1cs").toInt();
			BallsMerge::xz1fsjg = Obj.value("xz1fsjg").toInt();
			BallsMerge::xz1yc1 = Obj.value("xz1yc1").toInt();
			BallsMerge::xz1yc2 = Obj.value("xz1yc2").toInt();
			BallsMerge::xz1yc3 = Obj.value("xz1yc3").toInt();
			BallsMerge::xz1yc4 = Obj.value("xz1yc4").toInt();
			BallsMerge::xz1yc5 = Obj.value("xz1yc5").toInt();
			BallsMerge::xz1yc6 = Obj.value("xz1yc6").toInt();
			BallsMerge::xz1yc7 = Obj.value("xz1yc7").toInt();
			BallsMerge::xz1yc8 = Obj.value("xz1yc8").toInt();
			BallsMerge::xz1yc9 = Obj.value("xz1yc9").toInt();
			BallsMerge::xz2cs = Obj.value("xz2cs").toInt();
			BallsMerge::xz2fsjg = Obj.value("xz2fsjg").toInt();
			BallsMerge::xz2yc1 = Obj.value("xz2yc1").toInt();
			BallsMerge::xz2yc2 = Obj.value("xz2yc2").toInt();
			BallsMerge::xz2yc3 = Obj.value("xz2yc3").toInt();
			BallsMerge::xz2yc4 = Obj.value("xz2yc4").toInt();
			BallsMerge::xz2yc5 = Obj.value("xz2yc5").toInt();
			BallsMerge::xz2yc6 = Obj.value("xz2yc6").toInt();
			BallsMerge::xz2yc7 = Obj.value("xz2yc7").toInt();
			BallsMerge::xz2yc8 = Obj.value("xz2yc8").toInt();
			BallsMerge::xz2yc9 = Obj.value("xz2yc9").toInt();
			BallsMerge::zfcs = Obj.value("zfcs").toInt();
			BallsMerge::zffsjg = Obj.value("zffsjg").toInt();
			BallsMerge::zfyc1 = Obj.value("zfyc1").toInt();
			BallsMerge::zfyc2 = Obj.value("zfyc2").toInt();
			BallsMerge::zfyc3 = Obj.value("zfyc3").toInt();
			BallsMerge::zfyc4 = Obj.value("zfyc4").toInt();
			BallsMerge::zfyc5 = Obj.value("zfyc5").toInt();
			BallsMerge::hycs = Obj.value("hycs").toInt();
			BallsMerge::hyfsjg = Obj.value("hyfsjg").toInt();
			BallsMerge::hyyc1 = Obj.value("hyyc1").toInt();
			BallsMerge::hyyc2 = Obj.value("hyyc2").toInt();
			BallsMerge::hyyc3 = Obj.value("hyyc3").toInt();
			BallsMerge::hyyc4 = Obj.value("hyyc4").toInt();
			BallsMerge::hyyc5 = Obj.value("hyyc5").toInt();
			BallsMerge::bx1cs = Obj.value("bx1cs").toInt();
			BallsMerge::bx1fsjg = Obj.value("bx1fsjg").toInt();
			BallsMerge::bx1yc1 = Obj.value("bx1yc1").toInt();
			BallsMerge::bx1yc2 = Obj.value("bx1yc2").toInt();
			BallsMerge::bx1yc3 = Obj.value("bx1yc3").toInt();
			BallsMerge::bx1yc4 = Obj.value("bx1yc4").toInt();
			BallsMerge::bx1yc5 = Obj.value("bx1yc5").toInt();
			BallsMerge::bx1yc6 = Obj.value("bx1yc6").toInt();
			BallsMerge::bx1yc7 = Obj.value("bx1yc7").toInt();
			BallsMerge::bx1yc8 = Obj.value("bx1yc8").toInt();
			BallsMerge::bx1yc9 = Obj.value("bx1yc9").toInt();
			BallsMerge::bx2cs = Obj.value("bx2cs").toInt();
			BallsMerge::bx2fsjg = Obj.value("bx2fsjg").toInt();
			BallsMerge::bx2yc1 = Obj.value("bx2yc1").toInt();
			BallsMerge::bx2yc2 = Obj.value("bx2yc2").toInt();
			BallsMerge::bx2yc3 = Obj.value("bx2yc3").toInt();
			BallsMerge::bx2yc4 = Obj.value("bx2yc4").toInt();
			BallsMerge::bx2yc5 = Obj.value("bx2yc5").toInt();
			BallsMerge::bx2yc6 = Obj.value("bx2yc6").toInt();
			BallsMerge::bx2yc7 = Obj.value("bx2yc7").toInt();
			BallsMerge::bx2yc8 = Obj.value("bx2yc8").toInt();
			BallsMerge::bx2yc9 = Obj.value("bx2yc9").toInt();
			BallsMerge::sfcs = Obj.value("sfcs").toInt();
			BallsMerge::sffsjg = Obj.value("sffsjg").toInt();
			BallsMerge::sfyc1 = Obj.value("sfyc1").toInt();
			BallsMerge::sfyc2 = Obj.value("sfyc2").toInt();
			BallsMerge::sfyc3 = Obj.value("sfyc3").toInt();
			BallsMerge::sfyc4 = Obj.value("sfyc4").toInt();
			BallsMerge::sfyc5 = Obj.value("sfyc5").toInt();

			file.close();

			isUpdate = false;

			emit readFinished();	
		}
	}
}

void MainWindow::loadjson()
{
	QString filePath = "config.json";
	if (QFile::exists(filePath))
	{
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly))
		{
			QMessageBox::warning(this, "警告", "找不到config.json文件");
		}
		else
		{
			//璇诲彇json鏂囦欢
			QByteArray data = file.readAll();
			QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
			QJsonObject jsonObj = jsonDoc.object();

			bool checkBox = jsonObj.value("checkBox").toBool();
			bool checkBox_2 = jsonObj.value("checkBox_2").toBool();
			float doubleSpinBox = jsonObj.value("doubleSpinBox").toDouble();
			float doubleSpinBox_2 = jsonObj.value("doubleSpinBox_2").toDouble();
			bool checkBox_3 = jsonObj.value("checkBox_3").toBool();
			bool checkBox_4 = jsonObj.value("checkBox_4").toBool();
			float doubleSpinBox_3 = jsonObj.value("doubleSpinBox_3").toDouble();
			float doubleSpinBox_4 = jsonObj.value("doubleSpinBox_4").toDouble();
			float doubleSpinBox_5 = jsonObj.value("doubleSpinBox_5").toDouble();
			bool checkBox_5 = jsonObj.value("checkBox_5").toBool();
			float doubleSpinBox_6 = jsonObj.value("doubleSpinBox_6").toDouble();
			bool checkBox_6 = jsonObj.value("checkBox_6").toBool();
			bool checkBox_7 = jsonObj.value("checkBox_7").toBool();
			bool checkBox_8 = jsonObj.value("checkBox_8").toBool();
			float doubleSpinBox_7 = jsonObj.value("doubleSpinBox_7").toDouble();
			float doubleSpinBox_8 = jsonObj.value("doubleSpinBox_8").toDouble();
			bool checkBox_11 = jsonObj.value("checkBox_11").toBool();
			QString comboBox_4 = jsonObj.value("comboBox_4").toString();
			bool checkBox_12 = jsonObj.value("checkBox_12").toBool();
			bool checkBox_13 = jsonObj.value("checkBox_13").toBool();
			bool checkBox_14 = jsonObj.value("checkBox_14").toBool();
			float doubleSpinBox_10 = jsonObj.value("doubleSpinBox_10").toDouble();
			float doubleSpinBox_11 = jsonObj.value("doubleSpinBox_11").toDouble();
			float doubleSpinBox_12 = jsonObj.value("doubleSpinBox_12").toDouble();
			bool checkBox_15 = jsonObj.value("checkBox_15").toBool();
			bool checkBox_16 = jsonObj.value("checkBox_16").toBool();
			bool checkBox_18 = jsonObj.value("checkBox_18").toBool();
			bool checkBox_21 = jsonObj.value("checkBox_21").toBool();
			bool checkBox_22 = jsonObj.value("checkBox_22").toBool();
			bool checkBox_24 = jsonObj.value("checkBox_24").toBool();
			bool checkBox_25 = jsonObj.value("checkBox_25").toBool();
			bool checkBox_26 = jsonObj.value("checkBox_26").toBool();
			bool checkBox_28 = jsonObj.value("checkBox_28").toBool();
			bool checkBox_30 = jsonObj.value("checkBox_30").toBool();
			bool checkBox_31 = jsonObj.value("checkBox_31").toBool();
			bool checkBox_32 = jsonObj.value("checkBox_32").toBool();
			bool checkBox_33 = jsonObj.value("checkBox_33").toBool();
			bool checkBox_34 = jsonObj.value("checkBox_34").toBool();
			bool checkBox_35 = jsonObj.value("checkBox_35").toBool();
			bool checkBox_36 = jsonObj.value("checkBox_36").toBool();
			bool checkBox_37 = jsonObj.value("checkBox_37").toBool();
			bool checkBox_38 = jsonObj.value("checkBox_38").toBool();
			bool checkBox_39 = jsonObj.value("checkBox_39").toBool();
			bool checkBox_40 = jsonObj.value("checkBox_40").toBool();
			bool checkBox_41 = jsonObj.value("checkBox_41").toBool();
			bool checkBox_42 = jsonObj.value("checkBox_42").toBool();
			bool checkBox_43 = jsonObj.value("checkBox_43").toBool();
			bool checkBox_44 = jsonObj.value("checkBox_44").toBool();
			bool checkBox_45 = jsonObj.value("checkBox_45").toBool();
			bool checkBox_46 = jsonObj.value("checkBox_46").toBool();
			bool checkBox_47 = jsonObj.value("checkBox_47").toBool();
			bool checkBox_48 = jsonObj.value("checkBox_48").toBool();
			bool checkBox_49 = jsonObj.value("checkBox_49").toBool();
			bool checkBox_50 = jsonObj.value("checkBox_50").toBool();
			bool checkBox_51 = jsonObj.value("checkBox_51").toBool();
			bool checkBox_52 = jsonObj.value("checkBox_52").toBool();
			bool checkBox_53 = jsonObj.value("checkBox_53").toBool();
			bool checkBox_54 = jsonObj.value("checkBox_54").toBool();
			bool checkBox_56 = jsonObj.value("checkBox_56").toBool();
			QString comboBox_10 = jsonObj.value("comboBox_10").toString();
			QString comboBox = jsonObj.value("comboBox").toString();
			QString comboBox_11 = jsonObj.value("comboBox_11").toString();
			QString comboBox_12 = jsonObj.value("comboBox_12").toString();
			QString comboBox_13 = jsonObj.value("comboBox_13").toString();
			QString comboBox_14 = jsonObj.value("comboBox_14").toString();
			QString comboBox_15 = jsonObj.value("comboBox_15").toString();
			QString comboBox_16 = jsonObj.value("comboBox_16").toString();
			QString comboBox_17 = jsonObj.value("comboBox_17").toString();
			QString comboBox_18 = jsonObj.value("comboBox_18").toString();
			QString comboBox_19 = jsonObj.value("comboBox_19").toString();
			QString comboBox_2 = jsonObj.value("comboBox_2").toString();
			QString comboBox_20 = jsonObj.value("comboBox_20").toString();
			QString comboBox_21 = jsonObj.value("comboBox_21").toString();
			QString comboBox_22 = jsonObj.value("comboBox_22").toString();
			QString comboBox_23 = jsonObj.value("comboBox_23").toString();
			QString comboBox_24 = jsonObj.value("comboBox_24").toString();
			QString comboBox_25 = jsonObj.value("comboBox_25").toString();
			QString comboBox_26 = jsonObj.value("comboBox_26").toString();
			QString comboBox_27 = jsonObj.value("comboBox_27").toString();
			QString comboBox_3 = jsonObj.value("comboBox_3").toString();
			QString comboBox_5 = jsonObj.value("comboBox_5").toString();
			QString comboBox_6 = jsonObj.value("comboBox_6").toString();
			QString comboBox_7 = jsonObj.value("comboBox_7").toString();
			QString comboBox_8 = jsonObj.value("comboBox_8").toString();
			QString comboBox_9 = jsonObj.value("comboBox_9").toString();
			float doubleSpinBox_14 = jsonObj.value("doubleSpinBox_14").toDouble();
			float doubleSpinBox_15 = jsonObj.value("doubleSpinBox_15").toDouble();
			float doubleSpinBox_16 = jsonObj.value("doubleSpinBox_16").toDouble();
			float doubleSpinBox_18 = jsonObj.value("doubleSpinBox_18").toDouble();
			float doubleSpinBox_20 = jsonObj.value("doubleSpinBox_20").toDouble();
			float doubleSpinBox_21 = jsonObj.value("doubleSpinBox_21").toDouble();
			float doubleSpinBox_31 = jsonObj.value("doubleSpinBox_31").toDouble();
			float doubleSpinBox_32 = jsonObj.value("doubleSpinBox_32").toDouble();
			float doubleSpinBox_33 = jsonObj.value("doubleSpinBox_33").toDouble();
			float doubleSpinBox_34 = jsonObj.value("doubleSpinBox_34").toDouble();
			float doubleSpinBox_35 = jsonObj.value("doubleSpinBox_35").toDouble();
			float doubleSpinBox_36 = jsonObj.value("doubleSpinBox_36").toDouble();
			float doubleSpinBox_37 = jsonObj.value("doubleSpinBox_37").toDouble();
			float doubleSpinBox_38 = jsonObj.value("doubleSpinBox_38").toDouble();
			float doubleSpinBox_39 = jsonObj.value("doubleSpinBox_39").toDouble();
			float doubleSpinBox_40 = jsonObj.value("doubleSpinBox_40").toDouble();
			float doubleSpinBox_41 = jsonObj.value("doubleSpinBox_41").toDouble();
			float doubleSpinBox_42 = jsonObj.value("doubleSpinBox_42").toDouble();
			float doubleSpinBox_43 = jsonObj.value("doubleSpinBox_43").toDouble();
			float doubleSpinBox_44 = jsonObj.value("doubleSpinBox_44").toDouble();
			float doubleSpinBox_45 = jsonObj.value("doubleSpinBox_45").toDouble();
			float doubleSpinBox_46 = jsonObj.value("doubleSpinBox_46").toDouble();
			float doubleSpinBox_47 = jsonObj.value("doubleSpinBox_47").toDouble();
			float doubleSpinBox_48 = jsonObj.value("doubleSpinBox_48").toDouble();
			float doubleSpinBox_49 = jsonObj.value("doubleSpinBox_49").toDouble();
			float doubleSpinBox_50 = jsonObj.value("doubleSpinBox_50").toDouble();
			float doubleSpinBox_51 = jsonObj.value("doubleSpinBox_51").toDouble();
			float doubleSpinBox_52 = jsonObj.value("doubleSpinBox_52").toDouble();
			float doubleSpinBox_53 = jsonObj.value("doubleSpinBox_53").toDouble();
			float doubleSpinBox_54 = jsonObj.value("doubleSpinBox_54").toDouble();
			float doubleSpinBox_55 = jsonObj.value("doubleSpinBox_55").toDouble();
			float doubleSpinBox_56 = jsonObj.value("doubleSpinBox_56").toDouble();
			float doubleSpinBox_57 = jsonObj.value("doubleSpinBox_57").toDouble();
			float doubleSpinBox_58 = jsonObj.value("doubleSpinBox_58").toDouble();
			float doubleSpinBox_59 = jsonObj.value("doubleSpinBox_59").toDouble();
			float doubleSpinBox_60 = jsonObj.value("doubleSpinBox_60").toDouble();
			float doubleSpinBox_61 = jsonObj.value("doubleSpinBox_61").toDouble();
			float doubleSpinBox_62 = jsonObj.value("doubleSpinBox_62").toDouble();
			float doubleSpinBox_63 = jsonObj.value("doubleSpinBox_63").toDouble();
			float doubleSpinBox_64 = jsonObj.value("doubleSpinBox_64").toDouble();
			float doubleSpinBox_65 = jsonObj.value("doubleSpinBox_65").toDouble();
			float doubleSpinBox_66 = jsonObj.value("doubleSpinBox_66").toDouble();
			float doubleSpinBox_67 = jsonObj.value("doubleSpinBox_67").toDouble();
			float doubleSpinBox_68 = jsonObj.value("doubleSpinBox_68").toDouble();
			float doubleSpinBox_69 = jsonObj.value("doubleSpinBox_69").toDouble();
			float doubleSpinBox_70 = jsonObj.value("doubleSpinBox_70").toDouble();
			float doubleSpinBox_71 = jsonObj.value("doubleSpinBox_71").toDouble();
			float doubleSpinBox_72 = jsonObj.value("doubleSpinBox_72").toDouble();
			float doubleSpinBox_73 = jsonObj.value("doubleSpinBox_73").toDouble();
			float doubleSpinBox_74 = jsonObj.value("doubleSpinBox_74").toDouble();
			float doubleSpinBox_75 = jsonObj.value("doubleSpinBox_75").toDouble();
			float doubleSpinBox_76 = jsonObj.value("doubleSpinBox_76").toDouble();
			float doubleSpinBox_77 = jsonObj.value("doubleSpinBox_77").toDouble();
			float doubleSpinBox_78 = jsonObj.value("doubleSpinBox_78").toDouble();

			QString lineEdit_2 = jsonObj.value("lineEdit_2").toString();
			//memory->ygjiexianValue = lineEdit_2.toDouble();
			memory->huitanValue = doubleSpinBox_6;
			// 娣诲姞鏍戝舰鎺т欢澶嶉€夋鐘舵€佺殑璇诲彇
			BallsMerge::sanjiao1Flog = jsonObj.value("sanjiao1Flog").toBool();
			BallsMerge::sanjiao2Flog = jsonObj.value("sanjiao2Flog").toBool();
			BallsMerge::sanjiao3Flog = jsonObj.value("sanjiao3Flog").toBool();
			BallsMerge::chongqiuFlog = jsonObj.value("chongqiuFlog").toBool();
			BallsMerge::sifenFlog = jsonObj.value("sifenFlog").toBool();
			BallsMerge::zhongfenFlog = jsonObj.value("zhongfenFlog").toBool();
			BallsMerge::xuanzhuan1Flog = jsonObj.value("xuanzhuan1Flog").toBool();
			BallsMerge::banxuan1Flog = jsonObj.value("banxuan1Flog").toBool();
			BallsMerge::xuanzhuan2Flog = jsonObj.value("xuanzhuan2Flog").toBool();
			BallsMerge::banxuan2Flog = jsonObj.value("banxuan2Flog").toBool();
			BallsMerge::sheshouFlog = jsonObj.value("sheshouFlog").toBool();
			BallsMerge::houyangFlog = jsonObj.value("houyangFlog").toBool();

			//璇诲彇table鍙傛暟
			//涓夎1
			BallsMerge::sj1yc1 = jsonObj.value("sj1yc1").toInt();
			BallsMerge::sj1yc2 = jsonObj.value("sj1yc2").toInt();
			BallsMerge::sj1yc3 = jsonObj.value("sj1yc3").toInt();
			BallsMerge::sj1yc4 = jsonObj.value("sj1yc4").toInt();
			BallsMerge::sj1yc5 = jsonObj.value("sj1yc5").toInt();
			BallsMerge::sj1yc6 = jsonObj.value("sj1yc6").toInt();
			BallsMerge::sj1cs = jsonObj.value("sj1cs").toInt();
			BallsMerge::sj1fsjg = jsonObj.value("sj1fsjg").toInt();
			
			//涓夎2
			BallsMerge::sj2yc1 = jsonObj.value("sj2yc1").toInt();
			BallsMerge::sj2yc2 = jsonObj.value("sj2yc2").toInt();
			BallsMerge::sj2yc3 = jsonObj.value("sj2yc3").toInt();
			BallsMerge::sj2yc4 = jsonObj.value("sj2yc4").toInt();
			BallsMerge::sj2yc5 = jsonObj.value("sj2yc5").toInt();
			BallsMerge::sj2yc6 = jsonObj.value("sj2yc6").toInt();
			BallsMerge::sj2cs = jsonObj.value("sj2cs").toInt();
			BallsMerge::sj2fsjg = jsonObj.value("sj2fsjg").toInt();
			//涓夎3
			BallsMerge::sj3yc1 = jsonObj.value("sj3yc1").toInt();
			BallsMerge::sj3yc2 = jsonObj.value("sj3yc2").toInt();
			BallsMerge::sj3yc3 = jsonObj.value("sj3yc3").toInt();
			BallsMerge::sj3yc4 = jsonObj.value("sj3yc4").toInt();
			BallsMerge::sj3yc5 = jsonObj.value("sj3yc5").toInt();
			BallsMerge::sj3yc6 = jsonObj.value("sj3yc6").toInt();
			BallsMerge::sj3cs = jsonObj.value("sj3cs").toInt();
			BallsMerge::sj3fsjg = jsonObj.value("sj3fsjg").toInt();
			//鍐茬悆
			BallsMerge::cqyc1 = jsonObj.value("cqyc1").toInt();
			BallsMerge::cqyc2 = jsonObj.value("cqyc2").toInt();
			BallsMerge::cqcs = jsonObj.value("cqcs").toInt();
			BallsMerge::cqfsjg = jsonObj.value("cqfsjg").toInt();
			//鍥涘垎
			BallsMerge::sfyc1 = jsonObj.value("sfyc1").toInt();
			BallsMerge::sfyc2 = jsonObj.value("sfyc2").toInt();
			BallsMerge::sfyc3 = jsonObj.value("sfyc3").toInt();
			BallsMerge::sfyc4 = jsonObj.value("sfyc4").toInt();
			BallsMerge::sfyc5 = jsonObj.value("sfyc5").toInt();
			BallsMerge::sfcs = jsonObj.value("sfcs").toInt();
			BallsMerge::sffsjg = jsonObj.value("sffsjg").toInt();
			//涓垎
			BallsMerge::zfyc1 = jsonObj.value("zfyc1").toInt();
			BallsMerge::zfyc2 = jsonObj.value("zfyc2").toInt();
			BallsMerge::zfyc3 = jsonObj.value("zfyc3").toInt();
			BallsMerge::zfyc4 = jsonObj.value("zfyc4").toInt();
			BallsMerge::zfyc5 = jsonObj.value("zfyc5").toInt();
			BallsMerge::zfcs = jsonObj.value("zfcs").toInt();
			BallsMerge::zffsjg = jsonObj.value("zffsjg").toInt();
			//鍚庝话
			BallsMerge::hyyc1 = jsonObj.value("hyyc1").toInt();
			BallsMerge::hyyc2 = jsonObj.value("hyyc2").toInt();
			BallsMerge::hyyc3 = jsonObj.value("hyyc3").toInt();
			BallsMerge::hyyc4 = jsonObj.value("hyyc4").toInt();
			BallsMerge::hyyc5 = jsonObj.value("hyyc5").toInt();
			BallsMerge::hycs = jsonObj.value("hycs").toInt();
			BallsMerge::hyfsjg = jsonObj.value("hyfsjg").toInt();
			//鏃嬭浆1
			BallsMerge::xz1yc1 = jsonObj.value("xz1yc1").toInt();
			BallsMerge::xz1yc2 = jsonObj.value("xz1yc2").toInt();
			BallsMerge::xz1yc3 = jsonObj.value("xz1yc3").toInt();
			BallsMerge::xz1yc4 = jsonObj.value("xz1yc4").toInt();
			BallsMerge::xz1yc5 = jsonObj.value("xz1yc5").toInt();
			BallsMerge::xz1yc6 = jsonObj.value("xz1yc6").toInt();
			BallsMerge::xz1yc7 = jsonObj.value("xz1yc7").toInt();
			BallsMerge::xz1yc8 = jsonObj.value("xz1yc8").toInt();
			BallsMerge::xz1yc9 = jsonObj.value("xz1yc9").toInt();
			BallsMerge::xz1cs = jsonObj.value("xz1cs").toInt();
			BallsMerge::xz1fsjg = jsonObj.value("xz1fsjg").toInt();
			//鏃嬭浆2
			BallsMerge::xz2yc1 = jsonObj.value("xz2yc1").toInt();
			BallsMerge::xz2yc2 = jsonObj.value("xz2yc2").toInt();
			BallsMerge::xz2yc3 = jsonObj.value("xz2yc3").toInt();
			BallsMerge::xz2yc4 = jsonObj.value("xz2yc4").toInt();
			BallsMerge::xz2yc5 = jsonObj.value("xz2yc5").toInt();
			BallsMerge::xz2yc6 = jsonObj.value("xz2yc6").toInt();
			BallsMerge::xz2yc7 = jsonObj.value("xz2yc7").toInt();
			BallsMerge::xz2yc8 = jsonObj.value("xz2yc8").toInt();
			BallsMerge::xz2yc9 = jsonObj.value("xz2yc9").toInt();
			BallsMerge::xz2cs = jsonObj.value("xz2cs").toInt();
			BallsMerge::xz2fsjg = jsonObj.value("xz2fsjg").toInt();
			//鍗婃棆1
			BallsMerge::bx1yc1 = jsonObj.value("bx1yc1").toInt();
			BallsMerge::bx1yc2 = jsonObj.value("bx1yc2").toInt();
			BallsMerge::bx1yc3 = jsonObj.value("bx1yc3").toInt();
			BallsMerge::bx1yc4 = jsonObj.value("bx1yc4").toInt();
			BallsMerge::bx1yc5 = jsonObj.value("bx1yc5").toInt();
			BallsMerge::bx1yc6 = jsonObj.value("bx1yc6").toInt();
			BallsMerge::bx1yc7 = jsonObj.value("bx1yc7").toInt();
			BallsMerge::bx1yc8 = jsonObj.value("bx1yc8").toInt();
			BallsMerge::bx1yc9 = jsonObj.value("bx1yc9").toInt();
			BallsMerge::bx1cs = jsonObj.value("bx1cs").toInt();
			BallsMerge::bx1fsjg = jsonObj.value("bx1fsjg").toInt();
			//鍗婃棆2
			BallsMerge::bx2yc1 = jsonObj.value("bx2yc1").toInt();
			BallsMerge::bx2yc2 = jsonObj.value("bx2yc2").toInt();
			BallsMerge::bx2yc3 = jsonObj.value("bx2yc3").toInt();
			BallsMerge::bx2yc4 = jsonObj.value("bx2yc4").toInt();
			BallsMerge::bx2yc5 = jsonObj.value("bx2yc5").toInt();
			BallsMerge::bx2yc6 = jsonObj.value("bx2yc6").toInt();
			BallsMerge::bx2yc7 = jsonObj.value("bx2yc7").toInt();
			BallsMerge::bx2yc8 = jsonObj.value("bx2yc8").toInt();
			BallsMerge::bx2yc9 = jsonObj.value("bx2yc9").toInt();
			BallsMerge::bx2cs = jsonObj.value("bx2cs").toInt();
			BallsMerge::bx2fsjg = jsonObj.value("bx2fsjg").toInt();
			//铔囨墜
			BallsMerge::ssyc1 = jsonObj.value("ssyc1").toInt();
			BallsMerge::ssyc2 = jsonObj.value("ssyc2").toInt();
			BallsMerge::ssyc3 = jsonObj.value("ssyc3").toInt();
			BallsMerge::ssyc4 = jsonObj.value("ssyc4").toInt();
			BallsMerge::ssyc5 = jsonObj.value("ssyc5").toInt();
			BallsMerge::ssyc6 = jsonObj.value("ssyc6").toInt();
			BallsMerge::ssyc7 = jsonObj.value("ssyc7").toInt();
			BallsMerge::ssyc8 = jsonObj.value("ssyc8").toInt();
			BallsMerge::ssyc9 = jsonObj.value("ssyc9").toInt();
			BallsMerge::sscs = jsonObj.value("sscs").toInt();
			BallsMerge::ssfsjg = jsonObj.value("ssfsjg").toInt();	

			//灏嗚鍙栧埌鐨勫€艰祴鍊肩粰瀵瑰簲鐨則able涓?
			ui.tableWidget->setItem(0, 1, new QTableWidgetItem(QString::number(BallsMerge::sj1yc1)));
			ui.tableWidget->setItem(1, 1, new QTableWidgetItem(QString::number(BallsMerge::sj1yc2)));
			ui.tableWidget->setItem(2, 1, new QTableWidgetItem(QString::number(BallsMerge::sj1yc3)));
			ui.tableWidget->setItem(3, 1, new QTableWidgetItem(QString::number(BallsMerge::sj1yc4)));
			ui.tableWidget->setItem(4, 1, new QTableWidgetItem(QString::number(BallsMerge::sj1yc5)));
			ui.tableWidget->setItem(5, 1, new QTableWidgetItem(QString::number(BallsMerge::sj1yc6)));
			ui.tableWidget->setItem(6, 1, new QTableWidgetItem(QString::number(BallsMerge::sj1cs)));
			ui.tableWidget->setItem(7, 1, new QTableWidgetItem(QString::number(BallsMerge::sj1fsjg)));
			
			ui.tableWidget_2->setItem(0, 1, new QTableWidgetItem(QString::number(BallsMerge::sj2yc1)));
			ui.tableWidget_2->setItem(1, 1, new QTableWidgetItem(QString::number(BallsMerge::sj2yc2)));
			ui.tableWidget_2->setItem(2, 1, new QTableWidgetItem(QString::number(BallsMerge::sj2yc3)));
			ui.tableWidget_2->setItem(3, 1, new QTableWidgetItem(QString::number(BallsMerge::sj2yc4)));
			ui.tableWidget_2->setItem(4, 1, new QTableWidgetItem(QString::number(BallsMerge::sj2yc5)));
			ui.tableWidget_2->setItem(5, 1, new QTableWidgetItem(QString::number(BallsMerge::sj2yc6)));
			ui.tableWidget_2->setItem(6, 1, new QTableWidgetItem(QString::number(BallsMerge::sj2cs)));
			ui.tableWidget_2->setItem(7, 1, new QTableWidgetItem(QString::number(BallsMerge::sj2fsjg)));
			ui.tableWidget_3->setItem(0, 1, new QTableWidgetItem(QString::number(BallsMerge::sj3yc1)));
			ui.tableWidget_3->setItem(1, 1, new QTableWidgetItem(QString::number(BallsMerge::sj3yc2)));
			ui.tableWidget_3->setItem(2, 1, new QTableWidgetItem(QString::number(BallsMerge::sj3yc3)));
			ui.tableWidget_3->setItem(3, 1, new QTableWidgetItem(QString::number(BallsMerge::sj3yc4)));
			ui.tableWidget_3->setItem(4, 1, new QTableWidgetItem(QString::number(BallsMerge::sj3yc5)));
			ui.tableWidget_3->setItem(5, 1, new QTableWidgetItem(QString::number(BallsMerge::sj3yc6)));
			ui.tableWidget_3->setItem(6, 1, new QTableWidgetItem(QString::number(BallsMerge::sj3cs)));
			ui.tableWidget_3->setItem(7, 1, new QTableWidgetItem(QString::number(BallsMerge::sj3fsjg)));
			ui.tableWidget_4->setItem(0, 1, new QTableWidgetItem(QString::number(BallsMerge::cqyc1)));
			ui.tableWidget_4->setItem(1, 1, new QTableWidgetItem(QString::number(BallsMerge::cqyc2)));
			ui.tableWidget_4->setItem(2, 1, new QTableWidgetItem(QString::number(BallsMerge::cqcs)));
			ui.tableWidget_4->setItem(3, 1, new QTableWidgetItem(QString::number(BallsMerge::cqfsjg)));
			ui.tableWidget_5->setItem(0, 1, new QTableWidgetItem(QString::number(BallsMerge::sfyc1)));
			ui.tableWidget_5->setItem(1, 1, new QTableWidgetItem(QString::number(BallsMerge::sfyc2)));
			ui.tableWidget_5->setItem(2, 1, new QTableWidgetItem(QString::number(BallsMerge::sfyc3)));
			ui.tableWidget_5->setItem(3, 1, new QTableWidgetItem(QString::number(BallsMerge::sfyc4)));
			ui.tableWidget_5->setItem(4, 1, new QTableWidgetItem(QString::number(BallsMerge::sfyc5)));
			ui.tableWidget_5->setItem(5, 1, new QTableWidgetItem(QString::number(BallsMerge::sfcs)));
			ui.tableWidget_5->setItem(6, 1, new QTableWidgetItem(QString::number(BallsMerge::sffsjg)));
			ui.tableWidget_6->setItem(0, 1, new QTableWidgetItem(QString::number(BallsMerge::zfyc1)));
			ui.tableWidget_6->setItem(1, 1, new QTableWidgetItem(QString::number(BallsMerge::zfyc2)));
			ui.tableWidget_6->setItem(2, 1, new QTableWidgetItem(QString::number(BallsMerge::zfyc3)));
			ui.tableWidget_6->setItem(3, 1, new QTableWidgetItem(QString::number(BallsMerge::zfyc4)));
			ui.tableWidget_6->setItem(4, 1, new QTableWidgetItem(QString::number(BallsMerge::zfyc5)));
			ui.tableWidget_6->setItem(5, 1, new QTableWidgetItem(QString::number(BallsMerge::zfcs)));
			ui.tableWidget_6->setItem(6, 1, new QTableWidgetItem(QString::number(BallsMerge::zffsjg)));
			ui.tableWidget_12->setItem(0, 1, new QTableWidgetItem(QString::number(BallsMerge::hyyc1)));
			ui.tableWidget_12->setItem(1, 1, new QTableWidgetItem(QString::number(BallsMerge::hyyc2)));
			ui.tableWidget_12->setItem(2, 1, new QTableWidgetItem(QString::number(BallsMerge::hyyc3)));
			ui.tableWidget_12->setItem(3, 1, new QTableWidgetItem(QString::number(BallsMerge::hyyc4)));
			ui.tableWidget_12->setItem(4, 1, new QTableWidgetItem(QString::number(BallsMerge::hyyc5)));
			ui.tableWidget_12->setItem(5, 1, new QTableWidgetItem(QString::number(BallsMerge::hycs)));
			ui.tableWidget_12->setItem(6, 1, new QTableWidgetItem(QString::number(BallsMerge::hyfsjg)));
			ui.tableWidget_7->setItem(0, 1, new QTableWidgetItem(QString::number(BallsMerge::xz1yc1)));
			ui.tableWidget_7->setItem(1, 1, new QTableWidgetItem(QString::number(BallsMerge::xz1yc2)));
			ui.tableWidget_7->setItem(2, 1, new QTableWidgetItem(QString::number(BallsMerge::xz1yc3)));
			ui.tableWidget_7->setItem(3, 1, new QTableWidgetItem(QString::number(BallsMerge::xz1yc4)));
			ui.tableWidget_7->setItem(4, 1, new QTableWidgetItem(QString::number(BallsMerge::xz1yc5)));
			ui.tableWidget_7->setItem(5, 1, new QTableWidgetItem(QString::number(BallsMerge::xz1yc6)));
			ui.tableWidget_7->setItem(6, 1, new QTableWidgetItem(QString::number(BallsMerge::xz1yc7)));
			ui.tableWidget_7->setItem(7, 1, new QTableWidgetItem(QString::number(BallsMerge::xz1yc8)));
			ui.tableWidget_7->setItem(8, 1, new QTableWidgetItem(QString::number(BallsMerge::xz1yc9)));
			ui.tableWidget_7->setItem(9, 1, new QTableWidgetItem(QString::number(BallsMerge::xz1cs)));
			ui.tableWidget_7->setItem(10, 1, new QTableWidgetItem(QString::number(BallsMerge::xz1fsjg)));
			ui.tableWidget_9->setItem(0, 1, new QTableWidgetItem(QString::number(BallsMerge::xz2yc1)));
			ui.tableWidget_9->setItem(1, 1, new QTableWidgetItem(QString::number(BallsMerge::xz2yc2)));
			ui.tableWidget_9->setItem(2, 1, new QTableWidgetItem(QString::number(BallsMerge::xz2yc3)));
			ui.tableWidget_9->setItem(3, 1, new QTableWidgetItem(QString::number(BallsMerge::xz2yc4)));
			ui.tableWidget_9->setItem(4, 1, new QTableWidgetItem(QString::number(BallsMerge::xz2yc5)));
			ui.tableWidget_9->setItem(5, 1, new QTableWidgetItem(QString::number(BallsMerge::xz2yc6)));
			ui.tableWidget_9->setItem(6, 1, new QTableWidgetItem(QString::number(BallsMerge::xz2yc7)));
			ui.tableWidget_9->setItem(7, 1, new QTableWidgetItem(QString::number(BallsMerge::xz2yc8)));
			ui.tableWidget_9->setItem(8, 1, new QTableWidgetItem(QString::number(BallsMerge::xz2yc9)));
			ui.tableWidget_9->setItem(9, 1, new QTableWidgetItem(QString::number(BallsMerge::xz2cs)));
			ui.tableWidget_9->setItem(10, 1, new QTableWidgetItem(QString::number(BallsMerge::xz2fsjg)));
			ui.tableWidget_8->setItem(0, 1, new QTableWidgetItem(QString::number(BallsMerge::bx1yc1)));
			ui.tableWidget_8->setItem(1, 1, new QTableWidgetItem(QString::number(BallsMerge::bx1yc2)));
			ui.tableWidget_8->setItem(2, 1, new QTableWidgetItem(QString::number(BallsMerge::bx1yc3)));
			ui.tableWidget_8->setItem(3, 1, new QTableWidgetItem(QString::number(BallsMerge::bx1yc4)));
			ui.tableWidget_8->setItem(4, 1, new QTableWidgetItem(QString::number(BallsMerge::bx1yc5)));
			ui.tableWidget_8->setItem(5, 1, new QTableWidgetItem(QString::number(BallsMerge::bx1yc6)));
			ui.tableWidget_8->setItem(6, 1, new QTableWidgetItem(QString::number(BallsMerge::bx1yc7)));
			ui.tableWidget_8->setItem(7, 1, new QTableWidgetItem(QString::number(BallsMerge::bx1yc8)));
			ui.tableWidget_8->setItem(8, 1, new QTableWidgetItem(QString::number(BallsMerge::bx1yc9)));
			ui.tableWidget_8->setItem(9, 1, new QTableWidgetItem(QString::number(BallsMerge::bx1cs)));
			ui.tableWidget_8->setItem(10, 1, new QTableWidgetItem(QString::number(BallsMerge::bx1fsjg)));
			ui.tableWidget_10->setItem(0, 1, new QTableWidgetItem(QString::number(BallsMerge::bx2yc1)));
			ui.tableWidget_10->setItem(1, 1, new QTableWidgetItem(QString::number(BallsMerge::bx2yc2)));
			ui.tableWidget_10->setItem(2, 1, new QTableWidgetItem(QString::number(BallsMerge::bx2yc3)));
			ui.tableWidget_10->setItem(3, 1, new QTableWidgetItem(QString::number(BallsMerge::bx2yc4)));
			ui.tableWidget_10->setItem(4, 1, new QTableWidgetItem(QString::number(BallsMerge::bx2yc5)));
			ui.tableWidget_10->setItem(5, 1, new QTableWidgetItem(QString::number(BallsMerge::bx2yc6)));
			ui.tableWidget_10->setItem(6, 1, new QTableWidgetItem(QString::number(BallsMerge::bx2yc7)));
			ui.tableWidget_10->setItem(7, 1, new QTableWidgetItem(QString::number(BallsMerge::bx2yc8)));
			ui.tableWidget_10->setItem(8, 1, new QTableWidgetItem(QString::number(BallsMerge::bx2yc9)));
			ui.tableWidget_10->setItem(9, 1, new QTableWidgetItem(QString::number(BallsMerge::bx2cs)));
			ui.tableWidget_10->setItem(10, 1, new QTableWidgetItem(QString::number(BallsMerge::bx2fsjg)));
			ui.tableWidget_11->setItem(0, 1, new QTableWidgetItem(QString::number(BallsMerge::ssyc1)));
			ui.tableWidget_11->setItem(1, 1, new QTableWidgetItem(QString::number(BallsMerge::ssyc2)));
			ui.tableWidget_11->setItem(2, 1, new QTableWidgetItem(QString::number(BallsMerge::ssyc3)));
			ui.tableWidget_11->setItem(3, 1, new QTableWidgetItem(QString::number(BallsMerge::ssyc4)));
			ui.tableWidget_11->setItem(4, 1, new QTableWidgetItem(QString::number(BallsMerge::ssyc5)));
			ui.tableWidget_11->setItem(5, 1, new QTableWidgetItem(QString::number(BallsMerge::ssyc6)));
			ui.tableWidget_11->setItem(6, 1, new QTableWidgetItem(QString::number(BallsMerge::ssyc7)));
			ui.tableWidget_11->setItem(7, 1, new QTableWidgetItem(QString::number(BallsMerge::ssyc8)));
			ui.tableWidget_11->setItem(8, 1, new QTableWidgetItem(QString::number(BallsMerge::ssyc9)));
			ui.tableWidget_11->setItem(9, 1, new QTableWidgetItem(QString::number(BallsMerge::sscs)));
			ui.tableWidget_11->setItem(10, 1, new QTableWidgetItem(QString::number(BallsMerge::ssfsjg)));

			ui.checkBox->setChecked(checkBox);
			ui.checkBox_2->setChecked(checkBox_2);
			ui.doubleSpinBox->setValue(doubleSpinBox);
			ui.doubleSpinBox_2->setValue(doubleSpinBox_2);
			ui.checkBox_3->setChecked(checkBox_3);
			ui.checkBox_4->setChecked(checkBox_4);
			ui.doubleSpinBox_3->setValue(doubleSpinBox_3);
			ui.doubleSpinBox_4->setValue(doubleSpinBox_4);
			ui.doubleSpinBox_5->setValue(doubleSpinBox_5);
			ui.checkBox_5->setChecked(checkBox_5);
			ui.doubleSpinBox_6->setValue(doubleSpinBox_6);
			ui.checkBox_6->setChecked(checkBox_6);
			ui.checkBox_7->setChecked(checkBox_7);
			ui.checkBox_8->setChecked(checkBox_8);
			ui.doubleSpinBox_7->setValue(doubleSpinBox_7);
			ui.doubleSpinBox_8->setValue(doubleSpinBox_8);
			ui.checkBox_11->setChecked(checkBox_11);
			ui.comboBox->setCurrentText(comboBox);
			ui.lineEdit_2->setText(lineEdit_2);
			//ui.checkBox_12->setChecked(checkBox_12);
			//ui.checkBox_13->setChecked(checkBox_13);
			//ui.checkBox_14->setChecked(checkBox_14);
			//ui.doubleSpinBox_10->setValue(doubleSpinBox_10);
			//ui.doubleSpinBox_11->setValue(doubleSpinBox_11);
			//ui.doubleSpinBox_12->setValue(doubleSpinBox_12);
			ui.comboBox->setCurrentText(comboBox);
			ui.comboBox_2->setCurrentText(comboBox_2);
			ui.comboBox_3->setCurrentText(comboBox_3);
			ui.checkBox_15->setChecked(checkBox_15);
			ui.checkBox_16->setChecked(checkBox_16);
			//ui.checkBox_18->setChecked(checkBox_18);
			ui.checkBox_21->setChecked(checkBox_21);
			//ui.checkBox_22->setChecked(checkBox_22);
			ui.checkBox_24->setChecked(checkBox_24);
			ui.checkBox_25->setChecked(checkBox_25);
			ui.checkBox_26->setChecked(checkBox_26);
			//ui.checkBox_28->setChecked(checkBox_28);
			ui.checkBox_30->setChecked(checkBox_30);
			ui.checkBox_31->setChecked(checkBox_31);
			ui.checkBox_32->setChecked(checkBox_32);
			ui.checkBox_33->setChecked(checkBox_33);
			ui.checkBox_34->setChecked(checkBox_34);
			ui.checkBox_35->setChecked(checkBox_35);
			ui.checkBox_36->setChecked(checkBox_36);
			ui.checkBox_37->setChecked(checkBox_37);
			ui.checkBox_38->setChecked(checkBox_38);
			ui.checkBox_39->setChecked(checkBox_39);
			ui.checkBox_40->setChecked(checkBox_40);
			ui.checkBox_41->setChecked(checkBox_41);
			ui.checkBox_42->setChecked(checkBox_42);
			ui.checkBox_43->setChecked(checkBox_43);
			ui.checkBox_44->setChecked(checkBox_44);
			ui.checkBox_45->setChecked(checkBox_45);
			ui.checkBox_46->setChecked(checkBox_46);
			ui.checkBox_47->setChecked(checkBox_47);
			ui.checkBox_48->setChecked(checkBox_48);
			ui.checkBox_49->setChecked(checkBox_49);
			ui.checkBox_50->setChecked(checkBox_50);
			ui.checkBox_51->setChecked(checkBox_51);
			//ui.checkBox_52->setChecked(checkBox_52);
			ui.checkBox_53->setChecked(checkBox_53);
			ui.checkBox_54->setChecked(checkBox_54);
			ui.checkBox_56->setChecked(checkBox_56);
			ui.comboBox_4->setCurrentText(comboBox_4);
			ui.comboBox_5->setCurrentText(comboBox_5);
			ui.comboBox_6->setCurrentText(comboBox_6);
			ui.comboBox_7->setCurrentText(comboBox_7);
			ui.comboBox_8->setCurrentText(comboBox_8);
			ui.comboBox_9->setCurrentText(comboBox_9);
			ui.comboBox_10->setCurrentText(comboBox_10);
			ui.comboBox_11->setCurrentText(comboBox_11);
			ui.comboBox_12->setCurrentText(comboBox_12);
			ui.comboBox_13->setCurrentText(comboBox_13);
			ui.comboBox_14->setCurrentText(comboBox_14);
			ui.comboBox_15->setCurrentText(comboBox_15);
			ui.comboBox_16->setCurrentText(comboBox_16);
			ui.comboBox_17->setCurrentText(comboBox_17);
			ui.comboBox_18->setCurrentText(comboBox_18);
			ui.comboBox_19->setCurrentText(comboBox_19);
			ui.comboBox_20->setCurrentText(comboBox_20);
			ui.comboBox_21->setCurrentText(comboBox_21);
			ui.comboBox_22->setCurrentText(comboBox_22);
			ui.comboBox_23->setCurrentText(comboBox_23);
			ui.comboBox_24->setCurrentText(comboBox_24);
			ui.comboBox_25->setCurrentText(comboBox_25);
			//ui.comboBox_26->setCurrentText(comboBox_26);
			//ui.comboBox_27->setCurrentText(comboBox_27);
			ui.doubleSpinBox_14->setValue(doubleSpinBox_14);
			ui.doubleSpinBox_15->setValue(doubleSpinBox_15);
			ui.doubleSpinBox_16->setValue(doubleSpinBox_16);
			ui.doubleSpinBox_18->setValue(doubleSpinBox_18);
			ui.doubleSpinBox_20->setValue(doubleSpinBox_20);
			ui.doubleSpinBox_21->setValue(doubleSpinBox_21);
			ui.doubleSpinBox_31->setValue(doubleSpinBox_31);
			ui.doubleSpinBox_32->setValue(doubleSpinBox_32);
			ui.doubleSpinBox_33->setValue(doubleSpinBox_33);
			ui.doubleSpinBox_34->setValue(doubleSpinBox_34);
			ui.doubleSpinBox_35->setValue(doubleSpinBox_35);
			ui.doubleSpinBox_36->setValue(doubleSpinBox_36);
			ui.doubleSpinBox_37->setValue(doubleSpinBox_37);
			ui.doubleSpinBox_38->setValue(doubleSpinBox_38);
			ui.doubleSpinBox_39->setValue(doubleSpinBox_39);
			ui.doubleSpinBox_40->setValue(doubleSpinBox_40);
			ui.doubleSpinBox_41->setValue(doubleSpinBox_41);
			ui.doubleSpinBox_42->setValue(doubleSpinBox_42);
			ui.doubleSpinBox_43->setValue(doubleSpinBox_43);
			ui.doubleSpinBox_44->setValue(doubleSpinBox_44);
			ui.doubleSpinBox_45->setValue(doubleSpinBox_45);
			ui.doubleSpinBox_46->setValue(doubleSpinBox_46);
			ui.doubleSpinBox_47->setValue(doubleSpinBox_47);
			ui.doubleSpinBox_48->setValue(doubleSpinBox_48);
			ui.doubleSpinBox_49->setValue(doubleSpinBox_49);
			ui.doubleSpinBox_50->setValue(doubleSpinBox_50);
			ui.doubleSpinBox_51->setValue(doubleSpinBox_51);
			ui.doubleSpinBox_52->setValue(doubleSpinBox_52);
			ui.doubleSpinBox_53->setValue(doubleSpinBox_53);
			ui.doubleSpinBox_54->setValue(doubleSpinBox_54);
			ui.doubleSpinBox_55->setValue(doubleSpinBox_55);
			ui.doubleSpinBox_56->setValue(doubleSpinBox_56);
			ui.doubleSpinBox_57->setValue(doubleSpinBox_57);
			ui.doubleSpinBox_58->setValue(doubleSpinBox_58);
			ui.doubleSpinBox_59->setValue(doubleSpinBox_59);
			ui.doubleSpinBox_60->setValue(doubleSpinBox_60);
			ui.doubleSpinBox_61->setValue(doubleSpinBox_61);
			ui.doubleSpinBox_62->setValue(doubleSpinBox_62);
			ui.doubleSpinBox_63->setValue(doubleSpinBox_63);
			ui.doubleSpinBox_64->setValue(doubleSpinBox_64);
			ui.doubleSpinBox_65->setValue(doubleSpinBox_65);
			ui.doubleSpinBox_66->setValue(doubleSpinBox_66);
			ui.doubleSpinBox_67->setValue(doubleSpinBox_67);
			ui.doubleSpinBox_68->setValue(doubleSpinBox_68);
			ui.doubleSpinBox_69->setValue(doubleSpinBox_69);
			ui.doubleSpinBox_70->setValue(doubleSpinBox_70);
			ui.doubleSpinBox_71->setValue(doubleSpinBox_71);
			ui.doubleSpinBox_72->setValue(doubleSpinBox_72);
			ui.doubleSpinBox_73->setValue(doubleSpinBox_73);
			ui.doubleSpinBox_74->setValue(doubleSpinBox_74);
			ui.doubleSpinBox_75->setValue(doubleSpinBox_75);
			ui.doubleSpinBox_76->setValue(doubleSpinBox_76);
			ui.doubleSpinBox_77->setValue(doubleSpinBox_77);
			ui.doubleSpinBox_78->setValue(doubleSpinBox_78);
			
			setCheckboxState(ui.treeWidget, "脚本-单方向", "一键三角1", jsonObj.value("sanjiao1Flog").toBool());
			setCheckboxState(ui.treeWidget, "脚本-单方向", "一键三角2", jsonObj.value("sanjiao2Flog").toBool());
			setCheckboxState(ui.treeWidget, "脚本-单方向", "一键三角3", jsonObj.value("sanjiao3Flog").toBool());
			setCheckboxState(ui.treeWidget, "脚本-单方向", "冲球-直线", jsonObj.value("chongqiuFlog").toBool());

			// 璁剧疆"鑴氭湰-鍙屾柟鍚?涓嬬殑澶嶉€夋
			setCheckboxState(ui.treeWidget, "脚本-双方向", "侧合-四分", jsonObj.value("sifenFlog").toBool());
			setCheckboxState(ui.treeWidget, "脚本-双方向", "侧合-中分", jsonObj.value("zhongfenFlog").toBool());
			setCheckboxState(ui.treeWidget, "脚本-双方向", "旋转-初级", jsonObj.value("xuanzhuan1Flog").toBool());
			setCheckboxState(ui.treeWidget, "脚本-双方向", "半旋-初级", jsonObj.value("banxuan1Flog").toBool());
			setCheckboxState(ui.treeWidget, "脚本-双方向", "旋转-高级", jsonObj.value("xuanzhuan2Flog").toBool());
			setCheckboxState(ui.treeWidget, "脚本-双方向", "半旋-高级", jsonObj.value("banxuan2Flog").toBool());
			setCheckboxState(ui.treeWidget, "脚本-双方向", "蛇手", jsonObj.value("sheshouFlog").toBool());
			setCheckboxState(ui.treeWidget, "脚本-双方向", "后仰", jsonObj.value("houyangFlog").toBool());


			//涓夎1鍙傛暟
			BallsMerge::sj1hqfd = doubleSpinBox_14;
			BallsMerge::sj1zyfd = doubleSpinBox_15;
			BallsMerge::sj1jd = comboBox_6.toInt();
			BallsMerge::sj1tqFlog = checkBox_15;
			BallsMerge::autosanjiao_1_Flog = checkBox_16;
			BallsMerge::sanjiaojian1 = virtualkey(comboBox_5);
			//涓夎2鍙傛暟
			BallsMerge::sj2hqfd = doubleSpinBox_18;
			BallsMerge::sj2zyfd = doubleSpinBox_16;
			BallsMerge::sj2jd = comboBox_8.toInt();
			BallsMerge::sj2tqFlog = checkBox_21;
			BallsMerge::autosanjiao_2_Flog = checkBox_24;
			BallsMerge::sanjiaojian2 = virtualkey(comboBox_7);
			//涓夎3鍙傛暟
			BallsMerge::sj3hqfd = doubleSpinBox_20;
			BallsMerge::sj3zyfd = doubleSpinBox_21;
			BallsMerge::sj3jd = comboBox_10.toInt();
			BallsMerge::sj3tqFlog = checkBox_26;
			BallsMerge::autosanjiao_3_Flog = checkBox_25;
			BallsMerge::sanjiaojian3 = virtualkey(comboBox_9);
			//鍐茬悆-鐩寸嚎鍙傛暟
			BallsMerge::chongqiujian = virtualkey(comboBox_11);
			BallsMerge::chongqiutqFlog = checkBox_30;
			BallsMerge::chongqiujiantouFlog = checkBox_32;
			//渚у悎-鍥涘垎鍙傛暟
			BallsMerge::sifenjian = virtualkey(comboBox_12);
			BallsMerge::sifentqFlog = checkBox_35;
			BallsMerge::autosifenFlog = checkBox_33;
			BallsMerge::sfjcfd = doubleSpinBox_32;
			//渚у悎-涓垎鍙傛暟
			BallsMerge::zhongfenjian = virtualkey(comboBox_13);
			BallsMerge::zhongfentqFlog = checkBox_38;
			BallsMerge::autozhongfenFlog = checkBox_36;
			BallsMerge::zfjcfd = doubleSpinBox_31;
			//鍚庝话鍙傛暟
			BallsMerge::houyangjian = virtualkey(comboBox_19);
			BallsMerge::houyangtqFlog = checkBox_56;
			BallsMerge::autohouyangFlog = checkBox_54;
			BallsMerge::hyjcfd1 = doubleSpinBox_73;
			BallsMerge::hyjcfd2 = doubleSpinBox_74;
			//鏃嬭浆1鍙傛暟
			BallsMerge::xuanzhuanjian1 = virtualkey(comboBox_14);
			BallsMerge::xuanzhuan1tqFlog = checkBox_41;
			BallsMerge::xuanzhuan1jd1 = doubleSpinBox_33;
			BallsMerge::xuanzhuan1jd2 = doubleSpinBox_39;
			BallsMerge::xuanzhuan1jd3 = doubleSpinBox_34;
			BallsMerge::xuanzhuan1jd4 = doubleSpinBox_40;
			BallsMerge::xuanzhuan1fd1 = doubleSpinBox_35;
			BallsMerge::xuanzhuan1fd2 = doubleSpinBox_37;
			BallsMerge::xuanzhuan1fd3 = doubleSpinBox_36;
			BallsMerge::xuanzhuan1fd4 = doubleSpinBox_38;
			//鏃嬭浆2鍙傛暟
			BallsMerge::xuanzhuanjian2 = virtualkey(comboBox_16);
			BallsMerge::xuanzhuan2tqFlog = checkBox_47;
			BallsMerge::xuanzhuan2jd1 = doubleSpinBox_49;
			BallsMerge::xuanzhuan2jd2 = doubleSpinBox_50;
			BallsMerge::xuanzhuan2jd3 = doubleSpinBox_52;
			BallsMerge::xuanzhuan2jd4 = doubleSpinBox_54;
			BallsMerge::xuanzhuan2fd1 = doubleSpinBox_51;
			BallsMerge::xuanzhuan2fd2 = doubleSpinBox_53;
			BallsMerge::xuanzhuan2fd3 = doubleSpinBox_55;
			BallsMerge::xuanzhuan2fd4 = doubleSpinBox_56;
			//鍗婃棆1鍙傛暟
			BallsMerge::banxuanjian1 = virtualkey(comboBox_15);
			BallsMerge::banxuan1tqFlog = checkBox_44;
			BallsMerge::banxuan1jd1 = doubleSpinBox_41;
			BallsMerge::banxuan1jd2 = doubleSpinBox_42;
			BallsMerge::banxuan1jd3 = doubleSpinBox_44;
			BallsMerge::banxuan1jd4 = doubleSpinBox_46;
			BallsMerge::banxuan1fd1 = doubleSpinBox_43;
			BallsMerge::banxuan1fd2 = doubleSpinBox_45;
			BallsMerge::banxuan1fd3 = doubleSpinBox_47;
			BallsMerge::banxuan1fd4 = doubleSpinBox_48;
			//鍗婃棆2鍙傛暟
			BallsMerge::banxuanjian2 = virtualkey(comboBox_17);
			BallsMerge::banxuan2tqFlog = checkBox_50;
			BallsMerge::banxuan2jd1 = doubleSpinBox_57;
			BallsMerge::banxuan2jd2 = doubleSpinBox_58;
			BallsMerge::banxuan2jd3 = doubleSpinBox_60;
			BallsMerge::banxuan2jd4 = doubleSpinBox_62;
			BallsMerge::banxuan2fd1 = doubleSpinBox_59;
			BallsMerge::banxuan2fd2 = doubleSpinBox_61;
			BallsMerge::banxuan2fd3 = doubleSpinBox_63;
			BallsMerge::banxuan2fd4 = doubleSpinBox_64;
			//铔囨墜鍙傛暟
			BallsMerge::sheshoujian = virtualkey(comboBox_18);
			BallsMerge::sheshoutqFlog = checkBox_53;
			BallsMerge::sheshoujd1 = doubleSpinBox_65;
			BallsMerge::sheshoujd2 = doubleSpinBox_66;
			BallsMerge::sheshoujd3 = doubleSpinBox_68;
			BallsMerge::sheshoujd4 = doubleSpinBox_70;
			BallsMerge::sheshoufd1 = doubleSpinBox_67;
			BallsMerge::sheshoufd2 = doubleSpinBox_69;
			BallsMerge::sheshoufd3 = doubleSpinBox_71;
			BallsMerge::sheshoufd4 = doubleSpinBox_72;
			
			//鍐呭瓨鍙傛暟
			memory->tuqiuFlag = checkBox;
			memory->yaoganjiexianFlag = checkBox_7;
			memory->fsjiexianFlag = checkBox_8;
			memory->yaoganhuitanFlag = checkBox_5;
			memory->gunlunshiyeFlag = checkBox_3;
			memory->juneiqupiFlag = checkBox_11;
			BallsMerge::qupijian = virtualkey(comboBox_4);
			memory->yaoganyouhuaFlag = checkBox_6;
			memory->step = doubleSpinBox_5;

			//鍒嗚韩鍚愮悆
			BallsMerge::tuqiujian = virtualkey(comboBox_2);
			BallsMerge::fenshenjian = virtualkey(comboBox_3);

			file.close();
		}
	}
	else
	{
		QMessageBox::warning(this, "错误", "加载配置失败");
	}
}

void MainWindow::setCheckboxState(QTreeWidget* treeWidget, const QString& parentText,
	const QString& itemText, bool checked) {
	// 鏌ユ壘鐖堕」鐩?
	QList<QTreeWidgetItem*> parentItems = treeWidget->findItems(parentText, Qt::MatchExactly);
	if (parentItems.isEmpty()) return;

	QTreeWidgetItem* parent = parentItems.first();

	// 鍦ㄧ埗椤圭洰涓煡鎵惧瓙椤圭洰
	for (int i = 0; i < parent->childCount(); ++i) {
		QTreeWidgetItem* child = parent->child(i);
		if (child->text(0) == itemText) {
			// 璁剧疆澶嶉€夋鐘舵€?
			child->setCheckState(0, checked ? Qt::Checked : Qt::Unchecked);
			break;
		}
	}
}

void MainWindow::treewidget_itemClicked()
{
	// 鑾峰彇褰撳墠閫変腑鐨勬爲褰㈡帶浠堕」
	QTreeWidgetItem* currentItem = ui.treeWidget->currentItem();
	if (currentItem)
	{
		// 鏍规嵁褰撳墠閫変腑鐨勯」鐨勬枃鏈垨鍏朵粬灞炴€ф潵鍐冲畾鏄剧ず鍝釜椤甸潰
		QString itemText = currentItem->text(0);
		if (itemText == "基础功能")
		{
			ui.stackedWidget->setCurrentIndex(0);
		}
		else if (itemText == "美化功能")
		{
			ui.stackedWidget->setCurrentIndex(1);
		}
		else if (itemText == "一键三角1")
		{
			ui.stackedWidget->setCurrentIndex(2);
		}
		else if (itemText == "一键三角2")
		{
			ui.stackedWidget->setCurrentIndex(3);
		}
		else if (itemText == "一键三角3")
		{
			ui.stackedWidget->setCurrentIndex(4);
		}
		else if (itemText == "冲球-直线")
		{
			ui.stackedWidget->setCurrentIndex(5);
		}
		else if (itemText == "侧合-四分")
		{
			ui.stackedWidget->setCurrentIndex(6);
		}
		else if (itemText == "侧合-中分")
		{
			ui.stackedWidget->setCurrentIndex(7);
		}
		else if (itemText == "旋转-初级")
		{
			ui.stackedWidget->setCurrentIndex(8);
		}
		else if (itemText == "半旋-初级")
		{
			ui.stackedWidget->setCurrentIndex(9);
		}
		else if (itemText == "旋转-高级")
		{
			ui.stackedWidget->setCurrentIndex(10);
		}
		else if (itemText == "半旋-高级")
		{
			ui.stackedWidget->setCurrentIndex(11);
		}
		else if (itemText == "蛇手")
		{
			ui.stackedWidget->setCurrentIndex(12);
		}
		else if (itemText == "后仰")
		{
			ui.stackedWidget->setCurrentIndex(13);
		}
		else if (itemText == "按键宏")
		{
			ui.stackedWidget->setCurrentIndex(14);
		}
		else if (itemText == "卡点功能")
		{
			ui.stackedWidget->setCurrentIndex(15);
		}
		else if (itemText == "追踪合球")
		{
			ui.stackedWidget->setCurrentIndex(16);
		}
		else if (itemText == "")
		{
			ui.stackedWidget->setCurrentIndex(17);
		}
	}
}




/*
* 
* 浠ヤ笅鏄竴浜涢挬瀛愮浉鍏崇殑鍥炶皟鍑芥暟锛屽寘鎷粴杞閲?
* 鍙抽敭杈呭姪鎺х悆
*/

// 鍦ㄥ洖璋冨嚱鏁颁腑娣诲姞鏇寸簿纭殑绐楀彛鍖归厤
// 鍦ㄥ洖璋冨嚱鏁颁腑娣诲姞鏇寸簿纭殑绐楀彛鍖归厤
LRESULT CALLBACK MainWindow::WheelHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION && wParam == WM_MOUSEWHEEL) {
		if (!instance) {
			return CallNextHookEx(WheelHookID, nCode, wParam, lParam);
		}

		// 鑾峰彇榧犳爣涓嬬殑绐楀彛
		MSLLHOOKSTRUCT* pMouseInfo = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
		HWND targetWindow = WindowFromPoint(pMouseInfo->pt);

		// 妫€鏌ョ洰鏍囩獥鍙ｆ槸鍚︽槸鏀寔鐨勬ā鎷熷櫒绐楀彛
		wchar_t className[256] = { 0 };
		// 淇锛氫娇鐢ㄥ瀛楃鐗堟湰 GetClassNameW
		GetClassNameW(targetWindow, className, 255);

		// 瀹氫箟鏀寔鐨勬ā鎷熷櫒绐楀彛绫诲悕鍒楄〃
		const wchar_t* supportedSimulators[] = {
			L"Qt5156QWindowIcon",  // 绗竴绉嶆ā鎷熷櫒绫诲悕
			L"LDPlayerMainFrame",   // 绗簩绉嶆ā鎷熷櫒绫诲悕
		};
		const int simulatorCount = sizeof(supportedSimulators) / sizeof(supportedSimulators[0]);

		// 妫€鏌ュ綋鍓嶇獥鍙ｆ槸鍚﹀湪鏀寔鐨勬ā鎷熷櫒鍒楄〃涓?
		bool isSupportedSimulator = false;
		for (int i = 0; i < simulatorCount; ++i) {
			if (wcscmp(className, supportedSimulators[i]) == 0) {
				isSupportedSimulator = true;
				break;
			}
		}

		// 濡傛灉涓嶆槸鏀寔鐨勬ā鎷熷櫒绐楀彛锛屽垯鏀捐浜嬩欢
		if (!isSupportedSimulator) {
			return CallNextHookEx(WheelHookID, nCode, wParam, lParam);
		}

		// 妫€鏌ュ閫夋checkBox_3鏄惁琚€変腑
		if (Memory::gunlunshiyeFlag) {
			// 澶嶉€夋琚€変腑锛氭墽琛岃嚜瀹氫箟鎿嶄綔骞堕樆姝簨浠朵紶閫?

			// 寮哄埗澶勭悊UI鏇存柊浜嬩欢
			QCoreApplication::processEvents();

			// 鑾峰彇婊氳疆鏂瑰悜
			int wheelDelta = GET_WHEEL_DELTA_WPARAM(pMouseInfo->mouseData);

			// 鑾峰彇UI璁剧疆鐨勬闀垮€?
			float stepMultiplier = static_cast<float>(Memory::step);

			// 鎵ц鑷畾涔夋搷浣?
			if (wheelDelta > 0) {
				instance->ui.doubleSpinBox_2->setValue(instance->ui.doubleSpinBox_2->value() + stepMultiplier);
			}
			else {
				instance->ui.doubleSpinBox_2->setValue(instance->ui.doubleSpinBox_2->value() - stepMultiplier);
			}

			// 瀹屽叏闃绘浜嬩欢浼犻€掞紙寮哄埗鎷︽埅锛?
			return 1;
		}
		else {
			// 澶嶉€夋鏈€変腑锛氭斁琛屼簨浠讹紙鎵ц妯℃嫙鍣ㄥ師璁剧殑浜嬩欢锛?
			return CallNextHookEx(WheelHookID, nCode, wParam, lParam);
		}
	}
	return CallNextHookEx(WheelHookID, nCode, wParam, lParam);
}


UINT MainWindow::virtualkey(QString key)
{
	if (key == "A") return 0x41;
	if (key == "B") return 0x42;
	if (key == "C") return 0x43;
	if (key == "D") return 0x44;
	if (key == "E") return 0x45;
	if (key == "F") return 0x46;
	if (key == "G") return 0x47;
	if (key == "H") return 0x48;
	if (key == "I") return 0x49;
	if (key == "J") return 0x4A;
	if (key == "K") return 0x4B;
	if (key == "L") return 0x4C;
	if (key == "M") return 0x4D;
	if (key == "N") return 0x4E;
	if (key == "O") return 0x4F;
	if (key == "P") return 0x50;
	if (key == "Q") return 0x51;
	if (key == "R") return 0x52;
	if (key == "S") return 0x53;
	if (key == "T") return 0x54;
	if (key == "U") return 0x55;
	if (key == "V") return 0x56;
	if (key == "W") return 0x57;
	if (key == "X") return 0x58;
	if (key == "Y") return 0x59;
	if (key == "Z") return 0x5A;
	if (key == "0") return 0x30;
	if (key == "1") return 0x31;
	if (key == "2") return 0x32;
	if (key == "3") return 0x33;
	if (key == "4") return 0x34;
	if (key == "5") return 0x35;
	if (key == "6") return 0x36;
	if (key == "7") return 0x37;
	if (key == "8") return 0x38;
	if (key == "9") return 0x39;
	if (key == "~") return 0xC0;
	if (key == "空格") return 0x20;
	return 0;
}
