#define NOMINMAX       // 禁用 Windows 的 min/max 宏
#define _NO_RPCNDR_H   // 禁用 Windows SDK 的 byte 定义
#include <windows.h>   // 优先包含 Windows 头文件
#include "ui_LogIn.h"
#include "LogIn.h"
#include <QObject>
#include "MainWindow.h"
#include "NetVerHelp/HelpHfile/NetworkVerHelp.h"
#include <iostream>

bool Log::cardState = false;
string Log::cardcode = "";


//平台类型  1:网络验证平台  2:自动发货平台
Model::PlatformType platformtypeid = Model::PlatformType::NetVer;
string encryptKey = "33521ca6";//加密的key 注意：如果软件设置了加密通讯那么此值必填(此参数是在软件列表里面进行设置)
string signSalt = "802a6d9e";//签名盐 注意：如果软件设置了加密通讯那么此值必填(此参数是在软件列表里面进行设置)
Model::EncryptType encrypttypeid = Model::EncryptType::RC4;//请求接口时，业务数据data参数加密码通讯类型(此参数是在软件列表里面进行设置)  0：无加密   3：RC4加密:
string goodscode = "0653d9a98a5fd1af";//必填，软件或者商品的编码。此值在哪获取：看接口文档有说明
string platformUserCode = "621f16d8ca8dbabf";//个人中心里面可以查看得到。代理商的话，那么在：代理管理--》代理商列表，可以查看得到 注意：如果是作者自己就填写自己的platformUserCode,如果是代理商的，得填写代理商的Code

string versionname = "v1.0";//当前软件版本号
string maccode = Help::NetworkVerHelp::GetMac();//机器码


//此账号是测试的账号，已过期了。请您在验证平台手动注册一个，或者通过接口注册一个用来测试
string userName = "test2";
string userpwd = "111111";
string newUserPwd = "222222";

//此卡号是测试的卡号，已过期了。您自己请从“卡密登录->>卡密列表,生成一个卡密，然后拿做测试”
string cardnum = Log::cardcode;
string TestVer = "测试变量名";//获取软件的变量，如果变量名称为空的话，那么接口返回来的是此软件所有变量，否则就是此变量名所以应的变量值

//
//
//心跳Key,每次心跳此值都会变
string heartbeatkey;
//登录成功后的令牌
string token;

