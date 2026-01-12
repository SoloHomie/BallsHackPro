#include <Windows.h>
#include "Memory.h"
#include "BallsMerge.h"

#define PI 3.14159265358979323846

BallsMerge::BallsMerge()
{
	//Memory* memory = new Memory();
}

BallsMerge::~BallsMerge()
{
	delete memory;
}

void BallsMerge::SendScanKey(BYTE vkCode) {
	// 获取虚拟键码对应的扫描码
	UINT scanCode = MapVirtualKeyW(vkCode, 0);

	INPUT inputs[2] = { 0 };
	// 按下按键（使用扫描码）
	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE;
	inputs[0].ki.wScan = scanCode;

	// 释放按键（使用扫描码）
	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	inputs[1].ki.wScan = scanCode;

	SendInput(2, inputs, sizeof(INPUT));
}

void BallsMerge::KeyDown(BYTE vkCode)
{
	// 获取虚拟键码对应的扫描码
	UINT scanCode = MapVirtualKeyW(vkCode, 0);

	INPUT input = { 0 };

	// 设置按键按下事件
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;  // 使用扫描码
	input.ki.wScan = scanCode;              // 设置扫描码

	// 发送按键按下事件
	SendInput(1, &input, sizeof(INPUT));
}

void BallsMerge::KeyUp(BYTE vkCode)
{
	// 获取虚拟键码对应的扫描码
	UINT scanCode = MapVirtualKeyW(vkCode, 0);

	INPUT input = { 0 };

	// 设置按键松开事件
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;  // 使用扫描码+松开标志
	input.ki.wScan = scanCode;                                // 设置扫描码

	// 发送按键松开事件
	SendInput(1, &input, sizeof(INPUT));
}

// 合球
void BallsMerge::sanjiao1(HANDLE hProcess)
{
	if (autosanjiao_1_Flog)
	{
		centerPos = memory->getCenterPoint();
		GetCursorPos(&mousePos);

		float mu = sqrt((mousePos.x - centerPos.x) * (mousePos.x - centerPos.x) + (mousePos.y - centerPos.y) * (mousePos.y - centerPos.y));
		if (mu > 700) mu = 700;
		if (mu < 350) mu = 350;
		//归一后的单位向量
		float jtygX1 = (mousePos.x - centerPos.x) / mu;
		float jtygY1 = -(mousePos.y - centerPos.y) / mu;
		//落点1
		float point1X = jtygX1 * sj1zyfd * cos(sj1jd * PI / 360) - jtygY1 * sj1zyfd * sin(sj1jd * PI / 360);
		float point1Y = jtygX1 * sj1zyfd * sin(sj1jd * PI / 360) + jtygY1 * sj1zyfd * cos(sj1jd * PI / 360);
		//落点2
		float point2X = jtygX1 * sj1zyfd * cos(sj1jd * PI / 360) + jtygY1 * sj1zyfd * sin(sj1jd * PI / 360);
		float point2Y = -jtygX1 * sj1zyfd * sin(sj1jd * PI / 360) + jtygY1 * sj1zyfd * cos(sj1jd * PI / 360);
		//落点3

		float point3X = (mousePos.x - centerPos.x) / mu * mu / 700;
		float point3Y = -((mousePos.y - centerPos.y) / mu * mu / 700);

		Sleep(sj1yc1);
		memory->writePos(hProcess,(float)point1X, (float)point1Y);
		Sleep(sj1yc2);
		SendScanKey(fenshenjian);
		Sleep(sj1yc3);
		memory->writePos(hProcess,(float)point2X, (float)point2Y);
		Sleep(sj1yc4);
		SendScanKey(fenshenjian);
		Sleep(sj1yc5);
		if (sj1tqFlog)
		{
			memory->writePos(hProcess,(float)point3X, (float)point3Y);
			Sleep(sj1yc6);
			KeyDown(tuqiujian);
			for (int i = 0; i < sj1cs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(sj1fsjg);
			}
			KeyUp(tuqiujian);
		}
		else
		{
			memory->writePos(hProcess,(float)point3X, (float)point3Y);
			Sleep(sj2yc6);
			for (int i = 0; i < sj1cs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(sj1fsjg);
			}
		}
	}
	else
	{
		jtygX = memory->readjtygX(hProcess);
		jtygY = memory->readjtygY(hProcess);
		float mu = sqrt(jtygX * jtygX + jtygY * jtygY);

		//归一后的单位向量
		float jtygX1 = jtygX / mu;
		float jtygY1 = jtygY / mu;
		//落点1
		float point1X = jtygX1 * sj1zyfd * cos(sj1jd * PI / 360) - jtygY1 * sj1zyfd * sin(sj1jd * PI / 360);
		float point1Y = jtygX1 *sj1zyfd * sin(sj1jd * PI / 360) + jtygY1 * sj1zyfd * cos(sj1jd * PI / 360);
		//落点2
		float point2X = jtygX1 * sj1zyfd * cos(sj1jd * PI / 360) + jtygY1 * sj1zyfd * sin(sj1jd * PI / 360);
		float point2Y = -jtygX1 * sj1zyfd * sin(sj1jd * PI / 360) + jtygY1 * sj1zyfd * cos(sj1jd * PI / 360);
		//落点3
		float point3X = jtygX1 * sj1hqfd;
		float point3Y = jtygY1 * sj1hqfd;

		Sleep(sj1yc1);
		memory->writePos(hProcess,(float)point1X, (float)point1Y);
		Sleep(sj1yc2);
		SendScanKey(fenshenjian);
		Sleep(sj1yc3);
		memory->writePos(hProcess,(float)point2X, (float)point2Y);
		Sleep(sj1yc4);
		SendScanKey(fenshenjian);
		Sleep(sj1yc5);
		if (sj1tqFlog)
		{
			memory->writePos(hProcess,(float)point3X, (float)point3Y);
			Sleep(sj1yc6);
			KeyDown(tuqiujian);
			for (int i = 0; i < sj1cs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(sj1fsjg);
			}
			KeyUp(tuqiujian);
		}
		else
		{
			memory->writePos(hProcess,(float)point3X, (float)point3Y);
			Sleep(sj1yc6);
			for (int i = 0; i < sj1cs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(sj1fsjg);
			}
		}
	}
}

