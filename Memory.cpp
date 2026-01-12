#define _WIN32_WINNT 0x0600
#include "Memory.h"
#include <Windows.h>
#include <mutex>
#include <thread>
#include <vector>
#include <TlHelp32.h>

Memory::Memory()
{
	
}
Memory::~Memory()
{

}

//粘合
void Memory::writenianhe(HANDLE hProcess,float nianheValue)
{
	WriteProcessMemory(hProcess, (LPVOID)nianheAdress, &nianheValue, sizeof(nianheValue), NULL);
}
//变速
void Memory::writebiansu(HANDLE hProcess, float biansuValue)
{
	WriteProcessMemory(hProcess, (LPVOID)biansuAdress, &biansuValue, sizeof(float), NULL);
}
//视野
void Memory::writeshiye(HANDLE hProcess,float shiyeValue)
{
	uintptr_t newshiyeAddress = (uintptr_t)shiyeAdress + 4;

	WriteProcessMemory(
		hProcess,
		(LPVOID)shiyeAdress,
		&shiyeValue,
		sizeof(float),
		NULL
	);

	WriteProcessMemory(
		hProcess,
		(LPVOID)newshiyeAddress,
		&shiyeValue,
		sizeof(float),
		NULL
	);
}
//昵称大小
void Memory::writencdx(HANDLE hProcess, float ncdxValue)
{
	WriteProcessMemory(hProcess, (LPVOID)ncdxAdress, &ncdxValue, sizeof(float), NULL);
}
//吐球-----------------------循环写入
void Memory::writetuqiu(HANDLE hProcess, float tuqiuValue)
{
	WriteProcessMemory(hProcess, (LPVOID)tuqiuAdress, &tuqiuValue, sizeof(float), NULL);
}
//摇杆解限------------------>循环写入
void Memory::writeyaoganjiexian(HANDLE hProcess,float ygjiexianValue)
{
	WriteProcessMemory(hProcess, (LPVOID)yaoganjiexianAdress, &ygjiexianValue, sizeof(float), NULL);
}
//分身解限------------------->循环写入
void Memory::writefsjiexian(HANDLE hProcess, float fenshenjiexianValue)
{
	WriteProcessMemory(hProcess, (LPVOID)fsjiexianAdress, &fenshenjiexianValue, sizeof(float), NULL);
}
//摇杆回弹------------------>循环写入
void Memory::writeyaoganhuitan(HANDLE hProcess,float huitanValue)
{
	WriteProcessMemory(hProcess, (LPVOID)yaoganhuitanAdress_1, &huitanValue, sizeof(float), NULL);
	WriteProcessMemory(hProcess, (LPVOID)yaoganhuitanAdress_2, &huitanValue, sizeof(float), NULL);
}
//局内去皮
void Memory::writejuneiqupi(HANDLE hProcess,float juneiqupiValue)
{
	WriteProcessMemory(hProcess, (LPVOID)juneiqupiAdress, &juneiqupiValue, sizeof(float), NULL);
}
//摇杆优化（清除摇杆延迟，摇杆容差修改为0）
void Memory::writeyaoganyouhua(HANDLE hProcess, float rongchaValue)
{
	//修改摇杆容差为0
	WriteProcessMemory(hProcess, (LPVOID)yaoganrongchaAdress, &rongchaValue, sizeof(float), NULL);
}
//摇杆延迟
void Memory::writeyaoganyanchi(HANDLE hProcess, float yaoganyanchiValue)
{
	//清除摇杆延迟
	WriteProcessMemory(hProcess, (LPVOID)ygycAdress_1, &yaoganyanchiValue, sizeof(float), NULL);
	WriteProcessMemory(hProcess, (LPVOID)ygycAdress_2, &yaoganyanchiValue, sizeof(float), NULL);
}
//摇杆一万
void Memory::writeyaogan10000(HANDLE hProcess, float yaogan10000Value)
{
	WriteProcessMemory(hProcess, (LPVOID)yaogan10000Adress_2, &yaogan10000Value, sizeof(float), NULL);
}
//写入位置，拉动摇杆
void Memory::writePos(HANDLE hProcess,float x, float y)
{
	uintptr_t newncygAdress = (uintptr_t)ncygAdress + 0x4;

	WriteProcessMemory(hProcess, (LPVOID)ncygAdress, &x, sizeof(float), NULL);
	WriteProcessMemory(hProcess, (LPVOID)newncygAdress, &y, sizeof(float), NULL);
}
//读取摇杆X
float Memory::readjtygX(HANDLE hProcess)
{
	float jtygX;
	ReadProcessMemory(hProcess, (LPVOID)ncygjsxAdress, &jtygX, sizeof(float), NULL);
	return jtygX;
}
//读取摇杆Y
float Memory::readjtygY(HANDLE hProcess)
{
	float jtygY;
	ReadProcessMemory(hProcess, (LPVOID)ncygjsyAdress, &jtygY, sizeof(float), NULL);
	return jtygY;
}
//获取窗口中心坐标
POINT Memory::getCenterPoint()
{
	POINT centerPoint;
	RECT rect;
	HWND hWnd;
	hWnd = FindWindowA("Qt5156QWindowIcon", NULL);
	GetClientRect(hWnd, &rect);
	centerPoint.x = (rect.left + rect.right) / 2;
	centerPoint.y = (rect.top + rect.bottom) / 2;
	return centerPoint;
}
//获取模拟器窗口句柄
HWND Memory::getRectHwnd()
{
	HWND hWnd;
	hWnd = FindWindowA("Qt5156QWindowIcon", NULL);
	return hWnd;
}
//获取hProcess
HANDLE Memory::getProcessHandle()
{
	const wchar_t* processName = L"MuMuVMMHeadless.exe";
	HANDLE hProcess = NULL;
	DWORD PID = 0;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hSnapShot, &pe32))
	{
		do
		{
			if(_wcsicmp(pe32.szExeFile,processName))
			{
				PID = pe32.th32ProcessID;
				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
				break;
			}
			
		} while (Process32Next(hSnapShot, &pe32));
		return hProcess;
	}	
}