#pragma region
//第一步，先初始化软件信息
bool IniSoftInfo()
{
	bool IniResult = false;
	//构建初化软件入参
	Model::In_IniSoftInfoArgs args;
	args.maccode = maccode;//必填
	args.timestamp = Help::NetworkVerHelp::GetTimeStamp();//必填
	args.requestflag = to_string(args.timestamp);//必填
	args.versionname = versionname;//必填
	string Msg = "";
	//请求接口，获取结果
	Model::Result result = Help::NetworkVerHelp::GetIniSoftInfo(args, platformtypeid, encryptKey, signSalt, encrypttypeid, goodscode, platformUserCode);
	if (result.code == 0)
	{
		switch (platformtypeid)
		{
		case Model::NetVer://网络验证平台
			//iniSoftInfoData:初始化完后的具体结果
			if (Help::iniSoftInfoData.inisoftkey != "")
			{
				if (Help::iniSoftInfoData.requestflag != args.requestflag)
				{
					IniResult = false;
					Msg = Msg + "接口返回的数据已被“破解者”截持，您可以强制关闭软件或者不做任何处理\n";
					std::cout << Msg + "\n";
					return IniResult;
				}

				Msg = Msg + "初始化软件成功\n";
				Msg = Msg + "接口返回的数据：\n";
				Msg = Msg + "编码：" + to_string(result.code) + "\n";
				Msg = Msg + "信息：" + result.msg + "\n";
				Msg = Msg + "data数据：" + result.data + "\n";
				if (Help::iniSoftInfoData.softInfo.newversionnum != "")
				{
					if (Help::iniSoftInfoData.softInfo.newversionnum != versionname)
					{
						Msg = Msg + "发现新版本：" + Help::iniSoftInfoData.softInfo.newversionnum + "\n";
						Msg = Msg + "新版本下载地址:" + Help::iniSoftInfoData.softInfo.networkdiskurl + "\n";
						Msg = Msg + "提取码:" + Help::iniSoftInfoData.softInfo.diskpwd + "\n";
					}
				}

				IniResult = true;

			}
			break;
		}




	}
	else
	{
		Msg = Msg + "初始化软件失败\n";
		Msg = Msg + "接口返回的数据：\n";
		Msg = Msg + "编码：" + to_string(result.code) + "\n";
		Msg = Msg + "信息：" + result.msg + "\n";
		Msg = Msg + "data数据：" + result.data + "\n";

	}

	std::cout << Msg + "\n";
	return IniResult;
}
#pragma endregion
//
#pragma region
//心跳示例
string HeartBeat(string cardnumorusername)
{

	//心跳那一块，你得加容错机制，心跳正常的操作如下：
	//错误失败次数 = 0;
	//while (true) {
	//	心跳结果 = 请求瑞科心跳接口(心跳参数);
	//	if (心跳结果.code != 0) {
	//		if (心跳结果.code == -999) {
	//			错误失败次数 = 错误失败次数 + 1;
	//		}
	//		else {
	//			强制关闭软件
	//		}
	//	}
	//	else {
	//		错误失败次数 = 0;
	//	}
	//	if (错误失败次数 >= 12) {
	//		//跳出循环啥不做处理
	//		//至余为什么连续失败12直接跳出不做任何处理的原因：
	//		//1、确实是本当前电脑网络有问题
	//		//2、有可能服务器当前有问题
	//		//以上两种情况，不能中断你用户的操作，如果用户确实到期了 
	//		//下次登录的时候反正肯定也是登录不上去的
	//		break;
	//	}
	//	延时(5分钟)
	//}

	string Msg = "";
	Msg = Msg + "开始心跳：\n";
	//注意：心跳是放在死循环里面，此只是做演示，所以没有做死循环了(注意：心跳间隔时间最低不能低于十秒钟一次，不然会引发心跳频繁)
	 //构建心跳入参
	Model::In_HeartBeatArgs HeartBeatArgs;
	HeartBeatArgs.maccode = maccode;//必填
	HeartBeatArgs.timestamp = Help::NetworkVerHelp::GetTimeStamp();//必填
	HeartBeatArgs.requestflag = to_string(HeartBeatArgs.timestamp);//必填
	HeartBeatArgs.cardnumorusername = cardnumorusername;//必填
	HeartBeatArgs.token = token;//必填
	HeartBeatArgs.heartbeatkey = heartbeatkey;//必填(每次把最新的心跳Key赋值)
	//请求接口，获取结果
	while (true)
	{
		Model::Result	result = Help::NetworkVerHelp::GetHeartBeat(HeartBeatArgs);
		Msg = Msg + "心跳返回结果：\n";
		Msg = Msg + "编码：" + to_string(result.code) + "\n";
		Msg = Msg + "信息：" + result.msg + "\n";
		if (result.code == 0)
		{
			//心跳具体数据
			Model::Out_heartBeat heartBeatResult = Help::NetworkVerHelp::ConvertHeartBeat(result.data);
			//这一块相当重要：一定要判断这个“心跳Key(heartbeatkey)”和"请求标识(requestflag)"，防止别人修改你请求的地址，给你返回一个假消息回来
			if (heartBeatResult.heartbeatkey != "" && heartBeatResult.requestflag == HeartBeatArgs.requestflag)
			{
				heartbeatkey = heartBeatResult.heartbeatkey;
				Msg = Msg + "最新的心跳Key：" + heartBeatResult.heartbeatkey + "\n";
				Msg = Msg + "到期时间：" + heartBeatResult.endtime + "\n";
				Msg = Msg + "剩余点数：" + to_string(heartBeatResult.surpluspointvalue) + "\n\n";
			}
			else
			{

				Msg = Msg + "本程序被不法分子修改了接口返回值，您可以强制关闭软件\n\n";
			}
		}
		else
		{
			Msg = Msg + "心跳失败\n\n";

		}

		return Msg;
	}
}
#pragma endregion

#pragma region 
//退出登录示例
void LoginOut(string cardnumorusername)
{
	std::cout << "退出登录：""\n";
	//构建退出登录入参
	Model::In_LoginOutArgs LoginOutArgs;
	LoginOutArgs.maccode = maccode;//必填
	LoginOutArgs.timestamp = Help::NetworkVerHelp::GetTimeStamp();//必填
	LoginOutArgs.cardnumorusername = cardnumorusername;//必填
	LoginOutArgs.token = token;//必填
	//请求接口，获取结果
	Model::Result result = Help::NetworkVerHelp::GeLoginOut(LoginOutArgs);
	std::cout << "退出登录返回结果：""\n";
	std::cout << "编码：" + to_string(result.code) + "\n";
	std::cout << "信息：" + result.msg + "\n";
	std::cout << "data数据：" + result.data + "\n\n";

}
#pragma endregion