void BallsMerge::sanjiao2(HANDLE hProcess)
{
	if (autosanjiao_2_Flog)
	{
		centerPos = memory->getCenterPoint();
		GetCursorPos(&mousePos);

		float mu = sqrt((mousePos.x - centerPos.x) * (mousePos.x - centerPos.x) + (mousePos.y - centerPos.y) * (mousePos.y - centerPos.y));
		if (mu > 700) mu = 700;
		if (mu < 350) mu = 350;
		//归一后的单位向量
		float jtygX1 = (mousePos.x - centerPos.x) / mu;
		float jtygY1 = -(mousePos.y - centerPos.y) / mu;
		//落点1
		float point1X = jtygX1 * sj2zyfd * cos(sj2jd * PI / 360) - jtygY1 * sj2zyfd * sin(sj2jd * PI / 360);
		float point1Y = jtygX1 * sj2zyfd * sin(sj2jd * PI / 360) + jtygY1 * sj2zyfd * cos(sj2jd * PI / 360);
		//落点2
		float point2X = jtygX1 * sj2zyfd * cos(sj2jd * PI / 360) + jtygY1 * sj2zyfd * sin(sj2jd * PI / 360);
		float point2Y = -jtygX1 * sj2zyfd * sin(sj2jd * PI / 360) + jtygY1 * sj2zyfd * cos(sj2jd * PI / 360);
		//落点3
		float point3X = (mousePos.x - centerPos.x) / mu * mu / 700;
		float point3Y = -((mousePos.y - centerPos.y) / mu * mu / 700);

		Sleep(sj2yc1);
		memory->writePos(hProcess,(float)point1X, (float)point1Y);
		Sleep(sj2yc2);
		SendScanKey(fenshenjian);
		Sleep(sj2yc3);
		memory->writePos(hProcess,(float)point2X, (float)point2Y);
		Sleep(sj2yc4);
		SendScanKey(fenshenjian);
		Sleep(sj2yc5);
		if (sj2tqFlog)
		{
			memory->writePos(hProcess,(float)point3X, (float)point3Y);
			Sleep(sj2yc6);
			KeyDown(tuqiujian);
			for (int i = 0; i < sj2cs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(sj2fsjg);
			}
			KeyUp(tuqiujian);
		}
		else
		{
			memory->writePos(hProcess,(float)point3X, (float)point3Y);
			Sleep(sj2yc6);
			for (int i = 0; i < sj2cs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(sj2fsjg);
			}
		}
	}
	else
	{
		jtygX = memory->readjtygX(hProcess);
		jtygY = memory->readjtygY(hProcess);
		float mu = sqrt(jtygX * jtygX + jtygY * jtygY);

		//归一后的单位向量
		float jtygX1 = jtygX / mu;
		float jtygY1 = jtygY / mu;
		//落点1
		float point1X = jtygX1 * sj2zyfd * cos(sj2jd * PI / 360) - jtygY1 * sj2zyfd * sin(sj2jd * PI / 360);
		float point1Y = jtygX1 * sj2zyfd * sin(sj2jd * PI / 360) + jtygY1 * sj2zyfd * cos(sj2jd * PI / 360);
		//落点2
		float point2X = jtygX1 * sj2zyfd * cos(sj2jd * PI / 360) + jtygY1 * sj2zyfd * sin(sj2jd * PI / 360);
		float point2Y = -jtygX1 * sj2zyfd * sin(sj2jd * PI / 360) + jtygY1 * sj2zyfd * cos(sj2jd * PI / 360);
		//落点3
		float point3X = jtygX1 * sj2hqfd;
		float point3Y = jtygY1 * sj2hqfd;

		Sleep(sj2yc1);
		memory->writePos(hProcess,(float)point1X, (float)point1Y);
		Sleep(sj2yc2);
		SendScanKey(fenshenjian);
		Sleep(sj2yc3);
		memory->writePos(hProcess,(float)point2X, (float)point2Y);
		Sleep(sj2yc4);
		SendScanKey(fenshenjian);
		Sleep(sj2yc5);
		if (sj2tqFlog)
		{
			memory->writePos(hProcess,(float)point3X, (float)point3Y);
			Sleep(sj2yc6);
			KeyDown(tuqiujian);
			for (int i = 0; i < sj2cs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(sj2fsjg);
			}
			KeyUp(tuqiujian);
		}
		else
		{
			memory->writePos(hProcess,(float)point3X, (float)point3Y);
			Sleep(sj2yc6);
			for (int i = 0; i < sj2cs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(sj2fsjg);
			}
		}
	}
}