void Memory::Searcher()
{
	//待实现
}

// 静态成员变量定义
DWORD_PTR Memory::patternAddresses[16] = {0};
long long Memory::nianheAdress = 0;
long long Memory::ncdxAdress = 0;
long long Memory::biansuAdress = 0;
long long Memory::ygycAdress_1 = 0;
long long Memory::ygycAdress_2 = 0;
long long Memory::yaoganhuitanAdress_1 = 0;
long long Memory::yaoganhuitanAdress_2 = 0;
long long Memory::yaoganrongchaAdress = 0;
long long Memory::ygrongchaAdress = 0;
long long Memory::yaogan10000Adress_1 = 0;
long long Memory::yaogan10000Adress_2 = 0;
long long Memory::yaoganjiexianAdress = 0;
long long Memory::fsjiexianAdress = 0;
long long Memory::juneiqupiAdress = 0;
long long Memory::ncygAdress = 0;
long long Memory::shiyeAdress = 0;
long long Memory::tuqiuAdress = 0;
long long Memory::ncygjsxAdress = 0;
long long Memory::ncygjsyAdress = 0;
long long Memory::tijiAdress = 0;
long long Memory::zhanjuzhiAdress = 0;
long long Memory::selfsize = 0;
long long Memory::xmin = 0;
long long Memory::xmax = 0;
long long Memory::ymin = 0;
long long Memory::ymax = 0;

float Memory::huitanValue = 0.0f;
float Memory::yaogan10000Value = 100.0f;
float Memory::step = 1.0f;
float Memory::yaoganycValue = 0.0f;

bool Memory::yaoganyouhuaFlag = false;
bool Memory::yaoganhuitanFlag = false;
bool Memory::yaoganjiexianFlag = false;
bool Memory::fsjiexianFlag = false;
bool Memory::yaogan10000Flag = false;
bool Memory::tuqiuFlag = true;
bool Memory::gunlunshiyeFlag = false;
bool Memory::juneiqupiFlag = false;
bool Memory::yaoganycFlag = false;