#pragma region
//卡密登录示例
bool LoginByCard(string _CardNum, bool IsLoginOut)
{
	bool IsLoginOk = false;
	string Msg = "";
	//构建登录入参
	Model::In_CardLoginArgs args;
	args.maccode = maccode;//必填
	args.timestamp = Help::NetworkVerHelp::GetTimeStamp();//必填
	args.requestflag = to_string(args.timestamp);//必填
	args.cardnum = _CardNum;//必填
	//请求接口，获取结果
	Model::Result result = Help::NetworkVerHelp::GetLoginByCard(args);
	Msg = Msg + "----------卡密登录操作----------\n";
	if (result.code == 0)
	{
		//登录成功后的具体数据
		Model::Out_Login LoginData = Help::NetworkVerHelp::ConvertLoginData(result.data);
		if (LoginData.requestflag != args.requestflag)
		{
			Msg = Msg + "接口返回的数据已被“破解者”截持，您可以强制关闭软件或者不做任何处理\r\n";
			Msg = Msg + "--------------------\n\n\n";
			std::cout << Msg;
			IsLoginOk = false;
			return IsLoginOk;
		}

		Msg = Msg + "登录成功\n";
		Msg = Msg + "接口返回的数据：\n";
		Msg = Msg + "编码：" + to_string(result.code) + "\n";
		Msg = Msg + "信息：" + result.msg + "\n";
		Msg = Msg + "data数据：" + result.data + "\n";
		token = LoginData.token;
		heartbeatkey = LoginData.heartbeatkey;
		Msg = Msg + "token令牌：" + LoginData.token + "\n";
		Msg = Msg + "登录接口返回的心跳Key：" + LoginData.heartbeatkey + "\n";

		Msg = Msg + HeartBeat(_CardNum);//心跳示例

		if (IsLoginOut)
		{
			IsLoginOk = false;
			LoginOut(_CardNum);
		}
		else
		{
			IsLoginOk = true;
		}
	}
	else
	{

		Msg = Msg + "登录失败\n";
		Msg = Msg + "接口返回的数据：\n";
		Msg = Msg + "编码：" + to_string(result.code) + "\n";
		Msg = Msg + "信息：" + result.msg + "\n";
		Msg = Msg + "data数据：" + result.data + "\n";

	}
	Msg = Msg + "--------------------\n\n\n";
	std::cout << Msg;
	return IsLoginOk;
}
#pragma endregion

#pragma region 
//禁用还是删除：单码或账号示例
void DisableCardOrAccount(string cardnumorusername)
{
	std::cout << "禁用还是删除：单码或账号：""\n";
	//构建禁用还是删除：单码或账号入参
	Model::In_DisableCardOrAccountArgs DisableCardOrAccountArgs;
	DisableCardOrAccountArgs.maccode = maccode;//必填
	DisableCardOrAccountArgs.timestamp = Help::NetworkVerHelp::GetTimeStamp();//必填
	DisableCardOrAccountArgs.cardnumorusername = cardnumorusername;//必填
	DisableCardOrAccountArgs.token = token;//必填
	DisableCardOrAccountArgs.disablecardoraccounttype = Model::DisableCardOrAccountType::Disable;//必填
	//请求接口，获取结果
	Model::Result result = Help::NetworkVerHelp::DisableCardOrAccount(DisableCardOrAccountArgs);
	std::cout << "禁用还是删除：单码或账号返回结果：""\n";
	std::cout << "编码：" + to_string(result.code) + "\n";
	std::cout << "信息：" + result.msg + "\n";
	std::cout << "data数据：" + result.data + "\n\n";

}
#pragma endregion

Log::Log()
{
	IniSoftInfo();

	ui = new Ui::LogIn;
	ui->setupUi(this);
	setWindowIcon(QIcon("./icon(1).ico"));
	setWindowTitle("BallsHackPro");
	ui->statusbar->showMessage(QString::fromUtf8("Right,连接服务器成功..."));

	connect(ui->pushButton, &QPushButton::clicked, [=] {
		QString input = ui->lineEdit->text();
		cardcode = input.toStdString();
		LoginByCard(cardcode, true);
		printf("卡号验证结果：%d\n", Log::cardState);
		
		SetConsoleOutputCP(CP_UTF8);
		SetConsoleCP(CP_UTF8);
		
		if (ui->lineEdit->text() != "")
		{
			QByteArray message = "卡号"+input.toUtf8();
			ui->statusbar->showMessage(QString::fromUtf8("正在验证卡号..."));
			//client->sendMessage(message);
			if (LoginByCard(cardcode,Log::cardState))
			{
				this->close();
				MainWindow* main = new MainWindow;
				main->show();
			}
			else
			{
				ui->statusbar->showMessage(QString::fromUtf8("卡号错误"));
				ui->lineEdit->clear();
			}
		}
		else
		{
			ui->statusbar->showMessage(QString::fromUtf8("请输入卡号"));
		}

		});
}

Log::~Log()
{
	delete ui;

}

void Log::checkNum()
{

}