void BallsMerge::sanjiao3(HANDLE hProcess)
{
	if (autosanjiao_3_Flog)
	{
		centerPos = memory->getCenterPoint();
		GetCursorPos(&mousePos);

		float mu = sqrt((mousePos.x - centerPos.x) * (mousePos.x - centerPos.x) + (mousePos.y - centerPos.y) * (mousePos.y - centerPos.y));
		if (mu > 700) mu = 700;
		if (mu < 350) mu = 350;
		//归一后的单位向量
		float jtygX1 = (mousePos.x - centerPos.x) / mu;
		float jtygY1 = -(mousePos.y - centerPos.y) / mu;
		//落点1
		float point1X = jtygX1 * sj3zyfd * cos(sj3jd * PI / 360) - jtygY1 * sj3zyfd * sin(sj3jd * PI / 360);
		float point1Y = jtygX1 * sj3zyfd * sin(sj3jd * PI / 360) + jtygY1 * sj3zyfd * cos(sj3jd * PI / 360);
		//落点2
		float point2X = jtygX1 * sj3zyfd * cos(sj3jd * PI / 360) + jtygY1 * sj3zyfd * sin(sj3jd * PI / 360);
		float point2Y = -jtygX1 * sj3zyfd * sin(sj3jd * PI / 360) + jtygY1 * sj3zyfd * cos(sj3jd * PI / 360);
		//落点3
		float point3X = (mousePos.x - centerPos.x) / mu * mu / 700;
		float point3Y = -((mousePos.y - centerPos.y) / mu * mu / 700);

		Sleep(sj3yc1);
		memory->writePos(hProcess,(float)point1X, (float)point1Y);
		Sleep(sj3yc2);
		SendScanKey(fenshenjian);
		Sleep(sj3yc3);
		memory->writePos(hProcess,(float)point2X, (float)point2Y);
		Sleep(sj3yc4);
		SendScanKey(fenshenjian);
		Sleep(sj3yc5);
		if (sj3tqFlog)
		{
			memory->writePos(hProcess,(float)point3X, (float)point3Y);
			Sleep(sj3yc6);
			KeyDown(tuqiujian);
			for (int i = 0; i < sj3cs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(sj3fsjg);
			}
			KeyUp(tuqiujian);
		}
		else
		{
			memory->writePos(hProcess,(float)point3X, (float)point3Y);
			Sleep(sj3yc6);
			for (int i = 0; i < sj3cs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(sj3fsjg);
			}
		}
	}
	else
	{
		jtygX = memory->readjtygX(hProcess);
		jtygY = memory->readjtygY(hProcess);
		float mu = sqrt(jtygX * jtygX + jtygY * jtygY);

		//归一后的单位向量
		float jtygX1 = jtygX / mu;
		float jtygY1 = jtygY / mu;
		//落点1
		float point1X = jtygX1 * sj3zyfd * cos(sj3jd * PI / 360) - jtygY1 * sj3zyfd * sin(sj3jd * PI / 360);
		float point1Y = jtygX1 * sj3zyfd * sin(sj3jd * PI / 360) + jtygY1 * sj3zyfd * cos(sj3jd * PI / 360);
		//落点2
		float point2X = jtygX1 * sj3zyfd * cos(sj3jd * PI / 360) + jtygY1 * sj3zyfd * sin(sj3jd * PI / 360);
		float point2Y = -jtygX1 * sj3zyfd * sin(sj3jd * PI / 360) + jtygY1 * sj3zyfd * cos(sj3jd * PI / 360);
		//落点3
		float point3X = jtygX1 * sj3hqfd;
		float point3Y = jtygY1 * sj3hqfd;

		Sleep(sj3yc1);
		memory->writePos(hProcess,(float)point1X, (float)point1Y);
		Sleep(sj3yc2);
		SendScanKey(fenshenjian);
		Sleep(sj3yc3);
		memory->writePos(hProcess,(float)point2X, (float)point2Y);
		Sleep(sj3yc4);
		SendScanKey(fenshenjian);
		Sleep(sj3yc5);
		if (sj3tqFlog)
		{
			memory->writePos(hProcess,(float)point3X, (float)point3Y);
			Sleep(sj3yc6);
			KeyDown(tuqiujian);
			for (int i = 0; i < sj3cs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(sj3fsjg);
			}
			KeyUp(tuqiujian);
		}
		else
		{
			memory->writePos(hProcess,(float)point3X, (float)point3Y);
			Sleep(sj3yc6);
			for (int i = 0; i < sj3cs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(sj3fsjg);
			}
		}
	}
}

// 静态成员变量定义
UINT BallsMerge::sanjiaojian1 = 0;
UINT BallsMerge::sanjiaojian2 = 0;
UINT BallsMerge::sanjiaojian3 = 0;
UINT BallsMerge::chongqiujian = 0;
UINT BallsMerge::sifenjian = 0;
UINT BallsMerge::zhongfenjian = 0;
UINT BallsMerge::xuanzhuanjian1 = 0;
UINT BallsMerge::banxuanjian1 = 0;
UINT BallsMerge::xuanzhuanjian2 = 0;
UINT BallsMerge::banxuanjian2 = 0;
UINT BallsMerge::sheshoujian = 0;
UINT BallsMerge::houyangjian = 0;
UINT BallsMerge::tuqiujian = 0;
UINT BallsMerge::fenshenjian = 0;
UINT BallsMerge::qupijian = 0;

bool BallsMerge::sanjiao1Flog = false;
bool BallsMerge::sanjiao2Flog = false;
bool BallsMerge::sanjiao3Flog = false;
bool BallsMerge::chongqiuFlog = false;
bool BallsMerge::sifenFlog = false;
bool BallsMerge::zhongfenFlog = false;
bool BallsMerge::xuanzhuan1Flog = false;
bool BallsMerge::banxuan1Flog = false;
bool BallsMerge::xuanzhuan2Flog = false;
bool BallsMerge::banxuan2Flog = false;
bool BallsMerge::sheshouFlog = false;
bool BallsMerge::houyangFlog = false;

bool BallsMerge::autosanjiao_1_Flog = true;
bool BallsMerge::autosanjiao_2_Flog = true;
bool BallsMerge::autosanjiao_3_Flog = true;
bool BallsMerge::autosifenFlog = true;
bool BallsMerge::autozhongfenFlog = true;
bool BallsMerge::autohouyangFlog = true;

float BallsMerge::sj1jd = 0.0f;
float BallsMerge::sj2jd = 0.0f;
float BallsMerge::sj3jd = 0.0f;
float BallsMerge::sj1hqfd = 0.0f;
float BallsMerge::sj1zyfd = 0.0f;
float BallsMerge::sj2hqfd = 0.0f;
float BallsMerge::sj2zyfd = 0.0f;
float BallsMerge::sj3hqfd = 0.0f;
float BallsMerge::sj3zyfd = 0.0f;
float BallsMerge::sfjcfd = 0.0f;
float BallsMerge::zfjcfd = 0.0f;
float BallsMerge::hyjcfd1 = 0.0f;
float BallsMerge::hyjcfd2 = 0.0f;

bool BallsMerge::sj1tqFlog = false;
bool BallsMerge::sj2tqFlog = false;
bool BallsMerge::sj3tqFlog = false;
bool BallsMerge::chongqiutqFlog = false;
bool BallsMerge::chongqiujiantouFlog = false;
bool BallsMerge::sifentqFlog = false;
bool BallsMerge::zhongfentqFlog = false;
bool BallsMerge::houyangtqFlog = false;
bool BallsMerge::xuanzhuan1tqFlog = false;
bool BallsMerge::xuanzhuan2tqFlog = false;
bool BallsMerge::banxuan1tqFlog = false;
bool BallsMerge::banxuan2tqFlog = false;
bool BallsMerge::sheshoutqFlog = false;

float BallsMerge::jtygX = 0.0f;
float BallsMerge::jtygY = 0.0f;
float BallsMerge::dx = 0.0f;
float BallsMerge::dy = 0.0f;
float BallsMerge::shiye = 1.0f;

