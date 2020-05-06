#include "base.h"
#include "resource.h"
#include <Windows.h>
#include <graphics.h>

#pragma comment(lib,"winmm.lib")
// UI静态链接库
#pragma comment(lib,"EasyXGUI.lib")

const int SCREENW = 960;
const int SCREENH = 540;

// 使窗口居中，不改变大小
void SetWindowCenter(HWND);
// 将img所指的大小图像与背景图像按alpha值进行混合，结果保存在img上,若tcolor为真，则img先进行指定色清除再与背景混合
// 隐含参数为img大小
void SetAlphaImageLayer(IMAGE* objimg, IMAGE* sourceimg, int x, int y, float alpha,bool tcolor = false,COLORREF transparentColor = WHITE);
// 设置程序字体
void setfontset(LOGFONT*);
// 设置指定字体大小
void SetFontHeight(LOGFONT* ofont, int size);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow)
{
	initgraph(SCREENW, SCREENH, NOCLOSE);
	SetWindowCenter(GetHWnd());
	setbkmode(TRANSPARENT);

	// 鼠标消息结构体
	MOUSEMSG msg;
	// 计时器
	DWORD sectime = 0;
	// 显示帧数所用的字符串
	wchar_t fpsinfo[5];
	unsigned count = 0, fps = 0;
	// 程序所使用的字体
	LOGFONT font;
	setfontset(&font);
	bool mainloop = true;
	RECT rect = { 0,70,960,120 };
	IMAGE bkimg, objimg, titleimg, logo;
	IMAGE buttonnormalimg,buttonfocusimg;
	loadimage(&bkimg, TEXT("bk.jpg"), 960, 540);
	//loadimage(&logo, TEXT("長門有希.png"), 400, 300);
	loadimage(&logo, TEXT("PNG"), TEXT("LOGO"), 400, 300);
	//注释掉的部分加载objimg图像
	//loadimage(&objimg, TEXT("pre.jpg"), 960, 400);
	loadimage(&buttonnormalimg, TEXT("PNG"), TEXT("Bnormal"), 150, 30);
	loadimage(&buttonfocusimg, TEXT("PNG"), TEXT("Bfocus"), 150, 30);

	// 退出按钮
	Button exit_button;
	TCHAR text[] = L"退出";
	exit_button.setStyle(STYLE::BS_IMAGE);
	exit_button.setText(text);
	exit_button.setColor(WHITE);
	exit_button.setfocusColor(WHITE);
	exit_button.setlineColor(RGB(14, 105, 61));
	exit_button.isDrawline(true);
	exit_button.setSize(150, 30);
	exit_button.setPos(405, 425);
	exit_button.setImage(&buttonnormalimg, &buttonfocusimg);

	// 对objimg图像的初始化
	objimg.Resize(960, 400);
	SetWorkingImage(&objimg);
	setfillcolor(BLACK);
	solidrectangle(0, 0, objimg.getwidth(), objimg.getheight());
	SetWorkingImage(NULL);

	// titleimg图像的初始化
	titleimg.Resize(960, 50);
	SetWorkingImage(&titleimg);
	setfillcolor(BLACK);
	solidrectangle(0, 0, titleimg.getwidth(), titleimg.getheight());
	SetWorkingImage(NULL);

	// 使objimg与titleimg与背景进行透明运算
	SetAlphaImageLayer(&objimg, &bkimg, 0, 70, 0.70f);
	SetAlphaImageLayer(&titleimg, &bkimg, 0, 70, 0.80f);
	// SetAlphaImageLayer(&logo, &objimg, 560, 100, 1.0f, true);

	BeginBatchDraw();
	while (mainloop)
	{
		// 处理鼠标消息
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (exit_button.isclick(msg))
				mainloop = false;
		}
		// 处理键盘消息
		if (GetAsyncKeyState(VK_ESCAPE) && 0x8000)
			mainloop = false;

		// 每秒更新帧数
		if (sectime + 1000L <= timeGetTime())
		{
			fps = count;
			count = 0;
			sectime = timeGetTime();
		}
		// 每帧计数
		count++;

		// 渲染
		cleardevice();
		putimage(0, 0, &bkimg);
		putimage(0, 70, &objimg);
		putimage(0, 70, &titleimg);
		transparentimage(NULL, 560, 260, &logo);
		settextcolor(WHITE);
		rect.top = 70;
		rect.bottom = 120;
		SetFontHeight(&font, 32);
		drawtext(TEXT("半透明图像示例"), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rect.top = 120;
		rect.bottom = 470;
		SetFontHeight(&font, 24);
		drawtext(TEXT("这是一行文本，用于显示一些东西。\n1234567890，abcdefg，あいうえお、このあほ！\n在同路径下创建同名ini以更改字体，如下所示：\n半透明示例.ini\n[General]\nFont=黑体\n\n注意，ini文件的编码需为ANSI，其他编码会出现读取错误。\n另外，此部分文字所占用的IMAGE可以载入其他大小合适的图片。\n右下角logo\"長門有希ちゃんの消失\"是内嵌资源，不会消失的。"), &rect, DT_WORDBREAK);
		settextcolor(RED);
		outtextxy(0, 422, TEXT("按下退出按钮或Esc以退出该程序。"));
		settextcolor(WHITE);
		wsprintf(fpsinfo, TEXT("%u"), fps);
		outtextxy(0, 0, fpsinfo);
		exit_button.draw();

		// 输出
		FlushBatchDraw();

		Sleep(16);
	}
	EndBatchDraw();
	closegraph();
}

void SetAlphaImageLayer(IMAGE* objimg, IMAGE* sourceimg, int x, int y, float alpha, bool tcolor,COLORREF transparentColor)
{
	DWORD* obj = GetImageBuffer(objimg);
	IMAGE tmp;
	// 从原始图像中获取裁剪区图像
	SetWorkingImage(sourceimg);
	getimage(&tmp, x, y, objimg->getwidth(), objimg->getheight());
	SetWorkingImage(NULL);
	DWORD* source = GetImageBuffer(&tmp);

	//transparentColor = 0xff000000 | BGR(transparentColor);
	
	for (int i = 0; i < objimg->getheight() * objimg->getwidth(); i++)
	{
		if (tcolor && (obj[i] & 0xff000000) >> 24 <= 75)
			obj[i] = source[i];
		else
			obj[i] = RGB(GetRValue(obj[i]) * alpha + GetRValue(source[i]) * (1 - alpha), GetGValue(obj[i]) * alpha + GetGValue(source[i]) * (1 - alpha), GetBValue(obj[i]) * alpha + GetBValue(source[i]) * (1 - alpha));
	}
}

void setfontset(LOGFONT *font)
{
	wchar_t str[32];
	gettextstyle(font);
	GetPrivateProfileString(TEXT("General"), TEXT("Font"), TEXT("黑体"), str, 32, TEXT(".\\半透明示例.ini"));
	wcscpy_s(font->lfFaceName, str);
	font->lfHeight = 32;
	font->lfQuality = PROOF_QUALITY | ANTIALIASED_QUALITY;
	settextstyle(font);
}

void SetFontHeight(LOGFONT* ofont, int size)
{
	gettextstyle(ofont);
	ofont->lfHeight = size;
	settextstyle(ofont);
}

void SetWindowCenter(HWND hwnd)
{
	int x = (GetSystemMetrics(SM_CXSCREEN) - SCREENW) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - SCREENH) / 2;
	RECT rect;

	GetWindowRect(hwnd, &rect);
	MoveWindow(hwnd, x, y, rect.right - rect.left, rect.bottom - rect.top, true);
}