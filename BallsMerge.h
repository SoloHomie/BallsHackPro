#pragma once

class BallsMerge
{
public:
	BallsMerge();
	~BallsMerge();

	void SendScanKey(BYTE vkCode);
	void KeyDown(BYTE vkCode);
	void KeyUp(BYTE vkCode);

	//合球
	void sanjiao1(HANDLE hProcess);
	void sanjiao2(HANDLE hProcess);
	void sanjiao3(HANDLE hProcess);
	void chongqiu(HANDLE hProcess);
	void sifen(HANDLE hProcess);
	void zhongfen(HANDLE hProcess);
	void houyang(HANDLE hProcess);
	void xuanzhuan1(HANDLE hProcess);
	void xuanzhuan2(HANDLE hProcess);
	void banxuan1(HANDLE hProcess);
	void banxuan2(HANDLE hProcess);
	void sheshou(HANDLE hProcess);

	//合球参数
	static UINT sanjiaojian1, sanjiaojian2, sanjiaojian3, chongqiujian, sifenjian, zhongfenjian, xuanzhuanjian1, banxuanjian1, xuanzhuanjian2, banxuanjian2, sheshoujian, houyangjian, tuqiujian, fenshenjian, qupijian;
	static bool sanjiao1Flog, sanjiao2Flog, sanjiao3Flog, chongqiuFlog, sifenFlog, zhongfenFlog, xuanzhuan1Flog, banxuan1Flog, xuanzhuan2Flog, banxuan2Flog, sheshouFlog, houyangFlog;
	static bool autosanjiao_1_Flog, autosanjiao_2_Flog, autosanjiao_3_Flog, autosifenFlog, autozhongfenFlog, autohouyangFlog;
	static float sj1jd, sj2jd, sj3jd, sj1hqfd, sj1zyfd, sj2hqfd, sj2zyfd, sj3hqfd, sj3zyfd, sfjcfd, zfjcfd, hyjcfd1, hyjcfd2;
	static bool sj1tqFlog, sj2tqFlog, sj3tqFlog, chongqiutqFlog, chongqiujiantouFlog, sifentqFlog, zhongfentqFlog, houyangtqFlog, xuanzhuan1tqFlog, xuanzhuan2tqFlog, banxuan1tqFlog, banxuan2tqFlog, sheshoutqFlog;
	static float jtygX, jtygY, dx, dy, shiye;
	//旋转1参数
	static float xuanzhuan1jd1, xuanzhuan1jd2, xuanzhuan1jd3, xuanzhuan1jd4, xuanzhuan1fd1, xuanzhuan1fd2, xuanzhuan1fd3, xuanzhuan1fd4;
	//旋转2参数
	static float xuanzhuan2jd1, xuanzhuan2jd2, xuanzhuan2jd3, xuanzhuan2jd4, xuanzhuan2fd1, xuanzhuan2fd2, xuanzhuan2fd3, xuanzhuan2fd4;
	//半旋1参数
	static float banxuan1jd1, banxuan1jd2, banxuan1jd3, banxuan1jd4, banxuan1fd1, banxuan1fd2, banxuan1fd3, banxuan1fd4;
	//半旋2参数
	static float banxuan2jd1, banxuan2jd2, banxuan2jd3, banxuan2jd4, banxuan2fd1, banxuan2fd2, banxuan2fd3, banxuan2fd4;
	//蛇手参数
	static float sheshoujd1, sheshoujd2, sheshoujd3, sheshoujd4, sheshoufd1, sheshoufd2, sheshoufd3, sheshoufd4;

	static int sj1yc1, sj1yc2, sj1yc3, sj1yc4, sj1yc5, sj1yc6, sj1cs, sj1fsjg;
	static int sj2yc1, sj2yc2, sj2yc3, sj2yc4, sj2yc5, sj2yc6, sj2cs, sj2fsjg;
	static int sj3yc1, sj3yc2, sj3yc3, sj3yc4, sj3yc5, sj3yc6, sj3cs, sj3fsjg;
	static int cqyc1, cqyc2, cqcs, cqfsjg;
	static int sfyc1, sfyc2, sfyc3, sfyc4, sfyc5, sfcs, sffsjg;
	static int zfyc1, zfyc2, zfyc3, zfyc4, zfyc5, zfcs, zffsjg;
	static int hyyc1, hyyc2, hyyc3, hyyc4, hyyc5, hycs, hyfsjg;
	static int xz1yc1, xz1yc2, xz1yc3, xz1yc4, xz1yc5, xz1yc6, xz1yc7, xz1yc8, xz1yc9, xz1cs, xz1fsjg;
	static int xz2yc1, xz2yc2, xz2yc3, xz2yc4, xz2yc5, xz2yc6, xz2yc7, xz2yc8, xz2yc9, xz2cs, xz2fsjg;
	static int bx1yc1, bx1yc2, bx1yc3, bx1yc4, bx1yc5, bx1yc6, bx1yc7, bx1yc8, bx1yc9, bx1cs, bx1fsjg;
	static int bx2yc1, bx2yc2, bx2yc3, bx2yc4, bx2yc5, bx2yc6, bx2yc7, bx2yc8, bx2yc9, bx2cs, bx2fsjg;
	static int ssyc1, ssyc2, ssyc3, ssyc4, ssyc5, ssyc6, ssyc7, ssyc8, ssyc9, sscs, ssfsjg;

	static POINT centerPos;   //中心点坐标
	static POINT mousePos;    //合球时鼠标位置
	static POINT direction; //鼠标指向
	static POINT cursorPos_1; //落点坐标
	static POINT cursorPos_2; //松开鼠标坐标

	Memory* memory;
};