float BallsMerge::xuanzhuan1jd1 = 0.0f;
float BallsMerge::xuanzhuan1jd2 = 0.0f;
float BallsMerge::xuanzhuan1jd3 = 0.0f;
float BallsMerge::xuanzhuan1jd4 = 0.0f;
float BallsMerge::xuanzhuan1fd1 = 0.0f;
float BallsMerge::xuanzhuan1fd2 = 0.0f;
float BallsMerge::xuanzhuan1fd3 = 0.0f;
float BallsMerge::xuanzhuan1fd4 = 0.0f;

float BallsMerge::xuanzhuan2jd1 = 0.0f;
float BallsMerge::xuanzhuan2jd2 = 0.0f;
float BallsMerge::xuanzhuan2jd3 = 0.0f;
float BallsMerge::xuanzhuan2jd4 = 0.0f;
float BallsMerge::xuanzhuan2fd1 = 0.0f;
float BallsMerge::xuanzhuan2fd2 = 0.0f;
float BallsMerge::xuanzhuan2fd3 = 0.0f;
float BallsMerge::xuanzhuan2fd4 = 0.0f;

float BallsMerge::banxuan1jd1 = 0.0f;
float BallsMerge::banxuan1jd2 = 0.0f;
float BallsMerge::banxuan1jd3 = 0.0f;
float BallsMerge::banxuan1jd4 = 0.0f;
float BallsMerge::banxuan1fd1 = 0.0f;
float BallsMerge::banxuan1fd2 = 0.0f;
float BallsMerge::banxuan1fd3 = 0.0f;
float BallsMerge::banxuan1fd4 = 0.0f;

float BallsMerge::banxuan2jd1 = 0.0f;
float BallsMerge::banxuan2jd2 = 0.0f;
float BallsMerge::banxuan2jd3 = 0.0f;
float BallsMerge::banxuan2jd4 = 0.0f;
float BallsMerge::banxuan2fd1 = 0.0f;
float BallsMerge::banxuan2fd2 = 0.0f;
float BallsMerge::banxuan2fd3 = 0.0f;
float BallsMerge::banxuan2fd4 = 0.0f;

float BallsMerge::sheshoujd1 = 0.0f;
float BallsMerge::sheshoujd2 = 0.0f;
float BallsMerge::sheshoujd3 = 0.0f;
float BallsMerge::sheshoujd4 = 0.0f;
float BallsMerge::sheshoufd1 = 0.0f;
float BallsMerge::sheshoufd2 = 0.0f;
float BallsMerge::sheshoufd3 = 0.0f;
float BallsMerge::sheshoufd4 = 0.0f;

int BallsMerge::sj1yc1 = 0;
int BallsMerge::sj1yc2 = 0;
int BallsMerge::sj1yc3 = 0;
int BallsMerge::sj1yc4 = 0;
int BallsMerge::sj1yc5 = 0;
int BallsMerge::sj1yc6 = 0;
int BallsMerge::sj1cs = 0;
int BallsMerge::sj1fsjg = 0;

int BallsMerge::sj2yc1 = 0;
int BallsMerge::sj2yc2 = 0;
int BallsMerge::sj2yc3 = 0;
int BallsMerge::sj2yc4 = 0;
int BallsMerge::sj2yc5 = 0;
int BallsMerge::sj2yc6 = 0;
int BallsMerge::sj2cs = 0;
int BallsMerge::sj2fsjg = 0;

int BallsMerge::sj3yc1 = 0;
int BallsMerge::sj3yc2 = 0;
int BallsMerge::sj3yc3 = 0;
int BallsMerge::sj3yc4 = 0;
int BallsMerge::sj3yc5 = 0;
int BallsMerge::sj3yc6 = 0;
int BallsMerge::sj3cs = 0;
int BallsMerge::sj3fsjg = 0;

int BallsMerge::cqyc1 = 0;
int BallsMerge::cqyc2 = 0;
int BallsMerge::cqcs = 0;
int BallsMerge::cqfsjg = 0;

int BallsMerge::sfyc1 = 0;
int BallsMerge::sfyc2 = 0;
int BallsMerge::sfyc3 = 0;
int BallsMerge::sfyc4 = 0;
int BallsMerge::sfyc5 = 0;
int BallsMerge::sfcs = 0;
int BallsMerge::sffsjg = 0;

int BallsMerge::zfyc1 = 0;
int BallsMerge::zfyc2 = 0;
int BallsMerge::zfyc3 = 0;
int BallsMerge::zfyc4 = 0;
int BallsMerge::zfyc5 = 0;
int BallsMerge::zfcs = 0;
int BallsMerge::zffsjg = 0;

int BallsMerge::hyyc1 = 0;
int BallsMerge::hyyc2 = 0;
int BallsMerge::hyyc3 = 0;
int BallsMerge::hyyc4 = 0;
int BallsMerge::hyyc5 = 0;
int BallsMerge::hycs = 0;
int BallsMerge::hyfsjg = 0;

int BallsMerge::xz1yc1 = 0;
int BallsMerge::xz1yc2 = 0;
int BallsMerge::xz1yc3 = 0;
int BallsMerge::xz1yc4 = 0;
int BallsMerge::xz1yc5 = 0;
int BallsMerge::xz1yc6 = 0;
int BallsMerge::xz1yc7 = 0;
int BallsMerge::xz1yc8 = 0;
int BallsMerge::xz1yc9 = 0;
int BallsMerge::xz1cs = 0;
int BallsMerge::xz1fsjg = 0;

int BallsMerge::xz2yc1 = 0;
int BallsMerge::xz2yc2 = 0;
int BallsMerge::xz2yc3 = 0;
int BallsMerge::xz2yc4 = 0;
int BallsMerge::xz2yc5 = 0;
int BallsMerge::xz2yc6 = 0;
int BallsMerge::xz2yc7 = 0;
int BallsMerge::xz2yc8 = 0;
int BallsMerge::xz2yc9 = 0;
int BallsMerge::xz2cs = 0;
int BallsMerge::xz2fsjg = 0;

int BallsMerge::bx1yc1 = 0;
int BallsMerge::bx1yc2 = 0;
int BallsMerge::bx1yc3 = 0;
int BallsMerge::bx1yc4 = 0;
int BallsMerge::bx1yc5 = 0;
int BallsMerge::bx1yc6 = 0;
int BallsMerge::bx1yc7 = 0;
int BallsMerge::bx1yc8 = 0;
int BallsMerge::bx1yc9 = 0;
int BallsMerge::bx1cs = 0;
int BallsMerge::bx1fsjg = 0;

int BallsMerge::bx2yc1 = 0;
int BallsMerge::bx2yc2 = 0;
int BallsMerge::bx2yc3 = 0;
int BallsMerge::bx2yc4 = 0;
int BallsMerge::bx2yc5 = 0;
int BallsMerge::bx2yc6 = 0;
int BallsMerge::bx2yc7 = 0;
int BallsMerge::bx2yc8 = 0;
int BallsMerge::bx2yc9 = 0;
int BallsMerge::bx2cs = 0;
int BallsMerge::bx2fsjg = 0;

int BallsMerge::ssyc1 = 0;
int BallsMerge::ssyc2 = 0;
int BallsMerge::ssyc3 = 0;
int BallsMerge::ssyc4 = 0;
int BallsMerge::ssyc5 = 0;
int BallsMerge::ssyc6 = 0;
int BallsMerge::ssyc7 = 0;
int BallsMerge::ssyc8 = 0;
int BallsMerge::ssyc9 = 0;
int BallsMerge::sscs = 0;
int BallsMerge::ssfsjg = 0;

POINT BallsMerge::centerPos = { 0, 0 };
POINT BallsMerge::mousePos = { 0, 0 };
POINT BallsMerge::direction = { 0, 0 };
POINT BallsMerge::cursorPos_1 = { 0, 0 };
POINT BallsMerge::cursorPos_2 = { 0, 0 };

void BallsMerge::chongqiu(HANDLE hProcess)
{
	if (chongqiujiantouFlog)
	{
		if (chongqiutqFlog)
		{
			KeyDown(tuqiujian);
			for (int i = 0; i < cqcs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(cqfsjg);
			}
			KeyUp(tuqiujian);
		}
		else
		{
			for (int i = 0; i < cqcs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(cqfsjg);
			}
		}
	}
	else
	{
		Sleep(cqyc1);
		Sleep(cqyc2);
		SendScanKey(fenshenjian);
	}
}

void BallsMerge::sifen(HANDLE hProcess)
{
	if (autosifenFlog)
	{
		jtygX = memory->readjtygX(hProcess);
		jtygY = memory->readjtygY(hProcess);
		float mu = sqrt(jtygX * jtygX + jtygY * jtygY);

		//归一后的单位向量
		float jtygX1 = jtygX / mu;
		float jtygY1 = jtygY / mu;

		Sleep(sfyc1);
		memory->writePos(hProcess,(float)jtygX1, (float)jtygY1);
		Sleep(sfyc2);
		SendScanKey(fenshenjian);
		Sleep(sfyc3);
		SendScanKey(fenshenjian);
		Sleep(sfyc4);
		memory->writePos(hProcess,(float)jtygX1 * sfjcfd, (float)jtygY1 * sfjcfd);
		Sleep(sfyc5);
		if (sifentqFlog)
		{
			KeyDown(tuqiujian);
			for (int i = 0; i < sfcs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(sffsjg);
			}
			KeyUp(tuqiujian);
		}
		else
		{
			for (int i = 0; i < sfcs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(sffsjg);
			}
		}
	}
}

void BallsMerge::zhongfen(HANDLE hProcess)
{
	if (autozhongfenFlog)
	{
		jtygX = memory->readjtygX(hProcess);
		jtygY = memory->readjtygY(hProcess);
		float mu = sqrt(jtygX * jtygX + jtygY * jtygY);

		//归一后的单位向量
		float jtygX1 = jtygX / mu;
		float jtygY1 = jtygY / mu;

		Sleep(zfyc1);
		memory->writePos(hProcess,(float)jtygX1, (float)jtygY1);
		Sleep(zfyc2);
		SendScanKey(fenshenjian);
		Sleep(zfyc3);
		memory->writePos(hProcess,(float)jtygX1 * zfjcfd, (float)jtygY1 * zfjcfd);
		Sleep(zfyc4);
		SendScanKey(fenshenjian);
		Sleep(zfyc5);
		if (zhongfentqFlog)
		{
			KeyDown(tuqiujian);
			for (int i = 0; i < zfcs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(zffsjg);
			}
			KeyUp(tuqiujian);
		}
		else
		{
			for (int i = 0; i < zfcs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(zffsjg);
			}
		}
	}
}

void BallsMerge::houyang(HANDLE hProcess)
{
	if (autohouyangFlog)
	{
		centerPos = memory->getCenterPoint();
		GetCursorPos(&mousePos);

		float mu = sqrt((mousePos.x - centerPos.x) * (mousePos.x - centerPos.x) + (mousePos.y - centerPos.y) * (mousePos.y - centerPos.y));
		if (mu > 700) mu = 700;
		if (mu < 350) mu = 350;
		//归一后的单位向量
		float jtygX1 = (mousePos.x - centerPos.x) / mu;
		float jtygY1 = -(mousePos.y - centerPos.y) / mu;

		Sleep(hyyc1);
		memory->writePos(hProcess,(float)jtygX1 * hyjcfd1, (float)jtygY1 * hyjcfd1);
		Sleep(hyyc2);
		SendScanKey(fenshenjian);
		Sleep(hyyc3);
		memory->writePos(hProcess,(float)jtygX1 * hyjcfd2, (float)jtygY1 * hyjcfd2);
		Sleep(hyyc4);
		SendScanKey(fenshenjian);
		Sleep(hyyc5);
		if (houyangtqFlog)
		{
			KeyDown(tuqiujian);
			for (int i = 0; i < hycs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(hyfsjg);
			}
			KeyUp(tuqiujian);
		}
		else
		{
			for (int i = 0; i < hycs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(hyfsjg);
			}
		}
	}
}

void BallsMerge::xuanzhuan1(HANDLE hProcess)
{
	jtygX = memory->readjtygX(hProcess);
	jtygY = memory->readjtygY(hProcess);
	float mu = sqrt(jtygX * jtygX + jtygY * jtygY);

	//归一后的单位向量
	float jtygX1 = jtygX / mu;
	float jtygY1 = jtygY / mu;

	//先逆时针旋转一次(计算第一次开分身的坐标)，然后顺时针旋转3次(计算三次的旋转坐标)
	float point1X = jtygX1 * xuanzhuan1fd1 * cos(xuanzhuan1jd1 * PI / 180) - jtygY1 * xuanzhuan1fd1 * sin(xuanzhuan1jd1 * PI / 180);
	float point1Y = jtygX1 * xuanzhuan1fd1 * sin(xuanzhuan1jd1 * PI / 180) + jtygY1 * xuanzhuan1fd1 * cos(xuanzhuan1jd1 * PI / 180);
	float point2X = jtygX1 * xuanzhuan1fd2 * cos(xuanzhuan1jd2 * PI / 180) - jtygY1 * xuanzhuan1fd2 * sin(xuanzhuan1jd2 * PI / 180);
	float point2Y = jtygX1 * xuanzhuan1fd2 * sin(xuanzhuan1jd2 * PI / 180) + jtygY1 * xuanzhuan1fd2 * cos(xuanzhuan1jd2 * PI / 180);
	float point3X = jtygX1 * xuanzhuan1fd3 * cos(xuanzhuan1jd3 * PI / 180) - jtygY1 * xuanzhuan1fd3 * sin(xuanzhuan1jd3 * PI / 180);
	float point3Y = jtygX1 * xuanzhuan1fd3 * sin(xuanzhuan1jd3 * PI / 180) + jtygY1 * xuanzhuan1fd3 * cos(xuanzhuan1jd3 * PI / 180);
	float point4X = jtygX1 * xuanzhuan1fd4 * cos(xuanzhuan1jd4 * PI / 180) - jtygY1 * xuanzhuan1fd4 * sin(xuanzhuan1jd4 * PI / 180);
	float point4Y = jtygX1 * xuanzhuan1fd4 * sin(xuanzhuan1jd4 * PI / 180) + jtygY1 * xuanzhuan1fd4 * cos(xuanzhuan1jd4 * PI / 180);
	SendScanKey(fenshenjian);
	Sleep(xz1yc1);
	memory->writePos(hProcess,(float)point1X, (float)point1Y);
	Sleep(xz1yc2);
	SendScanKey(fenshenjian);
	Sleep(xz1yc3);
	memory->writePos(hProcess,(float)point2X, (float)point2Y);
	Sleep(xz1yc4);
	SendScanKey(fenshenjian);
	Sleep(xz1yc5);
	memory->writePos(hProcess,(float)point3X, (float)point3Y);
	Sleep(xz1yc6);
	SendScanKey(fenshenjian);
	Sleep(xz1yc7);
	memory->writePos(hProcess,(float)point4X, (float)point4Y);
	Sleep(xz1yc8);
	SendScanKey(fenshenjian);
	Sleep(xz1yc9);
	if (xuanzhuan1tqFlog)
	{
		KeyDown(tuqiujian);
		for (int i = 0; i < xz1cs; i++)
		{
			SendScanKey(fenshenjian);
			Sleep(xz1fsjg);
		}
		KeyUp(tuqiujian);
	}
	else
	{
		for (int i = 0; i < xz1cs; i++)
		{
			SendScanKey(fenshenjian);
			Sleep(xz1fsjg);
		}
	}
}

void BallsMerge::xuanzhuan2(HANDLE hProcess)
{
	jtygX = memory->readjtygX(hProcess);
	jtygY = memory->readjtygY(hProcess);
	float mu = sqrt(jtygX * jtygX + jtygY * jtygY);

	//归一后的单位向量
	float jtygX1 = jtygX / mu;
	float jtygY1 = jtygY / mu;

	//先逆时针旋转一次(计算第一次开分身的坐标)，然后顺时针旋转3次(计算三次的旋转坐标)
	float point1X = jtygX1 * xuanzhuan2fd1 * cos(xuanzhuan2jd1 * PI / 180) - jtygY1 * xuanzhuan2fd1 * sin(xuanzhuan2jd1 * PI / 180);
	float point1Y = jtygX1 * xuanzhuan2fd1 * sin(xuanzhuan2jd1 * PI / 180) + jtygY1 * xuanzhuan2fd1 * cos(xuanzhuan2jd1 * PI / 180);
	float point2X = jtygX1 * xuanzhuan2fd2 * cos(xuanzhuan2jd2 * PI / 180) - jtygY1 * xuanzhuan2fd2 * sin(xuanzhuan2jd2 * PI / 180);
	float point2Y = jtygX1 * xuanzhuan2fd2 * sin(xuanzhuan2jd2 * PI / 180) + jtygY1 * xuanzhuan2fd2 * cos(xuanzhuan2jd2 * PI / 180);
	float point3X = jtygX1 * xuanzhuan2fd3 * cos(xuanzhuan2jd3 * PI / 180) - jtygY1 * xuanzhuan2fd3 * sin(xuanzhuan2jd3 * PI / 180);
	float point3Y = jtygX1 * xuanzhuan2fd3 * sin(xuanzhuan2jd3 * PI / 180) + jtygY1 * xuanzhuan2fd3 * cos(xuanzhuan2jd3 * PI / 180);
	float point4X = jtygX1 * xuanzhuan2fd4 * cos(xuanzhuan2jd4 * PI / 180) - jtygY1 * xuanzhuan2fd4 * sin(xuanzhuan2jd4 * PI / 180);
	float point4Y = jtygX1 * xuanzhuan2fd4 * sin(xuanzhuan2jd4 * PI / 180) + jtygY1 * xuanzhuan2fd4 * cos(xuanzhuan2jd4 * PI / 180);
	SendScanKey(fenshenjian);
	Sleep(xz2yc1);
	memory->writePos(hProcess,(float)point1X, (float)point1Y);
	Sleep(xz2yc2);
	SendScanKey(fenshenjian);
	Sleep(xz2yc3);
	memory->writePos(hProcess,(float)point2X, (float)point2Y);
	Sleep(xz2yc4);
	SendScanKey(fenshenjian);
	Sleep(xz2yc5);
	memory->writePos(hProcess,(float)point3X, (float)point3Y);
	Sleep(xz2yc6);
	SendScanKey(fenshenjian);
	Sleep(xz2yc7);
	memory->writePos(hProcess,(float)point4X, (float)point4Y);
	Sleep(xz2yc8);
	SendScanKey(fenshenjian);
	Sleep(xz2yc9);
	if (xuanzhuan2tqFlog)
	{
		KeyDown(tuqiujian);
		for (int i = 0; i < xz2cs; i++)
		{
			SendScanKey(fenshenjian);
			Sleep(xz2fsjg);
		}
		KeyUp(tuqiujian);
	}
	else
	{
		for (int i = 0; i < xz2cs; i++)
		{
			SendScanKey(fenshenjian);
			Sleep(xz2fsjg);
		}
	}
}

void BallsMerge::banxuan1(HANDLE hProcess)
{
	jtygX = memory->readjtygX(hProcess);
	jtygY = memory->readjtygY(hProcess);
	float mu = sqrt(jtygX * jtygX + jtygY * jtygY);

	//归一后的单位向量
	float jtygX1 = jtygX / mu;
	float jtygY1 = jtygY / mu;

	//先逆时针旋转一次(计算第一次开分身的坐标)，然后顺时针旋转3次(计算三次的旋转坐标)
	float point1X = jtygX1 * banxuan1fd1 * cos(banxuan1jd1 * PI / 180) - jtygY1 * banxuan1fd1 * sin(banxuan1jd1 * PI / 180);
	float point1Y = jtygX1 * banxuan1fd1 * sin(banxuan1jd1 * PI / 180) + jtygY1 * banxuan1fd1 * cos(banxuan1jd1 * PI / 180);
	float point2X = jtygX1 * banxuan1fd2 * cos(banxuan1jd2 * PI / 180) - jtygY1 * banxuan1fd2 * sin(banxuan1jd2 * PI / 180);
	float point2Y = jtygX1 * banxuan1fd2 * sin(banxuan1jd2 * PI / 180) + jtygY1 * banxuan1fd2 * cos(banxuan1jd2 * PI / 180);
	float point3X = jtygX1 * banxuan1fd3 * cos(banxuan1jd3 * PI / 180) - jtygY1 * banxuan1fd3 * sin(banxuan1jd3 * PI / 180);
	float point3Y = jtygX1 * banxuan1fd3 * sin(banxuan1jd3 * PI / 180) + jtygY1 * banxuan1fd3 * cos(banxuan1jd3 * PI / 180);
	float point4X = jtygX1 * banxuan1fd4 * cos(banxuan1jd4 * PI / 180) - jtygY1 * banxuan1fd4 * sin(banxuan1jd4 * PI / 180);
	float point4Y = jtygX1 * banxuan1fd4 * sin(banxuan1jd4 * PI / 180) + jtygY1 * banxuan1fd4 * cos(banxuan1jd4 * PI / 180);
	SendScanKey(fenshenjian);
	Sleep(bx1yc1);
	memory->writePos(hProcess,(float)point1X, (float)point1Y);
	Sleep(bx1yc2);
	SendScanKey(fenshenjian);
	Sleep(bx1yc3);
	memory->writePos(hProcess,(float)point2X, (float)point2Y);
	Sleep(bx1yc4);
	SendScanKey(fenshenjian);
	Sleep(bx1yc5);
	memory->writePos(hProcess,(float)point3X, (float)point3Y);
	Sleep(bx1yc6);
	SendScanKey(fenshenjian);
	Sleep(bx1yc7);
	memory->writePos(hProcess,(float)point4X, (float)point4Y);
	Sleep(bx1yc8);
	SendScanKey(fenshenjian);
	Sleep(bx1yc9);
	if (banxuan1tqFlog)
	{
		KeyDown(tuqiujian);
		for (int i = 0; i < bx1cs; i++)
		{
			SendScanKey(fenshenjian);
			Sleep(bx1fsjg);
		}
		KeyUp(tuqiujian);
	}
	else
	{
		for (int i = 0; i < bx1cs; i++)
		{
			SendScanKey(fenshenjian);
			Sleep(bx1fsjg);
		}
	}
}

void BallsMerge::banxuan2(HANDLE hProcess)
{
	jtygX = memory->readjtygX(hProcess);
	jtygY = memory->readjtygY(hProcess);
	float mu = sqrt(jtygX * jtygX + jtygY * jtygY);

	//归一后的单位向量
	float jtygX1 = jtygX / mu;
	float jtygY1 = jtygY / mu;

	//先逆时针旋转一次(计算第一次开分身的坐标)，然后顺时针旋转3次(计算三次的旋转坐标)
	float point1X = jtygX1 * banxuan2fd1 * cos(banxuan2jd1 * PI / 180) - jtygY1 * banxuan2fd1 * sin(banxuan2jd1 * PI / 180);
	float point1Y = jtygX1 * banxuan2fd1 * sin(banxuan2jd1 * PI / 180) + jtygY1 * banxuan2fd1 * cos(banxuan2jd1 * PI / 180);
	float point2X = jtygX1 * banxuan2fd2 * cos(banxuan2jd2 * PI / 180) - jtygY1 * banxuan2fd2 * sin(banxuan2jd2 * PI / 180);
	float point2Y = jtygX1 * banxuan2fd2 * sin(banxuan2jd2 * PI / 180) + jtygY1 * banxuan2fd2 * cos(banxuan2jd2 * PI / 180);
	float point3X = jtygX1 * banxuan2fd3 * cos(banxuan2jd3 * PI / 180) - jtygY1 * banxuan2fd3 * sin(banxuan2jd3 * PI / 180);
	float point3Y = jtygX1 * banxuan2fd3 * sin(banxuan2jd3 * PI / 180) + jtygY1 * banxuan2fd3 * cos(banxuan2jd3 * PI / 180);
	float point4X = jtygX1 * banxuan2fd4 * cos(banxuan2jd4 * PI / 180) - jtygY1 * banxuan2fd4 * sin(banxuan2jd4 * PI / 180);
	float point4Y = jtygX1 * banxuan2fd4 * sin(banxuan2jd4 * PI / 180) + jtygY1 * banxuan2fd4 * cos(banxuan2jd4 * PI / 180);
	SendScanKey(fenshenjian);
	Sleep(bx2yc1);
	memory->writePos(hProcess,(float)point1X, (float)point1Y);
	Sleep(bx2yc2);
	SendScanKey(fenshenjian);
	Sleep(bx2yc3);
	memory->writePos(hProcess,(float)point2X, (float)point2Y);
	Sleep(bx2yc4);
	SendScanKey(fenshenjian);
	Sleep(bx2yc5);
	memory->writePos(hProcess,(float)point3X, (float)point3Y);
	Sleep(bx2yc6);
	SendScanKey(fenshenjian);
	Sleep(bx2yc7);
	memory->writePos(hProcess,(float)point4X, (float)point4Y);
	Sleep(bx2yc8);
	SendScanKey(fenshenjian);
	Sleep(bx2yc9);
	if (banxuan2tqFlog)
	{
		KeyDown(tuqiujian);
		for (int i = 0; i < bx2cs; i++)
		{
			SendScanKey(fenshenjian);
			Sleep(bx2fsjg);
		}
		KeyUp(tuqiujian);
	}
	else
	{
		for (int i = 0; i < bx2cs; i++)
		{
			SendScanKey(fenshenjian);
			Sleep(bx2fsjg);
		}
	}
}

void BallsMerge::sheshou(HANDLE hProcess)
{
	jtygX = memory->readjtygX(hProcess);
	jtygY = memory->readjtygY(hProcess);
	float mu = sqrt(jtygX * jtygX + jtygY * jtygY);
	float cross = jtygX * 0 - jtygY * 1;
	
	//归一后的单位向量
	float jtygX1 = jtygX / mu;
	float jtygY1 = jtygY / mu;

	if (cross > 0)
	{
		//先逆时针旋转一次(计算第一次开分身的坐标)，然后顺时针旋转3次(计算三次的旋转坐标)
		float point1X = jtygX1 * sheshoufd1 * cos(sheshoujd1 * PI / 180) - jtygY1 * sheshoufd1 * sin(sheshoujd1 * PI / 180);
		float point1Y = jtygX1 * sheshoufd1 * sin(sheshoujd1 * PI / 180) + jtygY1 * sheshoufd1 * cos(sheshoujd1 * PI / 180);
		float point2X = jtygX1 * sheshoufd2 * cos(sheshoujd2 * PI / 180) - jtygY1 * sheshoufd2 * sin(sheshoujd2 * PI / 180);
		float point2Y = jtygX1 * sheshoufd2 * sin(sheshoujd2 * PI / 180) + jtygY1 * sheshoufd2 * cos(sheshoujd2 * PI / 180);
		float point3X = jtygX1 * sheshoufd3 * cos(sheshoujd3 * PI / 180) - jtygY1 * sheshoufd3 * sin(sheshoujd3 * PI / 180);
		float point3Y = jtygX1 * sheshoufd3 * sin(sheshoujd3 * PI / 180) + jtygY1 * sheshoufd3 * cos(sheshoujd3 * PI / 180);
		float point4X = jtygX1 * sheshoufd4 * cos(sheshoujd4 * PI / 180) - jtygY1 * sheshoufd4 * sin(sheshoujd4 * PI / 180);
		float point4Y = jtygX1 * sheshoufd4 * sin(sheshoujd4 * PI / 180) + jtygY1 * sheshoufd4 * cos(sheshoujd4 * PI / 180);
		SendScanKey(fenshenjian);
		Sleep(ssyc1);
		memory->writePos(hProcess,(float)point1X, (float)point1Y);
		Sleep(ssyc2);
		SendScanKey(fenshenjian);
		Sleep(ssyc3);
		memory->writePos(hProcess,(float)point2X, (float)point2Y);
		Sleep(ssyc4);
		SendScanKey(fenshenjian);
		Sleep(ssyc5);
		memory->writePos(hProcess,(float)point3X, (float)point3Y);
		Sleep(ssyc6);
		SendScanKey(fenshenjian);
		Sleep(ssyc7);
		memory->writePos(hProcess,(float)point4X, (float)point4Y);
		Sleep(ssyc8);
		SendScanKey(fenshenjian);
		Sleep(ssyc9);
		if (sheshoutqFlog)
		{
			KeyDown(tuqiujian);
			for (int i = 0; i < sscs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(ssfsjg);
			}
			KeyUp(tuqiujian);
		}
		else
		{
			for (int i = 0; i < sscs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(ssfsjg);
			}
		}
	}
	else
	{
		//先顺时针旋转一次(计算第一次开分身的坐标)，然后逆时针旋转3次(计算三次的旋转坐标)
		float point1X = jtygX1 * sheshoufd1 * cos(-sheshoujd1 * PI / 180) - jtygY1 * sheshoufd1 * sin(-sheshoujd1 * PI / 180);
		float point1Y = jtygX1 * sheshoufd1 * sin(-sheshoujd1 * PI / 180) + jtygY1 * sheshoufd1 * cos(-sheshoujd1 * PI / 180);
		float point2X = jtygX1 * sheshoufd2 * cos(-sheshoujd2 * PI / 180) - jtygY1 * sheshoufd2 * sin(-sheshoujd2 * PI / 180);
		float point2Y = jtygX1 * sheshoufd2 * sin(-sheshoujd2 * PI / 180) + jtygY1 * sheshoufd2 * cos(-sheshoujd2 * PI / 180);
		float point3X = jtygX1 * sheshoufd3 * cos(-sheshoujd3 * PI / 180) - jtygY1 * sheshoufd3 * sin(-sheshoujd3 * PI / 180);
		float point3Y = jtygX1 * sheshoufd3 * sin(-sheshoujd3 * PI / 180) + jtygY1 * sheshoufd3 * cos(-sheshoujd3 * PI / 180);
		float point4X = jtygX1 * sheshoufd4 * cos(-sheshoujd4 * PI / 180) - jtygY1 * sheshoufd4 * sin(-sheshoujd4 * PI / 180);
		float point4Y = jtygX1 * sheshoufd4 * sin(-sheshoujd4 * PI / 180) + jtygY1 * sheshoufd4 * cos(-sheshoujd4 * PI / 180);
		SendScanKey(fenshenjian);
		Sleep(ssyc1);
		memory->writePos(hProcess,(float)point1X, (float)point1Y);
		Sleep(ssyc2);
		SendScanKey(fenshenjian);
		Sleep(ssyc3);
		memory->writePos(hProcess,(float)point2X, (float)point2Y);
		Sleep(ssyc4);
		SendScanKey(fenshenjian);
		Sleep(ssyc5);
		memory->writePos(hProcess,(float)point3X, (float)point3Y);
		Sleep(ssyc6);
		SendScanKey(fenshenjian);
		Sleep(ssyc7);
		memory->writePos(hProcess,(float)point4X, (float)point4Y);
		Sleep(ssyc8);
		SendScanKey(fenshenjian);
		Sleep(ssyc9);
		if (sheshoutqFlog)
		{
			KeyDown(tuqiujian);
			for (int i = 0; i < sscs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(ssfsjg);
			}
			KeyUp(tuqiujian);
		}
		else
		{
			for (int i = 0; i < sscs; i++)
			{
				SendScanKey(fenshenjian);
				Sleep(ssfsjg);
			}
		}
	}
}